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
	FireParent(FPoint position,FPoint tPosition, int mSpeed, float fTime, float mFlyTimer, TowerType mType, IPoint dmg, Render::TexturePtr tex);
	~FireParent() {};
	
	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	virtual Ptr clone() = 0;
	virtual void LoadFromXml(std::string, int) {};

	FPoint Position();
	FPoint TargetPosition();
	void SetPosition(FPoint);
	TowerType Type();
	bool Fly();
	bool Hit();
	void MakePath();

	virtual MonsterParent::Ptr TakeAim(std::vector<MonsterParent::Ptr> & monsters, MonsterParent::Ptr target, int range);

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
	TowerType _missileType;
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
	MonsterParent * _target;
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
	struct NMissInfo {
		FPoint _position;
		int	_modSpeed;
		IPoint _damage;
		MonsterParent * _target;
	};

	NormalMissile();
	//NormalMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, IPoint dmg, Render::TexturePtr tex);
	NormalMissile(NMissInfo);
	
	~NormalMissile();
	
	void Draw();
	void Update(float dt);
	NormalMissile::Ptr clone() {
		return new NormalMissile(*this);
	}
	void LoadFromXml(std::string, int);
	void SetTarget(MonsterParent * target);
	//MonsterParent::Ptr  NormalMissile::TakeAim(std::vector<MonsterParent::Ptr> & monsters, MonsterParent::Ptr target, int range);
private:
	
};


//----------------------------------------------//
//----------------------------------------------//
//				Замедляющий снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class SlowMissile : public FireParent {
public:
	struct SlMissInfo {
		FPoint _position;
		FPoint _tPosition;
		int	_modSpeed;
		IPoint _damage;
		FPoint _sFactor;
		int _sRange;
	};


	SlowMissile();
	//SlowMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, FPoint sFactor, int sRange, IPoint dmg, Render::TexturePtr tex);
	SlowMissile(SlMissInfo, std::vector<MonsterParent::Ptr> & targets);
	~SlowMissile();

	void Draw();
	void Update(float dt);
	FireParent::Ptr clone() {
		return new SlowMissile(*this);
	}
	void LoadFromXml(std::string, int);
	MonsterParent::Ptr TakeAim(std::vector<MonsterParent::Ptr> & monsters, MonsterParent::Ptr target, int range);
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
	struct DMissInfo {
		FPoint _position;
		int	_modSpeed;
		IPoint _damage;
		MonsterParent * _target;
		FPoint _decay;
	};
	DecayMissile();
	//DecayMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint decay, IPoint dmg, Render::TexturePtr tex);
	DecayMissile(DMissInfo);
	~DecayMissile();

	void Draw();
	void Update(float dt);
	FireParent::Ptr clone() {
		return new DecayMissile(*this);
	}
	void LoadFromXml(std::string, int);
private:
	
	FPoint _decay;
	//FPoint decay;
 };

//----------------------------------------------//
//----------------------------------------------//
//				Оглушающий снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class BashMissile : public FireParent {
public:
	struct BMissInfo {
		FPoint _position;
		int	_modSpeed;
		IPoint _damage;
		MonsterParent * _target;
		FPoint _bash;
	};
	BashMissile();
	//BashMissile(FPoint position, MonsterParent * target, int mSpeed, float fTime, float mFlyTimer, FPoint bash, IPoint dmg, Render::TexturePtr tex);
	BashMissile(BMissInfo);
	~BashMissile();

	void Draw();
	void Update(float dt);
	FireParent::Ptr clone() {
		return new BashMissile(*this);
	}
	void LoadFromXml(std::string, int);
private:
	
	FPoint _bash;
};


//----------------------------------------------//
//----------------------------------------------//
//				Разрывной снаряд				//
//----------------------------------------------//
//----------------------------------------------//
class SplashMissile : public FireParent {
public:
	struct SpMissInfo {
		FPoint _position;
		FPoint _tPosition;
		int	_modSpeed;
		IPoint _damage;
		int _sRange;
	};
	SplashMissile();
	//SplashMissile(FPoint position, FPoint tPosition, std::vector<MonsterParent::Ptr> & targets, int mSpeed, float fTime, float mFlyTimer, int sRange, IPoint dmg, Render::TexturePtr tex);
	SplashMissile::SplashMissile(SpMissInfo inf, std::vector<MonsterParent::Ptr> & targets);
	~SplashMissile();

	void Draw();
	void Update(float dt);
	FireParent::Ptr clone() {
		return new SplashMissile(*this);
	}
	void LoadFromXml(std::string, int);
	MonsterParent::Ptr TakeAim(std::vector<MonsterParent::Ptr> & monsters, MonsterParent::Ptr target, int range);
private:
	int _splashRange;
	std::vector<MonsterParent::Ptr> _targets;
};



// Фабрика для создания снарядов
class MissilePrototypeFactory
{
public:
	MissilePrototypeFactory() {
		_Nloaded = false;
		_Slloaded = false;
		_Sploaded = false;
		_Bloaded = false;
		_Dloaded = false;
	}
	FireParent::Ptr createNormal() {
		static NormalMissile prototype;
		if (!_Nloaded) {
			//prototype.LoadFromXml("NewMap.xml");
			_Nloaded = true;
		}

		return prototype.clone();
	}
	FireParent::Ptr createSlow() {
		static SlowMissile prototype;
		if (!_Slloaded) {
			//prototype.LoadFromXml("NewMap.xml");
			_Slloaded = true;
		}
		return prototype.clone();
	}
	FireParent::Ptr createSplash() {
		static SplashMissile prototype;
		if (!_Sploaded) {
			//prototype.LoadFromXml("NewMap.xml");
			_Sploaded = true;
		}
		return prototype.clone();
	}
	FireParent::Ptr createBash() {
		static BashMissile prototype;
		if (!_Bloaded) {
			//prototype.LoadFromXml("NewMap.xml");
			_Bloaded = true;
		}
		return prototype.clone();
	}
	FireParent::Ptr createDecay() {
		static DecayMissile prototype;
		if (!_Dloaded) {
			//prototype.LoadFromXml("NewMap.xml");
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








