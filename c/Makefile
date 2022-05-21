
LIBS  = -lm
CFLAGS = -Werror -g

SRC=$(wildcard *.c)
HDR=$(wildcard *.h)

rtc: $(SRC) $(HDR) labrat
	./labrat
	gcc -o $@ $(SRC) $(CFLAGS) $(LIBS)

labrat: labrat.h
	gcc -x c labrat.h -D LR_GEN_EXECUTABLE -o labrat
	./labrat

.PHONY: clean
clean:
	rm -rf labrat labrat_data.c rtc

.PHONY: dev
dev:
	make rtc
	./rtc --lr-run-tests


