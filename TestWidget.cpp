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
	//_fieldMap.LoadFromFile("loadMap.txt");
	_fieldMap.LoadFromXml("NewMap.xml");

	IPoint curCell;
	for (int i = 0; i < _fieldMap.Size().x; i++) {
		for (int j = 0; j < _fieldMap.Size().y; j++) {
			curCell = IPoint(i* _fieldMap.CellSize().x + 32, j* _fieldMap.CellSize().y + 32);
			CellType cell = _fieldMap.PosCellType(curCell);
			if (cell == SPAWN)
				_spawn = IPoint(i, j);
		}
	}
	_texButtons = Core::resourceManager.Get<Render::Texture>("Towers");
	//TowerParent * t = new TowerParent(FPoint(32, 32), 0, 0, 0, 0, 0, nullptr);
	//_towers.push_back(t);
	_towerPs.push_back(_towerFactory.createNormal());
	_towerPs.push_back(_towerFactory.createSplash());
	_towerPs.push_back(_towerFactory.createSlow());
	_towerPs.push_back(_towerFactory.createDecay());
	_towerPs.push_back(_towerFactory.createBash());
    //anim = Core::resourceManager.Get<Render::Animation>("FireAntAttackAnimation");
}

void TestWidget::Draw()
{
	
	IPoint fieldSize = _fieldMap.Size();
	IPoint cellSize = _fieldMap.CellSize();

	IRect bRect = IRect(960, 256, 64, 64);
	FRect uvRect = FRect(0, 0.2, 0, 1.0);
	_texButtons->Draw(bRect, uvRect);
	bRect = IRect(960, 192, 64, 64);
	uvRect = FRect(0.2, 0.4, 0, 1.0);
	_texButtons->Draw(bRect, uvRect);
	bRect = IRect(960, 128, 64, 64);
	uvRect = FRect(0.4, 0.6, 0, 1.0);
	_texButtons->Draw(bRect, uvRect);
	bRect = IRect(960, 64, 64, 64);
	uvRect = FRect(0.6, 0.8, 0, 1.0);
	_texButtons->Draw(bRect, uvRect);
	bRect = IRect(960, 0, 64, 64);
	uvRect = FRect(0.8, 1.0, 0, 1.0);
	_texButtons->Draw(bRect, uvRect);
	
	
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
	
	World::Instance().Draw();
	//anim->Draw(IPoint(0, 0));

}

