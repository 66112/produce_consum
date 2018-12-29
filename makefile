cp:cp.hpp main.cc
	g++ -o $@ $^ -pthread -g
.PHONY:clean
clean:
	rm -f cp
