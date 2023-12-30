INCLIBS=-I../include -I../PL3D-KC -I../sdbm_tree
CFLAGS=-g -Wall -Wextra $(DEBUG) $(INCLIBS)
LINKABLES=\
  main.o \
  ../PL3D-KC/bin/libpl.o \
  ../PL3D-KC/bin/libfw.o \
  ../lib/libwalker.a \
  ../sdbm_tree/libbtree.a \
  ../naigama/src/lib/naigama/libnaigama.a

all: walker

walker: $(LINKABLES)
	$(CC) -o walker $(LINKABLES) \
	  -lX11 -lXext \
	  -lm \

../PL3D-KC/bin/libpl.o:
	cd ../PL3D-KC/ && make

../PL3D-KC/bin/libfw.o:
	cd ../PL3D-KC/ && make

../lib/libwalker.a:
	cd ../lib && make

../sdbm_tree/libbtree.a:
	cd ../sdbm_tree && make

../naigama/src/lib/naigama/libnaigama.a:
	cd ../naigama && make

clean:
	rm -f main.o walker