void TestWidget::Update(float dt)
{	

	//anim->Update(dt*0.5);
	if (World::Instance().State() == LOSE || World::Instance().State() == WIN)
		dt *= 0;
	//dt *= 0.5;
	_fieldMap.Update(dt);
	for (unsigned int i = 0; i < _monsters.size(); i++) {
		_monsters[i]->Update(dt);
	}
	for (unsigned int i = 0; i < _towers.size(); i++) {
		_towers[i]->Update(dt);
	}

	for (unsigned int i = 0; i < _towers.size(); i++) {
		//if (_towers[i]->TakeAim(_monsters)) {
		//	_towers[i]->Shoot();
		//}
		_towers[i]->TryShoot(_monsters);
	}

	//Задержка спавна монстра
	_spawnTimer += dt;

	if (World::Instance().State() == WAVE) {
		//Cпавн
		if (_curMonsterAttack < _monsterAttack.GetAttack().size()) {
			
			if (_monsterAttack.GetAttack()[_curMonsterAttack].Count()>_curMonsterCount && _spawnTimer>_spawnTime) {
				int hp = _monsterAttack.GetAttack()[_curMonsterAttack].MaxHp();
				int spd = _monsterAttack.GetAttack()[_curMonsterAttack].Speed();
				MonsterParent::Ptr m;
				m = _monsterAttack.GetAttackPrototypes()[_curMonsterAttack]->clone();
				m->SetPosition(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), &_fieldMap);
				
				/*
				
				if (_monsterAttack.GetAttack()[_curMonsterAttack].Type() == "Normal") {
					//Обычный монстр
					m = new NormalMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), spd, hp, &_fieldMap, nullptr);
				}
				else if (_monsterAttack.GetAttack()[_curMonsterAttack].Type() == "Boss") {
					//Босс
					m = new BossMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), spd, hp, &_fieldMap, 0.5, nullptr);
				}
				else if (_monsterAttack.GetAttack()[_curMonsterAttack].Type() == "Healing") {
					//Регенерирующий монстр
					m = new HealingMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), spd, hp, &_fieldMap, nullptr, 5);
				}
				else if (_monsterAttack.GetAttack()[_curMonsterAttack].Type() == "Immune") {
					//Иммунный монстр
					m = new ImmuneMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), spd, hp, &_fieldMap, nullptr);
				}
				else {
					//Обычный монстр
					m = new NormalMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), spd, hp, &_fieldMap, nullptr);
				}
				*/

				//Регенерирующий монстр
				//boost::intrusive_ptr<MonsterParent> m = new HealingMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), 64, 200, &_fieldMap, nullptr, 5);

				//Иммунный монстр
				//boost::intrusive_ptr<MonsterParent> m = new ImmuneMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), 64, 200, &_fieldMap, nullptr);

				//Босс
				//boost::intrusive_ptr<MonsterParent> m = new BossMonster(FPoint(_spawn.x * _fieldMap.CellSize().x + 32 + math::random(-31, 31), _spawn.y * _fieldMap.CellSize().y + 32 + math::random(-31, 31)), 64, 600, &_fieldMap, 0.5, nullptr);

				++_curMonsterCount;
				_monsters.push_back(m);

			}
		}

		for (std::vector<MonsterParent::Ptr>::iterator it = _monsters.begin(); it != _monsters.end();) {
			if ((*it)->Dead() && (*it)->EndDeadAnim()) {
				World::Instance().GoldAdd(_monsterAttack.GetAttack()[_curMonsterAttack].MGold());
				it = _monsters.erase(it);
			}else if((*it)->Finish()) {
				//World::Instance().GoldAdd(_monsterAttack.GetAttack()[_curMonsterAttack].MGold());
				World::Instance().LoseLife();
				it = _monsters.erase(it);
			}
			else {
				it++;
			}

		}
		//Проверка окончания волны
		float endWave = true;
		for (unsigned int i = 0; i < _monsters.size(); i++) {
			if (!_monsters[i]->Finish() && !_monsters[i]->Dead())
				endWave = false;
		}
		//Очистка старой и начало новой
		if (endWave && _curMonsterCount>0) {
			_monsters.clear();
			_curMonsterCount = 0;
			if(_curMonsterAttack+1<_monsterAttack.GetAttack().size())
 				++_curMonsterAttack;
			World::Instance().SetNewAttack(_monsterAttack.Delay(), _monsterAttack.GetAttack()[_curMonsterAttack]);
			//World::Instance().SetAttackIndex(_curMonsterAttack);
		}
		
	}

	//Сброс задержки спавна монстра
	if (_spawnTimer > _spawnTime)
		_spawnTimer = 0;
	
	
	World::Instance().Update(dt);
	
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
			
			TowerParent::Ptr t;
			switch (_curTowerType)
			{
			case NORMAL:
				t = _towerFactory.createNormal();
				break;
			case SPLASH:
				t = _towerFactory.createSplash();
				break;
			case SLOW:
				t = _towerFactory.createSlow();
				break;
			case DECAY:
				t = _towerFactory.createDecay();
				break;
			case BASH:
				t = _towerFactory.createBash();
				break;
			default:
				t = _towerFactory.createNormal();
				break;
			}

			if (World::Instance().GoldSpend(t->Price())) {
				t->SetCell(pos1);
				t->SetPosition(FPoint(pos1.x*cellSize.x + cellSize.x / 2, pos1.y*cellSize.y + cellSize.y / 2));
				_towers.push_back(t);
			}
			else{
				bool b=_fieldMap.DestroyTower(pos1);
			}
     			
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
		if (code == '1')
		{
			_curTowerType = NORMAL;
		}
		if (code == '2')
		{
			_curTowerType = SPLASH;
		}
		if (code == '3')
		{
			_curTowerType = SLOW;
		}
		if (code == '4')
		{
			_curTowerType = DECAY;
		}
		if (code == '5')
		{
			_curTowerType = BASH;
		}
		if (code == 's') {
			_fieldMap.Reset();
			_towers.clear();
			_monsters.clear();
			//_monsterAttack.LoadFromFile("loadMob.txt");
			_monsterAttack.LoadFromXml("NewMap.xml");
			_spawnTimer = 0;
			_spawnTime = 0.4;
			_curMonsterAttack = 0;
			_curTowerType = NORMAL;
			_curMonsterCount = 0;

			//for (int i = 0; i < _monsterAttack.GetAttack()[0].Count(); i++) {
			//	_monsters[i]->FindAWay();
			//}

			//_fieldMap.TryInit();
			//_fieldMap.SaveToFile("save.txt");

			World::Instance().Init(100, _monsterAttack.GetAttack().size(), _monsterAttack.GetAttack()[0].Count(), _monsterAttack.GetAttack()[0].Count(), 20, _monsterAttack);





			World::Instance().SetNewAttack(_monsterAttack.Delay(), _monsterAttack.GetAttack()[0]);
		}
		
	}
}

bool TestWidget::LoadMapFromXml(std::string filename) {

	




	try {
		rapidxml::file<> file(filename.c_str());
		// Может бросить исключение, если нет файла.

		rapidxml::xml_document<> doc;
		doc.parse<0>(file.data());
		// Может бросить исключение, если xml испорчен.

		rapidxml::xml_node<>* root = doc.first_node();
		if (!root) { Assert(false); throw std::runtime_error("No root node"); }

		rapidxml::xml_node<>* game = root->first_node("Game");
		for (; game != NULL; game = game->next_sibling("Game")) {
			rapidxml::xml_node<>* r = game->first_node();
			for (; r != NULL; r = r->next_sibling()) {
				if (utils::equals(r->name(), "Map")) {
					
				}
			}
		}
	}
	catch (std::exception const& e) {
		Log::log.WriteError(e.what());
		Assert(false);
	}
	return false;
};