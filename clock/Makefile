CC 	   = gcc
LD	   = gcc

TARGET     = clock.exe
INCDIR     = /usr/include/opengl
SRCDIR     = ./src
HEADERDIR  = ./include
OBJDIR	   = ./build

SRCS       = $(SRCDIR)/*.c
HEADERS    = $(HEADERDIR)/*.h
OBJS       = $(OBJDIR)/$(SRCS:%.c=%.o)
DEPENDS    = $(OBJS:.o=.d)
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
	$(CC) -o $@ $^ $(LDFLAGS)
.c.o:
	$(CC) -c $^ $(CCFLAGS)

$(OBJS) : $(HEADERS) Makefile

clean:
	-rm -f $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)
