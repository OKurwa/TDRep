#pragma once
#include "MonsterParent.h"

//////////////////////////////
//--------------------------//
//----------Снаряды---------//
//--------------------------//
//////////////////////////////
//----------------------------------------------//
//----------------------------------------------//
//			Базовый класс снаряда				//
//----------------------------------------------//
//----------------------------------------------//

class FireParent {
public:
	typedef boost::intrusive_ptr<FireParent> Ptr;
	FireParent();
	FireParent(FPoint position,FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, std::string mType, IPoint dmg, Render::TexturePtr tex);
	~FireParent() {};
	
	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	
	FPoint Position();
	FPoint TargetPosition();
	std::string Type();
	bool Fly();
	bool Hit();
	void MakePath();
	friend void intrusive_ptr_add_ref(FireParent*);
	friend void intrusive_ptr_release(FireParent*);
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
	std::string _missileType;
	FPoint _position;
	FPoint _targetPosition;
	FPoint _speed;
	int	   _modSpeed;
	float  _flyTime;
	float  _missileTimer;
	bool   _fly;
	bool   _hit;
	TimedSplinePath _missilePathX;
	TimedSplinePath _missilePathY;
	Render::TexturePtr _tex;
	IPoint _damage;
	
};
inline void intrusive_ptr_add_ref(FireParent* e) { e->AddRef(); }
inline void intrusive_ptr_release(FireParent* e) { e->Release(); }


//----------------------------------------------//
//----------------------------------------------//
//				Обычный снаряд					//
//----------------------------------------------//
//----------------------------------------------//
class NormalMissile : public FireParent {
public:
	NormalMissile();
	NormalMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, IPoint dmg, Render::TexturePtr tex);
	~NormalMissile();

	void Draw();
	void Update(float dt);
private:
	MonsterParent * _target;
};


//----------------------------------------------//
//----------------------------------------------//
//				Замедляющий снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class SlowMissile : public FireParent {
public:
	SlowMissile();
	SlowMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, FPoint sFactor, int sRange, IPoint dmg, Render::TexturePtr tex);
	~SlowMissile();

	void Draw();
	void Update(float dt);

private:
	FPoint _slow;
	int _splashRange;
	std::vector<MonsterParent::Ptr> _targets;
	
};


//----------------------------------------------//
//----------------------------------------------//
//				Отравляющий снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class DecayMissile : public FireParent {
public:
	DecayMissile();
	DecayMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint decay, IPoint dmg, Render::TexturePtr tex);
	~DecayMissile();

	void Draw();
	void Update(float dt);

private:
	MonsterParent * _target;
	FPoint _decay;
	FPoint decay;
 };

//----------------------------------------------//
//----------------------------------------------//
//				Оглушающий снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class BashMissile : public FireParent {
public:
	BashMissile();
	BashMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint bash, IPoint dmg, Render::TexturePtr tex);
	~BashMissile();

	void Draw();
	void Update(float dt);
private:
	MonsterParent * _target;
	FPoint _bash;
};


//----------------------------------------------//
//----------------------------------------------//
//				Разрывной снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class SplashMissile : public FireParent {
public:
	SplashMissile();
	SplashMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, int sRange, IPoint dmg, Render::TexturePtr tex);
	~SplashMissile();

	void Draw();
	void Update(float dt);
private:
	int _splashRange;
	std::vector<MonsterParent::Ptr> _targets;
};












