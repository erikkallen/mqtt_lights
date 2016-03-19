CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS= --std=c++11
LDFLAGS= -lwiringPi -lmosquittopp
LDLIBS=

SRCS=mqtt.cpp main.cpp NewRemoteTransmitter.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

TARGET=mqtt

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
