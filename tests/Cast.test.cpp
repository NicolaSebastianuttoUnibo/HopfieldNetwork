#include "doctest.h"
#include "../HopfieldSimulator/math/Cast.hpp"




TEST_SUITE("Cast") {
    
    TEST_CASE("Custom cast for arithmetic types") {
        CHECK(custom_cast<double>(5) == 5.0);
        CHECK(custom_cast<int>(5.7) == 5);
        CHECK(custom_cast<float>(10) == 10.0f);
        CHECK(custom_cast<int8_t>(127) == 127);
        CHECK(custom_cast<int8_t>(128) == -128); 
    }

    TEST_CASE("Custom cast for complex types") {
        std::complex<int> c_int(1, 2);
        std::complex<double> c_double_from_int = custom_cast<std::complex<double>>(c_int);
        CHECK(c_double_from_int.real() == 1.0);
        CHECK(c_double_from_int.imag() == 2.0);

        std::complex<double> c_double(3.5, 4.8);
        std::complex<int> c_int_from_double = custom_cast<std::complex<int>>(c_double);
        CHECK(c_int_from_double.real() == 3);
        CHECK(c_int_from_double.imag() == 4);

        std::complex<int8_t> c_int8(10, -5);
        std::complex<float> c_float_from_int8 = custom_cast<std::complex<float>>(c_int8);
        CHECK(c_float_from_int8.real() == 10.0f);
        CHECK(c_float_from_int8.imag() == -5.0f);
    }
}
