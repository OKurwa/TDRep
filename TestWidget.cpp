#include "stdafx.h"
#include <fstream>
#include "FieldMap.h"
#include "FireParent.h"
#include "TowerParent.h"
#include "MonsterParent.h"
#include "TestWidget.h"




TestWidget::TestWidget(const std::string& name, rapidxml::xml_node<>* elem)
	: Widget(name)
	, _curTex(0)
	, _timer(0)
	, _angle(0)
	, _eff(NULL)
	, _scale(0.f)
{
	Init();
}

void TestWidget::Init()
{
	
	_monsterAttack.LoadFromFile("loadMob.txt");
	_fieldMap.LoadFromFile("loadMap.txt");
	
	IPoint curCell;
	for (int i = 0; i < _fieldMap.Size().x; i++) {
		for (int j = 0; j < _fieldMap.Size().y; j++) {
			curCell = IPoint(i* _fieldMap.CellSize().x + 32, j* _fieldMap.CellSize().y + 32);
			CellType cell = _fieldMap.PosCellType(curCell);
			if (cell == SPAWN)
				_spawn = IPoint(i,j);
		}
	}
	//TowerParent * t = new TowerParent(FPoint(32, 32), 0, 0, 0, 0, 0, nullptr);
	//_towers.push_back(t);
	_spawnTimer = 0;
	_spawnTime = 0.4;
	_curMonsterAttack = 0;

	
	
	//for (int i = 0; i < _monsterAttack.GetAttack()[0].Count(); i++) {
	//	_monsters[i]->FindAWay();
	//}
	
	//_fieldMap.TryInit();
	//_fieldMap.SaveToFile("save.txt");




}

void TestWidget::Draw()
{
	IPoint fieldSize = _fieldMap.Size();
	IPoint cellSize = _fieldMap.CellSize();

	//Render::device.PushMatrix();
	
	//Render::device.MatrixTranslate(512, 384, 0);
	//Render::device.MatrixScale(1, 0.5, 1);
	//Render::device.MatrixRotate(math::Vector3(0, 0, 1), 45);
	//Render::device.MatrixTranslate(-fieldSize.x * cellSize.x / 2, -fieldSize.y*cellSize.y / 2, 0);
	Render::device.SetTexturing(false);
	_fieldMap.Draw();
	for (unsigned int i = 0; i < _monsters.size();i++) {
		_monsters[i]->Draw();
	}
	for (unsigned int i = 0; i < _towers.size(); i++) {
		_towers[i]->Draw();
	}
	Render::device.SetTexturing(true);
	//Render::device.PopMatrix();
	



}

void TestWidget::Update(float dt)
{	
	_fieldMap.Update(dt);
	for (unsigned int i = 0; i < _monsters.size(); i++) {
		_monsters[i]->Update(dt);
	}
	for (unsigned int i = 0; i < _towers.size(); i++) {
		_towers[i]->Update(dt);
	}

	for (unsigned int i = 0; i < _towers.size(); i++) {
		if (_towers[i]->TakeAim(&_monsters)) {
			_towers[i]->Shoot();
		}
	}

	//Задержка спавна монстра
	_spawnTimer += dt;

	//Cпавн
	if (_curMonsterAttack < _monsterAttack.GetAttack().size()) {
		if (_monsterAttack.GetAttack()[_curMonsterAttack].Count()>_monsters.size() && _spawnTimer>_spawnTime) {
			
			boost::intrusive_ptr<MonsterParent> m = new NormalMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31),_spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), 64, 200, &_fieldMap, nullptr);
			_monsters.push_back(m);

		}
	}
	//Проверка окончания волны
	float endWave = true;
	for (unsigned int i = 0; i < _monsters.size(); i++) {
		if (_monsters[i]->WayDistance()>0)
			endWave = false;
	}
	//Очистка старой и начало новой
	if (endWave && _monsters.size() > 0) {
		_monsters.clear();
		++_curMonsterAttack;
	}
	//Сброс задержки спавна монстра
	if (_spawnTimer > _spawnTime)
		_spawnTimer = 0;
	


	
}

