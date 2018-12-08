all: pngread.exe

pngread.exe: pngread.o
	gcc -Wall -I/usr/include/opengl -o pngread.exe pngread.o -lglpng -lglut32 -lglu32 -lopengl32

pngread.o: pngread.c
	gcc  -Wall -I/usr/include/opengl -c pngread.c -lglpng -lglut32 -lglu32 -lopengl32

clean:
	rm pngread.o pngread.exe
