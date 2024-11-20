#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

#include <SDL.h>
#include <SDL_image.h>

class TemperatureManager {
private:
    float temperature;
    bool isOnHotPlatform;
    TemperatureManager();

    SDL_Rect tempMeter;
    SDL_Color tempColour;
    SDL_Rect tempMeterBackground;
    SDL_Color tempMeterBackgroundColour;

public:
    static TemperatureManager& Instance();

    // decrease temperature
    void DecreaseTemperature(float amount);

    // get temperature
    float GetTemperature() const;

    // set temperature
    void SetTemperature(float newTemperature);

    // render temperature meter
    void RenderTemperature(SDL_Renderer* renderer);

    void SetHotPlatform(bool _isOnHotPlatform);

    bool GetHotPlatform();
};

#endif