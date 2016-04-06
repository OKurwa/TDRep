#include "stdafx.h"
#include "FireParent.h"
#include "MonsterParent.h"
//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс монстра 				//
//----------------------------------------------//
//----------------------------------------------//

MonsterParent::MonsterParent() {
	_position = FPoint(0,0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_hp = 0;
	_moveTimer = 0;
	_curWayDistance = 0;
	_damaged = false ;
	_map = nullptr;
	_skin = nullptr;
	_curWaySplineX.Clear();
	_curWaySplineY.Clear();
	_dead = false;
};

MonsterParent::MonsterParent(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin) {
	_position = position;
	/*
	float sX = math::random(-modSpeed, modSpeed);
	float sY;
	if (math::random(-1, 1) < 0) {
		sY = -1;

	}
	else {
		sY = 1;
	};

	sY *= sqrt(modSpeed*modSpeed - sX*sX);

	
	_speed = FPoint(sX, sY);
	*/
	_modSpeed = modSpeed;
	_map = map;
	_curCell = map->PosCell(_position);
	_curWayDistance = 0;
	float timer = 0;
	if (FindAWay()) {
		_curWaySplineX.addKey(timer, _position.x);
		_curWaySplineY.addKey(timer, _position.y);
		for (int i = 1; i < _currentWay.size(); i++) {
			FPoint newCell = FPoint(_currentWay[i].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i].y * _map->CellSize().y + _map->CellSize().y / 2);
			FPoint oldCell = FPoint(_currentWay[i - 1].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i - 1].y * _map->CellSize().y + _map->CellSize().y / 2);
			float distance = sqrt((newCell.x - oldCell.x)*(newCell.x - oldCell.x) + (newCell.y - oldCell.y)*(newCell.y - oldCell.y));
			_curWayDistance += distance;
			timer += distance / _modSpeed;
			_curWaySplineX.addKey(timer, newCell.x);
			_curWaySplineY.addKey(timer, newCell.y);

		}
		_curWaySplineX.CalculateGradient();
		_curWaySplineY.CalculateGradient();
	}
	else {
		_curWaySplineX.Clear();
		_curWaySplineY.Clear();
	}
	
	

	_moveTimer = 0;
	
	
	_hp = hp;
	_damaged = false;
	
	_skin = skin;
	_dead = false;
};
MonsterParent::~MonsterParent() {};

