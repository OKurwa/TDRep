#pragma once

class Attack {
public:
	Attack(); 
	Attack(int index,  const std::string mType, const std::string aName, const int mCount);
	~Attack();
	std::string Type();
	std::string Name();
	int			Count();
	int			Index();
	void		SetType(std::string);
	void		SetName(std::string);
	void		SetCount(int);
	void		SetIndex(int);
private:
	int _index;
	std::string _monsterType;
	std::string _attackName;
	int		    _monsterCount;
};

class MonsterAttack {
public:
	MonsterAttack();
	~MonsterAttack();
	void SetDelay(float d);
	void LoadFromFile(std::string fileName);
	void SaveToFile(std::string fileName);
	std::vector<Attack> & GetAttack();
private:
	std::vector<Attack> _attacks;
	float _attackDelay;
};


