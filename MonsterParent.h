#pragma once
#include "FieldMap.h"

class MonsterParent
{
public:
	typedef boost::intrusive_ptr<MonsterParent> Ptr;
	MonsterParent();
	MonsterParent(FPoint position, int modSpeed, int hp, FieldMap * map, Render::TexturePtr _skin);
	virtual ~MonsterParent();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void PostUpdate(float dt) {};
	void UpdateAnimAngle(float dt);
	virtual void TakeDamage(TowerType effType, FPoint values, float damage) = 0;
	virtual Ptr clone() = 0;
	bool FindAWay();
	std::vector<IPoint> FillAround(std::vector<IPoint> lastWaveFilled, std::vector<std::vector<int>> & map, int d);
	bool Dead();
	bool EndDeadAnim();
	bool Finish();
	float WayDistance();
	FPoint Position();
	FPoint HitPosition(float dt);
	void SetPosition(FPoint,FieldMap*);
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
	bool	   _dying;
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
	
	//јнимации
	Render::AnimationPtr _runAnim;
	Render::AnimationPtr _idleAnim;
	Render::AnimationPtr _dieAnim;

	AnimAngles _runAnimAngles;
	AnimAngles _idleAnimAngles;
	AnimAngles _dieAnimAngles;
	float _lastAngle;

};
inline void intrusive_ptr_add_ref(MonsterParent* e) { e->AddRef(); }
inline void intrusive_ptr_release(MonsterParent* e) { e->Release(); }



class BossMonster : public MonsterParent
{
public:
	struct BossMInfo {
		FPoint _position;
		int	_modSpeed;
		int _hp;
		FieldMap * _map;
		float _reduceDamage;
		Render::AnimationPtr _runAnim;
		Render::AnimationPtr _idleAnim;
		Render::AnimationPtr _dieAnim;
	};
	BossMonster();
	BossMonster(BossMonster& proto);
	BossMonster(BossMInfo);
	~BossMonster();
	MonsterParent::Ptr clone() {

		return new BossMonster(*this);
	}
	//void Draw();
	//void Update(float dt);
	void TakeDamage(TowerType effType, FPoint values, float damage);
private:
	float _reduceDamage;
};

class ImmuneMonster : public MonsterParent
{
public:
	struct ImmMInfo {
		FPoint _position;
		int	_modSpeed;
		int _hp;
		FieldMap * _map;
		Render::AnimationPtr _runAnim;
		Render::AnimationPtr _idleAnim;
		Render::AnimationPtr _dieAnim;
	};
	ImmuneMonster();
	ImmuneMonster(ImmuneMonster& proto);
	ImmuneMonster(ImmMInfo);
	~ImmuneMonster();
	MonsterParent::Ptr clone() {

		return new ImmuneMonster(*this);
	}
	//void Draw();
	//void Update(float dt);
	
	void TakeDamage(TowerType effType, FPoint values, float damage);
private:

};

class HealingMonster :public MonsterParent
{
public:
	struct HealMInfo {
		FPoint _position;
		int	_modSpeed;
		int _hp;
		FieldMap * _map;
		int _healPerSecond;
		Render::AnimationPtr _runAnim;
		Render::AnimationPtr _idleAnim;
		Render::AnimationPtr _dieAnim;
	};
	HealingMonster();
	HealingMonster(HealingMonster& proto);
	HealingMonster(HealMInfo);
	~HealingMonster();
	MonsterParent::Ptr clone() {

		return new HealingMonster(*this);
	}
	//void Draw();
	//void Update(float dt);
	void TakeDamage(TowerType effType, FPoint values, float damage);

private:
	int _healPerSecond;
};

class NormalMonster :public MonsterParent
{
public:
	struct NormMInfo {
		FPoint _position;
		int	_modSpeed;
		int _hp;
		FieldMap * _map;
		Render::AnimationPtr _runAnim;
		Render::AnimationPtr _idleAnim;
		Render::AnimationPtr _dieAnim;
	};
	NormalMonster();
	NormalMonster(NormalMonster& proto);
	NormalMonster(NormMInfo);
	~NormalMonster();
	MonsterParent::Ptr clone() {

		return new NormalMonster(*this);
	}
	//void Draw();
	//void Update(float dt);
	void TakeDamage(TowerType effType, FPoint values, float damage);

private:
	
};




const AnimAngles RUN_ANGLES = {
	IPoint(4, 11),
	IPoint(36, 43),
	IPoint(68, 75),
	IPoint(100, 107),
	IPoint(132, 139),
	IPoint(164, 171),
	IPoint(196, 203),
	IPoint(228, 235) };
const AnimAngles MOB_IDL_ANGLES = {
	IPoint(0,3),
	IPoint(32,35),
	IPoint(64,67),
	IPoint(96,99),
	IPoint(128,131),
	IPoint(160,163),
	IPoint(192,195),
	IPoint(224,227) };
const AnimAngles DIE_ANGLES = {
	IPoint(16,23),
	IPoint(48,55),
	IPoint(80,87),
	IPoint(112,119),
	IPoint(144,151),
	IPoint(176,183),
	IPoint(208,215),
	IPoint(240,247) };