CC = g++
CFLAGS = -Wall -Wextra -O2 -std=c++11
all : compile
compile : run lib1.so lib2.so lib3.so lib4.so
run : main.o Field.o RealAnt.o 
	$(CC) $(CFLAGS) -shared -o libloader.so Field.o RealAnt.o -ldl
	$(CC) $(CFLAGS) main.o -ldl -o run -L. -lloader -Wl,-rpath,.
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp
Field.o: Field.hpp Field.cpp
	$(CC) $(CFLAGS) -fPIC -c Field.cpp
RealAnt.o: RealAnt.hpp RealAnt.cpp
	$(CC) $(CFLAGS) -fPIC -c RealAnt.cpp
lib1.so: sample1.o
	$(CC) $(CFLAGS) -shared -o lib1.so sample1.o
lib2.so: sample2.o
	$(CC) $(CFLAGS) -shared -o lib2.so sample2.o
lib3.so: sample3.o
	$(CC) $(CFLAGS) -shared -o lib3.so sample3.o
lib4.so: sample4.o
	$(CC) $(CFLAGS) -shared -o lib4.so sample4.o
sample1.o: sample1.cpp
	$(CC) $(CFLAGS) -fPIC -c sample1.cpp
sample2.o: sample2.cpp
	$(CC) $(CFLAGS) -fPIC -c sample2.cpp
sample3.o: sample3.cpp
	$(CC) $(CFLAGS) -fPIC -c sample3.cpp
sample4.o: sample4.cpp
	$(CC) $(CFLAGS) -fPIC -c sample4.cpp
sample1.cpp: prepare code1.cpp
	./prepare code1.cpp sample1.cpp
sample2.cpp: prepare code2.cpp
	./prepare code2.cpp sample2.cpp
sample3.cpp: prepare code3.cpp
	./prepare code3.cpp sample3.cpp
sample4.cpp: prepare code4.cpp
	./prepare code4.cpp sample4.cpp
prepare: prepare.cpp
	$(CC) $(CFLAGS) prepare.cpp -o prepare
clean:
	rm sample1.cpp sample2.cpp sample3.cpp sample4.cpp prepare run *.so *.o
