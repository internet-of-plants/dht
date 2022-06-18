#ifndef IOP_LIB_SENSORS_DHT_NOOP_HPP
#define IOP_LIB_SENSORS_DHT_NOOP_HPP

#include <dht.hpp>

namespace dht {
Dht::Dht(const iop_hal::PinRaw pin, const Version version) noexcept {
    (void) pin;
    (void) version;
}

auto Dht::begin() noexcept -> void {}
auto Dht::measureTemperature() noexcept -> float { return 0.; }
auto Dht::measureHumidity() noexcept -> float { return 0.; }

auto Dht::operator=(Dht && other) noexcept -> Dht & { (void) other; return *this; }
Dht::~Dht() noexcept {}
}

#endif