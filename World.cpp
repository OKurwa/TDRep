#include "stdafx.h"
#include "World.h"


World::World() {};
World::~World() {};


void World::Init(	int gold,
			int attacksRemaining,
			int nextAttackMonstersCount,
			int monstersRemaining,
			int lives,
			MonsterAttack *attacks) {
	_gold = gold;
	_attacksRemaining = attacksRemaining;
	_nextAttackMonstersCount = nextAttackMonstersCount;
	_monstersRemaining = monstersRemaining;
	_lives = lives;
	_curAttackIndex = 0;
	_curAttackName = attacks->GetAttack() [_curAttackIndex].Name();
	_nextAttackName = attacks->GetAttack()[_curAttackIndex + 1].Name();
	_curAttackType = attacks->GetAttack()[_curAttackIndex].Type();
	_nextAttackType = attacks->GetAttack()[_curAttackIndex + 1].Type();
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

void World::SetAttackIndex(int index) {
	_curAttackIndex = index;
};

void World::SetAttackDelay(float delay) {
	_delayTimer = delay;
};

void World::Update(float dt) {
	if (_delayTimer < 0) {
		_delayTimer -= dt;
	}
	else{
		_delayTimer = 0;
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

