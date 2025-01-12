
CC = gcc
STRIP = strip

ARGS = -march=native
DEBUG_ARGS = -g3 -Wall -Wextra -Wconversion -Wdouble-promotion \
		-Wno-unused-parameter -Wno-unused-function -Wno-sign-conversion \
		-fsanitize=address,undefined -fsanitize-undefined-trap-on-error \
		-std=c99 -pedantic -DDEBUG
RELEASE_ARGS = -O3 

LIBS = -lm -ldl

SRCS =         $(filter-out src/main.c, $(wildcard src/*.c))
HEADERS =      $(wildcard src/*.h)

TESTS =        $(filter-out tests/test_main.c, $(wildcard tests/*.c))
TEST_HEADERS = $(wildcard tests/*.h)

DEMOS =        $(filter-out demos/demo_main.c, $(wildcard demos/*.c))
DEMO_HEADERS = $(wildcard demos/*.h)

.PHONY: all
all: main_debug main_release test_debug test_release demo_debug demo_release

.PHONY: clean
clean: 
	rm -rfv main_debug main_release test_debug test_release demo_debug demo_release demo-out

demo-out: 
	mkdir -p demo-out

main_debug: src/main.c $(SRCS) $(HEADERS)
	$(CC) $(ARGS) $(DEBUG_ARGS) $(LIBS) -o $@ $< $(SRCS)

main_release: src/main.c $(SRCS) $(HEADERS)
	$(CC) $(ARGS) $(RELEASE_ARGS) $(LIBS) -o $@ $< $(SRCS)
	$(STRIP) $@

test_debug: tests/test_main.c $(SRCS) $(HEADERS) $(TESTS) $(TEST_HEADERS)
	$(CC) $(ARGS) $(DEBUG_ARGS) $(LIBS) -o $@ $< $(SRCS) $(TESTS)

test_release: tests/test_main.c $(SRCS) $(HEADERS) $(TESTS) $(TEST_HEADERS)
	$(CC) $(ARGS) $(RELEASE_ARGS) $(LIBS) -o $@ $< $(SRCS) $(TESTS)
	$(STRIP) $@

demo_debug: demos/demo_main.c $(SRCS) $(HEADERS) $(DEMOS) $(DEMO_HEADERS) demo-out
	$(CC) $(ARGS) $(DEBUG_ARGS) $(LIBS) -o $@ $< $(SRCS) $(DEMOS)

demo_release: demos/demo_main.c $(SRCS) $(HEADERS) $(DEMOS) $(DEMO_HEADERS) demo-out
	$(CC) $(ARGS) $(RELEASE_ARGS) $(LIBS) -o $@ $< $(SRCS) $(DEMOS)
	$(STRIP) $@
