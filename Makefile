CC=gcc
CXX=g++
RM=rm -f
CFLAGS= -ggdb -O0 --std=c99 `pkg-config --cflags json-c`
LDFLAGS= -L/usr/local/lib -lmosquitto -ljson-c -lyaml #-lwiringPi
LDLIBS=

SRCS=main.c switch.c
OBJS=$(subst .c,.o,$(SRCS))

TARGET=mqtt

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .depend

include .depend
