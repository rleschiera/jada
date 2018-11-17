##############################################################################
INCLUDES = -I../../include
DEFINES = -DUNIX -DJADA_SERVER
CFLAGS = -g $(INCLUDES) $(DEFINES)
OUTSITODIR = ../../../bin
OUTJADADIR = ../../../../jada/bin

##############################################################################
OBJS = srvcallback.o \
       srvdatabase.o \
       srvhandler.o \
       srvlog.o \
       srvmain.o \
       srvobjects.o \
       srvsession.o \
       srvtcp.o \
       srvthread.o \
       ../minini/minIni.o \
       ../jadalib/common.o \
       ../jadalib/communication.o \
       ../jadalib/list.o \
       ../jadalib/message.o
LIBS = -lpthread -lz
 
jadaserver: $(OBJS)
	cc $(OBJS) $(LIBS) -o $@
	cp jadaserver $(OUTSITODIR)
	cp jadaserver $(OUTJADADIR)

##############################################################################
clean:
	rm -f $(OBJS) core

