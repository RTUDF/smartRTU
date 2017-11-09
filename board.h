#pragma once

#include "widgets/WgClock.h"
#include "widgets/IWidget.h"
#include "Timer.h"

struct WidgetInfo {
	IWidget * widget;
	LongTimeMs lastUpdate;
	bool needRender;
	WidgetInfo * next;
	WidgetInfo(IWidget * w) { widget = w; next = NULL;  lastUpdate = 0; needRender = true; }
	~WidgetInfo() { delete widget; }
};

class Board
{
private:

	WidgetInfo *widgets, *current;

	void _addWidget(WidgetInfo* & list, IWidget * w) { if (list) _addWidget(list->next, w); else list = new WidgetInfo(w); }
	void _freeWidgets(WidgetInfo* & list) { if (list) { _freeWidgets(list->next); delete(list); list = NULL; } }
	int _cntWidgets(WidgetInfo* list) { if (list) return _cntWidgets(list->next) + 1; return 0; }
	
public:	

	Board();
	~Board();
	void update(bool Forced);
	void render(bool Forced);
	void cleanWidgets() { _freeWidgets( widgets ); }
	void addWidget(IWidget *widget) { _addWidget( widgets, widget ); }
	IWidget *findFirst() { 	current = widgets;	return current ? current->widget : NULL; }
	IWidget *findNext() { if ( current ) current = current->next; return current ? current->widget : NULL; }
	WidgetInfo * currentWidget() { return current; }
	int countWidgets() { return _cntWidgets( widgets ); }
	
};
