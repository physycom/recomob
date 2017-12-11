INCLUDE = -Isrc -Isrc/jsoncons/include -Isrc/utils
LINK = -lboost_filesystem -lboost_system

all: dir recomob #run

recomob: 
	$(CXX) -std=c++11 $(INCLUDE) -o bin/recomob src/main.cpp $(LINK)

run:
	bin/recomob data/datafile.csv

dir:
	mkdir -p bin
