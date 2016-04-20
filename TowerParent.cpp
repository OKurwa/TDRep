#include "stdafx.h"

#include "FireParent.h"
//#include "MonsterParent.h"
#include "TowerParent.h"
using namespace std;
using namespace rapidxml;
//----------------------------------------------//
//----------------------------------------------//
//			������� ����� ����� 				//
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
};

TowerParent::~TowerParent() {
};

void TowerParent::Draw() {


	for (int i = 0; i < _missiles.size(); i++) {
		_missiles[i]->Draw();
	}

	




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
	
	//else if (_tex) {
	//	IRect r = IRect(_position.x - 64, _position.y - 40, 128, 128);
	//	FRect tmp = FRect(0, 1 / 32.0, 0.125, 0.250);
	//	Render::device.SetTexturing(true);
	//	//_tex->TranslateUV(r, tmp);
	//	_tex->Draw(r, tmp);
	//	Render::device.SetTexturing(false);
	//}
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
	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(IPoint(_position.x-32,_position.y-32), utils::lexical_cast(_lvl+1), 1.00f, LeftAlign, BottomAlign);
	Render::EndColor();
	
};

void TowerParent::UpgradeDraw() {

	//���������

	if (_hint && _texHint) {
		IRect rect = IRect(_position.x - 120, _position.y - 32 - 128, 240, 128);
		if (_position.x - 120 < 0) {
			rect.x = 0;
		}
		if (_position.x + 120 > 768) {
			rect.x = 528;
		}

		if (_position.y - 32 - 128 < 0) {
			rect.y = _position.y + 32;
		}

		FRect uv = FRect(0, 1, 0, 1);
		Render::BeginAlphaMul(0.85);
		_texHint->Draw(rect, uv);
		Render::EndAlphaMul();

		DrawHintText(rect);


	}
	//������ ��������
	if (_showUpgradeButton && _lvl < _lvlCount) {
		Render::device.SetTexturing(false);
		Render::BeginAlphaMul(0.5);
		Render::BeginColor(Color(0, 0, 0, 255));
		Render::DrawRect(_upgradeButtonRect);
		Render::EndColor();
		Render::EndAlphaMul();
		Render::device.SetTexturing(true);
		if (_upTex)
			if (_curGold < UpgradePrice()) {
				
				_upTex->Draw(_upgradeButtonRect, FRect(0, 1, 0, 1));
			}
			else {
				_upTex->Draw(_upgradeButtonRect, FRect(0, 1, 0, 1));
			}
			
		if (UpgradePrice() > 0) {


			if (_curGold < UpgradePrice()) {
				Render::BindFont("arial");
				Render::BeginColor(Color(255, 0, 0, 255));
				Render::PrintString(_upgradeButtonRect.RightBottom(), utils::lexical_cast(UpgradePrice()), 1.00f, RightAlign, BottomAlign);
				Render::EndColor();
			}
			else {
				Render::BindFont("arial");
				Render::BeginColor(Color(255, 255, 255, 255));
				Render::PrintString(_upgradeButtonRect.RightBottom(), utils::lexical_cast(UpgradePrice()), 1.00f, RightAlign, BottomAlign);
				Render::EndColor();
			}
			
		}
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

	IPoint buttonPos = IPoint(_position.x, _position.y + 32);
	int width = 100;
	int height = 100;
	if (_position.y + height + 32 > 768) {
		buttonPos.y = _position.y - height - 32;
	}
	if (_position.x + width > 768) {
		buttonPos.x = 768 - width;
	}
	if (_position.x < 0) {
		buttonPos.x = 0;
	}
	//_upgradeButtonRect = IRect(buttonPos, width, height);
};

void TowerParent::SetUButtonPosition() {
	IPoint buttonPos = IPoint(_position.x - 20, _position.y + 20);
	int width = 40;
	int height = 40;
	if (_position.y + height + 20 > 768) {
		buttonPos.y = _position.y - height - 20;
	}
	if (_position.x -20 + width > 768) {
		buttonPos.x = 768 - width;
	}
	if (_position.x - 20 < 0) {
		buttonPos.x = 0;
	}
	_upgradeButtonRect = IRect(buttonPos, width, height);
	_upTex = Core::resourceManager.Get<Render::Texture>("Up");
};

void TowerParent::SetCell(IPoint cell) {
	_cell = cell;
};

void TowerParent::SetUpgradeButton(bool b) {
	_showUpgradeButton = b;
};

bool TowerParent::UpgradeButtonActive() {
	return _showUpgradeButton;
};


IRect TowerParent::UpgradeIRect() {
	return _upgradeButtonRect;
};

void TowerParent::Upgrade() {
	
	if (_lvl < _lvlCount - 1) {
		_lvl++;
		MM::manager.PlaySample("Up");
	}
};

void TowerParent::SetPrice(int p) {
	_price = p;
};

int	 TowerParent::Price() {
	return _price;
};

void  TowerParent::SetCurGold(int g) {
	_curGold = g;
};

void TowerParent::SetHint(IPoint pos) {
	IRect r = IRect(_position.x-32, _position.y-32, 64, 64);
	if (r.Contains(pos)) {
		_hint = true;
	}
	else {
		_hint = false;
	}
};






//----------------------------------------------//
//----------------------------------------------//
//				������� �����	 				//
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
	_texHint = nullptr;
	_damage = IPoint(0, 0);
	_showUpgradeButton = false;
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_damage = dmg;
	_showUpgradeButton = false;
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
/*
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
*/
void NormalTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {
	
	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new NormalMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		
		if (_target) {

			UpdateAnimAngle(_target);
			
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;
			MM::manager.PlaySample("Shoot");
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
		// ����� ������� ����������, ���� ��� �����.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// ����� ������� ����������, ���� xml ��������.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "NormalTower") {
				
				string value = tower->first_attribute("texture")->value();
				//_tex = Core::resourceManager.Get<Render::Texture>(value);
				_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
					string id = missile->first_attribute("id")->value();
					NormalMissile::NMissInfo info;
					info._target = nullptr;
					string value = missile->first_attribute("misSpeed")->value();
					info._modSpeed = utils::lexical_cast<int>(value);
					value = missile->first_attribute("minDMG")->value();
					info._damage.x = utils::lexical_cast<int>(value);
					value = missile->first_attribute("maxDMG")->value();
					info._damage.y = utils::lexical_cast<int>(value);
					value = missile->first_attribute("price")->value();
					info._price = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
					
				}
			}
		}

		_price = _missilesPrototypes[0]._price;
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
	SetUButtonPosition();
};
int  NormalTower::UpgradePrice() {
	if (_lvl < _lvlCount - 1) {
		return _missilesPrototypes[_lvl + 1]._price;
	}
	else {
		return -1;
	}
};
void NormalTower::DrawHintText(IRect rect) {

	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(FPoint(rect.x + 120, rect.y + 105), "Normal tower: "+ utils::lexical_cast(_lvl + 1) +" lvl", 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 90), "Damage : " + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.x) + "-" + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.y), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 75), "Range : " + utils::lexical_cast(_range), 1.0f, CenterAlign, BottomAlign);
	//Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Splash : " + utils::lexical_cast(0), 1.0f, CenterAlign, BottomAlign);
	if (_lvl<_lvlCount - 1)
		Render::PrintString(FPoint(rect.x + 120, rect.y + 30), "Upgrade cost : " + utils::lexical_cast(_missilesPrototypes[_lvl + 1]._price), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 15), "Destroy returns : " + utils::lexical_cast(_missilesPrototypes[_lvl]._price*0.75), 1.0f, CenterAlign, BottomAlign);
	Render::EndColor();
};
//----------------------------------------------//
//----------------------------------------------//
//				����������� �����	 			//
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_splashRange = 0;
	_slow = FPoint(0, 0);
	_damage = IPoint(0, 0);
	//_targets = nullptr;
	_showUpgradeButton = false;
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_splashRange = sRange;
	_slow = sFactor;
	_damage = dmg;
	_targets = targets;
	_showUpgradeButton = false;
};




