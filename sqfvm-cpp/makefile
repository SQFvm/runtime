CC_ARGS = -fpic -Wall
CXX_ARGS = -fpic -std=c++14
INCLUDE = -I ../json/include/ -I ../tclap-1.2.2/include/
LIBS = -lm -ldl -lpthread
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c*))
HEADERS = $(wildcard *.h)

all: libsqfvm.so sqfvm

%.o: %.c $(HEADERS)
	$(CC) $(CC_ARGS) $(INCLUDE) -c $< -o $@

libsqfvm.so: $(OBJECTS)
	$(CXX) $(CXX_ARGS) $(INCLUDE) -shared -o $@ $(OBJECTS) $(LIBS)

sqfvm: libsqfvm.so
	$(CXX) $(CXX_ARGS) $(INCLUDE) -o $@ -L. -lsqfvm $(LIBS)

clean:
	rm sqfvm
	rm -f *.so
	rm -f *.o
	rm -f *.bak *~

remake: clean all