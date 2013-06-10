CXX=g++
FILES=main.cc

all:
	$(CXX) -I/usr/local -I../../lib_olena/olena-2.0/milena/ $(FILES) -o detect

clean:
	rm -rf *.o \#* *~
