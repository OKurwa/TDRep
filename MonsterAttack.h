#pragma once

class Attack {
public:
	Attack(); 
	Attack(int index, const std::string mType, const std::string aName, const int mCount, int hp, int speed);
	~Attack();
	std::string Type();
	std::string Name();
	int			Count();
	int			Index();
	int			MaxHp();
	int			Speed();
	void		SetType(std::string);
	void		SetName(std::string);
	void		SetCount(int);
	void		SetIndex(int);
	void		SetMaxHp(int);
	void		SetSpeed(int);
private:
	int _index;
	std::string _monsterType;
	std::string _attackName;
	int		    _monsterCount;
	int         _maxHp;
	int         _modSpeed;
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


