#pragma once

class WorldObject
{


public:
	WorldObject();
	~WorldObject();
	//Перемещение
	void SetPos(float x, float y);
	void SetX(float newX);
	void SetY(float newY);

	//Доступ к членам
	float GetX();
	float GetY();
	IPoint GetPos();
	//Дистанция до 
	float GetDistanceTo(float tX, float tY);
	

protected:
	//Координаты
	float x;
	float y;
	


};