CXX=g++
FILES=main.cc
#-I/usr/local -I../../lib_olena/olena-2.0/milena/ $(FILES) -o detect

all:
	$(CXX) $(FILES) -o detect

clean:
	rm -rf *.o \#* *~