void MonsterParent::Draw() {
	if (!_dead) {
		IRect cRect = IRect(_position.x - 2, _position.y - 2, 5, 5);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(100, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
	
};

void MonsterParent::Update(float dt) {
	_moveTimer += dt;
	_curWayDistance -= _modSpeed*dt;
	_position.x = _curWaySplineX.getGlobalFrame(_moveTimer);
	_position.y = _curWaySplineY.getGlobalFrame(_moveTimer);
	
};



bool MonsterParent::FindAWay() {

	//Разметка карты проходимости
	std::vector<std::vector<int>> intMap;
	intMap.resize(_map->Size().x);
	for (int i = 0; i < intMap.size(); i++) {
		intMap[i].resize(_map->Size().y);
	}
	for (int i = 0; i < intMap.size(); i++) {
		for (int j = 0; j < intMap.size(); j++) {
			intMap[i][j] = -1;
			switch (_map->Cells()[i][j]->Type())
			{
			case PASS:
				intMap[i][j] = 9998;
				break;
			case SPAWN:
				intMap[i][j] = 9998;
				break;
			case LAIR:
				intMap[i][j] = 9999;
				break;
			default:
				intMap[i][j] = -1;
				break;
			}
		}
	}


	//Разметка прохождения волны
	//_curCell = IPoint(0, 5);
	//IPoint finish = _curCell;
	_currentWay.clear();
	intMap[_curCell.x][_curCell.y] = 0;
	int d = 0;
	bool end = 0;

	while (!end && d <= intMap.size() * intMap[0].size()) {//Пока не достигнут пункт назначения
		//Для каждой клетки карты,
		for (int i = 0; i < intMap.size(); i++) {
			for (int j = 0; j < intMap[i].size(); j++) {
				// которая имеет текущее значение волны,
				if (intMap[i][j] == d) {
					// обходим смежные клетки,
					for (int k = i - 1; k <= i + 1; k++) {
						for (int l = j - 1; l <= j + 1; l++) {
							//не выходя за границы карты.
							if (k >= 0 && k < intMap.size()) {
								if (l >= 0 && l < intMap[k].size()) {
									//Если смежная клетка не пункт назначения
									if (intMap[k][l] != 9999) {
										//и является проходимой и не помеченной волной
										if (intMap[k][l] == 9998) {
											intMap[k][l] = d + 1;//метим ее значением следующей волны.
										}
									}
									else {
										end = true;
										_currentWay.push_back(IPoint(k, l));//Достигнут конец, добавляем в путь конечную точку 
										break;//и прекращаем поиск пункта назначения... 
									}
								}
							}

						}

						if (end)
							break;//...среди смежных точек
					}

				}


				if (end)
					break;//Прекращаем обход ...
			}

			if (end)
				break;//... карты
		}

		++d;// Повышаем значение волны
	}


	bool recover = false;
	//Восстановление пути
	if (_currentWay.size() > 0) {//Если найден путь к пункту назначения
		
		while (d >= 0) {//Пока значение волны больше нуля
			//Для всех клеток
			for (int k = 0; k < intMap.size(); k++) {
				for (int l = 0; l < intMap[k].size(); l++) {
				//	if (k >= 0 && k < intMap.size()) {
				//		if (l >= 0 && l < intMap[k].size()) {
					if (intMap[k][l] == d - 1) {//если значение их волны меньше на 1
								//и они находятся в окрестности текущей клетки
						if (d > 0 && math::abs(_currentWay[_currentWay.size() - 1].x - k) <= 1 && math::abs(_currentWay[_currentWay.size() - 1].y - l <= 1)) {
							_currentWay.push_back(IPoint(k, l));//добавляем клетку в путь 
							recover = true;
							break;//и прекращаем поиск... 
						}
									
								
							}
							//else if(intMap[k][l]>d && intMap[k][l]<9999) {
							//	intMap[k][l] = 9998;
							//}
							
				//		}
				//	}

				}

				if (recover)
					break; //...для текущего значения волны
			}
			recover = false;
			--d;//Понижаем значение волны
		}
		
	} 

	for (int i = 0; i < intMap.size(); i++) {
		for (int j = 0; j < intMap[i].size(); j++) {
			if (intMap[i][j]!=-1 && intMap[i][j]!=9999) {
				intMap[i][j] = 9998;
			}
		}
	}


	//_currentWay.erase(_currentWay.begin());
	std::reverse(_currentWay.begin(), _currentWay.end());
	if (_currentWay.size() > 0) {
		return true;
	}
	else {
		return false;
	}
		

};

bool MonsterParent::Dead() {
	return _dead;
};


float MonsterParent::WayDistance() {
	return _curWayDistance;
};


FPoint MonsterParent::Position() {
	return _position;
};

FPoint MonsterParent::HitPosition(float dt) {
	float edt = dt;
	edt *=(1 - _slow.x)*(1 - _bash.x);
	return FPoint(_curWaySplineX.getGlobalFrame(edt + _moveTimer),
				  _curWaySplineY.getGlobalFrame(edt + _moveTimer));
};


//----------------------------------------------//
//----------------------------------------------//
//				Обычный монстр    				//
//----------------------------------------------//
//----------------------------------------------//


NormalMonster::NormalMonster() {
	_position = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_hp = 0;
	_moveTimer = 0;
	_curWayDistance = 0;
	_damaged = false;
	_map = nullptr;
	_skin = nullptr;
	_curWaySplineX.Clear();
	_curWaySplineY.Clear();
	_dead = false;
};
NormalMonster::NormalMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin) {
	_position = position;
	/*
	float sX = math::random(-modSpeed, modSpeed);
	float sY;
	if (math::random(-1, 1) < 0) {
	sY = -1;

	}
	else {
	sY = 1;
	};

	sY *= sqrt(modSpeed*modSpeed - sX*sX);


	_speed = FPoint(sX, sY);
	*/
	_modSpeed = modSpeed;
	
	_map = map;
	_curCell = map->PosCell(_position);
	_curWayDistance = 0;
	
	
	
	float timer = 0;
	if (FindAWay()) {
		_curWaySplineX.addKey(timer, _position.x);
		_curWaySplineY.addKey(timer, _position.y);
		for (int i = 1; i < _currentWay.size(); i++) {
			FPoint newCell = FPoint(_currentWay[i].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i].y * _map->CellSize().y + _map->CellSize().y / 2);
			FPoint oldCell = FPoint(_currentWay[i - 1].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i - 1].y * _map->CellSize().y + _map->CellSize().y / 2);
			float distance = sqrt((newCell.x - oldCell.x)*(newCell.x - oldCell.x) + (newCell.y - oldCell.y)*(newCell.y - oldCell.y));
			_curWayDistance += distance;
			timer += distance / _modSpeed;
			_curWaySplineX.addKey(timer, newCell.x);
			_curWaySplineY.addKey(timer, newCell.y);

		}
		_curWaySplineX.CalculateGradient();
		_curWaySplineY.CalculateGradient();
	}
	else {
		_curWaySplineX.Clear();
		_curWaySplineY.Clear();
	}



	_moveTimer = 0;


	_hp = hp;
	_damaged = false;

	_skin = skin;
	_dead = false;
};
NormalMonster::~NormalMonster() {
};

