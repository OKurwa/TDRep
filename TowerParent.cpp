#include "stdafx.h"

#include "FireParent.h"
//#include "MonsterParent.h"
#include "TowerParent.h"
using namespace std;
using namespace rapidxml;
//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс башни 				//
//----------------------------------------------//
//----------------------------------------------//

TowerParent::TowerParent() {
	//_towerType = "name";
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
	//_towerType = "name";
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

	if (_idleAnim && _atkAnim) {
		if (_atkAnim->IsFinished()) {
			Render::device.SetTexturing(true);

			_idleAnim->Draw(IPoint(math::round(_position.x) - 64, math::round(_position.y) - 40));
			
			Render::device.SetTexturing(false);
		}
		else if (!_atkAnim->IsFinished()) {
			Render::device.SetTexturing(true);
			_atkAnim->Draw(IPoint(math::round(_position.x) - 64, math::round(_position.y) - 40));
			Render::device.SetTexturing(false);
		}

	}
	
	else if (_tex) {
		IRect r = IRect(_position.x - 64, _position.y - 40, 128, 128);
		FRect tmp = FRect(0, 1 / 32.0, 0.125, 0.250);
		Render::device.SetTexturing(true);
		//_tex->TranslateUV(r, tmp);
		_tex->Draw(r, tmp);
		Render::device.SetTexturing(false);
	}
	else {
		IRect cRect = IRect(_position.x - 5, _position.y - 5, 11, 11);
		//Render::device.SetTexturing(false);
		if (_target) {
			Render::BeginColor(Color(255, 0, 0, 255));
		}
		else{
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
	if (_idleAnim  && _atkAnim->IsFinished()) {
		_idleAnim->Update(dt);
	}
	if (_atkAnim) {
		_atkAnim->Update(dt);
	}
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<FireParent::Ptr>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}
			
	}
};

void TowerParent::UpdateAnimAngle(MonsterParent * target) {
	if (target) {



		float angle = math::atan(target->Position().y - _position.y, target->Position().x - _position.x)*180.0 / math::PI;
		if (_idleAnim && _atkAnim) {
			_idleAnim->setLoop(false);
			_idleAnim->setPlayback(false);
			if (angle > -157.5 && angle <= -112.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a315.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a315.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a315.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a315.y);
			}

			if (angle > -112.5 && angle <= -67.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a270.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a270.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a270.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a270.y);
			}
			if (angle > -67.5 && angle <= -22.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a225.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a225.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a225.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a225.y);
			}

			if (angle > -22.5 && angle <= 22.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a180.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a180.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a180.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a180.y);
			}

			if (angle > 22.5 && angle <= 67.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a135.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a135.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a135.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a135.y);
			}

			if (angle > 67.5 && angle <= 112.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a90.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a90.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a90.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a90.y);
			}

			if (angle > 112.5 && angle <= 157.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a45.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a45.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a45.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a45.y);
			}
			if (angle > 157.5 || angle <= -157.5) {
				_atkAnim->setFirstPlayedFrame(_attackAnimAngles._a0.x);
				_atkAnim->setLastPlayedFrame(_attackAnimAngles._a0.y);
				_idleAnim->setFirstPlayedFrame(_idleAnimAngles._a0.x);
				_idleAnim->setLastPlayedFrame(_idleAnimAngles._a0.y);
			}
			_idleAnim->setCurrentFrame(0);
			_idleAnim->setLoop(true);
			_idleAnim->setPlayback(true);

			_atkAnim->setPlayback(true);

		}
	}
}


std::vector<FireParent::Ptr> & TowerParent::GetMissiles() {
	return _missiles;
};

FPoint TowerParent::Position() {
	return _position;
};

IPoint TowerParent::Cell() {
	return _cell;
};

void TowerParent::SetPosition(FPoint pos) {
	_position = pos;
	
};

void TowerParent::SetCell(IPoint cell) {
	_cell = cell;
};

