#pragma once
#include "WorldObject.h"
class CannonSlot : public WorldObject
{


public:
	CannonSlot();
	~CannonSlot();

	//Очистка
	void Clear();
	//Заполнение
	bool Fill(int index);
	//Пустота
	bool IsEmpty();
	//Номер пушки в слоте. Пустой слот вернет -1.
	int GetCannonIndex();

private:
	//Номер пушки в слоте. В пустом номер -1.
	int cannonIndex;
	//Пустота
	bool empty;

};

