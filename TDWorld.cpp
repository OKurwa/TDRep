#include "stdafx.h"
#include "TDWorld.h"
#include <iostream>
#include <string>

TDWorld::TDWorld() {
	
	//����
	score = 0;
	//������
	gold = 0;
	//�����
	wave = 0;
	//HP
	hp = 0;

	//������ ����
	height = 0;
	width = 0;
}

TDWorld::~TDWorld()
{
};


//������������� ����
void TDWorld::Init(int sSlots, int sGold, int h, int w, short sHP) {
	
	int tCount = 10;
	int mSpeed = 600;
	float rTime = 0.5;
	int gTime = 60;
	std::ifstream ifs;
	std::string str;
	std::vector<std::string> arr;
	std::string name;
	std::string value;
	//���������� �������� �� �����. ���� ���������� �� ����������, ����� �������������� �������� �� ��������� 
	ifs.open("input.txt");
	if (ifs.is_open()) {
		while (!ifs.eof()) {
			getline(ifs, str);
			arr.push_back(str);
		}
	}
	ifs.close();
	for (int i = 0; i < arr.size(); i++) {
		std::size_t pos = arr[i].find("=");
		name = arr[i].substr(0, pos);
		value = arr[i].substr(pos + 1);
		if (name == "targetCount") {
			tCount = std::stoi(value);
		}
		else if (name == "missileSpeed") {
			mSpeed = std::stoi(value);
		}
		else if (name == "reloadTime") {
			rTime = std::stof(value);
		} 
		else if (name == "gameTime") {
			gTime = std::stoi(value);
		}
	}


	slots.clear();
	CannonSlot newSlot;
	slots.push_back(newSlot);
	slots[0].SetPos(w/2, 70);
	cannons.clear();
	if (slots[0].Fill(0)) {
		Cannon newCannon;
		newCannon.SetPos(slots[0].GetX(), slots[0].GetY());
		newCannon.SetAttackSpeed(300);
		newCannon.SetDelay(rTime);
		newCannon.SetMissileSpeed(mSpeed);
		newCannon.SetSplashRange(60);
		cannons.push_back(newCannon);
	}
		//����: 10 ����� � ��������� �����
	units.clear();
	srand(time(NULL));
	for (int unitCount = 0; unitCount < tCount; unitCount++) {
		Runner newRunner;
		short mul = -1;
		if (rand() % 2 > 0) {
			mul *= -1;
		};
		float sX = rand() % 1000 / 5.0f - 100;
		float sY = sqrt(10000 - sX*sX) * mul;

		newRunner.SetMoveSpeed(sX, sY);
		newRunner.SetSize(25);
		newRunner.SetCage(0, 100, w, h);
		newRunner.SetPos(rand() % w, rand() % (h-100) + 100);
		units.push_back(newRunner);
	}

	//����
	score = 0;
	//������
	gold = 0;
	//�����
	wave = 0;
	//HP
	hp = 0;

	//������ ����
	height = h - 100;
	width = w;

	gameTimer = gTime;
	stop = true;
	win = false;
	timeover = false;
};

//����� ����
void TDWorld::Reset() {
	
	units.clear();
	cannons.clear();
	slots.clear();
	gameTimer = 0;
	score = 0;
	gold = 0;
	wave = 0;
	hp = 0;
	height = 0;
	width = 0;
	stop = true;
	win = false;
	timeover = false;
};

//���������� ����
void TDWorld::Update(float dt) {
	if (gameTimer <= 0) {
		stop = true;
		timeover = true;
	}
	
	if (units.size() == 1 && units[0].Damaged()) {
		win = true;
		stop = true;
	}

	
	if (!stop) {
		//��� �������
		if (gameTimer > 0)
			gameTimer -= dt;
		//��������� ������������
		for (int i = 0; i < units.size(); i++) {
			for (int j = 0; j < units.size(); j++) {
				if (i != j && !units[i].Damaged() && !units[j].Damaged()) {
					float iUnitPosX = units[i].GetX();
					float jUnitPosX = units[j].GetX();
					float iUnitPosY = units[i].GetY();
					float jUnitPosY = units[j].GetY();
					int size = units[i].GetSize() + units[j].GetSize();

					float distance = sqrt((iUnitPosX - jUnitPosX)*(iUnitPosX - jUnitPosX) + (iUnitPosY - jUnitPosY) * (iUnitPosY - jUnitPosY));
					if (distance < 10 + size) {
						float f = 1000;
						float iSpeedX = units[i].GetMoveSpeedX();
						float iSpeedY = units[i].GetMoveSpeedY();
						float jSpeedX = units[j].GetMoveSpeedX();
						float jSpeedY = units[j].GetMoveSpeedY();

						iSpeedX += f * (iUnitPosX - jUnitPosX) / distance / 5 * dt;
						iSpeedY += f * (iUnitPosY - jUnitPosY) / distance / 5 * dt;
						jSpeedX -= f * (iUnitPosX - jUnitPosX) / distance / 5 * dt;
						jSpeedY -= f * (iUnitPosY - jUnitPosY) / distance / 5 * dt;

						units[i].SetMoveSpeed(iSpeedX, iSpeedY);
						units[j].SetMoveSpeed(jSpeedX, jSpeedY);

					}
				}


			}
		} //*/

		  /*����� ��������� ���������*/
		for (int cannon = 0; cannon < cannons.size(); cannon++) {
			for (int missile = 0; missile < cannons[cannon].GetMissiles().size(); missile++) {
				if (cannons[cannon].GetMissiles()[missile].GetTargetPosY() != 0) {
					if (cannons[cannon].GetMissiles()[missile].Dive()) {
						float hitX = cannons[cannon].GetMissiles()[missile].GetTargetPosX();
						float hitY = cannons[cannon].GetMissiles()[missile].GetTargetPosY();
						int spRange = cannons[cannon].GetSplashRange();
						for (int i = 0; i < units.size(); i++) {
							if (units[i].DetectImpact(hitX, hitY, spRange)) {
								units[i].TakeDamage();
							};// ����� ������� �� ���������
						};// ����� ����� �� �����
					};// ����� ������� �� ������
				};// ����� ������� �� ����������� �������
			};// ����� ����� �� ��������
		};// ����� ����� �� ������
		//���������� �����
		for (int i = 0; i < units.size(); i++) {
			units[i].Update(dt);
		}
		//���������� �����
		for (int i = 0; i < cannons.size(); i++) {
			cannons[i].Update(dt);
		}
		

	}

};

