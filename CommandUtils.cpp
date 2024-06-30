#include "CommandUtils.h"
#include <iostream>
#include <iomanip>

namespace CommandUtils {
	void printTaskStatusToConsole(TaskStatus stat) {
		switch (stat) {
		case TaskStatus::ON_HOLD:    std::cout << "ON HOLD"; break;
		case TaskStatus::DONE:       std::cout << "DONE"; break;
		case TaskStatus::IN_PROCESS: std::cout << "IN PROCESS"; break;
		case TaskStatus::OVERDUE:    std::cout << "OVERDUE"; break;
		}
		std::cout << std::endl;
	}

	void printTaskToConsole(const Task* task) {
		if (!task) return;

		std::cout << "Task name: " << task->getName() << std::endl;
		std::cout << "Task ID: " << task->getId() << std::endl;

		if (task->hasDate()) {
			std::cout << "Due date: " << std::put_time(&task->getDueDate(), "%Y-%m-%d %H:%M:%S") << std::endl;
		}

		std::cout << "Status: ";
		printTaskStatusToConsole(task->getStatus());
		std::cout << "Task desc: " << task->getDescription() << std::endl;
	}
}