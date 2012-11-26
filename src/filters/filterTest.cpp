#include "filterMinMax.h"
#include "filterInvert.h"
#include "filterOffset.h"
#include <iostream>


using namespace std;

int main(int argc, const char* argv[])
{ 
	cout << "--- MinMax Filter ---" << endl;
	FilterMinMax<int> mm(-20, 20);
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << mm(i) << endl;
		
	cout << "--- Invert Filter ---" << endl;
	FilterInvert<int> inv;
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << inv(i) << endl;

	cout << "--- Offset Filter ---" << endl;
	FilterOffset<int> off(30);
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << off(i) << endl;

	cout << "--- Chained Filter ---" << endl;
	for(int i = -30; i <= 30; i += 5)
		cout << i << " -> " << mm(off(inv(i))) << endl;
	
	return 0;
}
