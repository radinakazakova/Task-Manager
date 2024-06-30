#pragma once
#include "Task.h"

namespace CommandUtils {
	void printTaskStatusToConsole(TaskStatus stat);
	void printTaskToConsole(const Task* task);
}