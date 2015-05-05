#
# $Id: Makefile 55 2006-12-18 22:04:32Z nicb $
#
#
TOP=../..
PLUGIN=integrators
TARGET=$(PLUGIN).pd_linux
OBJS=$(PLUGIN).o integrator~.o integrator.o

all:	$(TARGET)

$(TARGET):		$(OBJS)
	$(LD) -export_dynamic -shared -o $@ $(OBJS) $(PD_LIBS)

clean:
	$(RM) $(OBJS) $(TARGET)

include $(TOP)/Makefile.common
CFLAGS += -g
