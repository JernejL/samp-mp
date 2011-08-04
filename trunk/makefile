# Just run "make" to compile the math plugin.

GPP=g++
GCC=gcc
OUTFILE="MathPlugin.so"

COMPILE_FLAGS=-c -m32 -O3 -w -fPIC -DLINUX -include stddef.h -DHAVE_ALLOCA_H -I../../ -I../../../ -I../../include/sampgdk/  -I../../include/sampgdk/amx/  -I../../include/

all:
	$(GPP) $(COMPILE_FLAGS) ../../src/*.cpp
	$(GPP) $(COMPILE_FLAGS) *.cpp

	$(GPP) -O2 -W -m32  -march=core2 -fexpensive-optimizations -O3 -O2 -O1 -O  -DNDEBUG -fshort-wchar -shared -o $(OUTFILE) *.o
	rm -f *.o