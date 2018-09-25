# Watering progress

This is a special screen shown when garduino is watering the garden. It is shown
each time automatic or manual watering is in progress.

## Information

    --------------------------------------
    | WateringMessage                    |
    | SoilHumidity/SoilHumidityThreshold |
    --------------------------------------

- **WaterinMessage** - Message which says that watering is in progress.
- **SoilHumidity** - Latest soil humidity value read from sensor.
- **SoilHumidityThreshold** - Required soil humidity to stop watering.

When user moves with the joystick down:

    --------------------------------------
    | WaterUsage                         |
    |                                    |
    -------------------------------------|

- **WaterUsage** - Number of liters used so far for this watering.
