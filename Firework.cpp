
#include "stdafx.h"
#include "Firework.h"



FireBall::FireBall() : ref_cnt_(0)
{
	_x = 0;
	_y = 0;
	_xSpeed = 0;
	_ySpeed = 0;
	_modSpeed = 0;
	_lifeTime = 0;
	_lifeTimer = 0;
	_isDead = false;
	_stackSize = 0;
	_ballsCount = 0;
	myeffcount = 0;
	_boomed = false;
	_lightTimer = 0;
}

FireBall::~FireBall()
{
}

FireBall::FireBall(const FireBall& f) {
	_x = f._x;
	_y = f._y;
	_xSpeed = f._xSpeed;
	_ySpeed = f._ySpeed;
	_modSpeed = f._modSpeed;
	_lifeTime = f._lifeTime;
	_lifeTimer = f._lifeTimer;
	_isDead = f._isDead;
	_stackSize = f._stackSize;
	_ballsCount = f._ballsCount;
	_boomed = f._boomed;
	_lightTimer = f._lightTimer;
	_subFireBalls = f._subFireBalls;
	_fireCont = f._fireCont;
	_fireEff = f._fireEff;
	_boomCont = f._boomCont;
	_boomEff = f._boomEff;

};
FireBall& FireBall::operator = (const FireBall& f) {
	if (this == &f) return *this;
	_x = f._x;
	_y = f._y;
	_xSpeed = f._xSpeed;
	_ySpeed = f._ySpeed;
	_modSpeed = f._modSpeed;
	_lifeTime = f._lifeTime;
	_lifeTimer = f._lifeTimer;
	_isDead = f._isDead;
	_stackSize = f._stackSize;
	_ballsCount = f._ballsCount;
	_boomed = f._boomed;
	_lightTimer = f._lightTimer;
	_subFireBalls = f._subFireBalls;
	_fireCont = f._fireCont;
	_fireEff = f._fireEff;
	_boomCont = f._boomCont;
	_boomEff = f._boomEff;
	return *this;
};





void	FireBall::Draw() {
	if(_fireEff)
		if(!_fireEff->isEnd())
			_fireCont.Draw();
	if (_boomEff)
		if (!_boomEff->isEnd())
			_boomCont.Draw();
	
	//Render::BindFont("arial");
	//Render::PrintString(10, 10, utils::lexical_cast(myeffcount));
	if (!_isDead) {

		/*
		Render::device.SetTexturing(false);
		Render::BeginColor(Color(255, 128, 0, 255));
		Render::DrawRect(IRect(IPoint(round(_x),round(_y)), 5, 5));
		Render::EndColor();
		Render::device.SetTexturing(true);
		*/
		//Render::device.SetTexturing(false);
		
		//Render::device.SetTexturing(true);
		
	}
	else if (_subFireBalls.size() > 0) {
		for (unsigned int ball = 0; ball < _subFireBalls.size(); ball++)
			_subFireBalls[ball]->Draw();
		
	}
		
	
};

void	FireBall::Update(float dt) {
	_fireCont.Update(dt);
	_boomCont.Update(dt);
	if (_lifeTimer < 0) {
		_lightTimer = 0;
		_lifeTimer = 0;
		_isDead = true;
		if (_fireEff)
			_fireEff->Finish();
	}
	if (_lightTimer < 0) 
		_lightTimer = 0;
	_lifeTimer -= dt;
	_lightTimer -= dt;
	
	if (!_isDead) {

		_ySpeed -= 15000*dt*dt;
		
		_x += _xSpeed * dt;
		_y += _ySpeed * dt;
		for (unsigned int ball = 0; ball < _subFireBalls.size(); ball++) {
			_subFireBalls[ball]->SetPosX(_x);
			_subFireBalls[ball]->SetPosY(_y);
		}
		
		if (_fireEff) {
			_fireEff->posX = _x;
			_fireEff->posY = _y;
		}
		

		
		if (_lifeTimer <= 0) {
			if (_stackSize > 0) {
				for (unsigned int ball = 0; ball < _subFireBalls.size(); ball++)
					_subFireBalls[ball]->Launch(_x, _y, _modSpeed, 0, 0, _lifeTime, _ballsCount, _stackSize - 1, false);
				
			}

		}
	}
	else {
		if (!_boomEff && !_boomed) {
			_boomEff = _boomCont.AddEffect("Boom");
			_boomEff->SetScale(0.7);
			_boomEff->posX = _x;
			_boomEff->posY = _y;
			_boomed = true;
			_lightTimer = 0.3;
		}
		for (unsigned int ball = 0; ball < _subFireBalls.size(); ball++)
			_subFireBalls[ball]->Update(dt);
		
				
	}
	
	bool e = TryKillEffects();
	
	
};

