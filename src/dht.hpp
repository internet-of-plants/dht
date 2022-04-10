#ifndef IOP_LIB_SENSORS_DHT_HPP
#define IOP_LIB_SENSORS_DHT_HPP

#include <iop-hal/io.hpp>
#include <stdint.h>

namespace dht {
enum Version {
  DHT11 = 11,
  DHT12 = 12,
  DHT22 = 22,
  AM2301 = 21
};

class Dht {
    void *sensor;
public:
    Dht(iop_hal::PinRaw pin, Version version) noexcept;
    auto begin() noexcept -> void;
    auto measureTemperature() noexcept -> float;
    auto measureHumidity() noexcept -> float;
    auto measureHeatIndex() noexcept -> float;

    auto operator=(Dht && other) noexcept -> Dht &;
    ~Dht() noexcept;

    Dht(Dht && other) noexcept: sensor(other.sensor) {
        other.sensor = nullptr;
    }
    Dht(const Dht & other) noexcept = delete;
    auto operator=(const Dht & other) noexcept = delete;
};
}

#endif