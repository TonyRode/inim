CXX=g++
FILES=main.cc rgb.cc tab_bounds.cc
FLAGS=-std=c++0x

all:
	$(CXX) $(FLAGS) $(FILES) -o detect

clean:
	rm -rf *.o \#* *~


distclean: clean
	rm -rf detect