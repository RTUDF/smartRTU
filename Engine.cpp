#include "Engine.h"
#include "VG/openvg.h"

const char * fmt (const char * f, const char * s1, const char * s2, const char * s3 ) {
	static char buf[100];
	sprintf( buf, f, s1, s2, s3 );
	return buf;
}

//********************************************************************************
//*** Engine
//********************************************************************************

#define DEBUG_OUTPUT_PRD (60 * 60 * 1000) // 1hour
#define FLUSH_LOG_PRD 1000 // 1sec

Engine * engine = NULL;

Engine::Engine()
{
	terminated = false;
	board = new Board;
	isRunning = false;
	frameCap = 1000 / FRAMES_PER_SECOND; 
}

Engine::~Engine()
{
	delete board;
	if (timetable) delete timetable;
}

void Engine::start()
{
	if ( !isRunning ) run();
}

void Engine::run()
{	
	isRunning = true;

	LongTimeMs idleTime = 0;
	LongTimeMs lastRender = 0;
	LongTimeMs lastDebug = 0;
	LongTimeMs lastFlush = 0;
	LongTimeMs  t = 0;
	int fps = 0;

	forcedUpdate();
	render( true );

	printf( "%s\tEngine main circle is started\n", strNow() );

	while( isRunning && !terminated )
	{
		LongTimeMs time = timer.getTime();

		//kbhit();  //?

		update();

		if ( time - lastRender >= frameCap )
		{
			render( false );
			lastRender = time;
			fps++;	
		}
		
		#ifdef DEBUG_OUTPUT_PRD

		if ( time - lastDebug >= DEBUG_OUTPUT_PRD ) {
			unsigned int tsec = (unsigned int) (time / 1000LLU);
			unsigned int tm_d = tsec / (24 * 3600); 	tsec %= (24 * 3600);
			unsigned int tm_h = tsec / 3600;			tsec %= 3600;
			unsigned int tm_m = tsec / 60;				tsec %= 60;
			unsigned int tm_s = tsec;
			printf(
				"%s\tStatus: on time: %u:%02u:%02u:%02u, idle: %llu%%, fps: %.0f\n", 
				strNow(), tm_d, tm_h, tm_m, tm_s,
				idleTime * 100 / (time - lastDebug),
				(float)(fps) * 1000 / (time - lastDebug)
			);

			fps = 0;
			idleTime = 0;
			lastDebug = time;
		}

		#endif

		#ifdef FLUSH_LOG_PRD

		if ( time - lastFlush >= FLUSH_LOG_PRD ) {
			fflush( stdout );
			lastFlush = time;
		}

		#endif

		usleep(1000); // 1ms
		idleTime += 1;
	}

	printf( "%s\tEngine main circle is finished\n", strNow() );
	
}

void Engine::update()
{
	board->update( false );
}

void Engine::forcedUpdate()
{
	board->update( true );
}

void Engine::render(bool All)
{
	//Start(width, height);
	VGfloat color[4] = { 1, 1, 1, 1 };
	//vgSetfv(VG_CLEAR_COLOR, 4, color);
	//vgClear(0, 0, width, height);
	color[0] = 0, color[1] = 0, color[2] = 0;
	setfill(color);
	setstroke(color);
	StrokeWidth(0);
	vgLoadIdentity();
	
	//Background(0, 0, 0);
	VGfloat colour[4];
	RGB(0, 0, 0, colour);
	vgSetfv(VG_CLEAR_COLOR, 4, colour);
//	vgClear(0, 0, state->window_width, state->window_height);

	board->render( All );
	
	End();
}

/*
int Engine::kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);	
		isRunning = false;		
		return 1;
	}
	return 0;
}
*/