singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g test/SingleItemTest.cpp -o build/SingleItemText -std=c++23 -I. -D PERFECT_HASH_DEV