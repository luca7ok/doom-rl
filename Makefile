ifneq (,$(filter run train,$(firstword $(MAKECMDGOALS))))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

ENV_NAME := $(word 1,$(RUN_ARGS))
AGENT_NAME := $(word 2,$(RUN_ARGS))
DOOM_ARGS := $(if $(ENV_NAME),--env $(ENV_NAME)) $(if $(AGENT_NAME),--agent $(AGENT_NAME))

ifeq ($(shell uname),Darwin)
  C_COMPILER := /usr/bin/clang
  CXX_COMPILER := /usr/bin/clang++
else
  C_COMPILER := clang-22
  CXX_COMPILER := clang++-22
endif

.PHONY: build run train clean

build: build/build.ninja
	@cmake --build build > build/last_build.log 2>&1 || { cat build/last_build.log; exit 1; }

build/build.ninja:
	@cmake -S . -B build -G Ninja -DCMAKE_IGNORE_PREFIX_PATH=$(HOME)/anaconda3 -DCMAKE_C_COMPILER=$(C_COMPILER) -DCMAKE_CXX_COMPILER=$(CXX_COMPILER) > /dev/null || exit 1

run: build
	@clear
	./build/doom_rl run $(DOOM_ARGS)

train: build
	@clear
	./build/doom_rl train $(DOOM_ARGS)

clean:
	rm -rf build
