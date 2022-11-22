all: readprimitive

readprimitive: readprimitive.cpp
	g++ -g -O0 -no-pie -o readprimitive readprimitive.cpp

clean:
	rm readprimitive