#pragma once
static int myeffcount;

class FireBall
{
public:
	FireBall() ;
	~FireBall();
	
	FireBall(const FireBall&);
	FireBall& operator = (const FireBall&);
	void Draw();
	void Update(float dt);
	void UpdateChildsPos();

	void Launch(int startX, int startY, float speed, float speedX, float speedY, float _time, int subBallsCount, int stack, bool first);

	float PosX();
	float PosY();
	IPoint Pos();
	float SpeedX();
	float SpeedY();
	float Speed();
	float MaxLightTimer();
	bool IsDead();
	bool TryKillChilds();
	bool TryKillEffects();
	bool CheckHit(std::vector<IPoint> & target);
	std::vector<IPoint> GetChildsPos();

	void SetPosX(float x);
	void SetPosY(float y);

	void SetSpeedX(float sx);
	void SetSpeedY(float sy);
	void SetSpeed(float sy);
	friend void intrusive_ptr_add_ref(FireBall*);
	friend void intrusive_ptr_release(FireBall*);
	
	void AddRef() {
		++ref_cnt_;
	}

	void Release() {
		if (--ref_cnt_ == 0) {
			delete this;
			--myeffcount;
		}
			
	}

private:
	int ref_cnt_;
	float _x;
	float _y;
	float _xSpeed;
	float _ySpeed;
	float _modSpeed;
	float _lifeTime;
	float _lifeTimer;
	bool _isDead;
	int _ballsCount;
	int _stackSize;
	bool _boomed;
	float _lightTimer;
	

	std::vector<boost::intrusive_ptr<FireBall>> _subFireBalls;

	EffectsContainer _fireCont;
	ParticleEffectPtr _fireEff;
	EffectsContainer _boomCont;
	ParticleEffectPtr _boomEff;
	
};

inline void intrusive_ptr_add_ref(FireBall* e) { e->AddRef(); }
inline void intrusive_ptr_release(FireBall* e) { e->Release(); }