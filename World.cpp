#include "stdafx.h"
#include "World.h"


World::World() {};
World::~World() {};


void World::Init(	int gold,
			int attacksRemaining,
			int nextAttackMonstersCount,
			int monstersRemaining,
			int lives,
			MonsterAttack & attacks) {
	_gold = gold;
	_attacksRemaining = attacksRemaining;
	_nextAttackMonstersCount = nextAttackMonstersCount;
	_monstersRemaining = monstersRemaining;
	_lives = lives;
	_curAttackIndex = 0;
	//_attacks = attacks;
	if (attacks.GetAttack().size() >= 2) {
		_curAttackName = attacks.GetAttack()[_curAttackIndex].Name();
		_nextAttackName =attacks.GetAttack()[_curAttackIndex + 1].Name();
		_curAttackType = attacks.GetAttack()[_curAttackIndex].Type();
		_nextAttackType = attacks.GetAttack()[_curAttackIndex + 1].Type();
	}
	else if (attacks.GetAttack().size() == 1) {
		_curAttackName = attacks.GetAttack()[_curAttackIndex].Name();
		_nextAttackName = "None";
		_curAttackType = attacks.GetAttack()[_curAttackIndex].Type();
		_nextAttackType = "None";
	}
	else {
		_curAttackName = "None";
		_nextAttackName = "None";
		_curAttackType = "None";
		_nextAttackType = "None";
	}
	
	_state = START;
};

std::string World::CurAttackName() {
	return _curAttackName;
};

std::string World::NextAttackName() {
	return _nextAttackName;
};

std::string World::CurAttackType() {
	return _curAttackType;
};

std::string World::NextAttackType() {
	return _nextAttackType;
};

int World::Gold() {
	return _gold;
};

WorldState World::State() {
	return _state;
};

void World::SetState(WorldState state) {
	_state = state;
};

void World::SetAttackIndex(int index) {
	_curAttackIndex = index;
};

void World::SetNewAttack(float delay, Attack & atk) {
	
	if(_attacksRemaining>0){
		_state = DELAY;
		_delayTimer = delay;
		GoldAdd(atk.WGold());
		++_curAttackIndex;
		--_attacksRemaining;
	}
	else{
		if(_lives > 0)
			_state = WIN;
	}
	
	_curAttackName = atk.Name();
	_curAttackType = atk.Type();
	_maxHP = atk.MaxHp();
	_speed = atk.Speed();
	_gpm = atk.MGold();
		
};

void World::Draw() {

	if (_state == LOSE) {
		Render::device.SetTexturing(false);
		Render::BeginAlphaMul(0.5);
		Render::BeginColor(Color(0, 0, 0, 255));
		Render::DrawRect(0,0,1024,768);
		Render::EndColor();
		Render::EndAlphaMul();
		Render::device.SetTexturing(true);
		
	}
	else {
		Render::BindFont("arial");
		Render::device.SetTexturing(true);

		Render::BeginColor(Color(255, 255, 255, 255));
		Render::PrintString(FPoint(838, 680), "HUNGRY ANTS TD", 1.00f, LeftAlign, BottomAlign);

		Render::PrintString(FPoint(828, 600), "HP : " + utils::lexical_cast(_lives), 1.00f, LeftAlign, BottomAlign);
		Render::PrintString(FPoint(828, 580), "WAVE BEGINS IN : " + utils::lexical_cast(math::round(_delayTimer)) + "sec", 1.00f, LeftAlign, BottomAlign);
		Render::PrintString(FPoint(828, 560), "WAVES REMAINING : " + utils::lexical_cast(_attacksRemaining), 1.00f, LeftAlign, BottomAlign);
		Render::PrintString(FPoint(896, 520), "CURRENT WAVE : " + utils::lexical_cast(_curAttackIndex), 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 505), "Name : " + _curAttackName, 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 490), "Type: " + _curAttackType, 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 475), "Monster HP : " + utils::lexical_cast(_maxHP), 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 460), "Monster speed : " + utils::lexical_cast(_speed), 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 445), "Meat per monster : " + utils::lexical_cast(_gpm), 1.00f, CenterAlign, BottomAlign);

		Render::PrintString(FPoint(896, 335), "MEAT", 1.00f, CenterAlign, BottomAlign);
		Render::PrintString(FPoint(896, 300), utils::lexical_cast(_gold), 1.00f, CenterAlign, BottomAlign);
		Render::EndColor();
		Render::device.SetTexturing(false);
	}
	
};

void World::Update(float dt) {
	
	if (_lives <= 0)
		_state = LOSE;
	if (_state == DELAY) {
		if (_delayTimer > 0) {
			_delayTimer -= dt;
		}
		else {
			_delayTimer = 0;
			_state = WAVE;
		}
	}

};
void World::GoldAdd(int gold) {
	_gold += gold;
};

bool World::GoldSpend(int gold) {
	if (gold > _gold) {
		return false;
	}
	else{
		_gold -= gold;
		return true;
	}
};

void World::LoseLife() {
	_lives--;
};
