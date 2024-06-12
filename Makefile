singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g test/SingleItemTest.cpp -o build/SingleItemTest -std=c++23 -I. -D PERFECT_HASH_DEV

primedemosearch: demo/PrimesDemoSearch.cpp demo/PrimesDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -g demo/PrimesDemoSearch.cpp -o build/PrimesDemoSearch -std=c++23 -I.

primedemovalidate: demo/PrimesDemoValidation.cpp demo/PrimesDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g demo/PrimesDemoValidation.cpp -o build/PrimesDemoValidate -std=c++23 -I.

clean:
	rm -rf build