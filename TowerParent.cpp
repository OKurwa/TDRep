#include "stdafx.h"

#include "FireParent.h"
//#include "MonsterParent.h"
#include "TowerParent.h"
//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс башни 				//
//----------------------------------------------//
//----------------------------------------------//

TowerParent::TowerParent() {
	_towerType = "name";
	_position= FPoint(0,0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime=0;
	_reloadTimer=0;
	_range=0;
	_missileSpeed=0;
	_missiles.clear();
	_tex = nullptr;
};

TowerParent::TowerParent(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "name";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_damage = dmg;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
};

TowerParent::~TowerParent() {
};


void TowerParent::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}else{
			Render::BeginColor(Color(255, 200, 100, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};
void TowerParent::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}
			
	}
};

bool TowerParent::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new NormalMissile(_position, _target, 0, 0.2, 0,_damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
	
};

std::vector<boost::intrusive_ptr<FireParent>> & TowerParent::GetMissiles() {
	return _missiles;
};

FPoint TowerParent::Position() {
	return _position;
};

IPoint TowerParent::Cell() {
	return _cell;
};

bool TowerParent::TakeAim(std::vector<boost::intrusive_ptr<MonsterParent>> * monsters) {
	if(_target == nullptr || _target->Dead()){
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters->size(); i++) {
			if (!(*monsters)[i].get()->Dead()) {
				FPoint tarPos = (*monsters)[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
			}
			
		}
		if (d <= _range && tarIndex < 9999) {
			_target = (*monsters)[tarIndex].get();
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
	else {
		FPoint tarPos = _target->Position();
		float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}
			
	}
	
	
};


//----------------------------------------------//
//----------------------------------------------//
//				Обычная башня	 				//
//----------------------------------------------//
//----------------------------------------------//

NormalTower::NormalTower() {
	_towerType = "Normal";
	_position = FPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime = 0;
	_reloadTimer = 0;
	_range = 0;
	_missileSpeed = 0;
	_missiles.clear();
	_tex = nullptr;
	_damage = IPoint(0, 0);
};

NormalTower::NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Normal";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
	_damage = dmg;
	
};

NormalTower::~NormalTower() {
};


void NormalTower::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else {
			Render::BeginColor(Color(255, 200, 100, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};


void NormalTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}

};
bool NormalTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new NormalMissile(_position, _target, 0, 0.2, 0, _damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Замедляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//

SlowTower::SlowTower() {
	_towerType = "Slow";
	_position = FPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime = 0;
	_reloadTimer = 0;
	_range = 0;
	_missileSpeed = 0;
	_missiles.clear();
	_tex = nullptr;
	_splashRange = 0;
	_slow = FPoint(0, 0);
	_damage = IPoint(0, 0);
	_targets = nullptr;
};
SlowTower::SlowTower(FPoint position, IPoint cell, std::vector<boost::intrusive_ptr<MonsterParent>> * targets, float rTime, float rTimer, int range, int sRange, FPoint sFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Slow";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
	_splashRange = sRange;
	_slow = sFactor;
	_damage = dmg;
	_targets = targets;
};
SlowTower::~SlowTower() {};


void SlowTower::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else {
			Render::BeginColor(Color(50, 50, 255, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};
void SlowTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}
};

bool SlowTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new SlowMissile(_position, _target->Position(), _targets, 0, 0.5, 0, _slow, 30, _damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

bool SlowTower::TakeAim(std::vector<boost::intrusive_ptr<MonsterParent>> * monsters) {
	if (_target == nullptr || _target->Dead()) {
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters->size(); i++) {
			if (!(*monsters)[i].get()->Dead()) {
				FPoint tarPos = (*monsters)[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
			}

		}
		if (d <= _range && tarIndex < 9999) {
			_target = (*monsters)[tarIndex].get();
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
	else {
		FPoint tarPos = _target->Position();
		float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Отравляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
DecayTower::DecayTower() {
	_towerType = "Decay";
	_position = FPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime = 0;
	_reloadTimer = 0;
	_range = 0;
	_missileSpeed = 0;
	_missiles.clear();
	_tex = nullptr;
	_decay = FPoint(0, 0);
	_damage = IPoint(0, 0);
};
DecayTower::DecayTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint dFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Decay";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
	_decay = dFactor;
	_damage = dmg;

};
DecayTower::~DecayTower() {};


void DecayTower::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else {
			Render::BeginColor(Color(50, 255, 50, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};
void DecayTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}
};

bool DecayTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new DecayMissile(_position,_target,0,0.1,0, _decay, _damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Оглушающая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
BashTower::BashTower() {
	_towerType = "Bash";
	_position = FPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime = 0;
	_reloadTimer = 0;
	_range = 0;
	_missileSpeed = 0;
	_missiles.clear();
	_tex = nullptr;
	_bash = FPoint(0, 0);
	_damage = IPoint(0, 0);
};
BashTower::BashTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint bash, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Bash";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
	_bash = bash;
	_damage = dmg;
};
BashTower::~BashTower() {};


void BashTower::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else {
			Render::BeginColor(Color(50, 50, 50, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};
void BashTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}
};

bool BashTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new BashMissile(_position, _target, 0, 0.3, 0,_bash, _damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};





//----------------------------------------------//
//----------------------------------------------//
//				Разрывная башня	 				//
//----------------------------------------------//
//----------------------------------------------//

SplashTower::SplashTower() {
	_towerType = "Splash";
	_position = FPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_reloadTime = 0;
	_reloadTimer = 0;
	_range = 0;
	_missileSpeed = 0;
	_missiles.clear();
	_tex = nullptr;
	_splashRange = 0;
	_targets = nullptr;
	_damage = IPoint(0,0);
	
};
SplashTower::SplashTower(FPoint position, IPoint cell, std::vector<boost::intrusive_ptr<MonsterParent>> * targets, float rTime, float rTimer, int range, int sRange, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Splash";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = tex;
	_splashRange = sRange;
	_targets = targets;
	_damage = dmg;
};
SplashTower::~SplashTower() {};


void SplashTower::Draw() {
	if (_tex) {
		_tex->Draw(_position);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else {
			Render::BeginColor(Color(255, 50, 50, 255));
		}
		//Render::BeginColor(Color(255, 200, 100, 255));
		Render::DrawRect(cRect);
		Render::EndColor();
		//Render::device.SetTexturing(true);

	}
	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}
};
void SplashTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<boost::intrusive_ptr<FireParent>>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}
};

bool SplashTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		boost::intrusive_ptr<FireParent> mis = new SplashMissile(_position, _target->Position(), _targets, 0, 0.5, 0, 90, _damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

bool SplashTower::TakeAim(std::vector<boost::intrusive_ptr<MonsterParent>> * monsters) {
	if (_target == nullptr || _target->Dead()) {
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters->size(); i++) {
			if (!(*monsters)[i].get()->Dead()) {
				FPoint tarPos = (*monsters)[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
			}

		}
		if (d <= _range && tarIndex < 9999) {
			_target = (*monsters)[tarIndex].get();
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
	else {
		FPoint tarPos = _target->Position();
		float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
};