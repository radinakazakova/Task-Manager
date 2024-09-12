#include "Commands.h"
#include <iostream>
#include <exception>

Command::Command( System& system) : system(system)
{
}

RegisterCommand::RegisterCommand( System& system, const MyString& username, const MyString& password) : Command(system), username(username), password(password)
{
}

void RegisterCommand::execute()
{
	try
	{
		system.registerUser(username, password);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

LoginCommand::LoginCommand( System& system, const MyString& username, const MyString& password) : Command(system), username(username), password(password)
{
}

void LoginCommand::execute()
{
	try
	{
		system.loginUser(username, password);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AddTaskCommand::AddTaskCommand( System& system, const MyString& name, const std::tm& due_date,
	const MyString& description) : Command(system), name(name), due_date(due_date), description(description), hasDueDate(true)
{
}

AddTaskCommand::AddTaskCommand( System& system, const MyString& name, const MyString& description) : Command(system), name(name), due_date(due_date), description(description)
{
}

void AddTaskCommand::execute()
{
	try
	{
		if (hasDueDate)
		{
			system.addTask(name, due_date, description);
		}
		else
		{
			system.addTask(name, description);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

}

UpdateTaskNameCommand::UpdateTaskNameCommand( System& system, int id, const MyString& name): Command(system), id(id), name(name)
{
}

void UpdateTaskNameCommand::execute()
{
	try
	{
		system.updateTaskName(id, name);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

StartTaskCommand::StartTaskCommand( System& system, int id): Command(system), id(id)
{
}

void StartTaskCommand::execute()
{
	try
	{
		system.startTask(id);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

UpdateTaskDescriptionCommand::UpdateTaskDescriptionCommand( System& system, int id, const MyString& desc) : Command(system), id(id), desc(desc)
{
}

void UpdateTaskDescriptionCommand::execute()
{
	try
	{
		system.updateTaskDescription(id, desc);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

RemoveTaskFromDashboardCommand::RemoveTaskFromDashboardCommand( System& system, int id) : Command(system), id(id)
{
}

void RemoveTaskFromDashboardCommand::execute()
{
	try
	{
		system.removeTaskFromDashboard(id);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AddTaskToDashboardCommand::AddTaskToDashboardCommand( System& system, int id) : Command(system), id(id)
{
}

void AddTaskToDashboardCommand::execute()
{
	try
	{
		system.addTaskToDashboard(id);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

DeleteTaskCommand::DeleteTaskCommand( System& system, int id) : Command(system), id(id)
{
}

void DeleteTaskCommand::execute()
{
	try
	{
		system.deleteTask(id);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

GetTaskCommand::GetTaskCommand( System& system, int id): Command(system), id(id)
{
}

GetTaskCommand::GetTaskCommand( System& system, const MyString& name):Command(system), name(name)
{
}

void GetTaskCommand::execute()
{
	try
	{
		if(id ==-1)
		{
			system.getTask(name);
		}
		else
		{
			system.getTask(id);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

ListTasksCommand::ListTasksCommand( System& system, const std::tm& date): Command(system), date(date), hasDate(true)
{
}

ListTasksCommand::ListTasksCommand( System& system):Command(system)
{
}

ListTasksCommand::ListTasksCommand( System& system, const MyString& collabName): Command(system), collabName(collabName), hasCollabName(true)
{
}

void ListTasksCommand::execute()
{
	try
	{
		if(hasCollabName == false && hasDate == false)
		{
			system.listTasks();
		}
		else if (hasCollabName == true && hasDate == false)
		{
			system.listTasks(collabName);
		}
		else if (hasCollabName == false && hasDate == true)
		{
			system.listTasks(date);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

ListCompletedTasksCommand::ListCompletedTasksCommand( System& system): Command(system)
{
}

void ListCompletedTasksCommand::execute()
{
	try
	{
		system.listCompletedTasks();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

ListDashboardCommand::ListDashboardCommand( System& system): Command(system)
{
}

void ListDashboardCommand::execute()
{
	try
	{
		system.listDashboard();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

FinishTaskCommand::FinishTaskCommand( System& system, int id):Command(system), id(id)
{
}

void FinishTaskCommand::execute()
{
	try
	{
		system.finishTask(id);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

LogoutCommand::LogoutCommand( System& system) :Command(system)
{
}

void LogoutCommand::execute()
{
	try
	{
		system.logout();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AddCollaborationCommand::AddCollaborationCommand( System& system, const MyString& name): Command(system), name(name)
{
}

void AddCollaborationCommand::execute()
{
	try
	{
		system.addCollaboration(name);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

DeleteCollaborationCommand::DeleteCollaborationCommand( System& system, const MyString& name) : Command(system), name(name)
{
}

void DeleteCollaborationCommand::execute()
{
	try
	{
		system.deleteCollaboration(name);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

listCollaborationsCommand::listCollaborationsCommand( System& system):Command(system)
{
}

void listCollaborationsCommand::execute()
{
	try
	{
		system.listCollaborations();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AddUserCommand::AddUserCommand( System& system, const MyString& collaborationName, const MyString& username): Command(system), collaborationName(collaborationName), username(username)
{
}

void AddUserCommand::execute()
{
	try
	{
		system.addUser(collaborationName, username);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AssignTaskCommand::AssignTaskCommand( System& system, const MyString& collaborationName, const MyString& username,
	const MyString& taskName, const std::tm& due_date, const MyString& desc): Command(system), collaborationName(collaborationName), username(username), taskName(taskName), due_date(due_date), desc(desc), hasDate(true)
{
}

AssignTaskCommand::AssignTaskCommand(System& system, const MyString& collaborationName, const MyString& username,
	const MyString& taskName, const MyString& desc): Command(system), collaborationName(collaborationName), username(username), taskName(taskName), desc(desc)
{
}

void AssignTaskCommand::execute()
{
	try
	{
		if(hasDate)
		{
			system.assignTask(collaborationName, username, taskName, due_date, desc);
		}
		else
		{
			system.assignTask(collaborationName, username, taskName, desc);
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}


