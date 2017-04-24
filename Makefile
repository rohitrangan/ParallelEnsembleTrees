# Makefile

CXX=c++
CXXFLAGS= -Wall -Wextra -std=c++0x
CXXSRCS=src/main.cpp src/data.cpp src/node.cpp src/decision_tree.cpp src/random_forest.cpp
CXXOBJS=$(CXXSRCS:.cpp=.o)
LIBS=
INCLUDES=-I./include
TARGET=ensemble_trees
#DEBUGFLAGS=-g -ggdb -O0 -fno-inline

# Required for the target clean.
RM=rm -rf

CXXFLAGS+=$(DEBUGFLAGS) $(INCLUDES) $(LIBS)

all : $(CXXSRCS) $(CXXOBJS) $(TARGET)

$(TARGET) : $(CXXOBJS)
	$(CXX) $(CXXOBJS) -o $(TARGET)

.cpp.o :
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) src/*.o src/*.dSYM $(TARGET)

depend : .depend

.depend : $(CXXSRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ -MF ./.depend

.PHONY : clean

include .depend

