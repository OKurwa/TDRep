#include "stdafx.h"
#include "FireParent.h"


//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс снаряда				//
//----------------------------------------------//
//----------------------------------------------//

FireParent::FireParent() : ref_cnt_(0) {
	_missileType = "";
	_position = FPoint(0,0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_tex = nullptr;
	_damage = IPoint(0, 0);
};

FireParent::FireParent(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, std::string mType, IPoint dmg, Render::TexturePtr tex) : ref_cnt_(0) {
	_missileType = mType;
	_position = position;
	_targetPosition = tPosition;
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_damage = dmg;
	MakePath();

};

void FireParent::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};

void FireParent::Update(float dt) {
	//_missileTimer += dt;
	if (_missileTimer >= _flyTime && !_hit) {
		_hit = true;
		_fly = false;
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};
FPoint FireParent::Position() { 
	return _position; 
};
FPoint FireParent::TargetPosition() {
	return _targetPosition;
};
std::string FireParent::Type() {
	return _missileType;

};

bool FireParent::Fly() {
	return _fly;
};

bool FireParent::Hit() {
	return _hit;
};

void FireParent::MakePath() {
	_missilePathX.Clear();
	_missilePathY.Clear();
	if (_flyTime == 0 && _missileTimer == 0 && _modSpeed > 0) {
		float distance = _position.GetDistanceTo(_targetPosition); 
		float time = distance / (float)_modSpeed;
		_flyTime = time;
		_missileTimer = 0;
		_missilePathX.addKey(0, _position.x);
		_missilePathY.addKey(0, _position.y);
		_missilePathX.addKey(time, _targetPosition.x);
		_missilePathY.addKey(time, _targetPosition.y);
	}
	else {
		_missilePathX.addKey(_missileTimer, _position.x);
		_missilePathY.addKey(_missileTimer, _position.y);
		_missilePathX.addKey(_flyTime, _targetPosition.x);
		_missilePathY.addKey(_flyTime, _targetPosition.y);
	}
	_missilePathX.CalculateGradient();
	_missilePathY.CalculateGradient();
};


//----------------------------------------------//
//----------------------------------------------//
//				Обычный снаряд					//
//----------------------------------------------//
//----------------------------------------------//
NormalMissile::NormalMissile() {
	_missileType = "Normal";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_tex = nullptr;
	_target = nullptr;
	_damage = IPoint(0, 0);
};

NormalMissile::NormalMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, IPoint dmg, Render::TexturePtr tex) {
	
	_missileType = "Normal";
	_position = position;
	_targetPosition = target->HitPosition(fTime);
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_damage = dmg;
	_target = target;
	MakePath();
};


NormalMissile::~NormalMissile() {

};

void NormalMissile::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};

void NormalMissile::Update(float dt) {
	if (_missileTimer >= _flyTime && !_hit) {
		_hit = true;
		_fly = false;
		_target->TakeDamage(_missileType, FPoint(0,0), math::random(_damage.x, _damage.y));
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};


//----------------------------------------------//
//----------------------------------------------//
//				Замедляющий снаряд				//
//----------------------------------------------//
//----------------------------------------------//

SlowMissile::SlowMissile() {
	_missileType = "Slow";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_tex = nullptr;
	_slow = FPoint(0,0);
	_splashRange = 0;
	_damage = IPoint(0, 0);
	
};

SlowMissile::SlowMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, FPoint sFactor, int sRange, IPoint dmg, Render::TexturePtr tex) {
	_missileType = "Slow";
	_position = position;
	_targetPosition = tPosition;
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_slow = sFactor;
	_splashRange = sRange;
	_targets = targets;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_damage = dmg;
	MakePath();
};

SlowMissile::~SlowMissile() {
};

void SlowMissile::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};

void SlowMissile::Update(float dt) {
	if (_missileTimer >= _flyTime && !_hit) {
		_hit = true;
		_fly = false;
		for (int i = 0; i < _targets.size(); i++) {
			FPoint tPos = _targets[i]->Position();
			float d = sqrt((tPos.x - _position.x)*(tPos.x - _position.x) + (tPos.y - _position.y)*(tPos.y - _position.y));
			if (d < _splashRange) {
				_targets[i]->TakeDamage(_missileType, _slow, math::random(_damage.x, _damage.y));
			}
		}
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};


//----------------------------------------------//
//----------------------------------------------//
//				Отравляющий снаряд				//
//----------------------------------------------//
//----------------------------------------------//

DecayMissile::DecayMissile() {
	_missileType = "Decay";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_decay = FPoint(0,0);
	_tex = nullptr;
	_target = nullptr;
	_damage = IPoint(0, 0);
};
DecayMissile::DecayMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint decayFactor, IPoint dmg, Render::TexturePtr tex) {
	_missileType = "Decay";
	_position = position;
	_targetPosition = target->HitPosition(fTime);
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_target = target;
	_decay = decayFactor;
	_damage = dmg;
	MakePath();
};
DecayMissile::~DecayMissile() {};

