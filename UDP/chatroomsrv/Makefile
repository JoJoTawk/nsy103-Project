OBJS = chatroomsrv.o
#LIBS = -l
#PATH = -L
#HEAD = -I
CFLAGS = -Wall -g

chatroomsrv : ${OBJS}
	cc ${CFLAGS} -o chatroomsrv ${OBJS}

echosrv.o : unphead.h

.PHONY : clean

clean : 
	-rm -f chatroomsrv ${OBJS}
