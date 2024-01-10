all: tnt.h rock.h

tnt.h: tnt.png
	perl ./make_texture.pl tnt.png > tnt.h

rock.h: rock.png
	perl ./make_texture.pl rock.png > rock.h

clean:
	rm -f tnt.h
