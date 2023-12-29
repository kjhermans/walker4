INCLIBS=-I../include -I../PL3D-KC -I../sdbm_tree
CFLAGS=-g -Wall -Wextra $(DEBUG) $(INCLIBS)

all: walker

walker: main.o
	$(CC) -o walker main.o \
	  ../PL3D-KC/bin/libpl.o \
	  ../PL3D-KC/bin/libfw.o \
	  ../lib/libwalker.a \
	  ../sdbm_tree/libbtree.a \
	  ../naigama/src/lib/naigama/libnaigama.a \
	  -lX11 -lXext \
	  -lm \

clean:
	rm -f main.o walker
