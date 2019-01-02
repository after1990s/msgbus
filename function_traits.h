#ifndef FUNCTION_TRAITS_H
#define FUNCTION_TRAITS_H
#include <tuple>
#include <functional>

template<class T>
struct function_traits;

template<class R, class... Args>
struct function_traits<R(Args...)>
{
public:
    enum{arity = sizeof...(Args)};
    using function_type = R(Args...);
    using return_type = R;
    using stl_function_type = std::function<function_type>;
    using pointer = R(*)(Args...);

    template<int I>
    struct args
    {
        static_assert(I < arity, "index args out of range");
        using type = typename std::tuple_element<I, std::tuple<Args...>>::type;
    };
};

//function pointer
template<class R, class... Args>
struct function_traits<R(*)(Args...)> :
    function_traits<R(Args...)>{};

//std function
template<class R, class... Args>
struct function_traits<std::function<R(Args...)>> :
    function_traits<R(Args...)> {
    static_assert(true, "std::function partialize");
};

//member function
#define FUNCTION_TRAITS(...) \
    template<class R, class C, class... Args> \
    struct function_traits<R(C::*)(Args...) __VA_ARGS__> : \
        function_traits<R(Args...)> {};

FUNCTION_TRAITS()
FUNCTION_TRAITS(const)
FUNCTION_TRAITS(volatile)
FUNCTION_TRAITS(const volatile)

template<class Callable>
struct function_traits :
    function_traits<decltype(&Callable::operator())> {

    static_assert(true, "callable ");
};

template<class F>
typename function_traits<F>::stl_function_type to_function(const F& lambda)
{
    return static_cast<typename function_traits<F>::stl_function_type>(
                lambda);
}

template<class F>
typename function_traits<F>::stl_function_type to_function(F&& lambda)
{
    return static_cast<typename function_traits<F>::stl_function_type>(
                std::forward<F>(lambda));
}

template<class F>
typename function_traits<F>::stl_function_type to_function_pointer(const F& lambda)
{
    return static_cast<typename function_traits<F>::pointer>(lambda);
}


template<class T>
struct  test_function;
template<class R, class... Args>
struct test_function<R(Args...)>{};

template<class R, class... Args>
struct test_function<std::function<R(Args...)>> : test_function<R(Args...)>{
    static_assert(true, "std::function");
};

template<class Callable>
struct test_function :
    test_function<decltype(&Callable::operator())> {
    static_assert(true, "Callable");
};

#endif // FUNCTION_TRAITS_H
