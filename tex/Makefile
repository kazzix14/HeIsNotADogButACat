CC 	   = gcc
LD	   = gcc

SRCDIR     = src
HEADERDIR  = src
OBJDIR     = build
ICONDIR    = .
ICONIMGDIR = resource/image
INCDIR     = /usr/include/opengl

TARGET     = j15435.exe
SRCS       = $(wildcard $(SRCDIR)/*.c)
HEADERS    = $(wildcard $(HEADERDIR)/*.h)
OBJS       = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
ICONSRC    = $(ICONDIR)/icon.rc
ICONIMG    = $(ICONIMGDIR)/icon.ico
ICONOBJ    = $(ICONSRC:%.rc=%.o)
LIBS 	   = glpng 	  \
       	     glut32       \
       	     glu32	  \
       	     opengl32     \
       	     m

CCFLAGS    = -Wall        \
	     -I$(INCDIR)  \

LDFLAGS    = $(addprefix -l, $(LIBS))	\

DEBUG_FLAGS   = -g3 -O0
RELEASE_FLAGS = -O2 -s

.PHONY: all debug release clean

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : all

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: all

all    : $(TARGET)

$(TARGET):$(OBJS) $(ICONOBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HEADERS)
	-@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(ICONOBJ):$(ICONSRC) $(ICONIMG)
	windres -i $< -o $@
clean:
	-rm -rf $(OBJS) $(ICONOBJ) $(TARGET)
