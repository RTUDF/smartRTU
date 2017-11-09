#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <ctime>

#include "lib/json.h"
#include "Timer.h"

using namespace std;
using json = nlohmann::json;

#define DEFAULT_TIMETABLE_FILE "timetable.json"

struct TimetableDate { 
	int y, m, d; 
	TimetableDate( json &tt ) {
        y = tt.value("y", 0);
        if (y <= 0) {
            time_t t = time( NULL );
            struct tm * now = localtime( & t );
            y = now->tm_year + 1900;
        }
        if (y < 1000) y += 2000;
		m = tt["m"];
		d = tt["d"];
    }
    TimetableDate( struct tm date ) {
        d = date.tm_mday;
        m = date.tm_mon + 1;
        y = date.tm_year + 1900;
    }
    TimetableDate( const TimetableDate & date ) {
        d = date.d;
        m = date.m;
        y = date.y;
    }
};

struct TimetableDateRange {
    enum { drUnknown, drSemester, drSession } type;
    int number;
    TimetableDate begin, end;
    TimetableDateRange( json &tt ): begin( tt["begin"] ), end( tt["end"] ) {
        if ( tt["semester"].is_number_integer() ) { type = drSemester; number = tt["semester"]; }
        else if ( tt["session"].is_number_integer() ) { type = drSession; number = tt["session"]; }
        else { type = drUnknown; number = tt["number"]; }
    }
};

struct TimetableTime { 
	int h, m; 
	TimetableTime( json &tt ) {
		m = tt["m"];
		h = tt["h"];
    }
};

struct TimetableLecture {  
	TimetableTime begin, beginBreak, endBreak, end; 
	TimetableLecture( json &tt ): 
		begin(tt["begin"]), 
		end(tt["end"]), 
		beginBreak(tt["begin-break"]), 
		endBreak(tt["end-break"]) 
    {}
    bool isFinished( struct tm time ); 
    bool isNotStarted( struct tm time );
    bool isRunning( struct tm time );
    bool isBreak( struct tm time );
    bool isFirstHour( struct tm time );
    bool isSecondHour( struct tm time );
};

struct TimetableWeekDay {
	int count; 
	TimetableLecture ** lectures; 
	TimetableWeekDay( json &tt, json &defs );
	~TimetableWeekDay();
};

struct TimetableDay : TimetableWeekDay {
    TimetableDate date;
    TimetableDay( json &tt, json &defs ): TimetableWeekDay( tt["lectures"], defs ), date( tt["date"] ) {}
    ~TimetableDay() {}
};

struct TimeTableSocketTime {
    TimetableTime on, off;
    TimeTableSocketTime ( json &tt ): on( tt["on"] ), off ( tt["off"] ) {}
    bool isSwitchedOn( struct tm time );
};

struct TimetableSocket {
    int socket;
    TimeTableSocketTime * weekdays;
    TimeTableSocketTime * sundays;
    TimeTableSocketTime * holidays;
    TimetableSocket ( json &tt, int sock ) {
        socket = sock;
        if ( tt["weekdays"].size() ) weekdays = new TimeTableSocketTime( tt["weekdays"] ); else weekdays = NULL;
        if ( tt["sundays"].size() )  sundays =  new TimeTableSocketTime( tt["sundays"] );  else sundays = NULL;
        if ( tt["holidays"].size() ) holidays = new TimeTableSocketTime( tt["holidays"] ); else holidays = NULL;
    }
    ~TimetableSocket() {
        if ( weekdays ) delete weekdays;
        if ( sundays )  delete sundays;
        if ( holidays ) delete holidays;
    }
};

enum TimeState { tsUnknown, tsLecture1h, tsLecture2h, tsShortBreak, tsBreak, tsFree, tsSession };
enum DateState { dsUnknown, dsSemester, dsSession, dsVacation,  dsHoliday  };

extern struct tm makeNow();

class Timetable {
private:
    TimetableWeekDay * Week[7];
    int SinglesCount;
    TimetableDay ** Singles;
    int HolidaysCount;
    TimetableDate ** Holidays;
    int CalendarCount;
    TimetableDateRange ** Calendar;
    int SocketsCount;
    TimetableSocket ** Sockets;

    int getWeekNumber( const TimetableDate & origin, struct tm now = makeNow()  );
    int getWeeksCount( const TimetableDateRange * dates );
public:
    TimeState getCurrentTimeState( int & secToEnd, int & lectNumber );
    DateState getCurrentDateState( int & weekNumber );
    bool getCurrentSocketState( int Socket );
    
    int getHoliday( struct tm now = makeNow() );
        
	Timetable( const char * FileName = DEFAULT_TIMETABLE_FILE );
    ~Timetable();
};

extern Timetable * timetable;