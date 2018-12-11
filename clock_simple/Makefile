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

component2d.o: component2d.c component2d.h vector2d.h
component3d.o: component3d.c component3d.h
image2d.o: image2d.c component2d.h vector2d.h image2d.h
main.o: main.c component2d.h vector2d.h image2d.h view.h
vector2d.o: vector2d.c vector2d.h
view.o: view.c view.h
