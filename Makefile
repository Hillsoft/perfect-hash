singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g test/SingleItemTest.cpp -o build/SingleItemTest -std=c++23 -I. -D PERFECT_HASH_DEV

clean:
	rm -rf build