bool TowerParent::TakeAim(std::vector<MonsterParent::Ptr> & monsters) {
	
	if(_target==nullptr || _target ->Finish() || _target->Dead()){
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters.size(); i++) {
			if (!monsters[i].get()->Dead()) {
				FPoint tarPos = monsters[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
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

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}
			
	}
	
	
};

bool TowerParent::TakeAimMiss(std::vector<MonsterParent::Ptr> & monsters) {
	return true;
};



void TowerParent::SetPrice(int p) {
	_price = p;
};

int	 TowerParent::Price() {
	return _price;
};

//----------------------------------------------//
//----------------------------------------------//
//				Обычная башня	 				//
//----------------------------------------------//
//----------------------------------------------//

NormalTower::NormalTower() {
	_towerType = TowerType::NORMAL;
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

NormalTower::NormalTower(NormalTower& proto) {
	*this = proto;
	if(proto._idleAnim)
		this->_idleAnim = proto._idleAnim->Clone();
	if (proto._atkAnim)
		this->_atkAnim = proto._atkAnim->Clone();
};

NormalTower::NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = TowerType::NORMAL;
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = Core::resourceManager.Get<Render::Texture>("Ant");
	_damage = dmg;
	
};

NormalTower::~NormalTower() {
};
/*
void NormalTower::Update(float dt) {
	if (_reloadTimer > 0)
		_reloadTimer -= dt;
	if (_reloadTimer < 0)
		_reloadTimer = 0;

	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Update(dt);
	}
	for (std::vector<FireParent::Ptr>::iterator it = _missiles.begin(); it != _missiles.end();) {
		if ((*it)->Hit()) {
			it = _missiles.erase(it);
		}
		else {
			it++;
		}

	}

};
*/

bool NormalTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		_missilesPrototypes[_lvl]._position=_position;
		_missilesPrototypes[_lvl]._target = _target;
		FireParent::Ptr mis = new NormalMissile(_missilesPrototypes[_lvl]);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

void NormalTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {
	
	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new NormalMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		
		if (_target) {

			UpdateAnimAngle(_target);
			
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}
	

}
void NormalTower::LoadFromXml(std::string filename) {
	_lvl = 0;
	_towerType = TowerType::NORMAL;
	_position = IPoint(0,0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();
	
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
				
				string value = tower->first_attribute("texture")->value();
				_tex = Core::resourceManager.Get<Render::Texture>(value);
				value = tower->first_attribute("price")->value();
				_price = utils::lexical_cast<int>(value);
				value = tower->first_attribute("reload")->value();
				_reloadTime = utils::lexical_cast<float>(value);
				_reloadTimer = 0;
				value = tower->first_attribute("range")->value();
				_range = utils::lexical_cast<int>(value);
				value = tower->first_attribute("lvlCount")->value();
				_lvlCount = utils::lexical_cast<int>(value);

				value = tower->first_attribute("idleAnimation")->value();
				_idleAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				
				_idleAnimAngles = IDL_ANGLES;
				/*
				_idleAnimAngles._a0 = IPoint(0,3);
				_idleAnimAngles._a45 = IPoint(32,35);
				_idleAnimAngles._a90 = IPoint(64,67);
				_idleAnimAngles._a135 = IPoint(96,99);
				_idleAnimAngles._a180 = IPoint(128,131);
				_idleAnimAngles._a225 = IPoint(160,163);
				_idleAnimAngles._a270 = IPoint(192,195);
				_idleAnimAngles._a315 = IPoint(224,227);
				*/
				value = tower->first_attribute("atkAnimation")->value();
				_atkAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				_atkAnim->setSpeed(_atkAnim->getSpeed()*_reloadTime);

				_attackAnimAngles = ATK_ANGLES;
				/*
				_attackAnimAngles._a0 = IPoint(12, 15);
				_attackAnimAngles._a45 = IPoint(44, 47);
				_attackAnimAngles._a90 = IPoint(76, 79);
				_attackAnimAngles._a135 = IPoint(108, 111);
				_attackAnimAngles._a180 = IPoint(140, 143);
				_attackAnimAngles._a225 = IPoint(172, 175);
				_attackAnimAngles._a270 = IPoint(204, 207);
				_attackAnimAngles._a315 = IPoint(236, 239);
				*/
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					string id = missile->first_attribute("id")->value();
					NormalMissile::NMissInfo info;
					info._target = nullptr;
					string value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
					
				}
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}
	


};
void NormalTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Замедляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//