void DecayMissile::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};
void DecayMissile::Update(float dt) {
	if (_missileTimer >= _flyTime && !_hit) {
		
		_hit = true;
		_fly = false;
		_target->TakeDamage(_missileType, _decay, math::random(_damage.x, _damage.y));
		
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}

};

//----------------------------------------------//
//----------------------------------------------//
//				Оглушающий снаряд				//
//----------------------------------------------//
//----------------------------------------------//

BashMissile::BashMissile() {
	_missileType = "Bash";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_bash = FPoint(0,0);
	_tex = nullptr;
	_target = nullptr;
	_damage = IPoint(0, 0);
};
BashMissile::BashMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint bash, IPoint dmg, Render::TexturePtr tex) {
	_missileType = "Bash";
	_position = position;
	_targetPosition = target->HitPosition(fTime);
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_bash = bash;
	_target = target;
	_damage = dmg;
	MakePath();
};
BashMissile::~BashMissile() {};

void BashMissile::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};
void BashMissile::Update(float dt) {
	if (_missileTimer >= _flyTime && !_hit) {
		_hit = true;
		_fly = false;
		_target->TakeDamage(_missileType, _bash, math::random(_damage.x, _damage.y));
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Разрывной снаряд				//
//----------------------------------------------//
//----------------------------------------------//
SplashMissile::SplashMissile() {
	_missileType = "Splash";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_speed = FPoint(0, 0);
	_modSpeed = 0;
	_flyTime = 0;
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_tex = nullptr;
	_splashRange = 0;
	_damage = IPoint(0, 0);
	
	
};
SplashMissile::SplashMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, int sRange, IPoint dmg, Render::TexturePtr tex) {
	_missileType = "Splash";
	_position = position;
	_targetPosition = tPosition;
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	_flyTime = fTime;
	_missileTimer = mFlyTimer;
	_fly = true;
	_hit = false;
	_tex = tex;
	_splashRange = sRange;
	_targets = targets;
	_missilePathX.Clear();
	_missilePathY.Clear();
	_damage = dmg;
	MakePath();
};
SplashMissile::~SplashMissile() {};

void SplashMissile::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 1, _position.y - 1, 3, 3);
		//Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);
	}
};
void SplashMissile::Update(float dt) {
	if (_missileTimer >= _flyTime && !_hit) {
		_hit = true;
		_fly = false;
		for (int i = 0; i < _targets.size(); i++) {
			FPoint tPos = _targets[i]->Position();
			float d = sqrt((tPos.x - _position.x)*(tPos.x - _position.x) + (tPos.y - _position.y)*(tPos.y - _position.y));
			if (d < _splashRange) {

				_targets[i]->TakeDamage(_missileType, FPoint(0,0),math::random(_damage.x, _damage.y));
			}
		}
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};




/*

//////////////////////////////
//--------------------------//
//----------Снаряд----------//
//--------------------------//
//////////////////////////////


Missile::Missile() {
	x = 0;
	y = 0;
	speed = 0;
	targetPosX = 0;
	targetPosY = 0;
	missileFlyTime = 0;
	missileTimer = 0;
	fly = true;
	dive = false;
	missilePathX.Clear();
	missilePathY.Clear();
};

Missile::Missile(float startX, float startY, float endX, float endY, int nSpeed) {
	
	x = startX;
	y = startY;
	fly = true;
	dive = false;
	//Запуск таймеров
	
	float distance = GetDistanceTo(endX, endY);
	speed = nSpeed;
	missileFlyTime = distance / speed;
	missileTimer = missileFlyTime;
	//Координаты цели
	targetPosX = endX;
	targetPosY = endY;
	

	//Расчет траектории
	//Начальная точка
	missilePathX.addKey(missileFlyTime, x);
	missilePathY.addKey(missileFlyTime, y);
	//Конечная точка
	missilePathX.addKey(0, endX);
	missilePathY.addKey(0, endY);
	//Расчет
	missilePathX.CalculateGradient();
	missilePathY.CalculateGradient();
};



Missile::~Missile() {
	missilePathX.Clear();
	missilePathY.Clear();
};


void Missile::Update(float dt) {
	//Тик таймера снаряда
	if (missileTimer - 2*dt <= 0) {
		dive = true;
	}
	if (missileTimer > 0) {
		missileTimer -= dt;
	}
	if (missileTimer < 0) {
		missileTimer = 0;
	}


	if (missileTimer > 0) {
		x = missilePathX.getGlobalFrame(missileTimer);
		y = missilePathY.getGlobalFrame(missileTimer);;
		
	}
	else {
		missilePathX.Clear();
		missilePathY.Clear();
		x = 0;
		y = 0;
		
		fly = false;
	}
	
	


};




void  Missile::SetSpeed(int nSpeed) {
	speed = nSpeed;
};

void  Missile::SetTargetPosX(float nTargetPosX) {
	targetPosX = nTargetPosX;
};

void  Missile::SetTargetPosY(float nTargetPosY) {
	targetPosY = nTargetPosY;
};



void  Missile::SetMissileFlyTime(float nFlyTime) {
	missileFlyTime = nFlyTime;
};

void  Missile::SetMissileTimer(float nTimer) {
	missileTimer = nTimer;
};





void Missile::CalculateFly() {
	fly = true;
	dive = false;
	//Запуск таймеров
	
	float distance = GetDistanceTo(targetPosX, targetPosY);
	missileFlyTime = distance / speed;
	missileTimer = missileFlyTime;
	missilePathX.addKey(missileFlyTime, x);
	missilePathY.addKey(missileFlyTime, y);
	//Конечная точка
	missilePathX.addKey(0, targetPosX);
	missilePathY.addKey(0, targetPosY);
	//Расчет
	missilePathX.CalculateGradient();
	missilePathY.CalculateGradient();
};



int	  Missile::GetSpeed() {
	return speed;
};

float Missile::GetTargetPosX() {
	return targetPosX;
};

float Missile::GetTargetPosY() {
	return targetPosY;
};

IPoint Missile::GetTargetPos() {
	return IPoint(round(targetPosX), round(targetPosY));
};

float Missile::GetMissileFlyTime() {
	return missileFlyTime;
};

float Missile::GetMissileTimer() {
	return missileTimer;
};

bool Missile::Fly() {
	return fly;
};

bool Missile::Dive() {
	return dive;
};


//////////////////////////////
//--------------------------//
//----------Пушка-----------//
//--------------------------//
//////////////////////////////

Cannon::Cannon()
{
	x = 0;
	y = 0;
	delay = 0.0;
	reloadTimer = 0.0;
	attackSpeed = 0;
	range = 0;
	splashRange = 0;
    missileSpeed = 300;
	missiles.clear();

}


Cannon::~Cannon()
{
}

void Cannon::Update(float dt) {
	
	//Тик таймера перезарядки
	if (reloadTimer > 0) {
		reloadTimer -= dt;
	}
	if (reloadTimer < 0) {
		reloadTimer = 0;
	}
	
	for (int missile = 0; missile < missiles.size(); missile++) {
		missiles[missile].Update(dt);
	}
	
};

bool Cannon::DelMissile(unsigned int missile) {
	
	if (missiles.size()>missile) {
		missiles.erase(missiles.begin() + missile);
		
		return true;
	}
	else {
		return false;
	}
};


//Установка параметров атаки
void Cannon::SetDelay(float newDelay) {
	delay = newDelay;
};

void Cannon::SetReloadTimer(float timer) {
	reloadTimer = timer;
};

void Cannon::SetAttackSpeed(int aSpeed) {
	attackSpeed = aSpeed;
};

void Cannon::SetRange(int aRange) {
	range = aRange;
};

void Cannon::SetSplashRange(int aSplash) {
	splashRange = aSplash;
};

void Cannon::SetMissileSpeed(int mSpeed) {
	missileSpeed = mSpeed;
};

//Выстрел
bool Cannon::Shoot(float ltX, float ltY) {
	if (reloadTimer == 0) {
		
		//Запуск таймеров
		reloadTimer = delay;

		float distance = GetDistanceTo(ltX, ltY);

		float missileFlyTime = distance / missileSpeed;
		
		Missile missile = Missile(x, y, ltX, ltY, missileSpeed);
		missiles.push_back(missile);
		return true;
		
	}
	else {
		return false;
	}
	
};





//Доступ к членам

float Cannon::GetDelay() {
	return delay;
};

float Cannon::GetReloadTimer() {
	return reloadTimer;
};

int	  Cannon::GetAttackSpeed() {
	return attackSpeed;
};

int	  Cannon::GetRange() {
	return range;
};

int   Cannon::GetSplashRange() {
	return splashRange;
};

int	   Cannon::GetMissileSpeed() {
	return missileSpeed;
};
std::vector<Missile> & Cannon::GetMissiles() {
	return missiles;
};
*/