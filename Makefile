CXX=g++
FILES=main.cc
LIBPATH=../../lib_olena/olena-2.0/milena/mln #../lib_olena/olena-build/milena/mln/

all:
	$(CXX) -I/usr/local -I../../lib_olena/olena-2.0/milena/ $(FILES) -o detect #-I$(LIBPATH) $(FILES) -o detect

clean:
	rm -rf *.o \#* *~
