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

	static constexpr int MAX_LINE_SIZE = 1024;

	static bool isNumber(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	static bool strToTm(const MyString& str, const MyString& format, std::tm& tm)
	{
		std::istringstream ss(str.c_str());
		ss >> std::get_time(&tm, format.c_str());
		return !(ss.fail());
	}

	static int charToInt(const char* str)
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

void executeCommand(const System& system, const MyString& command, std::stringstream& ss)
{
	if (command == "register")
	{
		MyString username;
		MyString password;

		ss >> username;
		ss >> password;

		RegisterCommand commandToExecute(system, username, password);
		commandToExecute.execute();
	}
	else if (command == "login")
	{
		MyString username;
		MyString password;

		ss >> username;
		ss >> password;

		LoginCommand commandToExecute(system, username, password);
		commandToExecute.execute();
	}
	else if (command == "add-task")
	{
		MyString name;
		MyString nextArg;

		ss >> name >> nextArg;

		if (isNumber(nextArg[0]))
		{
			MyString format = "%Y-%m-%d %H:%M:%S";
			std::tm tm = {};
			if (strToTm(nextArg, format, tm))
			{
				MyString description;
				ss >> description;
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
			if (isNumber(nextArg[0]))
			{
				std::cout << "Descriptions shouldn't begin with a number." << std::endl;
			}
			else
			{
				AddTaskCommand commandToExecute(system, name, nextArg);
				commandToExecute.execute();
			}
		}
	}
	else if (command == "update-task-name")
	{
		int id;
		MyString name;
		ss >> id >> name;

		UpdateTaskNameCommand commandToExecute(system, id, name);
		commandToExecute.execute();
	}
	else if (command == "start-task")
	{
		int id;
		ss >> id;
		StartTaskCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == "update-task-description")
	{
		int id;
		MyString desc;
		ss >> id >> desc;

		UpdateTaskDescriptionCommand commandToExecute(system, id, desc);
		commandToExecute.execute();
	}
	else if (command == "remove-task-from-dashboard")
	{
		int id;
		ss >> id;

		RemoveTaskFromDashboardCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == "add-task-to-dashboard")
	{
		int id;
		ss >> id;

		AddTaskToDashboardCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == "delete-task")
	{
		int id;
		ss >> id;

		DeleteTaskCommand commandToExecute(system, id);
		commandToExecute.execute();
	}
	else if (command == "get-task")
	{
		MyString arg;
		ss >> arg;

		if(isNumber(arg[0]))
		{
			try
			{
				int id = charToInt(arg.c_str());
				GetTaskCommand commandToExecute(system, id);
				commandToExecute.execute();
			}
			catch(std::exception& e)
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
	else if (command == "list-tasks")
	{
		MyString arg;

		if(arg.getLength() == 0)
		{
			ListTasksCommand command(system);
			command.execute();
		}
		else if(isNumber(arg[0]))
		{
			MyString format = "%Y-%m-%d %H:%M:%S";
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
	else if (command == "list-completed-tasks")
	{
		ListCompletedTasksCommand command(system);
		command.execute();
	}
	else if (command == "list-dashboard")
	{
		ListDashboardCommand command(system);
		command.execute();
	}
	else if (command == "finish-task")
	{
		int id;
		ss >> id;

		FinishTaskCommand command(system, id);
		command.execute();
	}
	else if (command == "logout")
	{
		LogoutCommand command(system);
		command.execute();
	}
	else
	{
		std::cout << "Invalid command. Try again." << std::endl;
	}
}

void readDatabaseFromFile(System& system)
{
	system.loadDataFromFile();
}

int main()
{
	try
	{
		System system;
		readDatabaseFromFile(system);

		while (true)
		{
			char line[MAX_LINE_SIZE];

			std::cout << "Enter command: ";
			std::cin.getline(line, MAX_LINE_SIZE);

			std::stringstream ss(line);
			MyString command;

			ss.getline(&command[0], MAX_LINE_SIZE, ' ');

			if (command == "exit")
			{
				break;
			}

			executeCommand(system, command, ss);
		}
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return 0;
	
}