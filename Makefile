all: clock.exe

clock.exe: clock.o
	gcc -Wall -I/usr/include/opengl -o clock.exe clock.o -lglpng -lglut32 -lglu32 -lopengl32

clock.o: clock.c
	gcc  -Wall -I/usr/include/opengl -c clock.c -lglpng -lglut32 -lglu32 -lopengl32

clean:
	rm clock.o clock.exe
