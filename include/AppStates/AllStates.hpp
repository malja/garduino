#ifndef ALL_STATES_HPP
#define ALL_STATES_HPP

#include "WelcomeAppState.hpp"
#include "OverrideAppState.hpp"
#include "WaterLevelsAppState.hpp"

WelcomeAppState app_state_welcome;
OverrrideAppState app_state_override;
WaterLevelsAppState app_state_water_levels;

typedef enum AppStateIDs {
	WELCOME,
	OVERRIDE,
	WATER_LEVELS,
	__MAX__
} AppStateIDs;

#endif // !ALL_STATES_HPP
