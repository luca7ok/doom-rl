ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

ENV_NAME := $(word 1,$(RUN_ARGS))
AGENT_NAME := $(word 2,$(RUN_ARGS))

.PHONY: build run clean

build: build/build.ninja
	@cmake --build build > build/last_build.log 2>&1 || { cat build/last_build.log; exit 1; }

build/build.ninja:
	@cmake -S . -B build -G Ninja -DCMAKE_IGNORE_PREFIX_PATH=$(HOME)/anaconda3 -DCMAKE_C_COMPILER=clang-22 -DCMAKE_CXX_COMPILER=clang++-22 > /dev/null || exit 1

run: build
	@clear
	./build/doom_rl $(if $(ENV_NAME),--env $(ENV_NAME)) $(if $(AGENT_NAME),--agent $(AGENT_NAME))

clean:
	rm -rf build
