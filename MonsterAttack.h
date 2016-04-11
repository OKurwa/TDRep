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
	int		WGold();
	int		MGold();
	bool		Finished();
	void		SetType(std::string);
	void		SetName(std::string);
	void		SetCount(int);
	void		SetIndex(int);
	void		SetMaxHp(int);
	void		SetSpeed(int);
	void		SetWGold(int);
	void		SetMGold(int);
	void		SetFinished(bool finish);

	
private:
	int _index;
	std::string _monsterType;
	std::string _attackName;
	int		    _monsterCount;
	int         _maxHp;
	int         _modSpeed;
	bool		_finished;
	int         _waveGold;
	int         _monsterGold;
};

class MonsterAttack {
public:
	MonsterAttack();
	~MonsterAttack();
	void SetDelay(float d);
	void LoadFromFile(std::string fileName);
	void LoadFromXml(std::string);
	void SaveToFile(std::string fileName);
	float Delay();
	std::vector<Attack> & GetAttack();
private:
	std::vector<Attack> _attacks;
	float _attackDelay;
};


