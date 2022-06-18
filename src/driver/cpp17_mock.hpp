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
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-3, 3);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 10., 35.), 25.);
}

auto randomHumidity() -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(60, 80);
    return static_cast<float>(dist(rng));
}

auto randomHumidityVariation(float current) -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-2, 2);
    return std::max(std::min(current + static_cast<float>(dist(rng)), 80.), 60.);
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