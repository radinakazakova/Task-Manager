#include <iostream>
#include <sstream>
#include <fstream>
#include "MyString.h"
#include "System.h"
#include "Commands.h"
#include <ctime>
#include <iomanip>

namespace
{
	constexpr int MAX_SIZE = 100;
	constexpr char commandRegister[] = "register";
	constexpr char commandLogin[] = "login";
	constexpr char commandAddTask[] = "add-task";
	constexpr char commandUpdateTaskName[] = "update-task-name";
	constexpr char commandStartTask[] = "start-task";
	constexpr char commandUpdateTaskDescription[] = "update-task-description";
	constexpr char commandRemoveTaskFromDashboard[] = "remove-task-from-dashboard";
	constexpr char commandAddTaskToDashboard[] = "add-task-to-dashboard";
	constexpr char commandDeleteTask[] = "delete-task";
	constexpr char commandGetTask[] = "get-task";
	constexpr char commandListTasks[] = "list-tasks";
	constexpr char commandListCompletedTasks[] = "list-completed-tasks";
	constexpr char commandListDashboard[] = "list-dashboard";
	constexpr char commandFinishTask[] = "finish-task";
	constexpr char commandLogout[] = "logout";
	constexpr char commandAddCollaboration[] = "add-collaboration";
	constexpr char commandDeleteCollaboration[] = "delete-collaboration";
	constexpr char commandListCollaborations[] = "list-collaborations";
	constexpr char commandAddUser[] = "add-user";
	constexpr char commandAssignTask[] = "assign-task";
	constexpr char commandExit[] = "exit";
	constexpr char dateFormat[] = "%Y-%m-%d %H:%M:%S";


