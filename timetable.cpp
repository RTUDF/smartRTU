#include "timetable.h"

Timetable * timetable = NULL;

//~~~ itoa ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const char* itoa(int val){
	static char buf[32] = { 0 };
	if (val) {
		int i = 30;
		for(; i && val; --i, val /= 10) {
			buf[i] = "0123456789"[val % 10];
		}
		return &buf[i+1];
	}
	else {
		buf[0] = '0';
		buf[1] = '\0';
		return buf;
	}
}

//~~~ TimetableWeekDay ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TimetableWeekDay::TimetableWeekDay( json &tt, json &defs )  {
    if ( tt.is_string() ) {
        std::string def = tt;
        tt = defs[ def ];
    }
    count = tt["lectures-count"]; 
    if (count) {
        lectures = new TimetableLecture* [count];
        for (int i = 0; i < count; i++) {
            lectures[i] = new TimetableLecture( tt[itoa( i + 1 )] );
        }
    }
    else lectures = NULL;
}

TimetableWeekDay::~TimetableWeekDay() { 
    if ( !lectures ) return;
    for (int i = 0; i < count; i++) delete lectures[i];
    delete lectures; 
}

//~~~ operators ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct tm makeTime( const time_t * time ) { return *localtime( time ); }
struct tm makeNow() { time_t now = time( NULL ); return *localtime( &now ); }

int secDay( const TimetableTime & t ) { return t.h * 60 * 60 + t.m * 60; }
int secDay( struct tm t ) { return t.tm_hour * 60 * 60 + t.tm_min * 60 + t.tm_sec; }

int secDif( struct tm t1, const TimetableTime & t2 ) { return secDay(t2) - secDay(t1); }
int secDif( const TimetableTime & t1, struct tm t2 ) { return secDay(t2) - secDay(t1); }

bool operator <  ( struct tm t1, const TimetableTime & t2 ) { return secDay(t1) < secDay(t2); }
bool operator >  ( struct tm t1, const TimetableTime & t2 ) { return secDay(t1) > secDay(t2); }
bool operator <= ( struct tm t1, const TimetableTime & t2 ) { return secDay(t1) <= secDay(t2); }
bool operator >= ( struct tm t1, const TimetableTime & t2 ) { return secDay(t1) >= secDay(t2); }
bool operator <  ( const TimetableTime & t1, struct tm t2 ) { return secDay(t1) < secDay(t2); }
bool operator >  ( const TimetableTime & t1, struct tm t2 ) { return secDay(t1) > secDay(t2); }
bool operator <= ( const TimetableTime & t1, struct tm t2 ) { return secDay(t1) <= secDay(t2); }
bool operator >= ( const TimetableTime & t1, struct tm t2 ) { return secDay(t1) >= secDay(t2); }

bool TimetableLecture::isFinished   ( struct tm time ) { return end < time; }
bool TimetableLecture::isNotStarted ( struct tm time ) { return begin > time; }
bool TimetableLecture::isRunning    ( struct tm time ) { return begin <= time && end >= time; }
bool TimetableLecture::isBreak      ( struct tm time ) { return beginBreak <= time && endBreak >= time;  }
bool TimetableLecture::isFirstHour  ( struct tm time ) { return begin <= time && beginBreak > time;  }
bool TimetableLecture::isSecondHour ( struct tm time ) { return endBreak < time && end >= time;  }

int compareDate(int d1, int m1, int y1, int d2, int m2, int y2) {
    if (y1 < y2) return -1;
    else if (y1 > y2) return 1;
    else if (m1 < m2) return -1;
    else if (m1 > m2) return 1;
    else if (d1 < d2) return -1;
    else if (d1 > d2) return 1;
    return 0;
}

int compareDate(const TimetableDate & d1, struct tm d2) {
    return compareDate(d1.d, d1.m, d1.y, d2.tm_mday, d2.tm_mon + 1, d2.tm_year + 1900);
}

