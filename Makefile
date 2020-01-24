CC 	   := gcc
LD	   := gcc

SRCDIR     := src
HEADERDIR  := src
OBJDIR     := build
ICONDIR    := .
ICONIMGDIR := resource/image
INCDIR     := /usr/include/opengl ./include
DEPENDDIR  := depend

TARGET     := game.exe
SRCS       := $(wildcard $(SRCDIR)/*.c)
HEADERS    := $(wildcard $(HEADERDIR)/*.h)
OBJS       := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
DEPENDS    := $(subst $(SRCDIR)/, $(DEPENDDIR)/, $(SRCS:%.c=%.d))
ICONSRC    := $(ICONDIR)/icon.rc
ICONIMG    := $(ICONIMGDIR)/icon.ico
ICONOBJ    := $(ICONSRC:%.rc=%.o)
ifeq ($(OS),Windows_NT))
LIBS 	   :=	glpng		\
		glut32		\
		glu32		\
		opengl32	\
		soft_oal	\
		alut		\
		m		  
else
LIBS 	   :=	glpng		\
		glut		\
		GLU		\
		GL		\
		OpenGL		\
		openal		\
		alut		\
		pthread		\
		m		  
endif

CCFLAGS    := -Wall       \
	     $(addprefix -I, $(INCDIR)) 
		 #-pthread\
	     $(addprefix -I, $(INCDIR)) 

LDFLAGS    := -L. $(addprefix -l, $(LIBS))

DEBUG_FLAGS   := -g3 -pg -O0 -DDEBUG
RELEASE_FLAGS := -O2 -s

GENDEP  = $(CC) $^ -MM -MP | sed -e 's/\(.*\.o\)/$(OBJDIR)\/\1/g' > $@
#GENDEP  = $(CC) $^ -MM -MP | sed 's/\(.*\.o\)/$(OBJDIR)\/\1/g' > $@
COMPILE = $(CC) -c $< -o $@ $(CCFLAGS)
LINK    = $(LD) $^ -o $@ $(LDFLAGS)

.PHONY: all debug release clean depend

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: all

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : all

all: $(TARGET)

$(TARGET):: $(DEPENDS)
ifeq ($(OS),Windows_NT)
$(TARGET):: $(OBJS) $(ICONOBJ)
	$(LINK)
else
$(TARGET):: $(OBJS)
	$(LINK)
endif

$(OBJDIR)/%.o:
	-@mkdir -p $(OBJDIR)
	$(COMPILE)

$(DEPENDDIR)/%.d: $(SRCDIR)/%.c
	-@mkdir -p $(DEPENDDIR)
	$(GENDEP)

$(ICONOBJ):$(ICONSRC) $(ICONIMG)
	windres -i $< -o $@

$(ICONSRC): 
	echo 'GLUT_ICON ICON $(ICONIMG)' > $(ICONSRC)

clean:
	-rm -rf $(OBJS) $(ICONOBJ) $(TARGET) $(DEPENDS) $(ICONSRC)

include $(DEPENDS)
