#ifndef src_filter_filteroffset_h
#define src_filter_filteroffset_h


template <typename T>
class FilterOffset
{
public:
	FilterOffset(const T& offset):offset_(offset){}
	const T operator ()(const T& input)const
	{
		return offset_ + input; 
	}
	
private:
	const T offset_;
};

#endif