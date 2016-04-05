#pragma once
#include "MonsterParent.h"

//////////////////////////////
//--------------------------//
//----------Снаряды---------//
//--------------------------//
//////////////////////////////

class FireParent {
public:

	FireParent();
	FireParent(FPoint position,FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, std::string mType, Render::TexturePtr tex);
	~FireParent() {};
	
	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	
	FPoint Position();
	FPoint TargetPosition();
	std::string Type();
	bool Fly();
	bool Hit();
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
};
inline void intrusive_ptr_add_ref(FireParent* e) { e->AddRef(); }
inline void intrusive_ptr_release(FireParent* e) { e->Release(); }


class NormalMissile : public FireParent {
public:
	NormalMissile();
	NormalMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, Render::TexturePtr tex);
	~NormalMissile();

	void Draw();
	void Update(float dt);
private:
	MonsterParent * _target;
};

class SlowMissile : public FireParent {
public:
	SlowMissile();
	SlowMissile(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, float sFactor, int sRange, Render::TexturePtr tex);
	~SlowMissile();

	void Draw();
	void Update(float dt);

private:
	float _slowFactor;
	int _splashRange;

};

class DecayMissile : public FireParent {
public:
	DecayMissile();
	DecayMissile(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, int dFactor, Render::TexturePtr tex);
	~DecayMissile();

	void Draw();
	void Update(float dt);

private:
	int _decayFactor;
};

class BashMissile : public FireParent {
public:
	BashMissile();
	BashMissile(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, float bTime, float bChance, Render::TexturePtr tex);
	~BashMissile();

	void Draw();
	void Update(float dt);
private:
	float _bashTime;
	float _bashChance;
};

class SplashMissile : public FireParent {
public:
	SplashMissile();
	SplashMissile(FPoint position, FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, int sRange, Render::TexturePtr tex);
	~SplashMissile();

	void Draw();
	void Update(float dt);
private:
	int _splashRange;
};












