C++ = g++

OPTMAC = -fast

PRODUCT=glut_example

HEADERS = $(wildcard *.hpp *.h)

# Mac specific stuff
FRAMEWORK = -framework GLUT
FRAMEWORK += -framework OpenGL
MACLIBS = -lGL -lGLU -lm -lstdc++ 
MACINCS = -L"/System/Library/Frameworks/OpenGL.framework/Libraries"

#-----------------------------------------
CCFLAGS = $(OPTMAC) $(MACINCS) -DOSX
LDFLAGS = $(OPTMAC) $(MACINCS) $(MACLIBS) -DOSX $(FRAMEWORK)

all: $(PRODUCT)

$(PRODUCT): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(C++) $(LDFLAGS) $^ -o $(PRODUCT)

%.o: %.cpp $(HEADERS)
	$(C++) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf $(PRODUCT)