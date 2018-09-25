# Water usage

This is monitoring screen which shows water usage, number of watering in last
day and other watering information.

It is loaded when user moves to the left from [welcome screen](./welcome.md)
(moving to the right, comes back).

## Information

This screen is divided into multiple parts. User navigates between them with the
joystick moving up and down.

    ------------------------------
    | WateringsInDay             |
    | TotalWaterUsage            |
    ------------------------------

- **WateringsInDay** - Number of watering per current day.
- **TotalWaterUsage** - Total number of liters used per whole run of garduino.

Next screen:

    --------------------------------------
    | SoilHumidity/SoilHumidityThreshold |
    | AverageWatering                    |
    --------------------------------------

- **SoilHumidity** - Latest soil humidity level reading value.
- **SoilHumidityThreshold** - Required humidity level for watering.
- **AverageWatering** - Average number of liters per one watering.