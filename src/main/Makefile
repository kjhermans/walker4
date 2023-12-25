CFLAGS=-g -Wall -Wextra $(DEBUG) -I../include

all: walker

walker: main.o
	$(CC) -o walker main.o \
	  ../lib/libwalker.a ../pishili/libpl.a \
	  ../lib/libwalker.a ../pishili/libpl.a \
	  -lbtree -lnaigama \
	  -lX11 -lXext \
	  -lm \
	  -lGL -lGLU -lglut \

clean:
	rm -f main.o walker
