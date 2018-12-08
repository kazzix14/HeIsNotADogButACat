all: map.exe

map.exe: map.o
	gcc -Wall -I/usr/include/opengl -o map.exe map.o -lglpng -lglut32 -lglu32 -lopengl32

map.o: map.c
	gcc  -Wall -I/usr/include/opengl -c map.c -lglpng -lglut32 -lglu32 -lopengl32

clean:
	rm map.o map.exe
