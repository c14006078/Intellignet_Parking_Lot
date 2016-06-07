#  Configuration  #

CC = gcc
CFLAGS =\
  -g -Wall -std=c99
LDFLAGS=

EXEC := intelligentparkinglot
ARDO := ArduinoInterface/term2Arduino.o
OBJS :=

test1o := test/arduino_connect_test.o
test1 := test_arduino

RMOBJ :=\
	$(ARDO) $(test1o) $(test1)

.PHONY: all
all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO)

clean:
	$(RM) -f $(RMOBJ)
