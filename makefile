#
# Sean Grimes
# 03.04.16

build: *.cpp
	-@g++ -std=c++11 -pthread *.cpp -o CSVGrader

clean: 
	-@rm -f CSVGrader results.csv

