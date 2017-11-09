#include "WgClock.h"
#include "../CFontStorage.h"

#define STR_TIME_EMPTY   	"--:--"

#define STR_TIMER_EMPTY		"- - : - -"
#define STR_TIMER_LONG		"> stunda"

#define STR_LECT_EMPTY 		"- - - - - -"
#define STR_LECT_LECTURE	"%d. lekcija"
#define STR_LECT_SHORTBRAKE	"starpbrīdis"
#define STR_LECT_BRAKE		"pārtraukums"
#define STR_LECT_FREETIME	"brīvlaiks"
#define STR_LECT_SESSION	"sessija"

#define STR_INFO_EMPTY		"saraksts nav ielādēts"
#define STR_INFO_ERROR		"saraksts ir kļūdains"
#define STR_INFO_HOUREND	"līdz starpbrīdim"
#define STR_INFO_LECTEND	"līdz lekcijas beigām"
#define STR_INFO_BRAKEEND	"līdz starpbrīža beigām"
#define STR_INFO_LECTBEG	"līdz lekcijas sākumām"
#define STR_INFO_DAYBEG		"līdz lekciju sākumām"
#define STR_INFO_EMPTYDAY	"šodien lekciju nav"


WgClock::WgClock(int Ax, int Ay, wgMode Amode):	WgBackground(Ax, Ay, Amode) 
{
	updateTime = 100; // 10 times per sec

	hour = -1; min = -1; sec = -1;
	strcpy( strTime, STR_TIME_EMPTY );
	strcpy( strLect, STR_LECT_EMPTY );
	strcpy( strInfo, STR_INFO_EMPTY );
	strcpy( strTimer, STR_TIMER_EMPTY );
	printf( "%s\tWgClock widget object is created\n", strNow() );
}

WgClock::~WgClock() {
	printf( "%s\tWgClock widget object is deleted\n", strNow() );
}

//~~~ update ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

bool WgClock::update(){
	time_t _now = time( NULL ); struct tm * now = localtime( &_now );
	if ( now->tm_hour == hour && now->tm_min == min && now->tm_sec == sec ) return false;
	hour = now->tm_hour; min = now->tm_min; sec = now->tm_sec;

	sprintf( strTime, "%02d:%02d", now->tm_hour, now->tm_min );

	int end, lect;
	TimeState ts;
	if ( timetable ) ts = timetable->getCurrentTimeState( end, lect );
	else { ts = tsUnknown; end = -1; lect = -1; }

	if ( ts == tsUnknown ) {
		strcpy( strLect, STR_LECT_EMPTY ); 
		strcpy( strInfo,  STR_INFO_ERROR );
		strcpy( strTimer, STR_TIMER_EMPTY );
		return true;
	}

	switch ( ts ) {
		case tsLecture1h:
		case tsLecture2h:  sprintf( strLect, STR_LECT_LECTURE, lect ); break;
		case tsShortBreak: sprintf( strLect, STR_LECT_SHORTBRAKE ); break;
		case tsBreak:      sprintf( strLect, STR_LECT_BRAKE ); break;
		case tsFree:       sprintf( strLect, STR_LECT_FREETIME ); break;
		case tsSession:    sprintf( strLect, STR_LECT_SESSION ); break;
	}

	switch ( ts ) {
		case tsLecture1h:  strcpy( strInfo, STR_INFO_HOUREND ); break;
		case tsLecture2h:  strcpy( strInfo, STR_INFO_LECTEND ); break;
		case tsShortBreak: strcpy( strInfo, STR_INFO_BRAKEEND ); break;
		case tsBreak:      strcpy( strInfo, STR_INFO_LECTBEG ); break;
		case tsSession:
		case tsFree:       strcpy( strInfo,  end >= 0 ? STR_INFO_DAYBEG : STR_INFO_EMPTYDAY ); break;
	}

	if ( end < 0 ) strcpy( strTimer, STR_TIMER_EMPTY );
	else if ( end > 60 * 60 ) strcpy( strTimer, STR_TIMER_LONG );
	else sprintf( strTimer, "%02d:%02d", end / 60, end % 60 );

	return true;
}

//~~~ rendering  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void WgClock::renderMode1()
{
	renderHeader( strTime );
}

void WgClock::renderMode2(){
	setTextColor( clHaki );
	FontStorage->getFont((char*)"arialBold")->SetSize( desktop->rowHeight / 3 );
	FontStorage->getFont((char*)"arialBold")->TextMid(
		strLect, 
		rectClient.left + ( rectClient.width / 2 ),
		rectClient.top - desktop->rowHeight / 16 * 11 //?
	);
}

void WgClock::renderMode3(){
	setTextColor( clHaki );
	FontStorage->getFont((char*)"arialBold")->SetSize( desktop->rowHeight / 4.5 );
	FontStorage->getFont((char*)"arialBold")->TextMid(
		strInfo, 
		rectClient.left + ( rectClient.width / 2 ),
		rectClient.top - desktop->rowHeight - ( desktop->rowHeight / 5 / 2 ) //?
	);
	
	setTextColor( color );
	FontStorage->getFont((char*)"arialBold")->SetSize( desktop->rowHeight / 2.2 );
	FontStorage->getFont((char*)"arialBold")->TextMid(
		strTimer, 
		rectClient.left + ( rectClient.width / 2 ), 
		rectClient.top - desktop->rowHeight - ( desktop->rowHeight / 4 ) * 3 //?
	);
}

void WgClock::render(){	
	WgBackground::render();
	switch (mode){
		case md1x1:{ renderMode1(); break; }
		case md1x2:{ renderMode1(); renderMode2(); break; }
		case md1x3:{ renderMode1(); renderMode2(); renderMode3(); break; }
	}
}
