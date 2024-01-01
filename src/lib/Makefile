OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')
LIB=libwalker.a
CC=$(ARCH)gcc
AR=$(ARCH)ar
INCLIBS=-I../include -I../PL3D-KC -I../sdbm_tree -I../naigama/src/include
CFLAGS=-g -Wall -Wextra -I../include $(INCLIBS) $(DEBUG) $(PROFILE)

all: $(LIB)

$(LIB): $(OBJECTS)
	$(AR) -rcs $(LIB) $(OBJECTS)

clean:
	@rm -f $(OBJECTS) $(LIB)
