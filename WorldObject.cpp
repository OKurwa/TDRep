#include "stdafx.h"
#include "WorldObject.h"


WorldObject::WorldObject() {};
WorldObject::~WorldObject() {};


//�����������
void WorldObject::SetPos(float newX, float newY) {
	x = newX;
	y = newY;
};

void WorldObject::SetX(float newX) {
	x = newX;
};

void WorldObject::SetY(float newY) {
	y = newY;
};

//������ � ������
float WorldObject::GetX() {
	return x;
};

float WorldObject::GetY() {
	return y;
};

IPoint WorldObject::GetPos() {
	return IPoint(round(x), round(y));
};

//��������� �� 
float WorldObject::GetDistanceTo(float tX, float tY) {
	return sqrt((tX - x)*(tX - x) + (tY - y)*(tY - y));
};