SlowTower::~SlowTower() {};


void SlowTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new SlowMissile(_missilesPrototypes[_lvl], monsters);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;
			MM::manager.PlaySample("Shoot");
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
		// ����� ������� ����������, ���� ��� �����.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// ����� ������� ����������, ���� xml ��������.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "SlowTower") {

				string value = tower->first_attribute("texture")->value();
				_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
					value = missile->first_attribute("price")->value();
					info._price = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
				}
				
			}
		}

		_price = _missilesPrototypes[0]._price;
	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}



};





void SlowTower::SetPosition(FPoint pos) {
	_position = pos;
	for (unsigned int i = 0; i < _missilesPrototypes.size(); i++) {
		_missilesPrototypes[i]._position = pos;
	}
	SetUButtonPosition();
};

int  SlowTower::UpgradePrice() {
	if (_lvl < _lvlCount - 1) {
		return _missilesPrototypes[_lvl + 1]._price;
	}
	else {
		return -1;
	}
};

void SlowTower::DrawHintText(IRect rect) {

	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(FPoint(rect.x + 120, rect.y + 105), "Slow tower: " + utils::lexical_cast(_lvl + 1) + " lvl", 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 90), "Damage : " + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.x) + "-" + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.y), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 75), "Range : " + utils::lexical_cast(_range), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Splash : " + utils::lexical_cast(_missilesPrototypes[_lvl]._sRange), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 45), "Slow target : " + utils::lexical_cast((int)(_missilesPrototypes[_lvl]._sFactor.x * 100))+"\\% on "+ utils::lexical_cast(_missilesPrototypes[_lvl]._sFactor.y)+" sec", 1.0f, CenterAlign, BottomAlign);

	
	if (_lvl<_lvlCount - 1)
		Render::PrintString(FPoint(rect.x + 120, rect.y + 30), "Upgrade cost : " + utils::lexical_cast(_missilesPrototypes[_lvl + 1]._price), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 15), "Destroy returns : " + utils::lexical_cast(_missilesPrototypes[_lvl]._price*0.75), 1.0f, CenterAlign, BottomAlign);
	Render::EndColor();
};