TimetableDate datePlusDays( const TimetableDate & date, int days )
{
    struct tm d = { 0 };
    d.tm_year = date.y - 1900; 
    d.tm_mon = date.m - 1;
    d.tm_mday = date.d;
    
    const time_t ONE_DAY = 24 * 60 * 60 ;
    time_t nd = mktime( &d ) + days * ONE_DAY ;
    return TimetableDate( makeTime( &nd ) );
}

bool operator <  ( const TimetableDate & d1, struct tm d2 ) { return compareDate(d1, d2) < 0; }
bool operator >  ( const TimetableDate & d1, struct tm d2 ) { return compareDate(d1, d2) > 0; }
bool operator <= ( const TimetableDate & d1, struct tm d2 ) { return compareDate(d1, d2) <= 0; }
bool operator >= ( const TimetableDate & d1, struct tm d2 ) { return compareDate(d1, d2) >= 0; }
bool operator == ( const TimetableDate & d1,  struct tm d2 ) { return compareDate(d1, d2) == 0; }
bool operator <  ( struct tm d1, const TimetableDate & d2 ) { return d2 > d1; }
bool operator >  ( struct tm d1, const TimetableDate & d2 ) { return d2 < d1; }
bool operator <= ( struct tm d1, const TimetableDate & d2 ) { return d2 >= d1; }
bool operator >= ( struct tm d1, const TimetableDate & d2 ) { return d2 <= d1; }
bool operator == ( struct tm d1, const TimetableDate & d2 ) { return d2 == d1; }
TimetableDate operator + (const TimetableDate & d1, int d2) { return datePlusDays(d1, d2); }
TimetableDate operator - (const TimetableDate & d1, int d2) { return d1 + -d2; }

bool TimeTableSocketTime::isSwitchedOn( struct tm time ) { return time >= on && time < off; }

//~~~ Timetable ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~ constructor

Timetable::Timetable(const char * FileName)
{
    for (int wd = 0; wd < 7; wd++) Week[wd] = NULL;
    Singles = NULL;
    SinglesCount = 0;
    Holidays = NULL;
    HolidaysCount = 0;
    Calendar = NULL;
    CalendarCount = 0;
    SocketsCount = 0;
    Sockets = NULL;
    
    try {
		json sch;
		ifstream i(FileName);
		i >> sch;
		
		json &week = sch[ "week" ];
		for (int wd = 0; wd < 7; wd++) Week[wd] = new TimetableWeekDay( week[ itoa( wd + 1 ) ], sch["defaults"] );
        
        json &singles = sch["singles"];
        SinglesCount = singles.size();
        if (SinglesCount) {
            Singles = new TimetableDay* [SinglesCount];
            for (int sd = 0; sd < SinglesCount; sd++ ) Singles[sd] = NULL;
            for (int sd = 0; sd < SinglesCount; sd++ ) Singles[sd] = new TimetableDay( singles[ sd ], sch["defaults"] );
        }

        json &holidays = sch["holidays"];
        HolidaysCount = holidays.size();
        if (HolidaysCount) {
            Holidays = new TimetableDate* [HolidaysCount];
            for (int i = 0; i < HolidaysCount; i++ ) Holidays[i] = NULL;
            for (int i = 0; i < HolidaysCount; i++ ) Holidays[i] = new TimetableDate( holidays[ i ] );
        }

        json &calendar = sch["calendar"];
        CalendarCount = calendar.size();
        if (CalendarCount) {
            Calendar = new TimetableDateRange* [CalendarCount];
            for (int i = 0; i < CalendarCount; i++ ) Calendar[i] = NULL;
            for (int i = 0; i < CalendarCount; i++ ) Calendar[i] = new TimetableDateRange( calendar[ i ] );
        }

        json &sockets = sch["sockets"];
        SocketsCount = 0;
        for (int soc = 1; soc <= 4; soc++) if ( sockets[ std::to_string(soc).c_str() ].size() ) SocketsCount++;
        if ( SocketsCount ) {
            Sockets = new TimetableSocket* [SocketsCount];
            for (int i = 0; i < SocketsCount; i++ ) Sockets[i] = NULL;
            for (int soc = 1, i = 0; soc <= 4; soc++) if ( sockets[ std::to_string(soc).c_str() ].size() ) {
                Sockets[i++] = new TimetableSocket( sockets[ std::to_string(soc).c_str() ], soc );
            }
        }

        printf("%s\tTimetable is loaded from %s:\n", strNow(), FileName);
        printf("\t\t\t\t%d date ranges of academic calendar\n", CalendarCount);
        printf("\t\t\t\t%d holidays\n", HolidaysCount);
        printf("\t\t\t\t7 lectures scheles for week days\n");
        printf("\t\t\t\t%d lectures schedules for standalone dates\n", SinglesCount);
        printf("\t\t\t\t%d schedules for power sockets control\n", SocketsCount);
	}
	catch(...) { printf("%s\tError loading timetable JSON from %s\n", strNow(), FileName); throw; }
}

