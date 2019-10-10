CFLAGS= -g -pedantic -std=c++11

doublets_shaker: doublets_shaker.cc
	clang++ $(CFLAGS) doublets_shaker.cc -o doublets_shaker

clean:
	rm doublets_shaker