void FireBall::UpdateChildsPos() {
	for (unsigned int i = 0; i < _subFireBalls.size(); i++) {
		_subFireBalls[i]->SetPosX(_x);
		_subFireBalls[i]->SetPosY(_y);
		_subFireBalls[i]->UpdateChildsPos();
	}
};

void FireBall::Launch(int startX, int startY, float speed, float speedX, float speedY, float lTime, int subBallsCount, int stack, bool first) {
	
	if (first) {
		if (speed > 0 && speedX == 0 && speedY == 0) {//если указан только значение скорости, задаем проекции случано
			int intSpeed = math::round(speed);
			//srand(time(NULL));
			_xSpeed = rand() % intSpeed * 2 - intSpeed;
			if (rand() % 2 > 0) {
				_ySpeed = -sqrt(speed * speed - _xSpeed * _xSpeed);
			}
			else {
				_ySpeed = sqrt(speed * speed - _xSpeed * _xSpeed);
			}
		}
		else {
			_xSpeed = speedX;
			_ySpeed = speedY;
		
		}

	}
	
	

	_x = startX;
	_y = startY;

	if (_modSpeed == 0)	
		_modSpeed = speed;
	//if (_modSpeed != 0 && _xSpeed !=0) {
	//	_ySpeed = _ySpeed * sqrt(_modSpeed*_modSpeed - _xSpeed * _xSpeed);
	//}

	if (sqrt(_ySpeed * _ySpeed + _xSpeed * _xSpeed) != _modSpeed) {// если скорость не соответствует проекци€м, корректируем ее
		_modSpeed = sqrt(_ySpeed * _ySpeed + _xSpeed * _xSpeed);
	} 

	_lifeTime = lTime;
	_lifeTimer = _lifeTime;
	
	if (first) {//ƒл€ первого зар€да увеличиваем врем€ жизни, дл€ дочерних повышаем скорость движени€
		_modSpeed *= 1.0;
		_modSpeed -= 200;
		_lifeTimer *= 2;
	}
	
	
	_isDead = false;
	_ballsCount = subBallsCount;
	_stackSize = stack;

	_subFireBalls.clear();
	int yM = -1;
	float sX = -_modSpeed;
	float step = 2 * _modSpeed / _ballsCount;
	if (_stackSize > 0) {//если остались внутренние зар€ды, наполн€ем их
		for (int ball = 0; ball < _ballsCount; ball++) {
			boost::intrusive_ptr<FireBall> subBall(new FireBall);
			//if (yM < 0)
			++myeffcount;
			float factor = math::random(-step , step * 2);
			subBall->SetSpeedX(sX + factor);
			subBall->SetSpeedY(yM * sqrt(_modSpeed*_modSpeed - (sX + factor) * (sX + factor)));
			//subBall.SetSpeed(_modSpeed);
			_subFireBalls.push_back(subBall);
			sX += step;
			yM *= -1;
		}
	}
	if (!_fireEff && !_isDead) {
		int id = math::random(3);

		switch (id)
		{
		case 0:
			_fireEff = _fireCont.AddEffect("IskraViol");
			break;
		case 1:
			_fireEff = _fireCont.AddEffect("IskraGreen");
			break;
		case 2:
			_fireEff = _fireCont.AddEffect("IskraBlue");
			break;
		case 3:
			_fireEff = _fireCont.AddEffect("Iskra");
			break;

		default:
			_fireEff = _fireCont.AddEffect("Iskra");
			break;
		}

		
		_fireEff->SetScale(0.5);
		_fireEff->posX = _x;
		_fireEff->posY = _y;
		_fireEff->Reset();
	}
	
	
};