//~~~ destructor

Timetable::~Timetable()
{
    for (int i = 0; i < 7; i++) if ( Week[i] ) delete Week[i];
    if (Singles) {
        for (int i = 0; i < SinglesCount; i++ ) if ( Singles[i] ) delete Singles[i];
        delete [] Singles;
    }

    if (Holidays) {
        for (int i = 0; i < HolidaysCount; i++ ) if ( Holidays[i] ) delete Holidays[i];
        delete [] Holidays;
    }
    if (Calendar) {
        for (int i = 0; i < CalendarCount; i++ ) if ( Calendar[i] ) delete Calendar[i];
        delete [] Calendar;
    }
    if (Sockets) {
        for (int i = 0; i < SocketsCount; i++ ) if ( Sockets[i] ) delete Sockets[i];
        delete [] Sockets;
    }
}

//~~~ getCurrentTimeState 

TimeState Timetable::getCurrentTimeState(int & secToEnd, int & lectNumber) {
    struct tm now = makeNow();
    
    secToEnd = -1; //unknown
    lectNumber = -1; // unknown;

    int week;
    DateState day = getCurrentDateState( week );

    if ( day == dsSession ) return tsSession;
    if ( day == dsVacation || day == dsHoliday ) return tsFree;
    
    int count = 0;
    TimetableLecture ** lectures = NULL;

    for ( int i = 0; i < SinglesCount; i++ ) {
        if ( Singles[i]->date == now ) {
            count = Singles[i]->count;
            lectures =  Singles[i]->lectures;
            break;
        }
    }

    if ( !lectures ) {
        int wd = (now.tm_wday + 6) % 7;
        count = Week[ wd ]->count;
        lectures = Week[ wd ]->lectures;
    }

    // Free day or last lecture is finished

    if ( count == 0 || lectures[ count-1 ]->isFinished( now ) ) return tsFree;

     // First lecture is not started

    if ( lectures[0]->isNotStarted( now ) ) {
        lectNumber = 1;
        secToEnd = secDif( now, lectures[0]->begin );
        return tsFree;
    }

    for (int i = 0; i < count; i++) {
        if ( lectures[i]->isRunning( now ) ) { 
            
            //~~~ Short break inside lecture

            if ( lectures[i]->isBreak( now ) ) {
                lectNumber = i + 1;
                secToEnd = secDif( now, lectures[i]->endBreak );
                return tsShortBreak;
            }

            //~~~ First lectures hour is running

            if ( lectures[i]->isFirstHour( now ) ) {
                lectNumber = i + 1;
                secToEnd = secDif( now,  lectures[i]->beginBreak );
                return tsLecture1h; 
            }

            //~~~ Second lectures hour is running

            else {
                lectNumber = i + 1;
                secToEnd = secDif( now,  lectures[i]->end );
                return tsLecture2h; 
            }
        }

        //~~~ Break between lectures

        if ( i != count-1 && lectures[i]->isFinished( now ) && lectures[i+1]->isNotStarted( now ) ) {
            lectNumber = i + 2;
            secToEnd = secDif( now,  lectures[i+1]->begin );
            return tsBreak; 
        }
    }

    return tsUnknown; // error
}

