#ifndef src_filter_filteraveraging_h
#define src_filter_filteraveraging_h

#include <iostream>

namespace filter
{
    template <typename Type, int Size = 5>
        class Samples
        {
            public:
                 Samples():fillPos_(0), nbrSamples_(0)
		{
			Type* p = &samples_[0];
			Type* pe =  p + Size;
			while(p < pe)
				*p++ = 0;
		}
		void add(const Type& s)
		{
			samples_[fillPos_] = s;
			if(++fillPos_ > Size)
				fillPos_ = 0;
			if(nbrSamples_ < Size)
				++nbrSamples_;
		}
		const Type average() const
                {
			const Type* p = &samples_[0];
			const Type* pe =  p +  nbrSamples_;
			Type sum = 0;
			while(p < pe)
				sum += *p++;
			return sum / nbrSamples_; 
                }
		const Type median() const
		{
			return 0;
		}

            private:
                Type samples_[Size];
		int nbrSamples_;
		int fillPos_;
        };
}

#endif
