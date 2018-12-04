all: font.exe

font.exe: font.o
	gcc -Wall -I/usr/include/opengl -o font.exe font.o -lglpng -lglut32 -lglu32 -lopengl32

font.o: font.c
	gcc  -Wall -I/usr/include/opengl -c font.c -lglpng -lglut32 -lglu32 -lopengl32

clean:
	rm font.o font.exe
