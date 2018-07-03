COMPILER = g++ -Wunused-function -I "$(shell readlink ../json/include/ -f)/" -I "$(shell readlink ../tclap-1.2.2/include/ -f)/"
ARGS = -fpic
CARGS = -g -Wall
LIBS = -lm -ldl -lpthread
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c*))
HEADERS = $(wildcard *.h)

all: libsqfvm.so sqfvm

%.o: %.c $(HEADERS)
        $(COMPILER) $(ARGS) $(CARGS) -c $< -o $@

libsqfvm.so: $(OBJECTS)
        $(COMPILER) $(ARGS) -shared -o $@ $(OBJECTS) $(LIBS)

sqfvm: libsqfvm.so
        $(COMPILER) $(ARGS) -o $@ -L. -lsqfvm $(LIBS)

clean:
        rm sqfvm
        rm -f *.so
        rm -f *.o
        rm -f *.bak *~

remake: clean all