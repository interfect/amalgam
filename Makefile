# Default settings
CPPFLAGS += -std=c++11 -Iinclude -g
# Make sure to set the RUNPATH so we can find libraries relative to our binary
LDFLAGS += -lm -Llib -ltcodxx_debug -ltcod_debug
CXX := g++-5

# Then specialize based on platform
UNAME := $(shell uname)
ifeq ($(UNAME), Linux)
CXX := g++-5
LDFLAGS += -Wl,-rpath,\$$ORIGIN/lib,--disable-new-dtags
endif
ifeq ($(UNAME), Darwin)
CXX := g++
LDFLAGS += -Wl,-rpath,@executable_path/lib
endif

all: amalgam

amalgam: main.o
	$(CXX) $^ -o $@ $(LDFLAGS)
	
clean:
	rm -f *.o alamgam
	
fix_dylibs:
	# We need to set the install names of the OS X dylibs. These get copied into
	# binaries linked against them, and then be binaries go back and find the
	# dylibs at those paths.
	# We need to go through pairs to fix any cross-references
	for DYLIB in `ls lib/*.dylib`; \
	do \
		install_name_tool -id @executable_path/lib/`basename $${DYLIB}` $${DYLIB}; \
		for DYLIB2 in `ls lib/*.dylib`; \
		do \
			install_name_tool -change `basename $${DYLIB2}` @executable_path/lib/`basename $${DYLIB2}` $${DYLIB}; \
		done; \
	done
