#include "gubg/testing/Testing.hpp"
#include "brain/Brain.hpp"
#include "brain/Data.hpp"
#include "test/SpringRocket.hpp"
#include "gubg/l.hpp"
#include <algorithm>
#include <cassert>
using namespace std;
using namespace test;

namespace 
{
    enum Pin {SetPoint, ProcessValue, P, I, D, ManipulatedValue, Nr_};

    template <typename T> struct Traits;
    template <> struct Traits<double>{typedef std::array<double, Pin::Nr_> Container;};
    typedef brain::Data<Traits, double> Data;

    class Brain: public brain::Brain_crtp<Brain, Data, chrono::high_resolution_clock::time_point>
    {
        public:
            Brain(SpringRocket &springRocket):
                springRocket_(springRocket){}

            void brain_initialize()
            {
                auto &c = current().container<double>();
                std::fill(c.begin(), c.end(), 0.0);
            }
            void brain_beforeUpdate()
            {
                    springRocket_.process(dT_double());
            }
            void brain_afterUpdate()
            {
                    springRocket_.setThrust(current().get<double>(ManipulatedValue));
                    L(current().get<double>(ProcessValue));
            }
            template <typename It>
                void brain_update(It it, It end)
                {
                    set_(SetPoint,         it, 4.242);
                    set_(ProcessValue,     it, springRocket_.x());
                    set_(P,                it, current().get<double>(SetPoint) - current().get<double>(ProcessValue));
                    set_(I,                it, previous().get<double>(I) + dT_double()*current().get<double>(P));
                    set_(D,                it, (current().get<double>(P) - previous().get<double>(P))/dT_double());
                    set_(ManipulatedValue, it, current().get<double>(P) + current().get<double>(I) + current().get<double>(D));
                    assert(it == end);
                }

        private:
            SpringRocket &springRocket_;
            template <typename It>
                void set_(Pin pin, It &it, double v)
                {
                    //L("Setting pin " << pin << " it to " << v);
                    *it++ = v;
                }
    };
}

int main()
{
    TEST_TAG(main);
    SpringRocket springRocket(1.0, 1.0, 9.81);
    Brain brain(springRocket);
    L(STREAM(sizeof(brain), sizeof(springRocket)));
    {
        TEST_TAG(swapData);
        auto c = &brain.current();
        auto p = &brain.previous();
        brain.swap();
        TEST_EQ(p, &brain.current());
        TEST_EQ(c, &brain.previous());
        brain.swap();
        TEST_EQ(c, &brain.current());
        TEST_EQ(p, &brain.previous());
    }
    {
        TEST_TAG(update);
        while (brain.age() < chrono::seconds(10))
            brain.update(chrono::milliseconds(100));
    }
    return 0;
}
