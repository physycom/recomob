### Purpose
This work has been developed as a bachelor's thesis project. The tools provided allows you to analyze GNSS data and build a one-step model of urban pedestrian mobility. 

### Installation
**CMake** and a **C++11** compatible compiler are required. To build the executable, clone the repo and then type  
```
mkdir build ; cd build ; cmake .. ; cmake --build .
```
With CMake you can also deploy projects for the most common IDEs.  
This project uses [boost libraries](http://www.boost.org).

### Synopsis
```
./bin/recomob data/datafile.csv
```
where *datafile.csv* must be an existing and valid .csv file, having the format defined as follows:
```
"GEID","CALL_ID","DATE","TIME","NETWORK","LATITUDE","LONGITUDE", 22 unused fields more
```
