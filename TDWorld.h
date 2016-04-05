#pragma once
#include "WorldObject.h"
#include "Runner.h"
#include "Cannon.h"
#include "CannonSlot.h"

class TDWorld
{
public:
	static TDWorld & Instance()
	{
		static TDWorld theSingleInstance;
		return theSingleInstance;
	}

	
	~TDWorld();

	//������������� ����
	void Init(int startSlotsCount, int startGold, int height, int width, short sHP);
	//����� ����
	void Reset();
	//�������� ���
	void Update(float dt);

	//�������� �����
	bool AddCannon(unsigned int slot);
	//��������� �����
	bool DelCannon(unsigned int slot);
	//������� �������
	bool DelUnit(unsigned int unit);
	//��������� ��������� �����
	void SetNextWave(int wave);
	//������ ��������� �����
	void StartNextWave();
	//���������� ������� �����
	void EndWave();
	//������� �����
	void TryWaveInit();
	




	//������ � ������
	//����
	std::vector<Runner> & GetUnits() ;
	

	//�����
	std::vector<Cannon> & GetCannons() ;
	

	//�����
	std::vector<CannonSlot> & GetSlots();
	

	//������� ������
	float GetTimer();
	//����
	int GetScore() const;
	//������
	int GetGold() const;
	//�����
	short GetWave() const;
	//HP
	short GetHP() const;

	//������ ����
	int GetHeight() const;
	int GetWidth() const;

	void Stop();
	void Start();
	bool Stoped();
	bool Win();
	bool TimeOver();

private:
	TDWorld();
	TDWorld(const TDWorld& root);
	TDWorld& operator =(const TDWorld&);


	//����
	std::vector<Runner> units;
	

	//�����
	std::vector<Cannon> cannons;
	
	
	//�����
	std::vector<CannonSlot> slots;
	

	//�����
	float gameTimer;
	//����
	int score;
	//������
	int gold;
	//�����
	short wave;
	//HP
	short hp;

	//������ ����
	int height;
	int width;


	bool stop;
	bool win;
	bool timeover;
	
};
