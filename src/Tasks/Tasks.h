#ifndef GARDUINO_TASKS_H
#define GARDUINO_TASKS_H

#include "TaskWakeUp.h"
#include "TaskShowMenu.h"
#include "TaskError.h"
#include "TaskSleepMode.h"
#include "TaskCheckHumidity.h"
#include "TaskRunWatering.h"

typedef void (*Task)();

#endif