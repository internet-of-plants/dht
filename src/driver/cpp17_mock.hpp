#ifndef IOP_LIB_SENSORS_DHT_CPP17_MOCK_HPP
#define IOP_LIB_SENSORS_DHT_CPP17_MOCK_HPP

#include <dht.hpp>

#include <random>

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
    return static_cast<float>(dist(rng)) / 100.;
}

auto randomHumidityVariation(float current) -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-2, 2);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 100., 0.08), 0.06);
}

namespace dht { 
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
auto Dht::measureHeatIndex() noexcept -> float {    
    float hi;

    auto *sensor = reinterpret_cast<Mock*>(this->sensor);
    // In F
    float temperature = sensor->temperature * 1.8 + 32;
    float humidity = sensor->humidity;

    hi = 0.5 * (temperature + 61.0 + ((temperature - 68.0) * 1.2) +
                (humidity * 0.094));

  if (hi > 79) {
    hi = -42.379 + 2.04901523 * temperature + 10.14333127 * humidity +
         -0.22475541 * temperature * humidity +
         -0.00683783 * std::pow(temperature, 2) +
         -0.05481717 * std::pow(humidity, 2) +
         0.00122874 * std::pow(temperature, 2) * humidity +
         0.00085282 * temperature * std::pow(humidity, 2) +
         -0.00000199 * std::pow(temperature, 2) * std::pow(humidity, 2);

    if ((humidity < 13) && (temperature >= 80.0) &&
        (temperature <= 112.0))
      hi -= ((13.0 - humidity) * 0.25) *
            std::sqrt((17.0 - abs(temperature - 95.0)) * 0.05882);

    else if ((humidity > 85.0) && (temperature >= 80.0) &&
             (temperature <= 87.0))
      hi += ((humidity - 85.0) * 0.1) * ((87.0 - temperature) * 0.2);
  }

  return (hi - 32) * 0.55555;
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