#include "run_application.hpp"
#include <complex>
#include <cstdint>
int main(int argc, char **argv) {
#ifdef USE_COMPLEX_TYPES
  return run_application<std::complex<int8_t>, std::complex<double>>(argc,
                                                                     argv);
#else
  return run_application<int8_t, double>(argc, argv);
#endif
}