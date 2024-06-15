singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g test/SingleItemTest.cpp -o build/SingleItemTest -std=c++23 -I. -D PERFECT_HASH_DEV

primedemosearch: demo/PrimesDemoSearch.cpp demo/PrimesDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -g demo/PrimesDemoSearch.cpp -o build/PrimesDemoSearch -std=c++23 -I.

primedemovalidate: demo/PrimesDemoValidation.cpp demo/PrimesDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g demo/PrimesDemoValidation.cpp -o build/PrimesDemoValidate -std=c++23 -I.

squaredemosearch: demo/SquaresDemoSearch.cpp demo/SquaresDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -g demo/SquaresDemoSearch.cpp -o build/SquaresDemoSearch -std=c++23 -I.

squaredemovalidate: demo/SquaresDemoValidation.cpp demo/SquaresDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -g demo/SquaresDemoValidation.cpp -o build/SquaresDemoValidate -std=c++23 -I.

clean:
	rm -rf build