bool TestWidget::MouseDown(const IPoint &mouse_pos)
{	
	math::Matrix4 m = math::Matrix4(1, 0, 0, 0,
									0, 1, 0, 0,
									0, 0, 1, 0,
									0, 0, 0, 1);
	
	math::Vector4 p=  math::Vector4(mouse_pos.x, mouse_pos.y, 0, 1);

	
	IPoint fieldSize = _fieldMap.Size();
	IPoint cellSize = _fieldMap.CellSize();
	
	
	//p = p.Transform(m.Translation(fieldSize.x * cellSize.x / 2, fieldSize.y*cellSize.y / 2, 0));
	//p = p.Transform(m.RotationAxis(math::Vector3(0, 0, 1), -45));
	//p = p.Transform(m.Scaling(1, 2, 1));
	//p = p.Transform(m.Translation(-512, -384, 0));
	
	IPoint pos1 = _fieldMap.PosCell(FPoint(p.x, p.y));
	_fieldMap.SelectCell(FPoint(p.x, p.y));

	////////////////////////////////////////
	//			 Создание башни           //
	////////////////////////////////////////


	if (Core::mainInput.GetMouseLeftButton()) {
		if (_fieldMap.AddTower(pos1)) {
			
			//Разрывная, направленная на точку, урон по области радиусом 60 точек
			//boost::intrusive_ptr<TowerParent> t = new SplashTower(FPoint(pos1.x*cellSize.x + cellSize.x / 2, pos1.y*cellSize.y + cellSize.y / 2), pos1, 0.5, 0, 192, 60, 0, IPoint(5, 10), nullptr);
			
			//Оглушающая, направленная на монстра, урон по 1 цели, 50% шанс оглушения на 1 секунду
			//boost::intrusive_ptr<TowerParent> t = new BashTower(FPoint(pos1.x*cellSize.x + cellSize.x / 2, pos1.y*cellSize.y + cellSize.y / 2), pos1, 0.5, 0, 192, FPoint(0.5, 1), 0,IPoint(5, 10), nullptr);

			//Замедляющая, направленная на точку, урон по области радиусом 30 точек, замедление 70% на 1 секунду 
			//boost::intrusive_ptr<TowerParent> t = new SlowTower(FPoint(pos1.x*cellSize.x + cellSize.x/2, pos1.y*cellSize.y + cellSize.y/2), pos1, 0.5, 0, 192,30, FPoint(0.7,1),0,IPoint(5, 10), nullptr);
			
			//Отравляющая, направленная на монстра, урон по времени
			boost::intrusive_ptr<TowerParent> t = new DecayTower(FPoint(pos1.x*cellSize.x + cellSize.x / 2, pos1.y*cellSize.y + cellSize.y / 2), pos1, 0.5, 0, 192, FPoint(15,10),0, IPoint(0, 0), nullptr);
			
			//Обычная, направленная на монстра, урон по 1 цели
			//boost::intrusive_ptr<TowerParent> t = new NormalTower(FPoint(pos1.x*cellSize.x + cellSize.x / 2, pos1.y*cellSize.y + cellSize.y / 2), pos1, 0.5, 0, 192, 0,IPoint(100, 100), nullptr);
			_towers.push_back(t);
		};
		
	}

	////////////////////////////////////////
	//			 Удаление башни           //
	////////////////////////////////////////

	if (Core::mainInput.GetMouseRightButton()) {
		if (_fieldMap.DestroyTower(pos1)) {
			int destrIndex = 0;
			bool found = false;
			for (int i = 0; i < _towers.size(); i++) {
				if (_towers[i]->Cell() == pos1) {
					found = true;
					destrIndex = i;
				}
			}
			if (found) {
				_towers.erase(_towers.begin() + destrIndex);
			}
		};

	}
	return false;
}

void TestWidget::MouseMove(const IPoint &mouse_pos)
{
	
}

void TestWidget::MouseUp(const IPoint &mouse_pos)
{
	
}

void TestWidget::AcceptMessage(const Message& message)
{
	//
	// Виджету могут посылаться сообщения с параметрами.
	//
	// Например, при вводе символа с клавиатуры виджетам на активном слое посылается
	// сообщение KeyPress с кодом символа в параметре.
	//
	if (message.getPublisher() == "KeyPress") 
	{
		int code = utils::lexical_cast<int>(message.getData());
		if (code < 0)
		{
			//
			// Отрицательные значения посылаются для виртуальных кодов клавиш,
			// например, -VK_RETURN, -VK_ESCAPE и т.д.
			//
			return;
		}
		
		//
		// Положительные значения посылаются для символов алфавита.
		//
		if (code == 'a')
		{
		}
	}
}
