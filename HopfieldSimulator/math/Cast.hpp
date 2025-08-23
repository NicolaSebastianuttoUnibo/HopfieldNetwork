#ifndef CAST_HPP
#define CAST_HPP
#include <complex>


template<typename To, typename From, typename = void>
struct custom_caster {
    static To cast(const From& source) {
        return static_cast<To>(source);
    }
};

template<typename T, typename F>
struct custom_caster<std::complex<T>, std::complex<F>> {
    static std::complex<T> cast(const std::complex<F>& source) {
        return std::complex<T>(
            static_cast<T>(source.real()),
            static_cast<T>(source.imag())
        );
    }
};

template<typename To, typename From>
To custom_cast(const From& source) {
    return custom_caster<To, From>::cast(source);
}


#endif