SlowTower::SlowTower() {
	_towerType = TowerType::SLOW;
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
	//_targets = nullptr;
};

SlowTower::SlowTower(SlowTower& proto) {
	*this = proto;
	if (proto._idleAnim)
		this->_idleAnim = proto._idleAnim->Clone();
	if (proto._atkAnim)
		this->_atkAnim = proto._atkAnim->Clone();
};

SlowTower::SlowTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, FPoint sFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = TowerType::SLOW;
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = Core::resourceManager.Get<Render::Texture>("IceAnt");
	_splashRange = sRange;
	_slow = sFactor;
	_damage = dmg;
	_targets = targets;
};




SlowTower::~SlowTower() {};

bool SlowTower::Shoot() {
	if (_reloadTimer == 0 && _target) {

		_missilesPrototypes[_lvl]._position = _position;
		_missilesPrototypes[_lvl]._tPosition = _target->Position();
		FireParent::Ptr mis = new SlowMissile(_missilesPrototypes[_lvl], _targets);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};
void SlowTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new SlowMissile(_missilesPrototypes[_lvl], monsters);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void SlowTower::LoadFromXml(std::string filename) {

	_towerType = TowerType::SLOW;
	_position = IPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();

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
			if (id == "SlowTower") {

				string value = tower->first_attribute("texture")->value();
				_tex = Core::resourceManager.Get<Render::Texture>(value);
				value = tower->first_attribute("price")->value();
				_price = utils::lexical_cast<int>(value);
				value = tower->first_attribute("reload")->value();
				_reloadTime = utils::lexical_cast<float>(value);
				_reloadTimer = 0;
				value = tower->first_attribute("range")->value();
				_range = utils::lexical_cast<int>(value);
				value = tower->first_attribute("lvlCount")->value();
				_lvlCount = utils::lexical_cast<int>(value);

				value = tower->first_attribute("idleAnimation")->value();
				_idleAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();

				_idleAnimAngles = IDL_ANGLES;
				/*
				_idleAnimAngles._a0 = IPoint(0,3);
				_idleAnimAngles._a45 = IPoint(32,35);
				_idleAnimAngles._a90 = IPoint(64,67);
				_idleAnimAngles._a135 = IPoint(96,99);
				_idleAnimAngles._a180 = IPoint(128,131);
				_idleAnimAngles._a225 = IPoint(160,163);
				_idleAnimAngles._a270 = IPoint(192,195);
				_idleAnimAngles._a315 = IPoint(224,227);
				*/
				value = tower->first_attribute("atkAnimation")->value();
				_atkAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				_atkAnim->setSpeed(_atkAnim->getSpeed()*_reloadTime);

				_attackAnimAngles = ATK_ANGLES;
				/*
				_attackAnimAngles._a0 = IPoint(12, 15);
				_attackAnimAngles._a45 = IPoint(44, 47);
				_attackAnimAngles._a90 = IPoint(76, 79);
				_attackAnimAngles._a135 = IPoint(108, 111);
				_attackAnimAngles._a180 = IPoint(140, 143);
				_attackAnimAngles._a225 = IPoint(172, 175);
				_attackAnimAngles._a270 = IPoint(204, 207);
				_attackAnimAngles._a315 = IPoint(236, 239);
				*/
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					SlowMissile::SlMissInfo info;
					string id = missile->first_attribute("id")->value();
					
					value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);
					
					value = missile->first_attribute("splashRange")->value();
					info._sRange = utils::lexical_cast<int>(value);
					value = missile->first_attribute("slow")->value();
					info._sFactor.x = utils::lexical_cast<float>(value);
					value = missile->first_attribute("slowLenght")->value();
					info._sFactor.y = utils::lexical_cast<float>(value);
					_missilesPrototypes.push_back(info);
				}
				
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}



};



