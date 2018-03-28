.PHONY : clean

NAME = infoboard

WOBJ = 	obj/IWidget.o \
		obj/WgBackground.o \
		obj/WgClock.o \
		obj/WgCalendar.o \
		obj/WgForecast.o \
		obj/WgAds.o \
		obj/WgSockets.o \
		obj/WgTimetable.o \
		obj/WgWatchdog.o \
		obj/WgMulAds.o \
		obj/Ads.o \
		obj/AdsText.o

INCS = 	-I/usr/include/freetype2 
 
HDRS = 	src/*.h \
		src/lib/*.h \
		src/lib/TFont/*.h
		
WHDRS = src/widgets/*.h

OBJ = 	obj/CPicturesStorage.o \
		obj/CFontStorage.o \
		obj/Timer.o \
		obj/Picture.o \
		obj/ftf.o \
		obj/TFont.o \
		obj/json.o \
		obj/timetable.o \
		obj/board.o \
		obj/Engine.o \
		obj/desktop.o \
		obj/Ads.o \
		obj/AdsText.o

VGINCS = \
		-I/opt/vc/include \
		-I/opt/vc/include/interface/vmcs_host/linux \
		-I/opt/vc/include/interface/vcos/pthreads

VGLIBS = \
		-L/opt/vc/lib \
		-lbrcmEGL \
		-lbrcmGLESv2

LIBS =	-lshapes \
		-lm  \
		-lpng  \
		-ljpeg \
		-fpermissive `curl-config --libs` \
		-lstdc++ \
		-lfreetype \
		-lwiringPi \
		-lbcm_host \
		-lpthread

FLAGS = -std=c++11 

$(NAME): src/main.cpp $(OBJ) $(WOBJ)
	@g++ -o $@ $< $(OBJ) $(WOBJ) $(INCS) $(LIBS) $(VGINCS) $(VGLIBS) $(FLAGS)
	@echo -ne '\007'

obj/CFontStorage.o : src/CFontStorage.cpp src/CFontStorage.h src/lib/TFont/TFont.h
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/TFont.o : src/lib/TFont/TFont.cpp src/lib/TFont/TFont.h src/lib/TFont/ftf.h
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/ftf.o : src/lib/TFont/ftf.cpp src/lib/TFont/ftf.h
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/json.o : src/lib/json.cpp src/lib/json.h
	@g++ -c -o $@ $< $(FLAGS)

obj/Picture.o : src/lib/Picture.cpp src/lib/Picture.h
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/CPicturesStorage.o : src/CPicturesStorage.cpp src/CPicturesStorage.h src/lib/Picture.h
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/board.o : src/board.cpp src/board.h $(WHDRS) $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/Engine.o : src/Engine.cpp src/Engine.h src/board.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/Timer.o : src/Timer.cpp src/Timer.h
	@g++ -c -o $@ $< $(INCS) $(FLAGS)

obj/IWidget.o : src/widgets/IWidget.cpp src/widgets/IWidget.h
	@g++ -c -o $@ $< $(FLAGS)

obj/timetable.o : src/timetable.cpp src/timetable.h
	@g++ -c -o $@ $< $(FLAGS)

obj/desktop.o : src/desktop.cpp src/desktop.h
	@g++ -c -o $@ $< $(FLAGS)

obj/Ads.o : src/widgets/Ads.cpp src/widgets/Ads.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/AdsText.o : src/widgets/AdsText.cpp src/widgets/AdsText.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgBackground.o : src/widgets/WgBackground.cpp src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgAds.o : src/widgets/WgAds.cpp src/widgets/WgAds.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgCalendar.o : src/widgets/WgCalendar.cpp src/widgets/WgCalendar.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgClock.o : src/widgets/WgClock.cpp src/widgets/WgClock.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgForecast.o : src/widgets/WgForecast.cpp src/widgets/WgForecast.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgSockets.o : src/widgets/WgSockets.cpp src/widgets/WgSockets.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgTimetable.o : src/widgets/WgTimetable.cpp src/widgets/WgTimetable.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgWatchdog.o : src/widgets/WgWatchdog.cpp src/widgets/WgWatchdog.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

obj/WgMulAds.o : src/widgets/WgMulAds.cpp src/widgets/WgMulAds.h src/widgets/Ads.cpp src/widgets/Ads.h src/widgets/WgBackground.h src/widgets/IWidget.h $(HDRS)
	@g++ -c -o $@ $< $(INCS) $(VGINCS) $(FLAGS)

clean :
	@rm -rf $(NAME) obj/*.o

#test: test.cpp lib/Picture.h obj/Picture.o
test: src/test.cpp 
#	@g++ -o $@ $< obj/*.o $(VGINCS) $(VGLIBS) $(LIBS) $(FLAGS) -I/home/pi/openvg -lwiringPi
	@g++ -o $@ $(VGINCS) $(VGLIBS) $(LIBS) $(FLAGS)
