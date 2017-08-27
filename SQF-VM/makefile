COMPILER = gcc -std=c99
ARGS = -fpic
CARGS = -g -Wall
LIBS = -lm -ldl
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

all: libsqfvm.so prog

%.o: %.c $(HEADERS)
	$(COMPILER) $(ARGS) $(CARGS) -c $< -o $@

libsqfvm.so: $(OBJECTS)
	$(COMPILER) $(ARGS) -shared -o $@ $(OBJECTS) $(LIBS)

prog: libsqfvm.so
	$(COMPILER) $(ARGS) -o $@ -L. -lsqfvm $(LIBS)

clean:
	rm prog
	rm -f *.so
	rm -f *.o
	rm -f *.bak *~

remake: clean all