//----------------------------------------------//
//----------------------------------------------//
//				����������� �����	 			//
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_decay = FPoint(0, 0);
	_damage = IPoint(0, 0);
	_showUpgradeButton = false;
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_decay = dFactor;
	_damage = dmg;
	_showUpgradeButton = false;
};

DecayTower::~DecayTower() {};


void DecayTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new DecayMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;
			MM::manager.PlaySample("Shoot");
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
		// ����� ������� ����������, ���� ��� �����.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// ����� ������� ����������, ���� xml ��������.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "DecayTower") {

				string value = tower->first_attribute("texture")->value();
				_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
					value = missile->first_attribute("price")->value();
					info._price = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}

		_price = _missilesPrototypes[0]._price;
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
	SetUButtonPosition();
};


int  DecayTower::UpgradePrice() {
	if (_lvl < _lvlCount - 1) {
		return _missilesPrototypes[_lvl + 1]._price;
	}
	else {
		return -1;
	}
};


void DecayTower::DrawHintText(IRect rect) {

	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(FPoint(rect.x + 120, rect.y + 105), "Poison tower: " + utils::lexical_cast(_lvl + 1) + " lvl", 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 90), "Damage : " + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.x) + "-" + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.y), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 75), "Range : " + utils::lexical_cast(_range), 1.0f, CenterAlign, BottomAlign);
	//Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Splash : " + utils::lexical_cast(_missilesPrototypes[_lvl]._sRange), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Poison : Deals " + utils::lexical_cast(_missilesPrototypes[_lvl]._decay.x) + " DPS on " + utils::lexical_cast(_missilesPrototypes[_lvl]._decay.y) + " sec", 1.0f, CenterAlign, BottomAlign);


	if (_lvl<_lvlCount - 1)
		Render::PrintString(FPoint(rect.x + 120, rect.y + 30), "Upgrade cost : " + utils::lexical_cast(_missilesPrototypes[_lvl + 1]._price), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 15), "Destroy returns : " + utils::lexical_cast(_missilesPrototypes[_lvl]._price*0.75), 1.0f, CenterAlign, BottomAlign);
	Render::EndColor();
};

//----------------------------------------------//
//----------------------------------------------//
//				���������� �����	 			//
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_bash = FPoint(0, 0);
	_damage = IPoint(0, 0);
	_showUpgradeButton = false;
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_bash = bash;
	_damage = dmg;
	_showUpgradeButton = false;
};

BashTower::~BashTower() {};



void BashTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new BashMissile(_missilesPrototypes[_lvl]);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;
			MM::manager.PlaySample("Shoot");
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
		// ����� ������� ����������, ���� ��� �����.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// ����� ������� ����������, ���� xml ��������.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "BashTower") {

				string value = tower->first_attribute("texture")->value();
				_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
					value = missile->first_attribute("price")->value();
					info._price = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}

		_price = _missilesPrototypes[0]._price;
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
	SetUButtonPosition();
};


