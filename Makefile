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
test1o1:= test/arduino_test_fence.o
test1 := arduino_test
test11:= arduino_fence
test2o := test/mysql_connect_test.o
test2 := mysql_test
test3o := test/alpr_test_jpg_stdout_redirect.o
test3 := alpr_jpg
test3o1 := test/alpr_test_webcame_com_pipe.o
test31 := alpr_webcam

RMOBJ :=\
	$(ARDO) $(test1o) $(test1)\
	$(test1o1) $(test11)\
	$(SQLO) $(test2o) $(test2)\
	$(test3o) $(test3)$(test3o1) $(test31)

.PHONY: all
all: $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(LDFLAGS) $(INC_PATH) $(SQLFLAGS)

$(test1): $(test1o) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o) $(ARDO)

$(test11): $(test1o1) $(ARDO)
	$(CC) $(CFLAGS) -o $@ $(test1o1) $(ARDO)

$(test2): $(test2o) $(SQLO)
	$(CC) $(CFLAGS) -o $@ $(test2o) $(SQLO) $(LDFLAGS) $(INC_PATH)

$(test3): $(test3o)
	$(CC) $(CFLAGS) -o $@ $(test3o)

$(test31): $(test3o1)
	$(CC) $(CFLAGS) -o $@ $(test3o1)

clean:
	$(RM) -f $(RMOBJ)
