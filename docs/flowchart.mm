graph TD
Startup((Startup)) --> |R0 = MenuMain <br/> S = MenuMain| ShowMenu[ShowMenu]
ShowMenu --> ShowMenuStateSwitch{State}
ShowMenuStateSwitch -.-> |MenuMain| MenuMainInput>Menu Item Selected]


MenuMainInput--> MenuMainInputSwitch{Input}
MenuMainInputSwitch --> |Settings <br/> S = MenuSettings| ShowMenu[ShowMenu]
MenuMainInputSwitch --> |Statistics <br/> S = MenuStatistics| ShowMenu[ShowMenu]
MenuMainInputSwitch --> |Run Watering <br/> S = HumidityCheck| HumidityCheck[CheckHumidity]
MenuMainInputSwitch--> |Go to sleep <br/> R0 = 0 <br/> S = EnterSleepMode| SleepMode[SleepMode]

ShowMenuStateSwitch -.-> |MenuSettings| MenuSettingsInput>Menu Item Selected]
MenuSettingsInput --> MenuSettingsInputSwitch{Input}
MenuSettingsInputSwitch --> |Hum. threshold <br/> S = MenuSettings| ShowMenu
MenuSettingsInputSwitch --> |liters/watering <br/> S = MenuSettings| ShowMenu
MenuSettingsInputSwitch --> |Check interval <br/> S = MenuSettings| ShowMenu
MenuSettingsInputSwitch --> |Back <br/> S = MenuMain| ShowMenu

ShowMenuStateSwitch -.-> |MenuStatistics| MenuStatisticsInput>Menu Item Selected]
MenuStatisticsInput --> MenuStatisticsInputSwitch{Input}
MenuStatisticsInputSwitch --> |Back <br/> S = MenuMain| ShowMenu

HumidityCheck -.-> HumidityCheckResults>Humidity Level]
HumidityCheckResults --> HumidityCheckSwitch{Humidity}
HumidityCheckSwitch --> |Sufficient| HumidityCheckSufficientTypeSwitch{State}
HumidityCheckSwitch --> |Insufficient| HumidityCheckInsufficientTypeSwitch{State}

HumidityCheckSufficientTypeSwitch--> |HumidityCheck <br/> S = MenuMain| ShowMenu
HumidityCheckSufficientTypeSwitch--> |HumidityCheckAndSleep <br/> S = EnterSleepMode| SleepMode[SleepMode]

HumidityCheckInsufficientTypeSwitch --> |HumidityCheck <br/> S = WateringManual| RunWatering
HumidityCheckInsufficientTypeSwitch --> |HumidityCheckAndSleep <br/> S = WateringAutomatic| RunWatering

SleepMode --> |S = Sleeping| SleepMode
SleepMode --> SleepInterrupted>Sleep Interrupted]
SleepInterrupted --> SleepInterruptedSwitch{Interrupt}
SleepInterruptedSwitch --> |User <br/> S = WakedUpByUser| WakeUp[WakeUp]
SleepInterruptedSwitch --> |Timer <br/> S = WakedUpByTimer| WakeUp[WakeUp]

WakeUp --> WakeUpSwitch{State}
WakeUpSwitch --> |WakedUpByUser| Startup
WakeUpSwitch --> |WakedUpByTimer<br/> S = HumidityCheckAndSleep| HumidityCheck 

RunWatering --> RunWateringSwitch{State}
RunWateringSwitch --> |WateringManual<br/> S = MenuMain | ShowMenu
RunWateringSwitch --> |WateringAutomatic <br/> S = EnterSleepMode | SleepMode