int Timetable::getHoliday( struct tm now )
{
    for (int i = 0; i < HolidaysCount; i++) if (*Holidays[i] == now) return i;
    return -1;
}

int Timetable::getWeekNumber( const TimetableDate & origin, struct tm now ) {
    struct tm org = { 0 };
    org.tm_year = origin.y - 1900; 
    org.tm_mon = origin.m - 1;
	org.tm_mday = origin.d;

    time_t t = mktime( &org );
    struct tm start = makeTime( &t );
    start.tm_yday -= (start.tm_wday + 6) % 7; // back to nearest monday

	return ( now.tm_yday - start.tm_yday ) / 7;
}

int Timetable::getWeeksCount( const TimetableDateRange * dates )
{
    time_t t;

    struct tm begin = { 0 };
    begin.tm_year = dates->begin.y - 1900; 
    begin.tm_mon = dates->begin.m - 1;
	begin.tm_mday = dates->begin.d;
    t = mktime( &begin );
    struct tm b = makeTime( &t );
    b.tm_yday -= (b.tm_wday + 6) % 7; // back to nearest monday

    struct tm end = { 0 };
    end.tm_year = dates->end.y - 1900; 
    end.tm_mon = dates->end.m - 1;
	end.tm_mday = dates->end.d;
    t = mktime( &end );
    struct tm e = makeTime( &t );
    e.tm_yday += (7 - e.tm_wday) % 7; // forward to nearest sunday

	return ( e.tm_yday - b.tm_yday + 1 ) / 7;
}


DateState Timetable::getCurrentDateState( int & weekNumber )
{
    struct tm now = makeNow();
    
    weekNumber = -1;

    if ( CalendarCount == 0 || now < Calendar[0]->begin )  return dsVacation;

    //~~~ holidays

    if ( getHoliday( now ) >= 0 ) return dsHoliday;

    //~~~ semesters and sessions

    for ( int i = 0; i < CalendarCount; i++ ) if ( now >= Calendar[i]->begin && now <= Calendar[i]->end ) {
        if ( Calendar[i]->type == TimetableDateRange::drUnknown ) continue;

        weekNumber = getWeekNumber( Calendar[i]->begin, now );
        
        for ( int j = i - 1; j >= 0; j-- ) if ( Calendar[i]->type == Calendar[j]->type && Calendar[i]->number == Calendar[j]->number ) {
            weekNumber += getWeeksCount( Calendar[j] );
        }
        
        switch ( Calendar[i]->type ) {
            case TimetableDateRange::drSemester: return dsSemester;
            case TimetableDateRange::drSession: return dsSession;
        }
    }

    //~~~ else - vacation

    if ( CalendarCount > 0 && now > Calendar[CalendarCount-1]->end ) {
        weekNumber = getWeekNumber( Calendar[CalendarCount-1]->end, now );
    }
    else for ( int i = 1; i < CalendarCount; i++ ) if ( now > Calendar[i-1]->end && now < Calendar[i]->begin ) {
        weekNumber = getWeekNumber( Calendar[i-1]->end + 1, now );
    }
    return dsVacation;
}

bool Timetable::getCurrentSocketState( int Socket )
{
    if ( !Sockets ) return false;
    for ( int i = 0; i < SocketsCount; i++ ) if ( Sockets[i]->socket == Socket ) {
        struct tm now = makeNow();
        int w;
        if ( getCurrentDateState(w) == dsHoliday ) return Sockets[i]->holidays ? Sockets[i]->holidays->isSwitchedOn( now ) : false;
        if ( now.tm_wday == 0 ) return Sockets[i]->sundays ? Sockets[i]->sundays->isSwitchedOn( now ) : false;
        return Sockets[i]->weekdays ? Sockets[i]->weekdays->isSwitchedOn( now ) : false;;
    }
    return false;
}

