#  Configuration  #

CC = gcc
CFLAGS =\
  -g -Wall
LDFLAGS=\
	-lmysqlclient
SQLFLAGS=\
	-D SQL_DEBUG=1

# YOU SHOULD CHECK THIS #
INC_PATH= -I /usr/include/mysql


EXEC := intelligentparkinglot
ARDO := ArduinoInterface/term2Arduino.o
SQLO := MySQL_Interface/mysql_api.o
OBJS :=

test1o := test/arduino_connect_test.o
test1 := arduino_test
test2o := test/mysql_connect_test.o
test2 := mysql_test

RMOBJ :=\
	$(ARDO) $(test1o) $(test1)\
	$(SQLO) $(test2o) $(test2)

.PHONY: all
all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INC_PATH) $(SQLFLAGS)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO)

$(test2): $(test2o) $(SQLO)
	$(CC) $(CFLAGS) -o $@ $(test2o) $(SQLO) $(LDFLAGS) $(INC_PATH)

clean:
	$(RM) -f $(RMOBJ)
