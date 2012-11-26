#ifndef src_snippets_minmaxfilter_h
#define src_snippets_minmaxfilter_h

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


#endif