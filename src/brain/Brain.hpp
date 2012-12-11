#ifndef rb_brain_Brain_hpp
#define rb_brain_Brain_hpp

#include "brain/Codes.hpp"
#include <chrono>

namespace brain
{
    template <typename Receiver, typename Data, typename TimePoint>
        class Brain_crtp
        {
            public:
                typedef typename TimePoint::duration duration;

                Brain_crtp():
                    currentIsFirst_(true){}

                void update(duration dT)
                {
                    if (now_ == start_)
                        receiver_().brain_initialize();

                    dT_ = dT;
                    now_ += dT_;
                    swap();

                    receiver_().brain_beforeUpdate();
                    Data &cd = current();
                    receiver_().brain_update(cd.template begin<double>(), cd.template end<double>());
                    //Still need to integrate some MultiCaller that goes over all Data types
                    //receiver_().brain_update(cd.template begin<int>(), cd.template end<int>());
                    receiver_().brain_afterUpdate();
                }

                //Access to current and previous data, and swapping them
                Data &current(){return (currentIsFirst_ ? datas_[0] : datas_[1]);}
                Data &previous(){return (currentIsFirst_ ? datas_[1] : datas_[0]);}
                void swap(){currentIsFirst_ = !currentIsFirst_;}

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

                Data datas_[2];
                bool currentIsFirst_;
        };
}

#endif
