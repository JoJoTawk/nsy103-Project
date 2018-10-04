OBJS = chatroomcli.o
#LIBS = -l
#PATH = -L
#HEAD = -I
CFLAGS = -Wall -g

chatroomcli : chatroomcli.o
	cc ${CFLAGS} -o chatroomcli ${OBJS}

echoroomcli.o : unphead.h

.PHONY : clean

clean : 
	-rm -f chatroomcli ${OBJS}
