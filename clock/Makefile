CC 	   = gcc
LD	   = gcc

SRCDIR     = src
HEADERDIR  = src
OBJDIR     = build
DEPENDDIR  = depends
INCDIR     = /usr/include/opengl

TARGET     = clock.exe
SRCS       = $(wildcard $(SRCDIR)/*.c)
HEADERS    = $(wildcard $(HEADERDIR)/*.h)
OBJS       = $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
DEPENDS    = $(subst $(SRCDIR)/, $(DEPENDDIR)/, $(SRCS:%.c=%.d))
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

.PHONY: all debug release clean

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : all

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: all

all    : $(DEPENDS) $(TARGET)
$(TARGET): $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(DEPENDDIR)/%.d: $(SRCDIR)/%.c $(HEADERS)
	-@mkdir -p $(DEPENDDIR)
	$(CC) -MM $^ $(CCFLAGS) > $@

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	-@mkdir -p $(OBJDIR)
	$(CC) -c $^ -o $@ $(CCFLAGS)

clean:
	-rm -f $(OBJS) $(TARGET)

-include $(DEOENDS)
