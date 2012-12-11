#include "brain/Data.hpp"
#include "gubg/l.hpp"
#include <array>
using namespace brain;
using namespace std;

namespace 
{
    struct Input_tag{};
    struct Output_tag{};
}

int main()
{
    Data<array<double, 10>, array<int, 3>> data;
    L(sizeof(data.get(0, (double*)(0))));
    L(sizeof(data.get(0, (int*)(0))));
    return 0;
}
