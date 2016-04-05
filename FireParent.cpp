#include "stdafx.h"
#include "FireParent.h"


//----------------------------------------------//
//----------------------------------------------//
//			������� ����� �������				//
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
	
};

FireParent::FireParent(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, std::string mType, Render::TexturePtr tex) : ref_cnt_(0) {
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



	_missilePathX.Clear();
	_missilePathY.Clear();
	if (_flyTime == 0 && _missileTimer == 0 && _modSpeed > 0) {
		float distance = sqrt((_position.x - _targetPosition.x)*(_position.x - _targetPosition.x) + (_position.y - _targetPosition.y)*(_position.y - _targetPosition.y));
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


//----------------------------------------------//
//----------------------------------------------//
//				������� ������					//
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
};

NormalMissile::NormalMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, Render::TexturePtr tex) {
	
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

	_missilePathX.Clear();
	_missilePathY.Clear();
	if (_flyTime == 0 && _missileTimer == 0 && _modSpeed > 0) {
		float distance = sqrt((_position.x - _targetPosition.x)*(_position.x - _targetPosition.x) + (_position.y - _targetPosition.y)*(_position.y - _targetPosition.y));
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
	}
	else {
		_missileTimer += dt;
		_position.x = _missilePathX.getGlobalFrame(_missileTimer);
		_position.y = _missilePathY.getGlobalFrame(_missileTimer);
	}
};


//----------------------------------------------//
//----------------------------------------------//
//				����������� ������				//
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
	
};

SlowMissile::SlowMissile(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, float sFactor, int sRange, Render::TexturePtr tex) {
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
	_slowFactor = sFactor;
	_splashRange = sRange;
	_missilePathX.Clear();
	_missilePathY.Clear();
	if (_flyTime == 0 && _missileTimer == 0 && _modSpeed > 0) {
		float distance = sqrt((_position.x - _targetPosition.x)*(_position.x - _targetPosition.x) + (_position.y - _targetPosition.y)*(_position.y - _targetPosition.y));
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
//----------������----------//
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
	//������ ��������
	
	float distance = GetDistanceTo(endX, endY);
	speed = nSpeed;
	missileFlyTime = distance / speed;
	missileTimer = missileFlyTime;
	//���������� ����
	targetPosX = endX;
	targetPosY = endY;
	

	//������ ����������
	//��������� �����
	missilePathX.addKey(missileFlyTime, x);
	missilePathY.addKey(missileFlyTime, y);
	//�������� �����
	missilePathX.addKey(0, endX);
	missilePathY.addKey(0, endY);
	//������
	missilePathX.CalculateGradient();
	missilePathY.CalculateGradient();
};



Missile::~Missile() {
	missilePathX.Clear();
	missilePathY.Clear();
};


void Missile::Update(float dt) {
	//��� ������� �������
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
	//������ ��������
	
	float distance = GetDistanceTo(targetPosX, targetPosY);
	missileFlyTime = distance / speed;
	missileTimer = missileFlyTime;
	missilePathX.addKey(missileFlyTime, x);
	missilePathY.addKey(missileFlyTime, y);
	//�������� �����
	missilePathX.addKey(0, targetPosX);
	missilePathY.addKey(0, targetPosY);
	//������
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
//----------�����-----------//
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
	
	//��� ������� �����������
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


//��������� ���������� �����
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

//�������
bool Cannon::Shoot(float ltX, float ltY) {
	if (reloadTimer == 0) {
		
		//������ ��������
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





//������ � ������

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