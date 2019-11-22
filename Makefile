all:
	cd lib; make
	cd src; make

doc:
	doxygen ./doc/Doxyfile

.PHONY: all doc
