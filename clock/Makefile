CC 	 = gcc

PROGRAM  = clock.exe
INCDIR   = /usr/include/opengl

SRCS = *.c
OBJS = $(SRCS:%.c=%.o)
LIBS = glpng 	  \
       glut32     \
       glu32	  \
       opengl32   \
       m

CFLAGS  = -Wall        \
	  -I$(INCDIR)  \

LDFLAGS = $(addprefix -l, $(LIBS))

DEBUG_FLAGS   = -g -O0
RELEASE_FLAGS = -O2

.PHONY: all debug release conf clean

debug  : CFLAGS   += $(DEBUG_FLAGS)
debug  : all

release: CFLAGS   += $(RELEASE_FLAGS)
release: all

all    : $(PROGRAM)
$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $^ $(CFLAGS)
clean:
	-rm -f $(OBJS) $(PROGRAM)
image2d.o: image2d.c transform2d.h vector2d.h vector3d.h image2d.h view.h
main.o: main.c transform2d.h vector2d.h vector3d.h image2d.h view.h
transform2d.o: transform2d.c transform2d.h vector2d.h vector3d.h
transform3d.o: transform3d.c transform3d.h vector3d.h vector4d.h
vector2d.o: vector2d.c vector2d.h
vector3d.o: vector3d.c vector3d.h
vector4d.o: vector4d.c vector4d.h
view.o: view.c view.h vector3d.h