void NormalMonster::Draw() {
	if (!_dead) {
		IRect cRect = IRect(_position.x - 2, _position.y - 2, 5, 5);
		//Render::device.SetTexturing(false);
		Render::BindFont("arial");

		Render::BeginColor(Color(100, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		Render::device.SetTexturing(true);
		Render::BeginColor(Color(255, 255, 255, 255));
		Render::PrintString(FPoint(_position.x - 2, _position.y - 2), utils::lexical_cast(math::round(_hp)), 0.70f);
		Render::EndColor();
		Render::device.SetTexturing(false);
		//Render::device.SetTexturing(true);
	}
	
};
void NormalMonster::Update(float dt) {
	//Таймер замедления
	if (_slow.y <= 0) {
		_slow = FPoint(0,0);
	}
	else {
		_slow.y -= dt;
	}
	//Таймер отравления
	if (_decay.y <= 0) {
		_decay = FPoint(0, 0);
	}
	else {
		_decay.y -= dt;
	}
	//Таймер оглушения
	if (_bash.y <= 0) {
		_bash = FPoint(0, 0);
	}
	else {
		_bash.y -= dt;
	}
	
	_hp -= _decay.x * dt;

	if (_hp <= 0) {
		_dead = true;
	}
	float edt = dt;
	edt *= (1 - _slow.x)*(1 - _bash.x);

	_moveTimer += edt;
	_curWayDistance -= _modSpeed*edt;
	_position.x = _curWaySplineX.getGlobalFrame(_moveTimer);
	_position.y = _curWaySplineY.getGlobalFrame(_moveTimer);
};
void NormalMonster::TakeDamage(std::string effType, FPoint values, float damage) {
	if (effType == "Slow")
		_slow = values;
	if (effType == "Decay")
		_decay = values;
	if (effType == "Bash") {
		values.x *= 100;
		if (math::random(0, 100) <= values.x)
			_bash = FPoint(1, values.y);
	}

	if (_hp > 0) {
		_hp -= damage;
	}
	
};


//----------------------------------------------//
//----------------------------------------------//
//						Босс    				//
//----------------------------------------------//
//----------------------------------------------//

BossMonster::BossMonster() {
	_position = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_hp = 0;
	_moveTimer = 0;
	_curWayDistance = 0;
	_damaged = false;
	_map = nullptr;
	_skin = nullptr;
	_curWaySplineX.Clear();
	_curWaySplineY.Clear();
	_dead = false;
	_reduceDamage = 0;
};
BossMonster::BossMonster(FPoint position, int modSpeed, int hp, FieldMap * map, float reduceDamage, Render::TexturePtr skin) {
	_position = position;
	/*
	float sX = math::random(-modSpeed, modSpeed);
	float sY;
	if (math::random(-1, 1) < 0) {
	sY = -1;

	}
	else {
	sY = 1;
	};

	sY *= sqrt(modSpeed*modSpeed - sX*sX);


	_speed = FPoint(sX, sY);
	*/
	_modSpeed = modSpeed;

	_map = map;
	_curCell = map->PosCell(_position);
	_curWayDistance = 0;



	float timer = 0;
	if (FindAWay()) {
		_curWaySplineX.addKey(timer, _position.x);
		_curWaySplineY.addKey(timer, _position.y);
		for (int i = 1; i < _currentWay.size(); i++) {
			FPoint newCell = FPoint(_currentWay[i].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i].y * _map->CellSize().y + _map->CellSize().y / 2);
			FPoint oldCell = FPoint(_currentWay[i - 1].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i - 1].y * _map->CellSize().y + _map->CellSize().y / 2);
			float distance = sqrt((newCell.x - oldCell.x)*(newCell.x - oldCell.x) + (newCell.y - oldCell.y)*(newCell.y - oldCell.y));
			_curWayDistance += distance;
			timer += distance / _modSpeed;
			_curWaySplineX.addKey(timer, newCell.x);
			_curWaySplineY.addKey(timer, newCell.y);

		}
		_curWaySplineX.CalculateGradient();
		_curWaySplineY.CalculateGradient();
	}
	else {
		_curWaySplineX.Clear();
		_curWaySplineY.Clear();
	}



	_moveTimer = 0;


	_hp = hp;
	_damaged = false;

	_skin = skin;
	_dead = false;
	_reduceDamage = reduceDamage;
};
BossMonster::~BossMonster() {
};

