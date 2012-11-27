#This test can be compiled and ran with following command if gubg is installed properly:
#r gb -re filterTest.cpp

.PHONY: all
.PHONY: clean

all: filterTest

clean:
	rm -f filterTest

filterTest: filterTest.cpp
	g++ -o filterTest filterTest.cpp -I../../
