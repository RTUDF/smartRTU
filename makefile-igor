
NEEDS=-lshapes -lm  -std=c++11 -lpng -fpermissive `curl-config --libs` -lstdc++ -I/usr/include/freetype2 -lfreetype

LIBFLAGS=-L/opt/vc/lib -lEGL -lGLESv2 -ljpeg
GENERAL_FOR_VG=-I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads

CC=g++
CFLAGS=-c -Wall $(GENERAL_FOR_VG) $(LIBFLAGS)
LDFLAGS= $(GENERAL_FOR_VG) $(LIBFLAGS)

GENERAL_SOURCES=CPicturesStorage.cpp CFontStorage.cpp Engine.cpp main.cpp board.cpp
LIB_SOURCES=./lib/TFont/TFont.cpp ./lib/TFont/ftf.cpp ./lib/Picture.cpp ./lib/json.cpp
WIDGET_SOURCES=./widgets/WgBackground.cpp ./widgets/WgCalendar.cpp ./widgets/WgClock.cpp ./widgets/WgForecast.cpp ./widgets/WgAds.cpp

SOURCES=$(GENERAL_SOURCES) $(LIB_SOURCES) $(WIDGET_SOURCES)   

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=infoboard

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(NEEDS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@ $(NEEDS)

clean:
	rm -rf $(EXECUTABLE) *.o
