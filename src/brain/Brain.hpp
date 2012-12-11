#ifndef rb_brain_Brain_hpp
#define rb_brain_Brain_hpp

#include "brain/Codes.hpp"
#include <chrono>

namespace brain
{
    template <typename Receiver, typename TimePoint>
        class Brain_crtp
        {
            public:
                typedef typename TimePoint::duration duration;
                void process(duration dT)
                {
                    dT_ = dT;
                    now_ += dT_;
                    receiver_().brain_processAll();
                }

                duration dT() const {return dT_;}
                double dT_double() const
                {
                    typedef std::chrono::duration<double, std::ratio<1>> double_duration;
                    return std::chrono::duration_cast<double_duration>(dT_).count();
                }
                TimePoint now() const {return now_;}
                duration age() const {return now_ - start_;}
                double age_double() const
                {
                    typedef std::chrono::duration<double, std::ratio<1>> double_duration;
                    return std::chrono::duration_cast<double_duration>(age()).count();
                }

            private:
                const TimePoint start_;
                TimePoint now_;
                duration dT_;
                Receiver &receiver_(){return static_cast<Receiver&>(*this);}
        };
}

#endif
