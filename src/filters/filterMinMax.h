#ifndef src_filter_filterminmax_h
#define src_filter_filterminmax_h

template <typename T>
class FilterMinMax
{
public:
	FilterMinMax(const T& min, const T& max):min_(min), max_(max){}
	
	const T operator ()(const T& input)const
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