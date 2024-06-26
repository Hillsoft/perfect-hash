all: singleitemtest primedemosearch build/PrimesDemoValidate squaredemosearch build/SquaresDemoValidate twoonesearch build/TwoOneDemoValidate

singleitemtest: test/SingleItemTest.cpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -O2 -g test/SingleItemTest.cpp -o build/SingleItemTest -std=c++23 -I. -D PERFECT_HASH_DEV

primedemosearch: demo/PrimesDemoSearch.cpp demo/PrimesDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -O2 -g demo/PrimesDemoSearch.cpp -o build/PrimesDemoSearch -std=c++23 -I.

build/PrimesDemoValidate: demo/PrimesDemoValidation.cpp demo/PrimesDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -O2 -g demo/PrimesDemoValidation.cpp -o build/PrimesDemoValidate -std=c++23 -I.

squaredemosearch: demo/SquaresDemoSearch.cpp demo/SquaresDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -O2 -g demo/SquaresDemoSearch.cpp -o build/SquaresDemoSearch -std=c++23 -I.

build/SquaresDemoValidate: demo/SquaresDemoValidation.cpp demo/SquaresDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -O2 -g demo/SquaresDemoValidation.cpp -o build/SquaresDemoValidate -std=c++23 -I.

twoonesearch: demo/TwoOneDemoSearch.cpp demo/TwoOneDemo.hpp searcher/Searcher.hpp
	@mkdir -p build
	g++ -O2 -g demo/TwoOneDemoSearch.cpp -o build/TwoOneDemoSearch -std=c++23 -I.

build/TwoOneDemoValidate: demo/TwoOneDemoValidation.cpp demo/TwoOneDemo.hpp lib/PerfectHash.hpp
	@mkdir -p build
	g++ -O2 -g demo/TwoOneDemoValidation.cpp -o build/TwoOneDemoValidate -std=c++23 -I.

test: build/PrimesDemoValidate build/SquaresDemoValidate build/TwoOneDemoValidate
	build/PrimesDemoValidate
	build/SquaresDemoValidate
	build/TwoOneDemoValidate

clean:
	rm -rf build