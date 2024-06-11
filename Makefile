singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g test/SingleItemTest.cpp -o build/SingleItemTest -std=c++23 -I. -D PERFECT_HASH_DEV

primesearchdemo: demo/PrimesDemo.cpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -g demo/PrimesDemo.cpp -o build/PrimesDemo -std=c++23 -I. -D PERFECT_HASH_DEV

clean:
	rm -rf build