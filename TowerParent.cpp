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
/*
bool TowerParent::Shoot() {
	if (_reloadTimer == 0 && _target) {
		FireParent::Ptr mis = new NormalMissile(_position, _target, 0, 0.2, 0,_damage, nullptr);
		_missiles.push_back(mis);
		_reloadTimer = _reloadTime;
		return true;
	}
	else {
		return false;
	}
	
};
*/
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
	_tex = Core::resourceManager.Get<Render::Texture>("Ant");
	_damage = dmg;
	
};

NormalTower::~NormalTower() {
};

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

			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}
	

}
void NormalTower::LoadFromXml(std::string filename) {
	_lvl = 0;
	_towerType = "Normal";
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
	//_targets = nullptr;
};

SlowTower::SlowTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, FPoint sFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Slow";
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

void SlowTower::Update(float dt) {
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
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void SlowTower::LoadFromXml(std::string filename) {

	_towerType = "Slow";
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
	_tex = Core::resourceManager.Get<Render::Texture>("PoisonAnt");
	_decay = dFactor;
	_damage = dmg;

};

DecayTower::~DecayTower() {};

void DecayTower::Update(float dt) {
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
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void DecayTower::LoadFromXml(std::string filename) {

	_towerType = "Decay";
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
	_tex = Core::resourceManager.Get<Render::Texture>("StunAnt");
	_bash = bash;
	_damage = dmg;
};

BashTower::~BashTower() {};

void BashTower::Update(float dt) {
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
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;

		}

	}


}

void BashTower::LoadFromXml(std::string filename) {

	_towerType = "Bash";
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
	//_targets = nullptr;
	_damage = IPoint(0,0);
	
};

SplashTower::SplashTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, int mSpeed, IPoint dmg, Render::TexturePtr tex) {
	_towerType = "Splash";
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

void SplashTower::Update(float dt) {
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

	_towerType = "Splash";
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