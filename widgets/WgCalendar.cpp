#include "WgCalendar.h"

using namespace std;

WgCalendar::WgCalendar(int Ax, int Ay, wgMode Amode): WgBackground(Ax, Ay, Amode) 
{
	updateTime = 60 * 60 * 1000; // 1 hour
	strcpy( bufDate, "- -" );
	strcpy( bufWeekDay, "-" );
	strcpy( bufWeekInfo, "- - -" );
	strcpy( bufWeek, "-" );
	printf( "%s\tWgCalendar widget object is created\n", strNow() );
}

WgCalendar::~WgCalendar()
{
	printf( "%s\tWgCalendar widget object is deleted\n", strNow() );
}

const char* WgCalendar::convertWeekDayFromInt(int wday)
{
	switch (wday)
	{
		case 0: return "svētdiena";
		case 1: return "pirmdiena";
		case 2: return "otrdiena";
		case 3: return "trešdiena";
		case 4: return "ceturtdiena";
		case 5: return "piektdiena";
		case 6: return "sestdiena";
		default: return "- - - -";
	}
}

const char* WgCalendar::convertMounthFromInt(int mon)
{
	switch (mon)
	{
		case 0: return "jan";
		case 1: return "feb";
		case 2: return "mar";
		case 3: return "apr";
		case 4: return "mai";
		case 5: return "jūn";
		case 6: return "jūl";
		case 7: return "aug";
		case 8: return "sep";
		case 9: return "okt";
		case 10: return "nov";
		case 11: return "dec";
		default: return "---";
	}
}

bool WgCalendar::update()
{
	time_t lt = time(NULL); struct tm * now = localtime(&lt);
	
	sprintf( bufDate, "%i.%s", now->tm_mday, convertMounthFromInt(now->tm_mon) );

	sprintf( bufWeekDay, "%s", convertWeekDayFromInt(now->tm_wday) );

	int week = 0;
	switch ( timetable->getCurrentDateState( week ) ) {
		case dsSemester : strcpy ( bufWeekInfo, "nedēļa semestrī" ); break;
		case dsSession : strcpy ( bufWeekInfo, "sesijas nedēļa" ); break;
		case dsVacation: strcpy ( bufWeekInfo, "brīvlaika nedēļa" ); break;
		case dsHoliday: strcpy ( bufWeekInfo, "svētki" ); break;
	}
	week += 1;

	if (week >= 1) sprintf( bufWeek, "%d.", week );
	else strcpy( bufWeek, "--" );

	return true;
}

void WgCalendar::renderMode1()
{
	renderHeader( bufDate );
}

void WgCalendar::renderMode2()
{
	TFont * font = FontStorage->getFont((char*)"arialBold");
	setTextColor( clHaki );
	font->SetSize( desktop->rowHeight / 3 );
	font->TextMid( 
		bufWeekDay, 
		rectClient.left + rectClient.width / 2,
		rectClient.top - desktop->rowHeight / 16 * 11
	);
}

void WgCalendar::renderMode3()
{
	TFont * font = FontStorage->getFont((char*)"arialBold");
	font->SetSize( desktop->rowHeight / 4.5 );
	font->TextMid(
		bufWeekInfo, 
		rectClient.left + rectClient.width / 2,
		rectClient.top - desktop->rowHeight - desktop->rowHeight / 5 / 2
	);

	setTextColor(color);
	font->SetSize( desktop->rowHeight / 2.2 );
	font->TextMid(
		bufWeek, 
		rectClient.left + rectClient.width / 2, 
		rectClient.top - desktop->rowHeight - desktop->rowHeight * 3 / 4
	);
}

void WgCalendar::render()
{
	WgBackground::render();
	
	switch (mode){
		case md1x1:{ renderMode1(); break; }
		case md1x2:{ renderMode1(); renderMode2(); break; }
		case md1x3:{ renderMode1(); renderMode2(); renderMode3(); break; }
	}
}
