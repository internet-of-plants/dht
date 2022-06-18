#ifndef IOP_LIB_SENSORS_DHT_ARDUINO_HPP
#define IOP_LIB_SENSORS_DHT_ARDUINO_HPP

#include <dht.hpp>
#include <DHT.h>

#include <iop-hal/panic.hpp>

namespace dht {
#define SENSOR(self) static_cast<DHT*>((self).sensor)

Dht::Dht(const iop_hal::PinRaw pin, const Version version) noexcept: sensor(new (std::nothrow) DHT(pin, static_cast<uint8_t>(version))) {
    iop_assert(this->sensor != nullptr, IOP_STR("Unable to allocate DHT"));
}

auto Dht::begin() noexcept -> void {
    iop_assert(this->sensor != nullptr, IOP_STR("Sensor is nullptr"));
    SENSOR(*this)->begin();
}
auto Dht::measureTemperature() noexcept -> float {
    iop_assert(this->sensor != nullptr, IOP_STR("Sensor is nullptr"));
    return SENSOR(*this)->readTemperature();
}
auto Dht::measureHumidity() noexcept -> float {
    iop_assert(this->sensor != nullptr, IOP_STR("Sensor is nullptr"));
    return SENSOR(*this)->readHumidity();
}
auto Dht::operator=(Dht && other) noexcept -> Dht & {
    delete SENSOR(*this);
    this->sensor = other.sensor;
    other.sensor = nullptr;
    return *this;
}
Dht::~Dht() noexcept {
    delete SENSOR(*this);
}
}

#endif