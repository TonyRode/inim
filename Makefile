CXX=g++
FILES=main.cc rgb.cc tab_bounds.cc
FLAGS=-std=c++0x
#-I/usr/local -I../../lib_olena/olena-2.0/milena/ $(FILES) -o detect

all:
	$(CXX) $(FLAGS) $(FILES) -o detect

clean:
	rm -rf *.o \#* *~
