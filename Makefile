all: funcheaders
	cd src/pishili && make
	cd src/lib && make LIB3D="-D_USE_PL"
	cd src/main && make

opengl: funcheaders
	touch src/pishili/libpl.a
	cd src/lib && make LIB3D="-D_USE_GL"
	cd src/main && make 

debug:	
	make all DEBUG="-D_DEBUG=1"

funcheaders: src/include/walker_functions.h

src/include/walker_functions.h: src/lib/*.c
	./bin/genfnchdr ./src/lib/ > src/include/walker_functions.h

clean:
	rm -f src/include/walker_functions.h
	@MFS=`find src/ -name Makefile | xargs grep -l 'clean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR clean; \
	  done

archive: clean
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/walker4-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf ~/walker4-src-$$RELEASE.tar.gz \
	  --exclude=\.git walker4/

