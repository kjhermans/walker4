all: funcheaders
	cd src && make

debug:	
	make all DEBUG="-D_DEBUG=1"

funcheaders: src/include/walker_functions.h

src/include/walker_functions.h: src/lib/*.c
	./bin/genfnchdr ./src/lib/ > src/include/walker_functions.h

cleanlib:
	rm -f src/include/walker_functions.h
	cd src/lib && make clean
	cd src/main && make clean

clean:
	rm -f src/include/walker_functions.h
	@MFS=`find src/ -name Makefile | xargs grep -l 'clean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR clean; \
	  done

superclean: clean
	@MFS=`find src/ -name Makefile | xargs grep -l 'superclean:'`; \
	  for MF in $$MFS; do \
	    DIR=`dirname $$MF`; \
	    make -C $$DIR superclean; \
	  done

archive: superclean
	RELEASE=$$(cat release); \
	/bin/echo "  [TAR] ~/walker4-src-$$RELEASE.tar.gz"; \
	cd .. && \
	  tar czf ~/walker4-src-$$RELEASE.tar.gz \
	  --exclude=\.git walker4/

