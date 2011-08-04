# Just run "make" to compile the math plugin.

GPP=g++
GCC=gcc
OUTFILE="MathPlugin.so"

COMPILE_FLAGS=-c -m32 -O3 -w -fPIC -DLINUX -include stddef.h -DHAVE_ALLOCA_H -I../../ -I../../../ -I../../SDK/amx/ -I../../include/sampgdk/ -I../../include/ -I../../include/sampgdk/wrapper -I../../include/sampgdk/game/

all:
	$(GCC) $(COMPILE_FLAGS) ../../SDK/amx/*.c
	$(GPP) $(COMPILE_FLAGS) ../../SDK/*.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/game/fakeamx.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/game/objects.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/game/players.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/game/samp.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/game/vehicles.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/logprintf.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/wrapper/callbacks.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/wrapper/eventhandler.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/wrapper/jump.cpp
	$(GPP) $(COMPILE_FLAGS) ../../src/wrapper/wrapper.cpp
	$(GPP) $(COMPILE_FLAGS) *.cpp

	$(GPP) -O2 -W -m32  -march=core2 -fexpensive-optimizations -O3 -O2 -O1 -O  -DNDEBUG -fshort-wchar -shared -o $(OUTFILE) *.o
	rm -f *.o