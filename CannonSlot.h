#pragma once
#include "WorldObject.h"
class CannonSlot : public WorldObject
{


public:
	CannonSlot();
	~CannonSlot();

	//�������
	void Clear();
	//����������
	bool Fill(int index);
	//�������
	bool IsEmpty();
	//����� ����� � �����. ������ ���� ������ -1.
	int GetCannonIndex();

private:
	//����� ����� � �����. � ������ ����� -1.
	int cannonIndex;
	//�������
	bool empty;

};

