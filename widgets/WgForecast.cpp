#include "WgForecast.h"

WgForecast::WgForecast(int Ax, int Ay, wgMode Amode): WgBackground(Ax, Ay, Amode) 
{
	updateTime = 60 * 60 * 1000;  // 1 hour
	weatherIcon = NULL;
	weatherIconName  = "";

	isConnection = false;
	
	strcpy( tempDegree, "" );
	strcpy( windSpeed, "" );
	windDegree = 0;
	printf( "%s\tWgForecast widget object is created\n", strNow() );
}

WgForecast::~WgForecast()
{
	if (weatherIcon) delete weatherIcon;
	printf( "%s\tWgForecast widget object is deleted\n", strNow() );
}

size_t WgForecast::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) //???
{
    ((std::string*)userp)->append((char*)contents, size * nmemb); // ???
    return size * nmemb; // ???
}

void WgForecast::getWeatherFromWeb(const char site[], json & weatherData)
{	
	CURL *curl; // object CURL tipa
	std::string readBuffer; //буффур куда будем записывать принятые данные
	CURLcode res;
	static char errorBuffer[CURL_ERROR_SIZE]; //объявляем буфер, для хранения возможной ошибки, размер определяется в самой библиотеке
	curl = curl_easy_init(); // curl initialization
	if (curl) //проверяем
	{
		//задаем все необходимые опции	
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer); //определяем, куда выводить ошибки
		curl_easy_setopt(curl, CURLOPT_URL, site); //задаем опцию - получить страницу по адресу site
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); //указываем функцию обратного вызова для записи получаемых данных
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer); //указываем куда записывать принимаемые данные
		res = curl_easy_perform(curl); //запускаем выполнение задачи
		if (res == CURLE_OK)
		{
			isConnection = true;
			auto buf = json::parse(readBuffer);
			weatherData = buf;
			printf( "%s\tNew current weather state is received\n", strNow() );
		}
		else  
		{    
			isConnection = false;
			printf( "%s\tError of current weather state receiving\n", strNow() );
		}
	}
	curl_easy_cleanup(curl); //выполняем обязательное завершение сессии
}

bool WgForecast::update()
{
	json weatherData;
	getWeatherFromWeb( CURRENT_WEATHER_URL, weatherData );

	if (isConnection && weatherData["main"]["temp"].is_number())
	{
		int bufTemp = weatherData["main"]["temp"];
		sprintf(tempDegree, "%+d°", bufTemp);
	}
	else
		sprintf(tempDegree, "---");

	if (isConnection && weatherData["wind"]["deg"].is_number())
	{
		windDegree = weatherData["wind"]["deg"];
		float bufSpeed = weatherData["wind"]["speed"];
		sprintf(windSpeed, "%.0f m/s", bufSpeed);
	}
	else
	{
		sprintf(windSpeed, "---");
		windDegree = 0;
	}

	//~~~ load weather icon 

	if ( isConnection && weatherData["weather"][0]["icon"].is_string() ) {
		std::string iconName = weatherData["weather"][0]["icon"];
		if ( iconName != weatherIconName ) {
			std::string iconPath = std::string("resources/weather/") + iconName + ".png";
			printf( "%s\tNew weather icon is loaded from file %s\n", strNow(), iconPath.c_str() );
			if ( weatherIcon ) delete weatherIcon;
			weatherIcon = new Picture(iconPath.c_str());
			weatherIconName = iconName;
		}
	}
	else {
		if ( weatherIcon ) delete weatherIcon;
		weatherIcon = NULL;
	}
	
	return true;
}

void WgForecast::renderMode1()
{
	renderHeader( tempDegree );
}

void WgForecast::renderMode2()
{
	float field = rectClient.width * 0.07;

	//~~ weather icon

	int iw = 0, ih = 0;
	if ( weatherIcon ) {
		float is = (float)rectClient.height * 1.0 / weatherIcon->getHeight();
		iw = weatherIcon->getWidth() * is;
		ih = weatherIcon->getHeight() * is;
		weatherIcon->render(
			rectClient.left + field,
			rectClient.bottom + (rectClient.height - ih) / 2,
			is, is, 0, 0, 0
		);
	}

	//~~ wind arrow

	float as = (float)rectClient.height * 0.35 / PicStorage->Arrow->getHeight();
	int aw = PicStorage->Arrow->getWidth() * as * 1.75; // 1.4 = sqrt(2)
	int ah = PicStorage->Arrow->getHeight() * as;
	PicStorage->Arrow->render(
		rectClient.right - aw - field,
		rectClient.bottom + ( rectClient.height - ah ) / 2,
		as, as, 0, 0,
		-windDegree
	);

	//~~~ wind speed

	setTextColor(clHaki);
	TFont * font = FontStorage->getFont((char*)"arialBold");
	font->SetSize( desktop->rowHeight / 3 );
	int ww = (int)font->TextWidth( windSpeed );
	int wh = (int)font->TextHeight();
	font->TextMid(
		windSpeed, 
		rectClient.left + iw + ( rectClient.width - iw - aw ) / 2, 
		rectClient.bottom + ( rectClient.height - wh ) / 2
	);
}

void WgForecast::renderMode3() // need to debug
{
	/*
	FontStorage->getFont((char*)"arialBold")->SetSize(desktop->rowHeight/5);
	FontStorage->getFont((char*)"arialBold")->TextMid("Veišs:", rectClient.left + (desktop->colWidth/2),
		rectClient.top - desktop->rowHeight - (desktop->rowHeight/5/2));
	setTextColor(color);
	FontStorage->getFont((char*)"arialBold")->SetSize(desktop->rowHeight/2.8);
	FontStorage->getFont((char*)"arialBold")->TextMid(windSpeed, rectClient.left + (desktop->colWidth/2.5),
		rectClient.top - desktop->rowHeight - desktop->rowHeight/16*11);
	PicStorage->Arrow->render(rectClient.left + (desktop->colWidth/1.3), rectClient.top - desktop->rowHeight - desktop->rowHeight/1.4, 1, 1, 0,0,-windDegree);
	*/
}

void WgForecast::render()
{
	if (isConnection)
	{
		WgBackground::render();
		switch (mode){
			case md1x1:{ renderMode1(); break; }
			case md1x2:{ renderMode1(); renderMode2(); break; }
			case md1x3:{ renderMode1(); renderMode2(); renderMode3(); break; }
		}
	}
}

