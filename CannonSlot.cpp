#include "stdafx.h"
#include "CannonSlot.h"
#include "WorldObject.h"


CannonSlot::CannonSlot()
{
	x = 0;
	y = 0;
	empty = true;
}


CannonSlot::~CannonSlot()
{
}

//�������
void CannonSlot::Clear() {
	empty = true;
	cannonIndex = -1;
};
//���������
bool CannonSlot::Fill(int index) {
	if (index >= 0) {
		empty = false;
		cannonIndex = index;
		return true;
	}
	else{
		return false;
	}
	
};

bool CannonSlot::IsEmpty() {
	return empty;
};

//����� ����� � �����. ������ ���� ������ -1.
int CannonSlot::GetCannonIndex() {
	return cannonIndex;
};