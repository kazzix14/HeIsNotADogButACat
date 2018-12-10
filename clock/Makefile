CC 	 = gcc

PROGRAM  = clock.exe
INCDIR   = /usr/include/opengl

SRCS = main.c image.c compoment.c
OBJS = $(SRCS:%.c=%.o)
LIBS = glpng 	  \
       glut32     \
       glu32	  \
       opengl32   \
       m

CFLAGS  = -Wall        \
	  -I$(INCDIR)  \
	  -MD

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

-include *.d
