CXX=g++
CXXOPTIMIZE= -O2
CXXFLAGS= -g -Wall -pthread -std=c++11 $(CXXOPTIMIZE)
USERID=EDIT_MAKE_FILE
CLASSES= Node.cpp RoutingTable.cpp bellmanford.cpp

all: my-router

my-router: $(CLASSES)
	$(CXX) -o $@ $^ $(CXXFLAGS) $@.cpp


clean:
	rm -rf *.o *~ *.gch *.swp *.dSYM my-router Node RoutingTable bellmanford *.tar.gz

tarball: clean
	tar -cvf project2_15315788.tar.gz *
