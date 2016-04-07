#pragma once
#include "FieldMap.h"

class MonsterParent
{
public:
	typedef boost::intrusive_ptr<MonsterParent> Ptr;
	MonsterParent();
	MonsterParent(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr _skin);
	virtual ~MonsterParent();

	virtual void Draw() = 0;
	virtual void Update(float dt);
	virtual void TakeDamage(std::string effType, FPoint values, float damage) = 0;
	bool FindAWay();
	bool Dead();
	bool Finish();
	float WayDistance();
	FPoint Position();
	FPoint HitPosition(float dt);

	friend void intrusive_ptr_add_ref(MonsterParent*);
	friend void intrusive_ptr_release(MonsterParent*);
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
	FPoint	   _position;
	FPoint	   _hitPosition;
	FPoint	   _speed;
	int		   _modSpeed;
	float	   _hp;
	float	   _maxHp;
	bool	   _dead;
	bool       _finish;
	bool	   _damaged;
	FieldMap   * _map;
	IPoint _curCell;
	float _moveTimer;
	float _curWayDistance;
	std::vector<IPoint> _currentWay;
	Render::TexturePtr _skin;
	TimedSplinePath _curWaySplineX;
	TimedSplinePath _curWaySplineY;

	//Ёффекты атак и счетчики

	FPoint _bash;
	FPoint _decay;
	FPoint _slow;
	



};
inline void intrusive_ptr_add_ref(MonsterParent* e) { e->AddRef(); }
inline void intrusive_ptr_release(MonsterParent* e) { e->Release(); }



class BossMonster : public MonsterParent
{
public:
	BossMonster();
	BossMonster(FPoint position, int modSpeed, int hp, FieldMap * map,  float reduceDamage, Render::TexturePtr skin);
	~BossMonster();

	void Draw();
	void Update(float dt);
	void TakeDamage(std::string effType, FPoint values, float damage);
private:
	float _reduceDamage;
};

class ImmuneMonster : public MonsterParent
{
public:
	ImmuneMonster();
	ImmuneMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr _skin);
	~ImmuneMonster();

	void Draw();
	void Update(float dt);
	
	void TakeDamage(std::string effType, FPoint values, float damage);
private:

};

class HealingMonster :public MonsterParent
{
public:
	HealingMonster();
	HealingMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin, int healPerSecond);
	~HealingMonster();

	void Draw();
	void Update(float dt);
	void TakeDamage(std::string effType, FPoint values, float damage);

private:
	int _healPerSecond;
};

class NormalMonster :public MonsterParent
{
public:
	NormalMonster();
	NormalMonster(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr skin);
	~NormalMonster();

	void Draw();
	void Update(float dt);
	void TakeDamage(std::string effType, FPoint values, float damage);

private:
	
};
