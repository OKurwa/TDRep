#include "stdafx.h"
#include "Menu.h"

Button::Button() {
	_tex = nullptr;
	_rect = IRect(0,0,0,0);
	_buttonUVRect  = FRect(0,0,0,0);
	_pos = FPoint(0, 0);
	_menuPos = IPoint(0,0);
	_size = IPoint(0, 0);
	_type = EMPTY;
	_empty = true;
	_light = false;
	_pressed = false;
};
Button::Button(ButtonInfo info) {
	_tex = info._tex;
	
	_buttonUVRect = info._buttonUVRect;
	_pos =info._pos;
	_menuPos = info._menuPos;
	_size = info._size;
	_type = info._type;
	_rect = IRect(_pos.x+1, _pos.y+1, _size.x-1, _size.y-1);
	_empty = true;
	_light = false;
	_pressed = false;
};

void Button::Draw() {
	if (_tex) {
		_tex->Draw(_rect, _buttonUVRect); // Рисуем текстуру.
		
		if (_light && !_pressed) {
			Render::device.SetBlendMode(BlendMode::Add); // Включаем аддитивный блендинг.
			Render::BeginAlphaMul(0.5f); // Дальше всё будет полупрозрачное.
			_tex->Draw(_rect, _buttonUVRect); // “Прибавляем” текстуру саму к себе, что даёт эффект свечения.

			Render::EndAlphaMul();
			Render::device.SetBlendMode(BlendMode::Alpha); // Возвращаем нормальный блендинг.
		}
		
		if (_pressed) {
			Render::device.SetBlendMode(BlendMode::Add); // Включаем аддитивный блендинг.
			
			_tex->Draw(_rect, _buttonUVRect); // “Прибавляем” текстуру саму к себе, что даёт эффект свечения.
			
			
			Render::device.SetBlendMode(BlendMode::Alpha); // Возвращаем нормальный блендинг.
		}
		

	}
	else {
		Render::device.SetTexturing(false);
		Render::BeginColor(Color(255,255,255,255));
		Render::DrawRect(_rect);
		Render::EndColor();
		Render::device.SetTexturing(true);
		
	}
};

void Button::Update(float dt) {
};

void Button::SetLighter(IPoint mPos) {

	if (mPos.x>_pos.x && mPos.y>_pos.y && mPos.x<_pos.x+64 && mPos.y<_pos.y+64) {
		_light = true;
	}
	else {
		_light = false;
	}
};

TowerType Button::Press(IPoint mPos) {
	if (mPos.x>_pos.x && mPos.y>_pos.y && mPos.x<_pos.x + _size.x && mPos.y<_pos.y + _size.x) {
		_pressed = !_pressed;
		if (_pressed) {
			return _type;
		}
		else {
			return EMPTY;
		}
	}
	else {
		_pressed = false;
		return EMPTY;
	}
	
	
};
void Button::Reset() {
	_pressed = false;
	_light = false;
};


Menu::Menu() {};
Menu::Menu(MenuInfo info) {

	_rect=info._rect;
	_buttonSize=info._buttonSize;
	_size=info._size;
	_buttonsTex=info._buttonsTex;
	int k = 0;

	for (int i = 0; i < 2; i++) {
		std::vector<Button::Ptr> vec;
		_buttons.push_back(vec);
		for (int j = 0; j < 3; j++) {
			Button::ButtonInfo info;
			info._tex = Core::resourceManager.Get<Render::Texture>("Towers");
			info._empty = false;
			info._menuPos = IPoint(i, j);
			info._pos = FPoint(_rect.LeftTop().x  + _buttonSize.x *(i), _rect.LeftTop().y - _buttonSize.y *(j + 1));
			info._size = _buttonSize;
			if ((i * 3 + j)* 1.0 / 6.0 + 1.0 / 6.0 <= 1) {
				info._buttonUVRect = FRect((i * 3 + j)*1.0/6.0, (i * 3 + j)*1.0 / 6.0 + 1.0 / 6.0, 0, 1.0);
			}
			else {
				info._buttonUVRect = FRect(0.8, 1.0, 0, 1.0);
			}
			
			info._type = static_cast<TowerType>(i * 3 + j);
			Button::Ptr button = new Button(info);
			_buttons[i].push_back(button);
		}
		k++;
	}
};
Menu::~Menu() {};
TowerType Menu::Press(IPoint mPos, TowerType cur) {
	TowerType tmp = cur;
	bool allEmpty = true;
	for (int i = 0; i < _buttons.size(); i++) {
		for (int j = 0; j < _buttons[i].size(); j++) {
			
			if (_rect.Contains(mPos)) {
				if (_buttons[i][j]) {
					tmp = _buttons[i][j]->Press(mPos);
					
					if (tmp != EMPTY ) {
						cur = tmp;
						allEmpty = false;
					}
				}
			}
			else {
				allEmpty = false;
			}
			
			
		}
	}
	if (allEmpty)
		cur = EMPTY;
	return cur;
};

void Menu::Draw() {
	for (int i = 0; i < _buttons.size(); i++) {
		for (int j = 0; j < _buttons[i].size(); j++) {
			if(_buttons[i][j])
				_buttons[i][j]->Draw();
		}
	}
};


void Menu::Update(float dt) {
	for (int i = 0; i < _buttons.size(); i++) {
		for (int j = 0; j < _buttons[i].size(); j++) {
			if (_buttons[i][j])
				_buttons[i][j]->Update(dt);
		}
	}
};

void Menu::SetLighter(IPoint pos) {
	for (int i = 0; i < _buttons.size(); i++) {
		for (int j = 0; j < _buttons[i].size(); j++) {
			if (_buttons[i][j])
				_buttons[i][j]->SetLighter(pos);
		}
	}
};
void Menu::Reset() {
	for (int i = 0; i < _buttons.size(); i++) {
		for (int j = 0; j < _buttons[i].size(); j++) {
			if (_buttons[i][j])
				_buttons[i][j]->Reset();
		}
	}
};