void BossMonster::Draw() {
	if (!_dead) {
		IRect cRect = IRect(_position.x - 2, _position.y - 2, 5, 5);
		//Render::device.SetTexturing(false);
		Render::BindFont("arial");

		Render::BeginColor(Color(100, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		Render::device.SetTexturing(true);
		Render::BeginColor(Color(255, 255, 255, 255));
		Render::PrintString(FPoint(_position.x - 2, _position.y - 2), utils::lexical_cast(math::round(_hp)), 0.70f);
		Render::EndColor();
		Render::device.SetTexturing(false);
		//Render::device.SetTexturing(true);
	}

};
void BossMonster::Update(float dt) {
	//Таймер замедления
	if (_slow.y <= 0) {
		_slow = FPoint(0, 0);
	}
	else {
		_slow.y -= dt;
	}
	//Таймер отравления
	if (_decay.y <= 0) {
		_decay = FPoint(0, 0);
	}
	else {
		_decay.y -= dt;
	}
	//Таймер оглушения
	if (_bash.y <= 0) {
		_bash = FPoint(0, 0);
	}
	else {
		_bash.y -= dt;
	}

	_hp -= _decay.x * dt;

	if (_hp <= 0) {
		_dead = true;
	}
	float edt = dt;
	edt *= (1 - _slow.x)*(1 - _bash.x);

	_moveTimer += edt;
	_curWayDistance -= _modSpeed*edt;
	_position.x = _curWaySplineX.getGlobalFrame(_moveTimer);
	_position.y = _curWaySplineY.getGlobalFrame(_moveTimer);
};
void BossMonster::TakeDamage(std::string effType, FPoint values, float damage) {
	
	values.y *= 1 - _reduceDamage;
	
	if (effType == "Slow")
		_slow = values;
	if (effType == "Decay")
		_decay = values;
	if (effType == "Bash") {
		values.x *= 100;
		if (math::random(0, 100) <= values.x)
			_bash = FPoint(1, values.y);
	}

	if (_hp > 0) {
		_hp -= damage * (1 - _reduceDamage);
	}

};







//----------------------------------------------//
//----------------------------------------------//
//					Имунный		   				//
//----------------------------------------------//
//----------------------------------------------//
ImmuneMonster::ImmuneMonster() {
	_position = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_hp = 0;
	_moveTimer = 0;
	_curWayDistance = 0;
	_damaged = false;
	_map = nullptr;
	_skin = nullptr;
	_curWaySplineX.Clear();
	_curWaySplineY.Clear();
	_dead = false;
};
ImmuneMonster::ImmuneMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin) {
	_position = position;
	/*
	float sX = math::random(-modSpeed, modSpeed);
	float sY;
	if (math::random(-1, 1) < 0) {
	sY = -1;

	}
	else {
	sY = 1;
	};

	sY *= sqrt(modSpeed*modSpeed - sX*sX);


	_speed = FPoint(sX, sY);
	*/
	_modSpeed = modSpeed;

	_map = map;
	_curCell = map->PosCell(_position);
	_curWayDistance = 0;



	float timer = 0;
	if (FindAWay()) {
		_curWaySplineX.addKey(timer, _position.x);
		_curWaySplineY.addKey(timer, _position.y);
		for (int i = 1; i < _currentWay.size(); i++) {
			FPoint newCell = FPoint(_currentWay[i].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i].y * _map->CellSize().y + _map->CellSize().y / 2);
			FPoint oldCell = FPoint(_currentWay[i - 1].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i - 1].y * _map->CellSize().y + _map->CellSize().y / 2);
			float distance = sqrt((newCell.x - oldCell.x)*(newCell.x - oldCell.x) + (newCell.y - oldCell.y)*(newCell.y - oldCell.y));
			_curWayDistance += distance;
			timer += distance / _modSpeed;
			_curWaySplineX.addKey(timer, newCell.x);
			_curWaySplineY.addKey(timer, newCell.y);

		}
		_curWaySplineX.CalculateGradient();
		_curWaySplineY.CalculateGradient();
	}
	else {
		_curWaySplineX.Clear();
		_curWaySplineY.Clear();
	}



	_moveTimer = 0;


	_hp = hp;
	_damaged = false;

	_skin = skin;
	_dead = false;
};
ImmuneMonster::~ImmuneMonster() {
};

