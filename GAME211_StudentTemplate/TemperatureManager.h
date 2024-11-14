#ifndef TEMPERATUREMANAGER_H
#define TEMPERATUREMANAGER_H

class TemperatureManager {
private:
    float temperature;
    TemperatureManager();

public:
    static TemperatureManager& Instance();

    // decrease temperature
    void DecreaseTemperature(float amount);

    // get temperature
    float GetTemperature() const;

    // set temperature
    void SetTemperature(float newTemperature);
};

#endif