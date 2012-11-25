#include <iostream>

template <typename T>
class MinMaxFilter
{
public:
	MinMaxFilter(const T& min, const T& max):min_(min), max_(max){}
	
	const T& operator ()(const T& input)const
	{
		if(input > max_) return max_;
		if(input < min_) return min_;
		return input;
	}
	
private:
	const T min_;
	const T max_;
};

using namespace std;

int main(int argc, const char* argv[])
{ 
	MinMaxFilter<int> mmf(-20, 20);
	
	for(int i = -30; i <= 30; i += 5)
		cout << mmf(i) << endl;
		
	return 0;
}
