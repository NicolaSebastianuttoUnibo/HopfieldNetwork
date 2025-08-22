#ifndef CAST_HPP
#define CAST_HPP
#include <complex>
#include <iostream>

// --- Definizione del Caster ---

// 1. Template di base: per tutti i casi generici, delega a static_cast
template<typename To, typename From, typename = void>
struct custom_caster {
    static To cast(const From& source) {
        // Comportamento di default: usa il classico static_cast
        return static_cast<To>(source);
    }
};

// 2. Specializzazione parziale: per il caso specifico da std::complex<F> a std::complex<T>
template<typename T, typename F>
struct custom_caster<std::complex<T>, std::complex<F>> {
    static std::complex<T> cast(const std::complex<F>& source) {
        // Comportamento specializzato: usa la costruzione diretta per i tipi complex
        return std::complex<T>(
            static_cast<T>(source.real()),
            static_cast<T>(source.imag())
        );
    }
};

// Funzione helper per un uso più pulito (opzionale ma raccomandata)
template<typename To, typename From>
To custom_cast(const From& source) {
    return custom_caster<To, From>::cast(source);
}


#endif