CC 	   := gcc
LD	   := gcc

SRCDIR     := src
HEADERDIR  := src
OBJDIR     := build
ICONDIR    := .
ICONIMGDIR := resource/image
INCDIR     := /usr/include/opengl ./include
DEPENDDIR  := depend

TARGET     := j15435.exe
SRCS       := $(wildcard $(SRCDIR)/*.c)
HEADERS    := $(wildcard $(HEADERDIR)/*.h)
OBJS       := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
DEPENDS    := $(subst $(SRCDIR)/, $(DEPENDDIR)/, $(SRCS:%.c=%.d))
ICONSRC    := $(ICONDIR)/icon.rc
ICONIMG    := $(ICONIMGDIR)/icon.ico
ICONOBJ    := $(ICONSRC:%.rc=%.o)
LIBS 	   :=	glpng		\
		glut32		\
		glu32		\
		opengl32	\
		soft_oal	\
		alut		\
		m		  

CCFLAGS    := -Wall       \
		 -pthread\
	     $(addprefix -I, $(INCDIR)) 

LDFLAGS    := $(addprefix -l, $(LIBS))	\
	      -L.

DEBUG_FLAGS   := -g3 -O0
RELEASE_FLAGS := -O2 -s

GENDEP  = $(CC) $^ -MM -MP | sed -e 's/\(.*\.o\)/$(OBJDIR)\/\1/g' >> $@
COMPILE = $(CC) -c $< -o $@ $(CCFLAGS)
LINK    = $(LD) $^ -o $@ $(LDFLAGS)

.PHONY: all debug release clean depend

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : all

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: all

all: $(TARGET)

$(TARGET):: $(DEPENDS)
$(TARGET):: $(OBJS) $(ICONOBJ)
	$(LINK)

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
