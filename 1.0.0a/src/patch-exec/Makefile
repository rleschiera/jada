##############################################################################
INCLUDES = -I../../include
DEFINES = -DUNIX -DJADA_SERVER
CFLAGS = -g $(INCLUDES) $(DEFINES)
OUTDIR = ../../../bin

##############################################################################
OBJS = main.o \
       ../minini/minIni.o \
       ../jadalib/common.o
LIBS = -lpthread -lz
 
patch-starter: $(OBJS)
	cc $(OBJS) $(LIBS) -o $@
	cp patch-starter $(OUTDIR)

##############################################################################
clean:
	rm -f $(OBJS) core

