#include "brain/Data.hpp"
#include "gubg/l.hpp"
#include <array>
#include <vector>
using namespace brain;
using namespace std;

template <typename T> struct Traits;
template <> struct Traits<double>{typedef std::array<double, 10> Container;};
template <> struct Traits<int>{typedef std::vector<int> Container;};

int main()
{
    Data<Traits, double, int> data;
    L(sizeof(data.get<double>(0)));
    L(sizeof(data.get<int>(0)));

    for (auto i = 0; i < 10; ++i)
        data.get<double>(i) = 1.0/(i+1);
    for (auto i = 0; i < 10; ++i)
        L(data.get<double>(i));

    data.container<int>().resize(3);
    for (auto &d: data.container<int>())
        d = 123;
    for (const auto d: data.container<int>())
        L(d);
    return 0;
}
