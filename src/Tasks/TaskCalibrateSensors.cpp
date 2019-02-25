#include "TaskCalibrateSensors.h"
#include "../Sensors/SoilHumidity.h"
#include "../App.h"

void TaskCalibrateSensors() {
    App::StateID state = APP.getState();
    uint16_t humidity = GetSoildHumidity();

    switch(state) {
        case App::StateID::CalibrateHumidityLow: {

            // Update display only when readings change 
            if (APP.getRegister(App::RegisterID::First) != humidity) {
                APP.display.clear();
                APP.display.println("Measure low humidity");
                APP.display.setCol(2);
                APP.display.print("> ");
                APP.display.println(humidity);

                APP.setRegister(APP::RegisterID::First, humidity);
            }

            // Move to next step
            if (APP.joystick.isClicked()) {
                APP.setState(App::StateID::CalibrateHumidityHigh, false);
            }

            break;
        }

        case App::StateID::CalibrateHumidityHigh: {

            // Update display only when readings change 
            if (APP.getRegister(App::RegisterID::Second) != humidity) {
                APP.display.clear();
                APP.display.println("Measure high humidity");
                APP.display.setCol(2);
                APP.display.print("> ");
                APP.display.println(humidity);

                APP.setRegister(App::RegisterID::Second, humidity);
            }

            // Move to next step
            if (APP.joystick.isClicked()) {
                APP.setState(App::StateID::CalibrateHumidityFinish, false);
            }

            break;
        }

        case App::StateID::CalibrateHumidityFinish: {

            // Write text out only once
            if (APP.getRegister(App::RegisterID::Third) == 0) {
                APP.display.clear();
                APP.display.println("Calibration finished");
                
                APP.display.print(APP.getRegister(App::RegisterID::First));
                APP.display.print("/");
                APP.display.println(APP.getRegister(App::RegisterID::Second));
                
                // Save new threshold
                APP.storage.update(HUMIDITY_THRESHOLD_INDEX, 
                    round(
                        (APP.getRegister(App::RegisterID::Second) + APP.getRegister(App::RegisterID::First)) * 0.5
                    )
                );

                APP.display.print("Calibrated to: ");
                APP.display.println(APP.storage.read(HUMIDITY_THRESHOLD_INDEX));

                APP.setRegister(App::RegisterID::Third, 1);
            }

            // Move back to menu
            if (APP.joystick.isClicked()) {
                APP.setNextTask(TaskShowMenu);
                APP.setState(App::StateID::MenuMain);
            }
        }

        default:
            APP.switchToErrorMode(App::ErrorCodeID::UnknownState);
            break;
    }

}
