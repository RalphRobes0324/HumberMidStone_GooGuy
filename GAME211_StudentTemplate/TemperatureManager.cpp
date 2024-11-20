#include "TemperatureManager.h"
#include <algorithm>

TemperatureManager& TemperatureManager::Instance() {
    static TemperatureManager instance;
    return instance;
}

// initializes temperature
TemperatureManager::TemperatureManager() :
    temperature(100.0f),
    isOnHotPlatform(false),
    tempMeter({ 14, 260, 20, -200 }),
    tempColour({ 255, 0, 0, 255 }),
    tempMeterBackground({ 10, 270, 28, -220 }),
    tempMeterBackgroundColour({ 50, 50, 50, 255 })
{}

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

void TemperatureManager::RenderTemperature(SDL_Renderer* renderer) {
    // draw background
    SDL_SetRenderDrawColor(renderer, tempMeterBackgroundColour.r, tempMeterBackgroundColour.g, tempMeterBackgroundColour.b, tempMeterBackgroundColour.a);
    SDL_RenderFillRect(renderer, &tempMeterBackground);

    // update meter based on temperature
    tempMeter.h = static_cast<int>(-temperature * 2);
    SDL_SetRenderDrawColor(renderer, tempColour.r, tempColour.g, tempColour.b, tempColour.a);
    SDL_RenderFillRect(renderer, &tempMeter);
}

// set hot platform bool
void TemperatureManager::SetHotPlatform(bool _isOnHotPlatform) {
    isOnHotPlatform = _isOnHotPlatform;
}

// get hot platform bool
bool TemperatureManager::GetHotPlatform() {
    return isOnHotPlatform;
}