bool SlowTower::TakeAim(std::vector<MonsterParent::Ptr> & monsters) {
	_targets = monsters;
	if (_target == nullptr || _target->Finish() || _target->Dead()) {
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters.size(); i++) {
			if (!monsters[i].get()->Dead()) {
				FPoint tarPos = monsters[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
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

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
};

void SlowTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Отравляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
DecayTower::DecayTower() {
	_towerType = TowerType::DECAY;
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
DecayTower::DecayTower(DecayTower& proto) {
	*this = proto;
	if (proto._idleAnim)
		this->_idleAnim = proto._idleAnim->Clone();
	if (proto._atkAnim)
		this->_atkAnim = proto._atkAnim->Clone();
};

DecayTower::DecayTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint dFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = TowerType::DECAY;
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = Core::resourceManager.Get<Render::Texture>("PoisonAnt");
	_decay = dFactor;
	_damage = dmg;

};

DecayTower::~DecayTower() {};

bool DecayTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		_missilesPrototypes[_lvl]._position = _position;
		_missilesPrototypes[_lvl]._target = _target;
		FireParent::Ptr mis = new DecayMissile(_missilesPrototypes[_lvl]);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

void DecayTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new DecayMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void DecayTower::LoadFromXml(std::string filename) {

	_towerType = TowerType::DECAY;
	_position = IPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();

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
			if (id == "DecayTower") {

				string value = tower->first_attribute("texture")->value();
				_tex = Core::resourceManager.Get<Render::Texture>(value);
				value = tower->first_attribute("price")->value();
				_price = utils::lexical_cast<int>(value);
				value = tower->first_attribute("reload")->value();
				_reloadTime = utils::lexical_cast<float>(value);
				_reloadTimer = 0;
				value = tower->first_attribute("range")->value();
				_range = utils::lexical_cast<int>(value);
				value = tower->first_attribute("lvlCount")->value();
				_lvlCount = utils::lexical_cast<int>(value);

				value = tower->first_attribute("idleAnimation")->value();
				_idleAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();

				_idleAnimAngles = IDL_ANGLES;
				
				value = tower->first_attribute("atkAnimation")->value();
				_atkAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				_atkAnim->setSpeed(_atkAnim->getSpeed()*_reloadTime);

				_attackAnimAngles = ATK_ANGLES;
				
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					DecayMissile::DMissInfo info;
					info._target = nullptr;
					string id = missile->first_attribute("id")->value();

					value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);

					value = missile->first_attribute("decay")->value();
					info._decay.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("decayLenght")->value();
					info._decay.y = utils::lexical_cast<float>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}



};

void DecayTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Оглушающая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
BashTower::BashTower() {
	_towerType = TowerType::BASH;
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

BashTower::BashTower(BashTower& proto) {
	*this = proto;
	if (proto._idleAnim)
		this->_idleAnim = proto._idleAnim->Clone();
	if (proto._atkAnim)
		this->_atkAnim = proto._atkAnim->Clone();
};

BashTower::BashTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint bash, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = TowerType::BASH;
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = Core::resourceManager.Get<Render::Texture>("StunAnt");
	_bash = bash;
	_damage = dmg;
};

BashTower::~BashTower() {};

bool BashTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		_missilesPrototypes[_lvl]._position = _position;
		_missilesPrototypes[_lvl]._target = _target;
		FireParent::Ptr mis = new BashMissile(_missilesPrototypes[_lvl]);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};

void BashTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new BashMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void BashTower::LoadFromXml(std::string filename) {

	_towerType = TowerType::BASH;
	_position = IPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();

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
			if (id == "BashTower") {

				string value = tower->first_attribute("texture")->value();
				_tex = Core::resourceManager.Get<Render::Texture>(value);
				value = tower->first_attribute("price")->value();
				_price = utils::lexical_cast<int>(value);
				value = tower->first_attribute("reload")->value();
				_reloadTime = utils::lexical_cast<float>(value);
				_reloadTimer = 0;
				value = tower->first_attribute("range")->value();
				_range = utils::lexical_cast<int>(value);
				value = tower->first_attribute("lvlCount")->value();
				_lvlCount = utils::lexical_cast<int>(value);

				value = tower->first_attribute("idleAnimation")->value();
				_idleAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();

				_idleAnimAngles = IDL_ANGLES;
				
				value = tower->first_attribute("atkAnimation")->value();
				_atkAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				_atkAnim->setSpeed(_atkAnim->getSpeed()*_reloadTime);

				_attackAnimAngles = ATK_ANGLES;
				
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					BashMissile::BMissInfo info;
					info._target = nullptr;
					string id = missile->first_attribute("id")->value();

					value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);
					value = missile->first_attribute("bashChance")->value();
					info._bash.x = utils::lexical_cast<float>(value);
					value = missile->first_attribute("bashLenght")->value();
					info._bash.y = utils::lexical_cast<float>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}



};


void BashTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
};

//----------------------------------------------//
//----------------------------------------------//
//				Разрывная башня	 				//
//----------------------------------------------//
//----------------------------------------------//

SplashTower::SplashTower() {
	_towerType = TowerType::SPLASH;
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
	//_targets = nullptr;
	_damage = IPoint(0,0);
	
};

SplashTower::SplashTower(SplashTower& proto) {
	*this = proto;
	if (proto._idleAnim)
		this->_idleAnim = proto._idleAnim->Clone();
	if (proto._atkAnim)
		this->_atkAnim = proto._atkAnim->Clone();
};

SplashTower::SplashTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = TowerType::SPLASH;
	_position = position;
	_cell = cell;
	_target = nullptr;
	_reloadTime = rTime;
	_reloadTimer = rTimer;
	_range = range;
	_missileSpeed = mSpeed;
	_missiles.clear();
	_tex = Core::resourceManager.Get<Render::Texture>("FireAnt");
	_splashRange = sRange;
	_targets = targets;
	_damage = dmg;
};

SplashTower::~SplashTower() {};

bool SplashTower::Shoot() {
	if (_reloadTimer == 0 && _target) {
		_missilesPrototypes[_lvl]._position = _position;
		_missilesPrototypes[_lvl]._tPosition = _target->Position();
		FireParent::Ptr mis = new SplashMissile(_missilesPrototypes[_lvl], _targets);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
};
void SplashTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new SplashMissile(_missilesPrototypes[_lvl],monsters);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

bool SplashTower::TakeAim(std::vector<MonsterParent::Ptr> & monsters) {
	_targets = monsters;
	
	if (_target == nullptr || _target->Finish() || _target->Dead()) {
		float d = 9999;
		int tarIndex = 9999;
		for (int i = 0; i < monsters.size(); i++) {
			if (!monsters[i].get()->Dead()) {
				FPoint tarPos = monsters[i]->Position();
				float tmpD = sqrt((tarPos.x - _position.x)*(tarPos.x - _position.x) + (tarPos.y - _position.y)*(tarPos.y - _position.y));
				if (tmpD<d) {
					d = tmpD;
					tarIndex = i;
				}
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

		if (tmpD <= _range) {
			return true;
		}
		else {
			_target = nullptr;
			return false;
		}

	}
};


void SplashTower::LoadFromXml(std::string filename) {

	_towerType = TowerType::SPLASH;
	_position = IPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();

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
			if (id == "SplashTower") {

				string value = tower->first_attribute("texture")->value();
				_tex = Core::resourceManager.Get<Render::Texture>(value);
				value = tower->first_attribute("price")->value();
				_price = utils::lexical_cast<int>(value);
				value = tower->first_attribute("reload")->value();
				_reloadTime = utils::lexical_cast<float>(value);
				_reloadTimer = 0;
				value = tower->first_attribute("range")->value();
				_range = utils::lexical_cast<int>(value);
				value = tower->first_attribute("lvlCount")->value();
				_lvlCount = utils::lexical_cast<int>(value);

				value = tower->first_attribute("idleAnimation")->value();
				_idleAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();

				_idleAnimAngles = IDL_ANGLES;
				
				value = tower->first_attribute("atkAnimation")->value();
				_atkAnim = Core::resourceManager.Get<Render::Animation>(value)->Clone();
				_atkAnim->setSpeed(_atkAnim->getSpeed()*_reloadTime);

				_attackAnimAngles = ATK_ANGLES;
				
				for (xml_node<>* missile = tower->first_node("Missile"); missile; missile = missile->next_sibling("Missile")) {
					SplashMissile::SpMissInfo info;
					string id = missile->first_attribute("id")->value();

					value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);

					value = missile->first_attribute("splashRange")->value();
					info._sRange = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}


	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}



};

void SplashTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
};