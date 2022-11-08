#ifndef IOP_LIB_SENSORS_DHT_CPP17_MOCK_HPP
#define IOP_LIB_SENSORS_DHT_CPP17_MOCK_HPP

#include <dht.hpp>

#include <random>

namespace dht {

struct Mock {
    float temperature;
    float humidity;

    Mock(float temperature, float humidity): temperature(temperature), humidity(humidity) {}
};

auto randomTemperature() -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(250, 350);
    return static_cast<float>(dist(rng)) / 10.;
}

auto randomTemperatureVariation(float current) -> float {
    if (current <= 26.) return current + 3.;
    if (current >= 34.) return current - 3.;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 10);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 10. - 0.5, 35.0), 25.0);
}

auto randomHumidity() -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(600, 800);
    return static_cast<float>(dist(rng)) / 10.;
}

auto randomHumidityVariation(float current) -> float {
    if (current <= 61.) return current + 3.;
    if (current >= 79.) return current - 3.;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 20);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 10. - 1., 80.0), 60.0);
}

Dht::Dht(const iop_hal::PinRaw pin, const Version version) noexcept: sensor(new Mock(randomTemperature(), randomHumidity())) {
    (void) pin;
    (void) version;
}

auto Dht::begin() noexcept -> void {}
auto Dht::measureTemperature() noexcept -> float {
    auto *ptr = &reinterpret_cast<Mock*>(this->sensor)->temperature;
    *ptr = randomTemperatureVariation(*ptr);
    return *ptr;
}
auto Dht::measureHumidity() noexcept -> float {
    auto *ptr = &reinterpret_cast<Mock*>(this->sensor)->humidity;
    *ptr = randomHumidityVariation(*ptr);
    return *ptr;
}
auto Dht::operator=(Dht && other) noexcept -> Dht & {
    delete reinterpret_cast<Mock*>(this->sensor);
    this->sensor = other.sensor;
    other.sensor = nullptr;
    return *this;
}
Dht::~Dht() noexcept {
    delete reinterpret_cast<Mock*>(this->sensor);
}
}

#endif