/*
    Menu.cpp - Library for graphical menus.
    Created by Daniel Esteban, December 23, 2012.
    Released into the public domain.
*/

#include "Menu.h"

const byte itemsPerPage = 6,
	menuPadding = 20,
	itemMargin = 10;

byte itemPadding,
	lastPress = 255;

UTFT _tft;

TouchEvent __onClick;
TouchEvent __onDown;

UIButton * _items[itemsPerPage];

void onClickHandler(byte id);
void onDownHandler(byte id);
void renderItem(UIButton * b, bool pressed = false);

Menu::Menu(byte numItems, char * items[], TouchEvent onClick, TouchEvent onDown) : UI() {
	_numItems = numItems;
	_onClick = onClickHandler;
	_onDown = onDownHandler;
	__onClick = onClick;
	__onDown = onDown;
	for(byte x=0; x<numItems; x++) addButton(items[x]);
}

void Menu::render(UTFT tft) {
	byte page = 0, 
		offset = page * itemsPerPage,
		x = 0;

	offset >= _numItems && (_page = offset = 0);
	byte to = offset + itemsPerPage;
	to > _numItems && (to = _numItems);

	int itemWidth = tft.getDisplayXSize() - (menuPadding * 2),
		itemHeight = (tft.getDisplayYSize() - (menuPadding * 2) - (itemMargin * (itemsPerPage - 1))) / itemsPerPage;

	itemPadding = (itemHeight - 10) / 2;
	page = _page;

	tft.clrScr();
	tft.setBackColor(255, 255, 255);
	_tft = tft;
	UIButton * b = _buttons;
	while(b != NULL) {
		b->width = itemWidth;
		b->height = itemHeight;
		if(x < offset || x >= to) b->x = b->y = -1;
		else {
			_items[x - offset] = b;
			b->x = menuPadding;
			b->y = menuPadding + ((x - offset) * (itemHeight + itemMargin));
			renderItem(b);
		}
		b = b->next;
		x++;
	}
}

void onClickHandler(byte id) {
	lastPress = 255;
	renderItem(_items[id]);
	if(__onClick != NULL) __onClick(id);
}

void onDownHandler(byte id) {
	if(lastPress != 255) renderItem(_items[lastPress]);
	lastPress = id;
	if(id != 255) renderItem(_items[id], true);
	if(__onDown != NULL) __onDown(id);
}

void renderItem(UIButton * b, bool pressed) {
	if(pressed) {
		_tft.setBackColor(102, 102, 102);
		_tft.setColor(102, 102, 102);
	} else {
		_tft.setBackColor(255, 255, 255);
		_tft.setColor(255, 255, 255);
	}
	_tft.fillRoundRect(b->x, b->y, b->x + b->width, b->y + b->height);
	if(pressed) _tft.setColor(255, 255, 255);
	else _tft.setColor(0, 0, 0);
	_tft.print(b->label, b->x + itemPadding, b->y + itemPadding);
}