#ifndef rb_brain_Data_hpp
#define rb_brain_Data_hpp

#include "gubg/tmp/If.hpp"
#include "gubg/tmp/Equal.hpp"
#include <cstddef>

namespace brain
{
    template <typename Container>
        struct Array
        {
            typedef typename Container::value_type value_type;

            Container &container_(value_type *){return array_;}

            Container array_;
        };

    template <template <typename TT> class Traits, typename... Ts>
        class Data{};
    template <template <typename TT> class Traits, typename T, typename... Ts>
        class Data<Traits, T, Ts...>: Array<typename Traits<T>::Container>, Data<Traits, Ts...>
        {
            public:
                typedef typename Traits<T>::Container Container;
                typedef Array<Container> Array_;
                typedef Data<Traits, Ts...> Rest;

                template <typename TT>
                    typename Traits<TT>::Container &container() { return container_((TT*)(0)); }
                template <typename TT>
                    TT &get(size_t ix) {return container<TT>()[ix];}

            protected:
                template <typename TT>
                    typename Traits<TT>::Container &container_(TT *tt)
                    {
                        return gubg::tmp::If<gubg::tmp::Equal<TT, T>::Value, Array_, Rest>::Type::container_(tt);
                    }
        };
}

#endif
