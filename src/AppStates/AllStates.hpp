#ifndef ALL_STATES_HPP
#define ALL_STATES_HPP

#include "AppStateMain.hpp"
#include "AppStateSettings.hpp"
#include "AppStateStatistics.hpp"

AppStateMain app_state_main;
AppStateSettings app_state_settings;
AppStateStatistics app_state_statistics;

typedef enum AppStateIDs {
	MAIN,
	SETTINGS,
	STATISTICS,
	__MAX__
} AppStateIDs;

#endif // !ALL_STATES_HPP
