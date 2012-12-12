#ifndef rb_brain_Brain_hpp
#define rb_brain_Brain_hpp

#include <chrono>

namespace brain
{
    //Generic CRTP class that provides the basics to implement dynamic behaviour
    //This class holds 2 Data slabs and keeps track of time
    //Upon each update(dT), time is updated, the data slabs are swapped (current becomes previous)
    //and the CRTP brain_update(It it, It end) event points are called to update the new current (it and end are the range of current)
    //When updating such a data array of current(), you should _only_ access data elements in current that are already updated (smaller ix)
    //Accessing data array elements from the previous() slab is OK
    template <typename Receiver, typename Data, typename TimePoint>
        class Brain_crtp
        {
            public:
                typedef typename TimePoint::duration duration;

                Brain_crtp():
                    currentIsFirst_(true){}

            private:
                //This callback is used in update() to call receiver_().brain_update() for the
                //different data containers
                struct Callback
                {
                    Receiver &receiver;
                    Callback(Receiver &r):receiver(r){}
                    template <typename Container>
                        void operator()(Container &container)
                        {
                            receiver.brain_update(container.begin(), container.end());
                        }
                };
            public:
                void update(duration dT)
                {
                    if (now_ == start_)
                        receiver_().brain_initialize();

                    //Update time
                    dT_ = dT;
                    now_ += dT_;

                    //Swap the data slabs
                    swap();

                    //Update the data arrays calling the CRTP event points
                    {
                        receiver_().brain_beforeUpdate();

                        Callback callback(receiver_());
                        current().eachContainer(callback);

                        receiver_().brain_afterUpdate();
                    }
                }

                //Access to current and previous data slabs, and swapping them
                Data &current(){return (currentIsFirst_ ? datas_[0] : datas_[1]);}
                Data &previous(){return (currentIsFirst_ ? datas_[1] : datas_[0]);}
                void swap(){currentIsFirst_ = !currentIsFirst_;}

                //Time info, both as duration and double (in seconds) as convenience
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

                //We keep two data slabs, the current and the previous
                Data datas_[2];
                bool currentIsFirst_;

                Receiver &receiver_(){return static_cast<Receiver&>(*this);}
        };
}

#endif
