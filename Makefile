# Makefile

CXX=c++
CXXFLAGS= -Wall -Wextra -std=c++0x
CXXSRCS=src/main.cpp src/data.cpp src/node.cpp src/decision_tree.cpp src/random_forest.cpp src/adaboost.cpp
CXXOBJS=$(CXXSRCS:.cpp=.o)
LIBS=
INCLUDES=-I./include
TARGET=ensemble_trees
DEBUGFLAGS=-g -ggdb -O0 -fno-inline

MPICXX=mpic++
MPICXXFLAGS= -Wall -Wextra -std=c++0x
MPICXXSRCS=mpisrc/main_pl.cpp mpisrc/data.cpp mpisrc/node.cpp mpisrc/decision_tree.cpp
MPICXXOBJS=$(MPICXXSRCS:.cpp=.o)
MPILIBS=
MPIINCLUDES=-I./include
MPITARGET=ensemble_trees_pl
MPIDEBUGFLAGS=-g -ggdb -O0 -fno-inline

# Required for the target clean.
RM=rm -rf

CXXFLAGS+=$(DEBUGFLAGS) $(INCLUDES) $(LIBS)

MPICXXFLAGS+=$(MPIDEBUGFLAGS) $(MPIINCLUDES) $(MPILIBS)

all : $(CXXSRCS) $(CXXOBJS) $(TARGET) $(MPICXXSRCS) $(MPICXXOBJS) $(MPITARGET)

$(TARGET) : $(CXXOBJS)
	$(CXX) $(CXXOBJS) -o $(TARGET)

$(MPITARGET) : $(MPICXXOBJS)
	$(MPICXX) $(MPICXXOBJS) -o $(MPITARGET)

src/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

mpisrc/%.o : mpisrc/%.cpp
	$(MPICXX) $(MPICXXFLAGS) -c $< -o $@

clean :
	$(RM) src/*.o src/*.dSYM mpisrc/*.o mpisrc/*.dSYM $(TARGET) $(MPITARGET)

depend : .depend

mpidepend : .mpidepend

.depend : $(CXXSRCS)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^ -MF ./.depend

.mpidepend : $(MPICXXSRCS)
	rm -f ./.mpidepend
	$(MPICXX) $(MPICXXFLAGS) -MM $^ -MF ./.mpidepend

.PHONY : clean

include .depend
include .mpidepend

