all:
		g++ cout_t.cc -std=c++0x -lpthread -o cout_t

clean:
		rm cout_t