//�������� �����
bool TDWorld::AddCannon(unsigned int slot) {
	if (slots.size()>slot && slots[slot].IsEmpty()) {
		if (slots[slot].Fill(cannons.size())) {
			Cannon newCannon;
			newCannon.SetPos(slots[slot].GetX(), slots[slot].GetY());
			cannons.push_back(newCannon);
			return true;
		}
		else {
			return false;
		}
		
	}
	else {
		return false;
	}
	
};
//��������� �����
bool TDWorld::DelCannon(unsigned int slot) {
	int cannonIndex = slots[slot].GetCannonIndex();
	if (cannons.size()>cannonIndex && cannonIndex >= 0) {
		cannons.erase(cannons.begin() + cannonIndex);
		//std::vector<Cannon>(cannons).swap(cannons);
		slots[slot].Clear();
		return true;
	}
	else {
		return false;
	}
};

//������� �������
bool TDWorld::DelUnit(unsigned int unit) {
	if (units.size() > unit) {
		units.erase(units.begin() + unit);
		return true;
	}	
	else{
		return false;
	}
};

//��������� ��������� �����
void TDWorld::SetNextWave(int nWave) {
	wave = nWave;
};
//������ ��������� �����
void TDWorld::StartNextWave() {

};
//���������� ������� �����
void TDWorld::EndWave() {
};
//������� �����
void TDWorld::TryWaveInit() {
	//����: 10 ����� � ��������� �����
	units.clear();
	srand(time(NULL));
	
	for (int unitCount = 0; unitCount < 10; unitCount++) {
		Runner newRunner;
		short mul = -1;
		if (rand() % 2 > 0) {
			mul *= -1;
		};
		float sX = rand() % 1000 / 5.0f - 100;
		float sY = sqrt(10000 - sX*sX) * mul;
		
		newRunner.SetMoveSpeed(sX, sY);
		newRunner.SetPos(rand() % 1024, rand() % 668 + 100);
		units.push_back(newRunner);
	}

	
	//�����: �������
	cannons.clear();
	

	//�����: �������
	slots.clear();
	CannonSlot newSlot;
	slots.push_back(newSlot);
	
	//����
	score = 0;
	//������
	gold = 0;
	//�����
	wave = 1;
	//HP
	hp = 1;

	//������ ����
	height = 768;
	width = 1024;
	
	gameTimer = 60;
	stop = true;
	
};








//������ � ������
//����
std::vector<Runner> & TDWorld::GetUnits()  {
	return units;
};
//���������� �����
//int TDWorld::GetUnitsCount() const {
//	return unitsCount;
//};

//�����
std::vector<Cannon> & TDWorld::GetCannons() {
	return cannons;
};
//���������� �����
//int TDWorld::GetCannonsCount() const {
//	return cannonsCount;
//};

//�����
std::vector<CannonSlot> & TDWorld::GetSlots() {
	return slots;
};
//���������� ������
//int TDWorld::GetSlotsCount() const {
//	return slotsCount;
//};

//������� ������
float TDWorld::GetTimer() {
	return gameTimer;
};
//����
int TDWorld::GetScore() const {
	return score;
};
//������
int TDWorld::GetGold() const {
	return gold;
};
//�����
short TDWorld::GetWave() const {
	return wave;
};
//HP
short TDWorld::GetHP() const {
	return hp;
};

//������ ����
int TDWorld::GetHeight() const {
	return	height;
};
int TDWorld::GetWidth() const {
	return	width;
};


void TDWorld::Stop() {
	stop = true;
};
void TDWorld::Start() {
	stop = false;
};

bool TDWorld::Stoped() {
	return stop;
};
bool TDWorld::Win() {
	return win;
};
bool TDWorld::TimeOver() {
	return timeover;
};