#ifndef ITRAINING_PATTERN_HPP
#define ITRAINING_PATTERN_HPP

#include <memory>
#include <string>
#include <vector>

#include "math/MathDimension.hpp"
#include <cstdint>
#include <complex>


namespace TP {

struct StbiImageDeleter {
    void operator()(unsigned char* data) const;
};

// ITrainingPattern
template <typename T>
class ITrainingPattern {
public:
    virtual ~ITrainingPattern() = default;
    virtual const std::vector<T>& getPattern() const noexcept = 0; // Contratto: fornisce un vettore
    virtual void regrid(const std::size_t numColumns, const std::size_t numRows) = 0; // Contratto: può essere regriddato
};
// VectorBasedTrainingPattern

template <typename T>
class VectorBasedTrainingPattern : public ITrainingPattern<T> {
protected:
    std::vector<T> pattern_;
public:

    const std::vector<T>& getPattern() const noexcept override {
        return pattern_;
    }
};

//ImageTrainingPattern
template <typename T>
class ImageTrainingPattern : public VectorBasedTrainingPattern<T> {
 static constexpr std::array<T,MD::getMathematicalNumberVertex<T>()> POINTS = MD::getMathematicalVertex<T>();

    std::unique_ptr<unsigned char[], StbiImageDeleter> imgData_;
    int imgWidth_ = 0;
    int imgHeight_ = 0;
    int imgChannels_ = 0;
    std::vector<long long> imgIntegral_; 
    void calculateIntegralImage();
public:
ImageTrainingPattern(const std::string& path, const std::size_t numColumns, const std::size_t numRows);
  ImageTrainingPattern() = delete;
    void regrid(const std::size_t numColumns, const std::size_t numRows) override;
};

//RandomTrainingPattern
template <typename T>
class RandomTrainingPattern : public VectorBasedTrainingPattern<T> {
 static constexpr std::array<T,MD::getMathematicalNumberVertex<T>()> POINTS = MD::getMathematicalVertex<T>();


    float randomDensity_;
      void generateRandomPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows);

public:
  RandomTrainingPattern(const float randomDensity, const std::size_t numColumns, const std::size_t numRows);
  RandomTrainingPattern() = delete;
    void regrid(const std::size_t numColumns, const std::size_t numRows) override;
};


template class ITrainingPattern<int8_t>;
template class ITrainingPattern<int>;
template class ITrainingPattern<std::complex<int>>;
template class ITrainingPattern<std::complex<int8_t>>;

template class VectorBasedTrainingPattern<int8_t>;
template class VectorBasedTrainingPattern<int>;
template class VectorBasedTrainingPattern<std::complex<int>>;
template class VectorBasedTrainingPattern<std::complex<int8_t>>;

template class ImageTrainingPattern<int8_t>;
template class ImageTrainingPattern<int>;
template class ImageTrainingPattern<std::complex<int>>;
template class ImageTrainingPattern<std::complex<int8_t>>;

template class RandomTrainingPattern<int8_t>;
template class RandomTrainingPattern<int>;
template class RandomTrainingPattern<std::complex<int>>;
template class RandomTrainingPattern<std::complex<int8_t>>;

} // namespace TP

#endif // ITRAINING_PATTERN_HPP