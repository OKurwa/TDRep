#pragma once
#include "MonsterParent.h"
#include "FireParent.h"
//////////////////////////////
//--------------------------//
//----------Пушка-----------//
//--------------------------//
//////////////////////////////

enum TowerType {
	NORMAL,
	SLOW,
	SPLASH,
	DECAY,
	BASH
};
//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс башни 				//
//----------------------------------------------//
//----------------------------------------------//
class TowerParent 
{
public:
	typedef boost::intrusive_ptr<TowerParent> Ptr;
	TowerParent();
	TowerParent(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~TowerParent();


	virtual void Draw();
	virtual void Update(float dt);
	virtual bool Shoot() = 0;
	virtual Ptr clone() = 0;


	std::vector<FireParent::Ptr> & GetMissiles();
	FPoint Position();
	IPoint Cell();
	void SetPosition(FPoint);
	void SetCell(IPoint);
	virtual bool TakeAim(std::vector<MonsterParent::Ptr> & monsters);

	friend void intrusive_ptr_add_ref(TowerParent*);
	friend void intrusive_ptr_release(TowerParent*);
	void AddRef() {
		++ref_cnt_;
	}
	void Release() {
		if (--ref_cnt_ == 0) {
			delete this;

		}
	}
	void SetPrice(int);
	int Price();
protected:
	int ref_cnt_;
	std::string _towerType;
	IPoint _damage;
	FPoint _position;
	IPoint _cell;
	MonsterParent * _target;
	float  _reloadTime;
	float  _reloadTimer;
	int	   _range;
	int    _missileSpeed;
	int    _price;
	int	   _lvl;
	int    _lvlCount;
	std::vector<FireParent::Ptr> _missiles;
	Render::TexturePtr _tex;

};
inline void intrusive_ptr_add_ref(TowerParent* e) { e->AddRef(); }
inline void intrusive_ptr_release(TowerParent* e) { e->Release(); }

//----------------------------------------------//
//----------------------------------------------//
//				Обычная башня	 				//
//----------------------------------------------//
//----------------------------------------------//
class NormalTower: public TowerParent
{
	friend class TowerPrototypeFactory;
public:
	
	NormalTower();
	NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~NormalTower();
	TowerParent::Ptr clone() {
		return new NormalTower(*this);
	}

	//void Draw();
	void Update(float dt);
	bool Shoot();
	void LoadFromXml(std::string filename);
private:
	std::vector<NormalMissile::NMissInfo> _missilesPrototypes;
};


//----------------------------------------------//
//----------------------------------------------//
//				Замедляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
class SlowTower : public TowerParent
{
	friend class TowerPrototypeFactory;
public:
	SlowTower();
	SlowTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, FPoint sFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~SlowTower();

	TowerParent::Ptr clone() {
		return new SlowTower(*this);
	}
	//void Draw();
	void Update(float dt);
	bool Shoot();
	void LoadFromXml(std::string filename);
	bool TakeAim(std::vector<MonsterParent::Ptr> & monsters);
private:
	int    _splashRange;
	FPoint _slow;
	std::vector<MonsterParent::Ptr> _targets;
	std::vector<SlowMissile::SlMissInfo> _missilesPrototypes;
};


//----------------------------------------------//
//----------------------------------------------//
//				Отравляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
class DecayTower : public TowerParent
{
	friend class TowerPrototypeFactory;
public:
	DecayTower();
	DecayTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint decay, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~DecayTower();
	TowerParent::Ptr clone() {
		return new DecayTower(*this);
	}

	//void Draw();
	void Update(float dt);

	bool Shoot();
	void LoadFromXml(std::string filename);
private:
	FPoint _decay;
};


//----------------------------------------------//
//----------------------------------------------//
//				Оглушающая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
class BashTower : public TowerParent
{
	friend class TowerPrototypeFactory;
public:
	BashTower();
	BashTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint bash, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~BashTower();
	TowerParent::Ptr clone() {
		return new BashTower(*this);
	}

	//void Draw();
	void Update(float dt);

	bool Shoot();
	void LoadFromXml(std::string filename);
private:
	FPoint _bash;
	std::vector<BashMissile::BMissInfo> _missilesPrototypes;
};


//----------------------------------------------//
//----------------------------------------------//
//				Разрывная башня	 				//
//----------------------------------------------//
//----------------------------------------------//
class SplashTower : public TowerParent
{
	friend class TowerPrototypeFactory;
public:
	SplashTower();
	SplashTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~SplashTower();
	TowerParent::Ptr clone() {
		return new SplashTower(*this);
	}

	//void Draw();
	void Update(float dt);

	bool Shoot();
	void LoadFromXml(std::string filename);
	bool TakeAim(std::vector<MonsterParent::Ptr> & monsters);
private:
	int    _splashRange;
	std::vector<MonsterParent::Ptr> _targets;
};




// Фабрика для создания башен
class TowerPrototypeFactory
{
public:
	TowerPrototypeFactory() {
		_Nloaded = false;
		_Slloaded = false;
		_Sploaded = false;
		_Bloaded = false;
		_Dloaded = false;
	}
	TowerParent::Ptr createNormal() {
		static NormalTower prototype;
		if (!_Nloaded) {
			prototype.LoadFromXml("NewMap.xml");
			_Nloaded = true;
		}
		
		return prototype.clone();
	}
	TowerParent::Ptr createSlow() {
		static SlowTower prototype;
		if (!_Slloaded) {
			prototype.LoadFromXml("NewMap.xml");
			_Slloaded = true;
		}
		return prototype.clone();
	}
	TowerParent::Ptr createSplash() {
		static SplashTower prototype;
		if (!_Sploaded) {
			prototype.LoadFromXml("NewMap.xml");
			_Sploaded = true;
		}
		return prototype.clone();
	}
	TowerParent::Ptr createBash() {
		static BashTower prototype;
		if (!_Bloaded) {
			prototype.LoadFromXml("NewMap.xml");
			_Bloaded = true;
		}
		return prototype.clone();
	}
	TowerParent::Ptr createDecay() {
		static DecayTower prototype;
		if (!_Dloaded) {
			prototype.LoadFromXml("NewMap.xml");
			_Dloaded = true;
		}
		return prototype.clone();
	}

private:
	bool _Nloaded;
	bool _Slloaded;
	bool _Sploaded;
	bool _Bloaded;
	bool _Dloaded;
};