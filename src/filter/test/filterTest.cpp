#include "filter/MinMax.h"
#include "filter/Invert.h"
#include "filter/Offset.h"
#include "filter/Samples.h"

#include <iostream>
using namespace filter;
using namespace std;

int main(int argc, const char* argv[])
{ 
	cout << "--- MinMax Filter ---" << endl;
	MinMax<int> mm(-20, 20);
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << mm(i) << endl;
		
	cout << "--- Invert Filter ---" << endl;
	Invert<int> inv;
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << inv(i) << endl;

	cout << "--- Offset Filter ---" << endl;
	Offset<int> off(30);
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << off(i) << endl;

	cout << "--- Chained Filter ---" << endl;
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << mm(off(inv(i))) << endl;
	
	cout << "--- Averaging Filter ---" << endl;
	Samples<int, 5> samples;
	
	samples.add(4);
	samples.add(5);
	samples.add(6);
	cout << samples.average() << endl;
	samples.add(7);
	samples.add(8);
	samples.add(9);
	cout << samples.average() << endl;
	cout << "--- End ---" << endl;
	
	return 0;
}
