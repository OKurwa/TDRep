#pragma once
#include "MonsterParent.h"
//////////////////////////////
//--------------------------//
//----------Пушка-----------//
//--------------------------//
//////////////////////////////

class TowerParent 
{
public:
	TowerParent();
	TowerParent(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, Render::TexturePtr tex);
	~TowerParent();


	virtual void Draw() = 0;
	virtual void Update(float dt);
	
	virtual bool Shoot();
	
	std::vector<boost::intrusive_ptr<FireParent>> & GetMissiles();
	FPoint Position();
	IPoint Cell();
	bool TakeAim(std::vector<boost::intrusive_ptr<MonsterParent>> monsters);

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


class NormalTower: public TowerParent
{
public:
	NormalTower();
	NormalTower(FPoint position, IPoint cell, float rTime, float rTimer, int range, int mSpeed, Render::TexturePtr tex);
	~NormalTower();


	void Draw();
	void Update(float dt);
	bool Shoot();
};

class SlowTower : public TowerParent
{
public:
	SlowTower();
	SlowTower(FPoint position, float rTime, float rTimer, int range, int sRange, int mSpeed);
	~SlowTower();


	void Draw();
	void Update(float dt);

	bool Shoot(FPoint tPosition);
private:
	int    _splashRange;
	float  _slowMul;
};

class DecayTower : public TowerParent
{
public:
	DecayTower();
	DecayTower(FPoint position, float rTime, float rTimer, int range, int sRange, int mSpeed);
	~DecayTower();


	void Draw();
	void Update(float dt);

	bool Shoot(FPoint tPosition);
};

class BashTower : public TowerParent
{
public:
	BashTower();
	BashTower(FPoint position, float rTime, float rTimer, int range, int sRange, int mSpeed);
	~BashTower();


	void Draw();
	void Update(float dt);

	bool Shoot(FPoint tPosition);
};

class SplashTower : public TowerParent
{
public:
	SplashTower();
	SplashTower(FPoint position, float rTime, float rTimer, int range, int sRange, int mSpeed);
	~SplashTower();


	void Draw();
	void Update(float dt);

	bool Shoot(FPoint tPosition);
private:
	int    _splashRange;
};