void ImmuneMonster::Draw() {
	if (!_dead) {
		IRect cRect = IRect(_position.x - 2, _position.y - 2, 5, 5);
		//Render::device.SetTexturing(false);
		Render::BindFont("arial");

		Render::BeginColor(Color(100, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		Render::device.SetTexturing(true);
		Render::BeginColor(Color(255, 255, 255, 255));
		Render::PrintString(FPoint(_position.x - 2, _position.y - 2), utils::lexical_cast(math::round(_hp)), 0.70f);
		Render::EndColor();
		Render::device.SetTexturing(false);
		//Render::device.SetTexturing(true);
	}

};
void ImmuneMonster::Update(float dt) {
	//Таймер замедления
	if (_slow.y <= 0) {
		_slow = FPoint(0, 0);
	}
	else {
		_slow.y -= dt;
	}
	//Таймер отравления
	if (_decay.y <= 0) {
		_decay = FPoint(0, 0);
	}
	else {
		_decay.y -= dt;
	}
	//Таймер оглушения
	if (_bash.y <= 0) {
		_bash = FPoint(0, 0);
	}
	else {
		_bash.y -= dt;
	}

	_hp -= _decay.x * dt;

	if (_hp <= 0) {
		_dead = true;
	}
	float edt = dt;
	edt *= (1 - _slow.x)*(1 - _bash.x);

	_moveTimer += edt;
	_curWayDistance -= _modSpeed*edt;
	_position.x = _curWaySplineX.getGlobalFrame(_moveTimer);
	_position.y = _curWaySplineY.getGlobalFrame(_moveTimer);
};
void ImmuneMonster::TakeDamage(std::string effType, FPoint values, float damage) {
	
	
	//if (effType == "Slow")
	//	_slow = values;
	//if (effType == "Decay")
	//	_decay = values;
	if (effType == "Bash") {
		values.x *= 100;
		if (math::random(0, 100) <= values.x)
			_bash = FPoint(1, values.y);
	}

	if (_hp > 0) {
		_hp -= damage;
	}

};

//----------------------------------------------//
//----------------------------------------------//
//				Регенерирующий    				//
//----------------------------------------------//
//----------------------------------------------//

HealingMonster::HealingMonster() {
	_position = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_hp = 0;
	_moveTimer = 0;
	_curWayDistance = 0;
	_damaged = false;
	_map = nullptr;
	_skin = nullptr;
	_curWaySplineX.Clear();
	_curWaySplineY.Clear();
	_dead = false;
	_healPerSecond = 0;
};
HealingMonster::HealingMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin, int healPerSecond) {
	_position = position;
	/*
	float sX = math::random(-modSpeed, modSpeed);
	float sY;
	if (math::random(-1, 1) < 0) {
	sY = -1;

	}
	else {
	sY = 1;
	};

	sY *= sqrt(modSpeed*modSpeed - sX*sX);


	_speed = FPoint(sX, sY);
	*/
	_modSpeed = modSpeed;

	_map = map;
	_curCell = map->PosCell(_position);
	_curWayDistance = 0;



	float timer = 0;
	if (FindAWay()) {
		_curWaySplineX.addKey(timer, _position.x);
		_curWaySplineY.addKey(timer, _position.y);
		for (int i = 1; i < _currentWay.size(); i++) {
			FPoint newCell = FPoint(_currentWay[i].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i].y * _map->CellSize().y + _map->CellSize().y / 2);
			FPoint oldCell = FPoint(_currentWay[i - 1].x * _map->CellSize().x + _map->CellSize().x / 2, _currentWay[i - 1].y * _map->CellSize().y + _map->CellSize().y / 2);
			float distance = sqrt((newCell.x - oldCell.x)*(newCell.x - oldCell.x) + (newCell.y - oldCell.y)*(newCell.y - oldCell.y));
			_curWayDistance += distance;
			timer += distance / _modSpeed;
			_curWaySplineX.addKey(timer, newCell.x);
			_curWaySplineY.addKey(timer, newCell.y);

		}
		_curWaySplineX.CalculateGradient();
		_curWaySplineY.CalculateGradient();
	}
	else {
		_curWaySplineX.Clear();
		_curWaySplineY.Clear();
	}



	_moveTimer = 0;


	_hp = hp;
	_damaged = false;

	_skin = skin;
	_dead = false;
	_healPerSecond = healPerSecond;
};
HealingMonster::~HealingMonster() {
};

