CC 	   = gcc
LD	   = gcc

TARGET     = clock.exe
SRCS       = $(wildcard *.c)
OBJS       = $(SRCS:%.c=%.o)

INCDIR     = /usr/include/opengl
LIBS 	   = glpng 	  \
       	     glut32       \
       	     glu32	  \
       	     opengl32     \
       	     m

CCFLAGS    = -Wall        \
	     -I$(INCDIR)  \

LDFLAGS    = $(addprefix -l, $(LIBS))

DEBUG_FLAGS   = -g -O0
RELEASE_FLAGS = -O2

.PHONY: all debug release conf clean

debug  : CFLAGS += $(DEBUG_FLAGS)
debug  : all

release: CFLAGS += $(RELEASE_FLAGS)
release: all

all    : $(TARGET)
$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $^ $(CCFLAGS)

clean:
	-rm -f $(OBJS) $(TARGET)

image2d.o: image2d.c transform2d.h vector2d.h vector3d.h image2d.h view.h
main.o: main.c transform2d.h vector2d.h vector3d.h image2d.h view.h
transform2d.o: transform2d.c transform2d.h vector2d.h vector3d.h
transform3d.o: transform3d.c transform3d.h vector3d.h vector4d.h
vector2d.o: vector2d.c vector2d.h
vector3d.o: vector3d.c vector3d.h
vector4d.o: vector4d.c vector4d.h
view.o: view.c view.h vector3d.h
