#include "TemperatureManager.h"
#include <algorithm>

TemperatureManager& TemperatureManager::Instance() {
    static TemperatureManager instance;
    return instance;
}

// initializes temperature
TemperatureManager::TemperatureManager() : temperature(100.0f) {}

// decrease temperature
void TemperatureManager::DecreaseTemperature(float amount) {
    temperature = std::max(0.0f, temperature - amount);
}

// get temperature
float TemperatureManager::GetTemperature() const {
    return temperature;
}

// set temperature
void TemperatureManager::SetTemperature(float newTemperature) {
    temperature = newTemperature;
}