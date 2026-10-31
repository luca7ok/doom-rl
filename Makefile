.PHONY: build run clean

build: build/build.ninja
	@cmake --build build > build/last_build.log 2>&1 || { cat build/last_build.log; exit 1; }

build/build.ninja:
	@cmake -S . -B build -G Ninja -DCMAKE_IGNORE_PREFIX_PATH=$(HOME)/anaconda3 -DCMAKE_C_COMPILER=clang-22 -DCMAKE_CXX_COMPILER=clang++-22 > /dev/null || exit 1

run: build
	@clear
	./build/doom_rl $(ARGS)

clean:
	rm -rf build
