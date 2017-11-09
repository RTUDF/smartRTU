#include "WgWatchdog.h"

WgWatchdog::WgWatchdog() 
{
    printf( "%s\tWgWatchdog widget object is created\n", strNow() );
}

WgWatchdog::~WgWatchdog() 
{
    printf( "%s\tWgWatchdog widget object is deleted\n", strNow() );
}

bool WgWatchdog::update()
{
    system( fmt( "rm -rf %s 2> /dev/null", WATCHDOG_FILES_NAME ) );
    return false;
}
