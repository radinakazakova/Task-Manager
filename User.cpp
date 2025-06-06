#include "User.h"
using namespace TimeManagement;

static constexpr int MAX_LEN = 100;

int User::findTask(int id) const
{
	size_t tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getId() == id)
		{
			return i;
		}
	}

	return -1;
}

int User::findTaskInDashboard(int id) const
{
	size_t dashboardTasksCount = dashboard.getSize();

	for (int i = 0; i < dashboardTasksCount; i++)
	{
		if (dashboard[i].getId() == id)
		{
			return i;
		}
	}

	return -1;
}

int User::getSmallestIdFromName(const MyString& name)
{
	int smallestId = INT_MAX;
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getName() == name && tasks[i].getId() != -1)
		{
			int currentId = tasks[i].getId();

			if (currentId < smallestId)
			{
				smallestId = currentId;
			}
		}
	}

	return smallestId;
}

bool User::taskIsInDashboard(const Task& task) const
{
	int size = dashboard.getSize();

	for (int i = 0; i < size; i++)
	{
		if (&dashboard[i] == &task)
		{
			return true;
		}
	}

	return false;
}

User::User(int index, const MyString& name, const MyString& pass) : index(index), username(name), password(pass) {}

MyString User::getUsername() const
{
	return username;
}

int User::getIndex() const
{
	return index;
}

bool User::checkPassword(const MyString& pass) const
{
	return (password == pass);
}

bool User::taskExists(const MyString& name, const std::tm& due_date) const
{
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getId() != -1 && tasks[i].hasDate() && tasks[i].getName() == name)
		{
			if (tasks[i].getDueDate() == due_date)
			{
				return true;
			}
		}
	}

	return false;
}

bool User::taskExists(const MyString& name) const
{
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getName() == name)
		{
			return true;
		}
	}

	return false;
}

void User::addDeserializedTaskToDashboard(Task& task)
{
	dashboard.addAtFirstFreeIndex(task);
}

void User::serialize(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&index), sizeof(index));

	int usernameSize = username.getLength();
	ofs.write(reinterpret_cast<const char*>(&usernameSize), sizeof(usernameSize));
	ofs.write(username.c_str(), usernameSize);

	int passwordSize = password.getLength();
	ofs.write(reinterpret_cast<const char*>(&passwordSize), sizeof(passwordSize));
	ofs.write(password.c_str(), passwordSize);

	int tasksCount = tasks.getSize();
	ofs.write(reinterpret_cast<const char*>(&tasksCount), sizeof(tasksCount));
	for (int i = 0; i < tasksCount; i++)
	{

		int currIndex = tasks[i].getIndex();
		ofs.write(reinterpret_cast<const char*>(&currIndex), sizeof(currIndex));

	}

	int dashboardTasksCount = dashboard.getSize();
	ofs.write(reinterpret_cast<const char*>(&dashboardTasksCount), sizeof(dashboardTasksCount));
	for (int i = 0; i < dashboardTasksCount; i++)
	{

		int currIndex = dashboard[i].getIndex();
		ofs.write(reinterpret_cast<const char*>(&currIndex), sizeof(currIndex));

	}
}

void User::deserialize(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&index), sizeof(index));

	int nameSize;
	ifs.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	char tempName[MAX_LEN] = {};
	ifs.read(tempName, nameSize);
	tempName[nameSize] = '\0';
	username = tempName;

	int passwordSize;
	ifs.read(reinterpret_cast<char*>(&passwordSize), sizeof(passwordSize));
	char tempPass[MAX_LEN] = {};
	ifs.read(tempPass, passwordSize);
	tempPass[nameSize] = '\0';
	password = tempPass;

}

void User::updateTaskStatus(const std::tm& loggedInTime)
{
	size_t tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].hasDate())
		{
			if (isBefore(tasks[i].getDueDate(), loggedInTime))
			{
				tasks[i].changeStatus(TaskStatus::OVERDUE);
			}
		}
	}
}

void User::updateDashboard(const std::tm& loggedInTime)
{
	size_t dashboardTasksCount = dashboard.getSize();

	for (int i = 0; i < dashboardTasksCount; i++)
	{
		if (dashboard[i].getStatus() == TaskStatus::OVERDUE || dashboard[i].getStatus() == TaskStatus::DONE)
		{
			dashboard.removeAt(i);
		}
	}

	size_t tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].hasDate())
		{
			if (isSameDay(tasks[i].getDueDate(), loggedInTime) && (tasks[i].getStatus() != TaskStatus::DONE && tasks[i].getStatus() != TaskStatus::OVERDUE) && !taskIsInDashboard(tasks[i]))
			{
				dashboard.addAtFirstFreeIndex(tasks[i]);
			}
		}
	}
}

