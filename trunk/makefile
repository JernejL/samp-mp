# Just run "make" to compile the math plugin.

GPP=g++
GCC=gcc
OUTFILE="MathPlugin.so"

COMPILE_FLAGS=-c -O3 -w -fPIC -DLINUX -I../../ -I../../SDK/amx/ -I../../include/sampgdk/ -I../../include/ -I../../include/sampgdk/wrapper -I../../include/sampgdk/game/

all:
	$(GCC) $(COMPILE_FLAGS) ../../SDK/amx/*.c
	$(GPP) $(COMPILE_FLAGS) ../../SDK/*.cpp
	$(GPP) $(COMPILE_FLAGS) *.cpp
	$(GPP) -O2 -fshort-wchar -shared -o $(OUTFILE) *.o
	rm -f *.o