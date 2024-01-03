all: examine_dependencies pictures
	cd PL3D-KC && make -f makefile.linux
	cd sdbm_tree && make
	cd naigama && make
	cd lib && make
	cd main && make

pictures:
	cd pics && make

examine_dependencies:
	../bin/assertlib \
	  https://github.com/LMP88959/PL3D-KC \
	  PL3D-KC \
	  pl3d-kc.patch	\
	  'mkdir bin'
	../bin/assertlib \
	  https://github.com/kjhermans/sdbm_tree \
	  sdbm_tree
	../bin/assertlib \
	  https://github.com/kjhermans/naigama \
	  naigama

clean:
	true

superclean:
	rm -rf PL3D-KC/ sdbm_tree/ naigama/
