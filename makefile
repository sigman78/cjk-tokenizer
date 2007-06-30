CPP = g++
CPP_FLAGS = -Wall -O3
MODULE = cjk-tokenizer

all: build

build: cjk-tokenizer.o libcjk-tokenizer.a

cjk-tokenizer.o:
	$(CPP) $(CFLAGS) -c cjk-tokenizer.cc

libcjk-tokenizer.a: cjk-tokenizer.o
	ar crv $@ cjk-tokenizer.o
	ranlib $@

test:
	$(CPP) $(CFLAGS) $(INC) $(LIB) $(DEFINES) -o $@ $@.cc \
	-L. -lcjk-tokenizer -lunicode
	./$@

clean:
	rm -rf *.o *~ *.a test
