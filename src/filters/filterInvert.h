#ifndef src_filter_filterinvert_h
#define src_filter_filterinvert_h

template <typename T>
class FilterInvert
{
public:
	const T operator ()(const T& input)const
	{
		return - input;
	}
};

#endif