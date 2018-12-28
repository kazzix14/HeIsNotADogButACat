CC 	   := gcc
LD	   := gcc

SRCDIR     := src
HEADERDIR  := src
OBJDIR     := build
ICONDIR    := .
ICONIMGDIR := resource/image
INCDIR     := /usr/include/opengl
DEPENDDIR  := depend

TARGET     := j15435.exe
SRCS       := $(wildcard $(SRCDIR)/*.c)
HEADERS    := $(wildcard $(HEADERDIR)/*.h)
OBJS       := $(subst $(SRCDIR)/, $(OBJDIR)/, $(SRCS:%.c=%.o))
DEPENDS    := $(subst $(SRCDIR)/, $(DEPENDDIR)/, $(SRCS:%.c=%.d))
ICONSRC    := $(ICONDIR)/icon.rc
ICONIMG    := $(ICONIMGDIR)/icon.ico
ICONOBJ    := $(ICONSRC:%.rc=%.o)
LIBS 	   := glpng 	  \
       	     glut32       \
       	     glu32	  \
       	     opengl32     \
       	     m

CCFLAGS    := -Wall       \
	     -I$(INCDIR)  \

LDFLAGS    := $(addprefix -l, $(LIBS))	\

DEBUG_FLAGS   := -g3 -O0
RELEASE_FLAGS := -O2 -s


GENDEP  = $(CC) $^ -MM -MP | sed -e 's/\(.*\.o\)/$(OBJDIR)\/\1/g' >> $@
COMPILE = $(CC) -c $< -o $@ $(CCFLAGS)
LINK    = $(LD) $^ -o $@ $(LDFLAGS)

depprog := 0
objprog := 0
lnkprog := 0
DNUM = $(words $(shell if [ -e $(TARGET) ];then find ./src/*.c -type f -newer $(TARGET); else echo $(DEPENDS);fi))
LNUM = $(words $(TARGET))
ONUM = $(words $(shell if [ -e $(TARGET) ];then find ./src/*.c -type f -newer $(TARGET); else echo $(OBJS);fi))

# progress, number of tasks, bar length, progress message, finish message, task
define PBAR
	$(call PBAR_SHOW,$1,$2,$3,$4,$5)
	$6	
	$(call PBAR_SHOW,$1,$2,$3,"",$5)
endef

# progress, number of tasks, bar length, progress message, finish message
PBAR_SHOW =												\
	if ( [ $($1) -eq $$(($2)) ] && [ $4 = "" ] ) || [ $4 != "" ];  					\
	then												\
		if [ $($1) -ne 0 ];									\
		then											\
      			echo -n -e "\e[2K\e[1A\e[2K\e[1A\e[2K\e[0G";					\
		fi;											\
		if [ $4 = "" ];										\
		then											\
			echo $5;									\
		else											\
      			echo $4;									\
		fi;											\
      		echo -n " [";										\
      		for i in {1..$3};									\
      		do											\
			if [ $$i -le $$(( $$(( $$(( $($1)000 / $2)) * $3)) / 1000)) ] || [ $4 = "" ];	\
      			then										\
				echo -n "=";								\
      			else										\
				echo -n " ";								\
      			fi;										\
      		done;											\
      		echo "]";										\
	fi; 												\
	$(eval $1 := $(shell if [ $4 = "" ]; then echo $($1); else echo $$(expr $($1) + 1); fi))

.PHONY: all debug release clean depend

debug  : CCFLAGS += $(DEBUG_FLAGS)
debug  : LDFLAGS += $(DEBUG_FLAGS)
debug  : $(TARGET)

release: CCFLAGS += $(RELEASE_FLAGS)
release: LDFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET):: $(DEPENDS)
$(TARGET):: $(OBJS) $(ICONOBJ)
	-@$(call PBAR,lnkprog,$(LNUM),50," Linking $@"," Link Completed",$(LINK))

$(OBJDIR)/%.o:
	-@mkdir -p $(OBJDIR)
	-@$(call PBAR,objprog,$(ONUM),50," Compiling $@"," Compilation Completed",$(COMPILE))

$(DEPENDDIR)/%.d: $(SRCDIR)/%.c
	-@mkdir -p $(DEPENDDIR)
	-@$(call PBAR,depprog,$(DNUM),50," Generating $@"," Generation Completed",$(GENDEP))

$(ICONOBJ):$(ICONSRC) $(ICONIMG)
	@windres -i $< -o $@

$(ICONSRC): 
	@echo 'GLUT_ICON ICON $(ICONIMG)' > $(ICONSRC)

clean:
	-rm -rf $(OBJS) $(ICONOBJ) $(TARGET) $(DEPENDS) $(ICONSRC)

include $(DEPENDS)
