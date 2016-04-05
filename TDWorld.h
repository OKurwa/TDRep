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

	//Инициализация мира
	void Init(int startSlotsCount, int startGold, int height, int width, short sHP);
	//Сброс мира
	void Reset();
	//Обновить мир
	void Update(float dt);

	//Добавить пушку
	bool AddCannon(unsigned int slot);
	//Разрушить пушку
	bool DelCannon(unsigned int slot);
	//Удалить мертвых
	bool DelUnit(unsigned int unit);
	//Установка следующей волны
	void SetNextWave(int wave);
	//Запуск следующей волны
	void StartNextWave();
	//Завершение текущей волны
	void EndWave();
	//Пробная волна
	void TryWaveInit();
	




	//Доступ к членам
	//Цели
	std::vector<Runner> & GetUnits() ;
	

	//Пушки
	std::vector<Cannon> & GetCannons() ;
	

	//Слоты
	std::vector<CannonSlot> & GetSlots();
	

	//Игровой таймер
	float GetTimer();
	//Очки
	int GetScore() const;
	//Золото
	int GetGold() const;
	//Волна
	short GetWave() const;
	//HP
	short GetHP() const;

	//Размер мира
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


	//Цели
	std::vector<Runner> units;
	

	//Пушки
	std::vector<Cannon> cannons;
	
	
	//Слоты
	std::vector<CannonSlot> slots;
	

	//Время
	float gameTimer;
	//Очки
	int score;
	//Золото
	int gold;
	//Волна
	short wave;
	//HP
	short hp;

	//Размер мира
	int height;
	int width;


	bool stop;
	bool win;
	bool timeover;
	
};
