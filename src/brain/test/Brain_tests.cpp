#define GUBG_LOG
#include "gubg/l.hpp"
#include "brain/Brain.hpp"
#include <array>
#include <cstddef>
#include <chrono>
#include <cassert>

namespace 
{
    class SetPoint
    {
        public:
            SetPoint(double wanted):wanted_(wanted){}
            //No processing required
            template <typename Brain>
                void process(Brain &brain) { }

            double operator()() const {return wanted_;}

        private:
            const double wanted_;
    };
    class ProcessValue
    {
        public:
            ProcessValue(Process &pr):process_(pr){}

            template <typename Brain>
                void process(Brain &brain)
                {
                    process_.process(brain.dT_double());
                }
            double operator()() const {return process_.x();}
        private:
            Process &process_;
    };
    template <typename Sum>
        class ManipulatedValue
        {
            public:
                ManipulatedValue(Process &pr):process_(pr){}

                template <typename Brain>
                    void process(Brain &brain)
                    {
                        process_.setThrust(brain.brain_get(0, (Sum*)0)());
                    }
            private:
                Process &process_;
        };
    class Value
    {
        public:
            Value():value_(0.0){}
            virtual double operator()() const {return value_;};
        protected:
            double value_;
    };
    template <typename Wanted, typename Actual>
        class Error: public Value
        {
            public:
                Error():preValue_(0.0){}

                template <typename Brain>
                void process(Brain &brain)
                {
                    preValue_ = value_;
                    value_ = brain.brain_get(0, (Wanted*)0)() - brain.brain_get(0, (Actual*)(0))();
                }
                double preValue() const {return preValue_;}
            private:
                double preValue_;
        };
    template <typename Error>
        class Integral: public Value
        {
            public:
                template <typename Brain>
                    void process(Brain &brain)
                    {
                        value_ += brain.dT_double()*brain.brain_get(0, (Error*)0)();
                    }
        };
    template <typename Error>
        class Differential: public Value
        {
            public:
                template <typename Brain>
                    void process(Brain &brain)
                    {
                        value_ = (brain.brain_get(0, (Error*)0)() - brain.brain_get(0, (Error*)0).preValue())/brain.dT_double();
                    }
            private:
        };
    template <size_t Nr>
    class Sum
    {
        public:
            typedef std::array<double, Nr> Factors;

            Sum(const Factors &fs):factors_(fs){}

            template <typename Brain>
                void process(Brain &brain)
                {
                    sum_ = 0.0;
                    for (size_t i = 0; i < Nr; ++i)
                        sum_ += factors_[i]*(brain.brain_get(i, (Value*)(0))());
                }
            double operator()() const { return sum_; }
        private:
            Factors factors_;
            double sum_;
    };

    class Brain: public brain::Brain_crtp<Brain, std::chrono::high_resolution_clock::time_point>
    {
        public:
            typedef Error<SetPoint, ProcessValue> ErrorT;
            typedef Integral<ErrorT> IntegralT;
            typedef Differential<ErrorT> DifferentialT;
            typedef Sum<3> SumT;
            typedef ManipulatedValue<SumT> ManipulatedValueT;

            Brain():
                setPoint_(4.242),
                process_(1.0, 1.0, 9.81),
                processValue_(process_),
                sum_({{1.0, 1.0, 1.0}}),
                manipulatedValue_(process_){}

            double pv() const {return process_.x();}
            double error() const {return error_();}
            double integral() const {return integral_();}
            double differential() const {return differential_();}
            double mv() const {return sum_();}

            SetPoint &brain_get(size_t ix, SetPoint *) { return setPoint_; }
            ProcessValue &brain_get(size_t ix, ProcessValue *) { return processValue_; }
            ErrorT &brain_get(size_t ix, ErrorT *) { return error_; }
            IntegralT &brain_get(size_t ix, IntegralT *) { return integral_; }
            DifferentialT &brain_get(size_t ix, DifferentialT *) { return differential_; }
            Value &brain_get(size_t ix, Value *)
            {
                switch (ix)
                {
                    case 0: return error_; break;
                    case 1: return integral_; break;
                    case 2: return differential_; break;
                }
                assert(false);
            }
            SumT &brain_get(size_t ix, SumT *) { return sum_; }

            void brain_processAll()
            {
                Brain &brain = *this;
                setPoint_.process(brain);
                processValue_.process(brain);
                error_.process(brain);
                integral_.process(brain);
                differential_.process(brain);
                sum_.process(brain);
                manipulatedValue_.process(brain);
            }

        private:
            SetPoint setPoint_;
            Process process_;
            ProcessValue processValue_;
            ErrorT error_;
            IntegralT integral_;
            DifferentialT differential_;
            SumT sum_;
            ManipulatedValueT manipulatedValue_;
    };
}

int main()
{
    Brain brain;
    const auto maxAge = std::chrono::seconds(100);
    while (brain.age() < maxAge)
    {
        L("Brain age: " << brain.age_double() << "\tPV: " << brain.pv() << "\tP: " << brain.error() << "\tI: " << brain.integral() << "\tD: " << brain.differential() << "\tMV: " << brain.mv());
        brain.process(std::chrono::milliseconds(100));
    }
    L(STREAM(sizeof(double), sizeof(brain.dT()), sizeof(brain.now())));
    L(STREAM(sizeof(brain), sizeof(Process), sizeof(SetPoint), sizeof(ProcessValue), sizeof(Value), sizeof(Brain::ErrorT), sizeof(Brain::SumT), sizeof(Brain::ManipulatedValueT)));
    return 0;
}
