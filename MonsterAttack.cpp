#include "stdafx.h"
#include "MonsterParent.h"
#include "MonsterAttack.h"
#include <fstream>
#include <iostream>
using namespace std;

Attack::Attack(){
	_monsterType = "";
	_attackName = "";
	_monsterCount = 0;
};
Attack::Attack(int index, const std::string mType, const std::string aName, const int mCount) {
	_index = index;
	_monsterType = mType;
	_attackName = aName;
	_monsterCount = mCount;
};

Attack::~Attack() {};

std::string Attack::Type() {
	return _monsterType;
};

std::string Attack::Name() {
	return _attackName;
};

int	Attack::Count() {
	return _monsterCount;
};

int Attack::Index() {
	return _index;
};

void Attack::SetType(std::string s) {
	_monsterType = s;
};
void Attack::SetName(std::string s) {
	_attackName = s;
};
void Attack::SetCount(int count) {
	_monsterCount = count;
};
void Attack::SetIndex(int index) {
	_index = index;
};



MonsterAttack::MonsterAttack() {
	_attacks.clear();
	_attackDelay = 0;
};

MonsterAttack::~MonsterAttack() {};

void MonsterAttack::SetDelay(float d) {
	_attackDelay = d;
};

void MonsterAttack::LoadFromFile(std::string file) {
	std::ifstream settingsFile(file);
	std::string line;
	std::vector<std::string> lines;
	lines.clear();

	if (settingsFile.is_open()) {
		while (std::getline(settingsFile, line)) {
			if (line != "")
				lines.push_back(line);

		}
	}
	settingsFile.close();


	int index = 0;
	
	std::string name;
	std::string value;
	line = lines[0];
	size_t pos = line.find("=", 0);
	name = line.substr(0, pos);
	value = line.substr(pos + 1);
	if (name == "TYPE" && value == "MA") {
		line = lines[1];
		pos = line.find("=", 0);
		name = line.substr(0, pos);
		value = line.substr(pos + 1);
		if (name == "attackCount") {
			_attacks.resize(utils::lexical_cast<int>(value));
		}

		line = lines[2];
		pos = line.find("=", 0);
		name = line.substr(0, pos);
		value = line.substr(pos + 1);
		if (name == "delay") {
			_attackDelay = utils::lexical_cast<float>(value);
		}
		
		for (unsigned int i = 3; i < lines.size(); i++) {
			line = lines[i];
			pos = line.find("=", 0);
			name = line.substr(0, pos);
			value = line.substr(pos + 1);

			if (name == "index") {
				index = utils::lexical_cast<int>(value);
				_attacks[index].SetIndex(index);
			}
			if (name == "type") {
				_attacks[index].SetType(value);
			}

			if (name == "name") {
				_attacks[index].SetName(value);
			}
			if (name == "count") {
				_attacks[index].SetCount(utils::lexical_cast<int>(value));
			}

		}
	}
};

void MonsterAttack::SaveToFile(std::string file) {
	std::ofstream sFile(file);
	std::string line;
	std::vector<std::string> lines;
	lines.clear();
	lines.push_back("TYPE=MA");
	lines.push_back("");
	lines.push_back("attackCount=" + utils::lexical_cast(_attacks.size()));
	lines.push_back("delay=" + utils::lexical_cast(_attackDelay));
	lines.push_back("");
	for (unsigned int attack = 0; attack < _attacks.size(); attack++) {
		lines.push_back("index=" + utils::lexical_cast(_attacks[attack].Index()));
		lines.push_back("type=" + _attacks[attack].Type());
		lines.push_back("name=" + _attacks[attack].Name());
		lines.push_back("count=" + utils::lexical_cast(_attacks[attack].Count()));
		lines.push_back("");
	}

	if (sFile.is_open()) {
		for (unsigned int i = 0; i < lines.size(); i++) {
			sFile << lines[i] << endl;
		}
	}
	sFile.close();
};

std::vector<Attack> & MonsterAttack::GetAttack() {
	return _attacks;
};











