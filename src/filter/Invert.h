#ifndef src_filter_filterinvert_h
#define src_filter_filterinvert_h

namespace filter
{
    template <typename T>
        class Invert
        {
            public:
                const T operator ()(const T& input)const
                {
                    return - input;
                }
        };
}

#endif
