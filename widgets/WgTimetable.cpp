#include "WgTimetable.h"

time_t WgTimetable::getFileTime()
{
	struct stat buff;
	if ( stat( DEFAULT_TIMETABLE_FILE, &buff ) == 0 ) return buff.st_mtime;
	return 0;
}

WgTimetable::WgTimetable() 
{
    fileTime = getFileTime();
    printf( "%s\tWgTimetable widget object is created\n", strNow() );
}

WgTimetable::~WgTimetable() 
{
    printf( "%s\tWgTimetable widget object is deleted\n", strNow() );
}

bool WgTimetable::update()
{
    if ( fileTime != getFileTime() ) {
        if ( timetable ) delete timetable;
        try {
            timetable = new Timetable;
        }
        catch (...) { 
            timetable = NULL; 
        }
        fileTime = getFileTime();
        engine->forcedUpdate();
    }
    return false;
}
