#pragma once
#include "MonsterParent.h"
//////////////////////////////
//--------------------------//
//----------Пушка-----------//
//--------------------------//
//////////////////////////////


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


	virtual void Draw() = 0;
	virtual void Update(float dt);
	
	virtual bool Shoot() = 0;
	
	std::vector<boost::intrusive_ptr<FireParent>> & GetMissiles();
	FPoint Position();
	IPoint Cell();
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
	std::vector<boost::intrusive_ptr<FireParent>> _missiles;
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
public:
	NormalTower();
	NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~NormalTower();


	void Draw();
	void Update(float dt);
	bool Shoot();
};


//----------------------------------------------//
//----------------------------------------------//
//				Замедляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
class SlowTower : public TowerParent
{
public:
	SlowTower();
	SlowTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, FPoint sFactor, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~SlowTower();


	void Draw();
	void Update(float dt);
	bool Shoot();
	bool TakeAim(std::vector<MonsterParent::Ptr> & monsters);
private:
	int    _splashRange;
	FPoint _slow;
	std::vector<MonsterParent::Ptr> _targets;
};


//----------------------------------------------//
//----------------------------------------------//
//				Отравляющая башня	 			//
//----------------------------------------------//
//----------------------------------------------//
class DecayTower : public TowerParent
{
public:
	DecayTower();
	DecayTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint decay, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~DecayTower();


	void Draw();
	void Update(float dt);

	bool Shoot();
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
public:
	BashTower();
	BashTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, FPoint bash, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~BashTower();


	void Draw();
	void Update(float dt);

	bool Shoot();
private:
	FPoint _bash;
};


//----------------------------------------------//
//----------------------------------------------//
//				Разрывная башня	 				//
//----------------------------------------------//
//----------------------------------------------//
class SplashTower : public TowerParent
{
public:
	SplashTower();
	SplashTower(FPoint position, IPoint cell, std::vector<MonsterParent::Ptr> & targets, float rTime, float rTimer, int range, int sRange, int mSpeed, IPoint dmg, Render::TexturePtr tex);
	~SplashTower();


	void Draw();
	void Update(float dt);

	bool Shoot();
	bool TakeAim(std::vector<MonsterParent::Ptr> & monsters);
private:
	int    _splashRange;
	std::vector<MonsterParent::Ptr> _targets;
};