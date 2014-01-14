CFLAGS = -O2 -Wall -std=c++0x
LDFLAGS = -ldivsufsort

OBJS = RankedBitmap.o WaveletTree.o FmIndex.o
MAINS = main main_simple main_construct

all: $(MAINS)

%.o: %.cpp
	g++ -c $< -o $@ $(CFLAGS) $(LDFLAGS)

main: $(OBJS) main.cpp
	g++ $+ -o $@ $(CFLAGS) $(LDFLAGS)

main_simple: $(OBJS) main_simple.cpp
	g++ $+ -o $@ $(CFLAGS) $(LDFLAGS)

main_construct: $(OBJS) main_construct.cpp
	g++ $+ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm *.o
	rm $(MAINS)
