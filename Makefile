dt: dtinduce.o dtclassify.o showconfmatrix.o
	g++ -std=c++11 -o dtinduce dtinduce.o
	g++ -std=c++11 -o dtclassify dtclassify.o
	g++ -std=c++11 -o showconfmatrix showconfmatrix.o

dtinduce.o: dtinduce.cpp
	g++ -std=c++11 -c dtinduce.cpp

dtclassify.o: dtclassify.cpp
	g++ -std=c++11 -c dtclassify.cpp

showconfmatrix.o: showconfmatrix.cpp
	g++ -std=c++11 -c showconfmatrix.cpp

rf: rf.o rfmerge.o
	g++ -std=c++11 -o rf rf.o
	g++ -std=c++11 -o rfmerge rfmerge.o

rf.o: rf.cpp
	g++ -std=c++11 -c rf.cpp

rfmerge.o: rfmerge.cpp
	g++ -std=c++11 -c rfmerge.cpp

clean:
	rm -rf *o dtinduce dtclassify showconfmatrix rf rfmerge
