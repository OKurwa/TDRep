#include "stdafx.h"
#include "FireParent.h"
using namespace std;
using namespace rapidxml;

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


void FireParent::SetPosition(FPoint pos) {
	_position = pos;
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



NormalMissile::NormalMissile(NMissInfo inf) {
	_missileType = "Normal";
	_position = inf._position;
	_speed = FPoint(0, 0);
	_modSpeed = inf._modSpeed;
	if (inf._target && _modSpeed>0) {
		float d = _position.GetDistanceTo(inf._target->Position());
		_flyTime = d / _modSpeed;
		_targetPosition = inf._target->HitPosition(_flyTime);
	}
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_tex = nullptr;
	_damage = inf._damage;
	_target = inf._target;
	_missilePathX.Clear();
	_missilePathY.Clear();
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

void NormalMissile::LoadFromXml(std::string filename, int index) {
	_missileType = "Normal";
	_position = FPoint(0, 0);
	_targetPosition = FPoint(0, 0);
	_target = nullptr;
	

	try {
		file<> file(filename.c_str());
		// Может бросить исключение, если нет файла.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// Может бросить исключение, если xml испорчен.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "NormalTower") {
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					string id = missile->first_attribute("id")->value();
					if (utils::lexical_cast<int>(id) == index) {
						string value = missile->first_attribute("misSpeed")->value();
						_modSpeed = utils::lexical_cast<int>(value);
						value = missile->first_attribute("minDMG")->value();
						_damage.x = utils::lexical_cast<int>(value);
						value = missile->first_attribute("maxDMG")->value();
						_damage.y = utils::lexical_cast<int>(value);
						
					}
				}
				
				
				
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}
};

void NormalMissile::SetTarget(MonsterParent * target) {
	_target = target;
	if (target && _modSpeed>0) {
		float d = _position.GetDistanceTo(target->Position());
		_flyTime = d / _modSpeed;
		_targetPosition = target->HitPosition(_flyTime);
		_missileTimer = 0;
		_fly = true;
		_hit = false;
		MakePath();
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
/*
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
};*/

SlowMissile::SlowMissile(SlMissInfo inf, std::vector<MonsterParent::Ptr> & targets) {
	_missileType = "Slow";
	_position = inf._position;
	_speed = FPoint(0, 0);
	_modSpeed = inf._modSpeed;
	float d = _position.GetDistanceTo(inf._tPosition);
	_flyTime = d / _modSpeed;
	_targetPosition = inf._tPosition;
	
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_tex = nullptr;
	_damage = inf._damage;
	_targets = targets;
	_slow = inf._sFactor;
	_splashRange = inf._sRange;
	_missilePathX.Clear();
	_missilePathY.Clear();
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

void SlowMissile::LoadFromXml(std::string, int) {
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
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	if (target && _modSpeed>0) {
		float d = _position.GetDistanceTo(target->Position());
		_flyTime = d / _modSpeed;
		_targetPosition = target->HitPosition(_flyTime);
	}
	_missileTimer = 0;
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

void DecayMissile::LoadFromXml(std::string, int) {
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
/*
BashMissile::BashMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint bash, IPoint dmg, Render::TexturePtr tex) {
	_missileType = "Bash";
	_position = position;
	_speed = FPoint(0, 0);
	_modSpeed = mSpeed;
	if (target && _modSpeed>0) {
		float d = _position.GetDistanceTo(target->Position());
		_flyTime = d / _modSpeed;
		_targetPosition = target->HitPosition(_flyTime);
	}
	else {
		_flyTime = fTime;
		_targetPosition = target->HitPosition(fTime);
	}
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_tex = tex;
	_bash = bash;
	_target = target;
	_damage = dmg;
	MakePath();
};
*/
BashMissile::BashMissile(BMissInfo inf) {
	_missileType = "Bash";
	_position = inf._position;
	_speed = FPoint(0, 0);
	_modSpeed = inf._modSpeed;
	if (inf._target && _modSpeed>0) {
		float d = _position.GetDistanceTo(inf._target->Position());
		_flyTime = d / _modSpeed;
		_targetPosition = inf._target->HitPosition(_flyTime);
	}
	_missileTimer = 0;
	_fly = true;
	_hit = false;
	_bash = inf._bash;
	_target = inf._target;
	_damage = inf._damage;
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

void BashMissile::LoadFromXml(std::string, int) {
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

void SplashMissile::LoadFromXml(std::string, int) {
};


