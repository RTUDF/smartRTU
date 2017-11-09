TARGET = test
LIBFLAGS = -L/opt/vc/lib -lEGL -lGLESv2 -ljpeg
GENERAL_FOR_VG = -I/opt/vc/include -I/opt/vc/include/interface/vmcs_host/linux -I/opt/vc/include/interface/vcos/pthreads
INCLUDES = -lshapes -lm -I/usr/include/freetype2 -lfreetype

test: TFont.cpp ftf.cpp
	g++ -O0 -g $(GENERAL_FOR_VG) $(LIBFLAGS) $(TARGET).cpp -o $(TARGET) TFont.cpp ftf.cpp $(INCLUDES)
clean:
	rm -rf $(TARGET)