bool FireBall::TryKillChilds() {
	bool allDead = true;


	if (!_isDead) {//если жив, чистка отмен€етс€
		return false;
	}
	else if (_fireCont.IsEffectAlive(_fireEff.get()) || _boomCont.IsEffectAlive(_boomEff.get())) {//если мертв, но живы эффекты, чистка отмен€етс€
		return false;
	}
	else {//поиск живых детей
		unsigned int curChild = 0;
		while (allDead && curChild < _subFireBalls.size()) {
			allDead = _subFireBalls[curChild]->TryKillChilds();
			++curChild;
		}
		if (allDead)
			_subFireBalls.clear();
		return allDead;
	}
	
};
bool FireBall::TryKillEffects() {
	if (_fireEff)
		if (_fireEff->isEnd()) {
			_fireEff->Kill();
			_fireCont.KillAllEffects();
			
		}
	if (_boomEff)
		if (_boomEff->isEnd()) {
			_boomEff->Kill();
			_boomCont.KillAllEffects();
			//--myeffcount;
		}
	for (unsigned int i = 0; i < _subFireBalls.size(); i++) {
		bool k = _subFireBalls[i]->TryKillEffects();
	}
	return true;
};

bool FireBall::CheckHit(std::vector<IPoint> & target) {
	bool hit = false;
	float distance = 0;
	unsigned int child = 0;
	while (!hit && child < target.size()) {
		distance = sqrt((target[child].x - _x)*(target[child].x - _x) + (target[child].y - _y)*(target[child].y - _y));
		++child;
		if (distance < 5 && !_isDead) {//проверка на столкновение с зар€дами фейерверка target
			_lifeTimer = -1;
			//_isDead = true;
			hit = true;
		}
	}
	
	if (!hit) {//если столкновени€ нет, проверка дл€ всех потомков
		for (unsigned int i = 0; i < _subFireBalls.size(); i++) {
			hit = _subFireBalls[i]->CheckHit(target);
		}
	}
	return hit;
};

std::vector<IPoint> FireBall::GetChildsPos() {
	std::vector<IPoint> result;
	if (!_isDead) {//если жив, добавл€ем позицию в вектор
		result.push_back(IPoint(round(_x), round(_y)));
		return result;
	}
	else {//если мертв, ищем позиции потомков и добавл€ем всех живых в вектор
		for (unsigned int i = 0; i < _subFireBalls.size(); i++) {
			std::vector<IPoint> childRes = _subFireBalls[i]->GetChildsPos();
			for (unsigned int j = 0; j < childRes.size(); j++) {
				result.push_back(childRes[j]);
			}
		}
		return result;
	}
};

float	FireBall::PosX() {
	return _x;
};

float	FireBall::PosY() {
	return _y;
};

IPoint FireBall::Pos() {
	return IPoint(round(_x), round(_y));
};

float	FireBall::SpeedX() {
	return _xSpeed;
};

float	FireBall::SpeedY() {
	return _ySpeed;
};

float	FireBall::Speed() {
	return _modSpeed;
};

bool	FireBall::IsDead() {
	return _isDead;
};

void FireBall::SetPosX(float x) {
	_x = x;
};
void FireBall::SetPosY(float y) {
	_y = y;
};

void FireBall::SetSpeedX(float sx) {
	_xSpeed = sx;
};

void FireBall::SetSpeedY(float sy) {
	_ySpeed = sy;
};

void FireBall::SetSpeed(float sy) {
	_modSpeed = sy;
};
float FireBall::MaxLightTimer() {
	float result = _lightTimer;
	for (unsigned int i = 0; i < _subFireBalls.size(); i++) {
		float tmp = _subFireBalls[i]->MaxLightTimer();
		if (tmp > result)
			result = tmp;
	}
	return result;

};