void User::addTask(Task& task)
{
	if (task.getId() != -1)
		tasks.addAtFirstFreeIndex(task);
}

void User::updateTaskName(int id, const MyString& name)
{
	int index = findTask(id);

	if (index != -1)
	{
		tasks[index].changeName(name);
		std::cout << "Task name updated!" << std::endl;
	}
	else
	{
		std::cout << "Task with that ID doesn't exist." << std::endl;
	}
}

void User::startTask(int id)
{
	int index = findTask(id);

	if (index != -1)
	{
		tasks[index].changeStatus(TaskStatus::IN_PROCESS);
		std::cout << "Task started!" << std::endl;
	}
	else
	{
		std::cout << "Task with that ID doesn't exist" << std::endl;
	}
}

void User::updateTaskDescription(int id, const MyString& desc)
{
	int index = findTask(id);

	if (index != -1)
	{
		tasks[index].changeDescription(desc);
		std::cout << "Task description updated!" << std::endl;
	}
	else
	{
		std::cout << "Task with that ID doesn't exist" << std::endl;
	}
}

void User::removeTaskFromDashboard(int id)
{
	int index = findTaskInDashboard(id);

	if (index != -1)
	{
		dashboard.removeAt(index);
		std::cout << "Task removed from dashboard!" << std::endl;
	}
	else
	{
		std::cout << "Task with that ID is not in dashboard." << std::endl;
	}
}

void User::addTaskToDashboard(int id)
{
	int indexTasks = findTask(id);

	if (indexTasks != -1)
	{
		int indexDashboard = findTaskInDashboard(id);

		if (indexDashboard == -1)
		{
			if (tasks[indexTasks].getStatus() != TaskStatus::OVERDUE && tasks[indexTasks].getStatus() != TaskStatus::DONE)
			{
				dashboard.addAtFirstFreeIndex(tasks[indexTasks]);
				std::cout << "Task added to dashboard!" << std::endl;
			}
			else
			{
				std::cout << "Cannot add a task that is overdue or done to the dashboard." << std::endl;
			}
		}
		else
		{
			std::cout << "Task is already in the dashboard." << std::endl;
		}
	}
	else
	{
		std::cout << "Task with that ID doesn't exist" << std::endl;
	}
}

bool User::deleteTask(int id)
{
	if (id != -1)
	{
		int indexTasks = findTask(id);

		if (indexTasks != -1)
		{
			int indexDashboard = findTaskInDashboard(id);

			if (indexDashboard != -1)
			{
				dashboard.removeAt(indexDashboard);

				tasks.removeAt(indexTasks);
				return true;
			}
			else
			{
				tasks.removeAt(indexTasks);
				return true;
			}
		}
		else
		{
			std::cout << "Task with that ID doesn't exist" << std::endl;
			return false;
		}
	}
}

void User::getTask(const MyString& name)
{
	int id = getSmallestIdFromName(name);

	if (id != INT_MAX)
	{
		getTask(id);
	}
	else
	{
		std::cout << "Task with that name doesn't exist" << std::endl;
	}
}

void User::getTask(int id)
{
	int taskIndex = findTask(id);

	if (taskIndex != -1)
	{
		tasks[taskIndex].printTask();
	}
	else
	{
		std::cout << "Task with this ID doesn't exist" << std::endl;
	}
}

void User::listTasks(const std::tm& date) const
{
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].hasDate() && tasks[i].getId() != -1)
		{
			if (isSameDay(tasks[i].getDueDate(), date))
			{
				tasks[i].printTask();
			}
		}

	}
}

void User::listTasks() const
{
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getId() != -1)
		{
			tasks[i].printTask();
		}
	}
}

void User::listCompletedTasks() const
{
	int tasksCount = tasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (tasks[i].getStatus() == TaskStatus::DONE && tasks[i].getId() != -1)
		{
			tasks[i].printTask();
		}
	}
}

void User::listDashboard() const
{
	int dashboardTasksCount = dashboard.getSize();

	for (int i = 0; i < dashboardTasksCount; i++)
	{
		if (dashboard[i].getId() != -1)
		{
			dashboard[i].printTask();
		}
	}
}

void User::finishTask(int id)
{
	int taskIndex = findTask(id);

	if (taskIndex != -1)
	{
		tasks[taskIndex].changeStatus(TaskStatus::DONE);
		std::cout << "Task status is successfully changed to DONE!" << std::endl;
	}
	else
	{
		std::cout << "Task with this ID doesn't exist" << std::endl;
	}
}