	bool isNumber(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	bool strToTm(const MyString& str, const MyString& format, std::tm& tm)
	{
		std::istringstream ss(str.c_str());
		ss >> std::get_time(&tm, format.c_str());
		return !(ss.fail());
	}

	int charToInt(const char* str)
	{
		if (str == nullptr || *str == '\0')
		{
			return -1;
		}

		int result = 0;
		const char* ptr = str;

		while (*ptr != '\0')
		{
			if (*ptr < '0' || *ptr > '9')
			{
				throw std::invalid_argument("Input string contains non-digit characters");
			}

			int digit = *ptr - '0';

			result = result * 10 + digit;
			++ptr;
		}

		return result;
	}
}

void executeCommand(System& system, const MyString& command, std::stringstream& ss)
{
	if (command == commandRegister)
	{
		MyString username;
		MyString password;

		ss >> username;
		ss >> password;

		RegisterCommand commandToExecute(system, username, password);
		commandToExecute.execute();
	}
	else if (command == commandLogin)
	{
		MyString username;
		MyString password;

		ss >> username;
		ss >> password;

		LoginCommand commandToExecute(system, username, password);
		commandToExecute.execute();
	}
	else if (command == commandAddTask)
	{
		MyString name;
		char nextArg[MAX_SIZE];

		ss >> name;
		ss.ignore();
		ss.getline(nextArg, MAX_SIZE);

		if (isNumber(nextArg[0]))
		{
			MyString format = dateFormat;
			std::tm tm = {};

			std::stringstream sstream(nextArg);
			MyString date;
			sstream >> date;

			if (strToTm(date, format, tm))
			{
				char description[MAX_SIZE];
				sstream.ignore();
				sstream.getline(description, MAX_SIZE);
				AddTaskCommand commandToExecute(system, name, tm, description);
				commandToExecute.execute();
			}
			else
			{
				std::cout << "Invalid arguments. Try again.";
			}
		}
		else
		{
			AddTaskCommand commandToExecute(system, name, nextArg);
			commandToExecute.execute();
		}
	}
	else if (command == commandUpdateTaskName)
	{
		int id;
		MyString name;
		ss >> id >> name;

		UpdateTaskNameCommand commandToExecute(system, id, name);
		commandToExecute.execute();
	}
	else if (command == commandStartTask)
	{
		int id;
		ss >> id;
		StartTaskCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == commandUpdateTaskDescription)
	{
		int id;
		char desc[MAX_SIZE];
		ss >> id;
		ss.ignore();
		ss.getline(desc, MAX_SIZE);

		UpdateTaskDescriptionCommand commandToExecute(system, id, desc);
		commandToExecute.execute();
	}
	else if (command == commandRemoveTaskFromDashboard)
	{
		int id;
		ss >> id;

		RemoveTaskFromDashboardCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == commandAddTaskToDashboard)
	{
		int id;
		ss >> id;

		AddTaskToDashboardCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == commandDeleteTask)
	{
		int id;
		ss >> id;

		DeleteTaskCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == commandGetTask)
	{
		MyString arg;
		ss >> arg;

		if (isNumber(arg[0]))
		{
			try
			{
				int id = charToInt(arg.c_str());
				GetTaskCommand commandToExecute(system, id);
				commandToExecute.execute();
			}
			catch (std::exception& e)
			{
				std::cout << "Invalid id input." << std::endl;
			}
		}
		else
		{
			GetTaskCommand commandToExecute(system, arg);
			commandToExecute.execute();
		}
	}
	else if (command == commandListTasks)
	{
		MyString arg;
		ss >> arg;

		if (arg.getLength() == 0)
		{
			ListTasksCommand command(system);
			command.execute();
		}
		else if (isNumber(arg[0]))
		{
			MyString format = dateFormat;
			std::tm tm = {};
			if (strToTm(arg, format, tm))
			{
				ListTasksCommand command(system, tm);
				command.execute();
			}
			else
			{
				std::cout << "Invalid arguments. Try again.";
			}
		}
		else
		{
			ListTasksCommand command(system, arg);
			command.execute();
		}
	}
	else if (command == commandListCompletedTasks)
	{
		ListCompletedTasksCommand command(system);
		command.execute();
	}
	else if (command == commandListDashboard)
	{
		ListDashboardCommand command(system);
		command.execute();
	}
	else if (command == commandFinishTask)
	{
		int id;
		ss >> id;

		FinishTaskCommand command(system, id);
		command.execute();
	}
	else if (command == commandLogout)
	{
		LogoutCommand command(system);
		command.execute();
	}
	else if (command == commandAddCollaboration)
	{
		MyString name;
		ss >> name;
		AddCollaborationCommand command(system, name);
		command.execute();
	}
	else if (command == commandDeleteCollaboration)
	{
		MyString name;
		ss >> name;

		DeleteCollaborationCommand command(system, name);
		command.execute();
	}
	else if (command == commandListCollaborations)
	{
		listCollaborationsCommand command(system);
		command.execute();
	}
	else if (command == commandAddUser)
	{
		MyString collabName;
		MyString userName;

		ss >> collabName >> userName;
		AddUserCommand command(system, collabName, userName);
		command.execute();
	}
	else if (command == commandAssignTask)
	{
		MyString collabName;
		MyString username;
		MyString name;
		char nextArg[MAX_SIZE];

		ss >> collabName >> username >> name;
		ss.ignore();
		ss.getline(nextArg, MAX_SIZE);

		if (isNumber(nextArg[0]))
		{
			std::stringstream sstream(nextArg);
			MyString due_date;
			char description[MAX_SIZE];
			MyString format = dateFormat;
			std::tm tm = {};

			sstream >> due_date;
			sstream.getline(description, MAX_SIZE);

			if (strToTm(due_date, format, tm))
			{
				AssignTaskCommand command(system, collabName, username, name, tm, description);
				command.execute();
			}
			else
			{
				std::cout << "Invalid arguments. Try again. Input a valid due date.";
			}
		}
		else
		{
			AssignTaskCommand command(system, collabName, username, name, nextArg);
			command.execute();
		}

	}
	else
	{
		std::cout << "Invalid command. Try again." << std::endl;
	}
}

int main()
{
	try
	{
		System system;
		system.loadDataFromFile();

		while (true)
		{
			char line[MAX_SIZE];

			std::cout << "Enter command: ";
			std::cin.getline(line, MAX_SIZE);

			std::stringstream ss(line);
			char command[MAX_SIZE];

			ss.getline(command, MAX_SIZE, ' ');

			if (strcmp(command, commandExit) == 0)
			{
				break;
			}

			executeCommand(system, command, ss);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;

}