void HealingMonster::Draw() {
	if (!_dead) {
		IRect cRect = IRect(_position.x - 2, _position.y - 2, 5, 5);
		//Render::device.SetTexturing(false);
		Render::BindFont("arial");

		Render::BeginColor(Color(100, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		Render::device.SetTexturing(true);
		Render::BeginColor(Color(255, 255, 255, 255));
		Render::PrintString(FPoint(_position.x - 2, _position.y - 2), utils::lexical_cast(math::round(_hp)), 0.70f);
		Render::EndColor();
		Render::device.SetTexturing(false);
		//Render::device.SetTexturing(true);
	}

};
void HealingMonster::Update(float dt) {
	//Таймер замедления
	if (_slow.y <= 0) {
		_slow = FPoint(0, 0);
	}
	else {
		_slow.y -= dt;
	}
	//Таймер отравления
	if (_decay.y <= 0) {
		_decay = FPoint(0, 0);
	}
	else {
		_decay.y -= dt;
	}
	//Таймер оглушения
	if (_bash.y <= 0) {
		_bash = FPoint(0, 0);
	}
	else {
		_bash.y -= dt;
	}

	_hp -= _decay.x * dt;

	if (_hp <= 0) {
		_dead = true;
	}
	if (!_dead)
		_hp += dt *_healPerSecond;

	float edt = dt;
	edt *= (1 - _slow.x)*(1 - _bash.x);

	_moveTimer += edt;
	_curWayDistance -= _modSpeed*edt;
	_position.x = _curWaySplineX.getGlobalFrame(_moveTimer);
	_position.y = _curWaySplineY.getGlobalFrame(_moveTimer);
};
void HealingMonster::TakeDamage(std::string effType, FPoint values, float damage) {
	if (effType == "Slow")
		_slow = values;
	if (effType == "Decay")
		_decay = values;
	if (effType == "Bash") {
		values.x *= 100;
		if (math::random(0, 100) <= values.x)
			_bash = FPoint(1, values.y);
	}

	if (_hp > 0) {
		_hp -= damage;
		if (_hp <= 0) {
			_dead = true;
		}
	}
	
	

};