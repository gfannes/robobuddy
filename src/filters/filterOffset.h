#ifndef src_filter_filteroffset_h
#define src_filter_filteroffset_h


namespace filter
{
    template <typename T>
        class Offset
        {
            public:
                Offset(const T& offset):offset_(offset){}
                const T operator ()(const T& input)const
                {
                    return offset_ + input; 
                }

            private:
                const T offset_;
        };
}

#endif