int  BashTower::UpgradePrice() {
	if (_lvl < _lvlCount - 1) {
		return _missilesPrototypes[_lvl + 1]._price;
	}
	else {
		return -1;
	}
};
void BashTower::DrawHintText(IRect rect) {

	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(FPoint(rect.x + 120, rect.y + 105), "Bash tower: " + utils::lexical_cast(_lvl + 1) + " lvl", 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 90), "Damage : " + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.x) + "-" + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.y), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 75), "Range : " + utils::lexical_cast(_range), 1.0f, CenterAlign, BottomAlign);
	//Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Splash : " + utils::lexical_cast(_missilesPrototypes[_lvl]._sRange), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Bash :"+ utils::lexical_cast((int)(_missilesPrototypes[_lvl]._bash.x*100)) + " \\% on " + utils::lexical_cast(_missilesPrototypes[_lvl]._bash.y) + " sec", 1.0f, CenterAlign, BottomAlign);
	

	if (_lvl<_lvlCount - 1)
		Render::PrintString(FPoint(rect.x + 120, rect.y + 30), "Upgrade cost : " + utils::lexical_cast(_missilesPrototypes[_lvl + 1]._price), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 15), "Destroy returns : " + utils::lexical_cast(_missilesPrototypes[_lvl]._price*0.75), 1.0f, CenterAlign, BottomAlign);
	Render::EndColor();
};

//----------------------------------------------//
//----------------------------------------------//
//				��������� �����	 				//
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_splashRange = 0;
	//_targets = nullptr;
	_damage = IPoint(0,0);
	_showUpgradeButton = false;
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
	_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
	_splashRange = sRange;
	_targets = targets;
	_damage = dmg;
	_showUpgradeButton = false;
};

SplashTower::~SplashTower() {};


void SplashTower::TryShoot(std::vector<MonsterParent::Ptr> & monsters) {

	if (_reloadTimer == 0) {
		FireParent::Ptr mis = new SplashMissile(_missilesPrototypes[_lvl],monsters);
		_target = mis->TakeAim(monsters, _target, _range).get();
		if (_target) {
			UpdateAnimAngle(_target);
			_missiles.push_back(mis);
			_reloadTimer = _reloadTime;
			MM::manager.PlaySample("Shoot");

		}

	}


}




void SplashTower::LoadFromXml(std::string filename) {

	_towerType = TowerType::SPLASH;
	_position = IPoint(0, 0);
	_cell = IPoint(0, 0);
	_target = nullptr;
	_missiles.clear();

	try {
		file<> file(filename.c_str());
		// ����� ������� ����������, ���� ��� �����.

		xml_document<> doc;
		doc.parse<0>(file.data());
		// ����� ������� ����������, ���� xml ��������.

		xml_node<>* game = doc.first_node();
		if (!game) { Assert(false); throw runtime_error("No root node"); }

		xml_node<>* towers = game->first_node("Towers");
		for (xml_node<>* tower = towers->first_node("Tower"); tower; tower = tower->next_sibling("Tower")) {
			string id = tower->first_attribute("id")->value();
			if (id == "SplashTower") {

				string value = tower->first_attribute("texture")->value();
				_texHint = Core::resourceManager.Get<Render::Texture>("Hint");
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
					value = missile->first_attribute("price")->value();
					info._price = utils::lexical_cast<int>(value);
					_missilesPrototypes.push_back(info);
				}
			}
		}

		_price = _missilesPrototypes[0]._price;
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
	SetUButtonPosition();
};

int  SplashTower::UpgradePrice() {
	if (_lvl < _lvlCount - 1) {
		return _missilesPrototypes[_lvl + 1]._price;
	}
	else {
		return -1;
	}
};

void SplashTower::DrawHintText(IRect rect) {

	Render::BindFont("arial");
	Render::BeginColor(Color(255, 255, 255, 255));
	Render::PrintString(FPoint(rect.x + 120, rect.y + 105), "Splash tower: " + utils::lexical_cast(_lvl+1) + " lvl", 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 90), "Damage : " + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.x) + "-" + utils::lexical_cast(_missilesPrototypes[_lvl]._damage.y), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 75), "Range : " + utils::lexical_cast(_range), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Splash : " + utils::lexical_cast(_missilesPrototypes[_lvl]._sRange), 1.0f, CenterAlign, BottomAlign);
	//Render::PrintString(FPoint(rect.x + 120, rect.y + 60), "Bash :" + utils::lexical_cast(_missilesPrototypes[_lvl]._bash.x * 100) + " % on " + utils::lexical_cast(_missilesPrototypes[_lvl]._bash.y) + " sec", 1.0f, CenterAlign, BottomAlign);


	if (_lvl<_lvlCount - 1)
		Render::PrintString(FPoint(rect.x + 120, rect.y + 30), "Upgrade cost : " + utils::lexical_cast(_missilesPrototypes[_lvl + 1]._price), 1.0f, CenterAlign, BottomAlign);
	Render::PrintString(FPoint(rect.x + 120, rect.y + 15), "Destroy returns : " + utils::lexical_cast(_missilesPrototypes[_lvl]._price*0.75), 1.0f, CenterAlign, BottomAlign);
	Render::EndColor();
};