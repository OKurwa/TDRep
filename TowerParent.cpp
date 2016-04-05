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

TowerParent::TowerParent(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, Render::TexturePtr tex) {
	_towerType = "name";
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	
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
		boost::intrusive_ptr<FireParent> mis = new NormalMissile(_position, _target, 0, 0.2, 0, "", nullptr);
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

bool TowerParent::TakeAim(std::vector<boost::intrusive_ptr<MonsterParent>> monsters) {
	if(_target == nullptr){
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters.size(); i++) {
			FPoint tarPos = monsters[i]->Position();
			float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
			if (tmpD<d) {
				d = tmpD;
				tarIndex = i;
			}
		}
		if (d <= _range && tarIndex < 9999) {
			_target = monsters[tarIndex].get();
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
		if(tmpD<=_range){
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
};

NormalTower::NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, Render::TexturePtr tex) {
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
		boost::intrusive_ptr<FireParent> mis = new NormalMissile(_position, _target, 0, 0.2, 0, "", nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};