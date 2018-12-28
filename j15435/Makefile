CC 	   = gcc
LD	   = gcc

SRCDIR     = src
HEADERDIR  = src
OBJDIR     = build
ICONDIR    = .
ICONIMGDIR = resource/image
INCDIR     = /usr/include/opengl
DEPENDDIR  = depend

TARGET     = j15435.exe
SRCS       = $(wildcard $(SRCDIR)/*.c)
HEADERS    = $(wildcard $(HEADERDIR)/*.h)
OBJS       = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
DEPENDS    = $(subst $(SRCDIR)/, $(DEPENDDIR)/, $(SRCS:%.c=%.d))
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

.PHONY: all debug release clean depend

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : all

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: all

all: $(DEPENDS) $(TARGET) 
include $(DEPENDS)

$(TARGET): $(OBJS) $(ICONOBJ)
	$(LD) $^ -o $@ $(LDFLAGS)

$(OBJDIR)/%.o:
	-@mkdir -p $(OBJDIR)
	$(CC) -c $< -o $@ $(CCFLAGS)

$(DEPENDDIR)/%.d: $(SRCDIR)/%.c
	-@mkdir -p $(DEPENDDIR)
	$(CC) $< -MM | sed -e 's/\(.*\.o\)/$(OBJDIR)\/\1/g' >> $@

$(ICONOBJ):$(ICONSRC) $(ICONIMG)
	windres -i $< -o $@
clean:
	-rm -rf $(OBJS) $(ICONOBJ) $(TARGET) $(DEPENDS)
