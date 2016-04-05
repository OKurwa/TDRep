#pragma once

class WorldObject
{


public:
	WorldObject();
	~WorldObject();
	//�����������
	void SetPos(float x, float y);
	void SetX(float newX);
	void SetY(float newY);

	//������ � ������
	float GetX();
	float GetY();
	IPoint GetPos();
	//��������� �� 
	float GetDistanceTo(float tX, float tY);
	

protected:
	//����������
	float x;
	float y;
	


};