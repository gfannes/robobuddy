#ifndef rb_brain_Data_hpp
#define rb_brain_Data_hpp

#include "gubg/tmp/If.hpp"
#include "gubg/tmp/Equal.hpp"
#include <cstddef>

namespace brain
{
    template <typename T>
        struct List
        {
            typedef typename T::value_type value_type;
            value_type &get(size_t ix, value_type *){return list_[ix];}
            T list_;
        };
    template <typename... Ts>
        struct Data{};
    template <typename T, typename... Ts>
        struct Data<T, Ts...>: List<T>, Data<Ts...>
        {
            typedef List<T> ListT;
            typedef Data<Ts...> Rest;
            template <typename TT>
                TT &get(size_t ix, TT *tt)
                {
                    return gubg::tmp::If<gubg::tmp::Equal<TT, typename T::value_type>::Value, ListT, Rest>::Type::get(ix, tt);
                }
        };
}

#endif
