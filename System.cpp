#include "System.h"

const char System::filePath[] = "users.dat";

int System::findUserIdx(const MyString& username) const
{
	size_t size = allUsers.getSize();

	for (int i = 0; i < size; i++)
	{
		if (allUsers[i].getUsername() == username)
		{
			return i;
		}
	}

	return -1;
}

int System::findTask(int id) const
{
	int tasksCount = allTasks.getSize();

	for (int i = 0; i < tasksCount; i++)
	{
		if (allTasks[i].getId() == id)
		{
			return i;
		}
	}

	return -1;
}

int System::findCollaboration(const MyString& name) const
{
	int size = allCollaborations.getSize();

	for (int i = 0; i< size; i++)
	{
		if(allCollaborations[i].getName() == name)
		{
			return i;
		}
	}

	return -1;
}

void System::deleteTasksFromCollaboration(Collaboration& collaboration)
{
	int size = collaboration.getTasksCount();
	for(int i = 0; i < size; i++)
	{
		allTasks.popAt(findTask(collaboration.getTaskFromIndex(i)->getId()));
	}
}

void System::saveDataToFile() const
{
	std::ofstream ofs(filePath, std::ios::binary);

	if(!ofs.is_open())
	{
		throw std::exception("Cannot open file");
	}

	saveDataToFile(ofs);
}

void System::saveDataToFile(std::ofstream& ofs) const
{
	int tasksCount = allTasks.getSize();
	ofs.write(reinterpret_cast<const char*>(tasksCount), sizeof(tasksCount));

	for(int i = 0; i < tasksCount; i++)
	{
		allTasks[i].serialize(ofs);
	}

	int usersCount = allUsers.getSize();
	ofs.write(reinterpret_cast<const char*>(&usersCount), sizeof(usersCount));

	for(int i = 0; i < usersCount; i++)
	{
		allUsers[i].serialize(ofs);
	}

	int collabsCount = allCollaborations.getSize();
	ofs.write(reinterpret_cast<const char*>(&collabsCount), sizeof(collabsCount));

	for (int i = 0; i < collabsCount; i++)
	{
		allCollaborations[i].serialize(ofs);
	}
}

void System::loadDataFromFile(std::ifstream& ifs)
{
	int tasksCount;

	if (ifs.read(reinterpret_cast<char*>(&tasksCount), sizeof(tasksCount))) {
		Vector<Task> readTasks(tasksCount);
		for (int i = 0; i < tasksCount; i++)
		{
			readTasks[i].deserialize(ifs);
		}
		allTasks = readTasks;

		int usersCount;
		ifs.read(reinterpret_cast<char*>(&usersCount), sizeof(usersCount));

		Vector<User> readUsers(usersCount);
		for (int i = 0; i < usersCount; i++)
		{
			readUsers[i].deserialize(ifs);

			int tasksIndexesCount;
			ifs.read(reinterpret_cast<char*>(&tasksIndexesCount), sizeof(tasksIndexesCount));
			for (int j = 0; j < tasksIndexesCount; j++)
			{
				int currIndex;
				ifs.read(reinterpret_cast<char*>(&currIndex), sizeof(currIndex));
				readUsers[i].addTask(allTasks[currIndex]);
			}
		}
		allUsers = readUsers;

		int collabsCount;
		ifs.read(reinterpret_cast<char*>(&collabsCount), sizeof(collabsCount));

		Vector<Collaboration> readCollaborations(collabsCount);
		for (int i = 0; i < collabsCount; i++)
		{
			readCollaborations[i].deserialize(ifs);

			int creatorIndex;
			ifs.read(reinterpret_cast<char*>(&creatorIndex), sizeof(creatorIndex));

			readCollaborations[i].setCreator(&allUsers[creatorIndex]);

			int tasksCount;
			ifs.read(reinterpret_cast<char*>(&tasksCount), sizeof(tasksCount));

			for (int j = 0; j < tasksCount; j++)
			{
				int currIndexTask;
				int currIndexAssignee;
				ifs.read(reinterpret_cast<char*>(&currIndexTask), sizeof(currIndexTask));
				ifs.read(reinterpret_cast<char*>(&currIndexAssignee), sizeof(currIndexAssignee));

				readCollaborations[i].addDeserializedTask(allUsers[currIndexAssignee], allTasks[currIndexTask]);
			}

			int usersCount;
			ifs.read(reinterpret_cast<char*>(&usersCount), sizeof(usersCount));

			for (int j = 0; j < usersCount; j++)
			{
				int currIndex;
				ifs.read(reinterpret_cast<char*>(&currIndex), sizeof(currIndex));
				readCollaborations[i].addDeserializedUser(allUsers[currIndex]);
			}
		}
		allCollaborations = readCollaborations;
	}
}

void System::loadDataFromFile()
{
	std::ifstream ifs(filePath, std::ios::binary);

	if(!ifs.is_open())
	{
		throw std::exception("Couldn't open file");
	}

	loadDataFromFile(ifs);

}

void System::registerUser(const MyString& username, const MyString& password)
{
	if(password.getLength() == 0 || username.getLength() == 0)
	{
		std::cout << "Please input password and/or username." << std::endl;
	}

	if (findUserIdx(username) == -1)
	{
		int userIndex = allUsers.getSize();
		allUsers.pushBack(User(userIndex, username, password));

		std::cout << "Successfully created account!" << std::endl;
	}
	else
	{
		std::cout << "Username already taken.";
	}
}

void System::loginUser(const MyString& username, const MyString& password)
{
	int userIdx = findUserIdx(username);

	if (userIdx != -1 && allUsers[userIdx].checkPassword(password))
	{
		std::time_t currTime = std::time(nullptr);

		if (localtime_s(&loggedInTime, &currTime) != 0)
		{
			std::cerr << "Failed to get loggedInTime" << std::endl; 
		}
		else 
		{
			loggedInUser = &allUsers[userIdx];
			loggedInUser->updateTaskStatus(loggedInTime);
			loggedInUser->updateDashboard(loggedInTime);
			std::cout << "Login successful" << std::endl;
		}
	}
	else
	{
		std::cout << "Login unsuccessful" << std::endl;
	}
}

void System::addTask(const MyString& name, const std::tm& due_date, const MyString& description)
{
	if(!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	if (loggedInUser->taskExists(name, due_date))
	{
		std::cout << "Task with that name and due date already exists" << std::endl;
	}
	else
	{
		int indexNewTask = allTasks.getSize();
		allTasks.pushBack(Task(name, due_date, description, indexNewTask));
	
		loggedInUser->addTask(allTasks[indexNewTask]);
		std::cout << "Task added successfully!" << std::endl;
	}
}

void System::addTask(const MyString& name, const MyString& description)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	if (loggedInUser->taskExists(name))
	{
		std::cout << "Task with that name and due date already exists" << std::endl;
	}
	else
	{
		int indexNewTask = allTasks.getSize();
		allTasks.pushBack(Task(name, description, indexNewTask));

		loggedInUser->addTask(allTasks[indexNewTask]);
		std::cout << "Task added successfully!" << std::endl;

	}
}

void System::updateTaskName(int id, const MyString& name)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->updateTaskName(id, name);
}

void System::startTask(int id)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->startTask(id);
}

void System::updateTaskDescription(int id, const MyString& desc)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->updateTaskDescription(id, desc);
}

void System::removeTaskFromDashboard(int id)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->removeTaskFromDashboard(id);
}

void System::addTaskToDashboard(int id)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->addTaskToDashboard(id);
}

void System::deleteTask(int id)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	if(loggedInUser->deleteTask(id))
	{
		int index = findTask(id);

		if(index != -1)
		{
			allTasks.popAt(index);
			std::cout << "Task deleted!" << std::endl;
		}
	}
}

void System::getTask(const MyString& name) const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->getTask(name);

}

void System::getTask(int id) const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->getTask(id);
}

void System::listTasks(const std::tm& date) const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->listTasks(date);
}

void System::listTasks() const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->listTasks();

}

void System::listCompletedTasks() const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->listCompletedTasks();

}

void System::listDashboard() const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->listDashboard();

}

void System::finishTask(int id)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	loggedInUser->finishTask(id);
	loggedInUser->updateDashboard(loggedInTime);
}

void System::logout()  
{
	if (!loggedInUser)
	{
		std::cout << "You are already logged out." << std::endl;
		return;
	}

	loggedInUser = nullptr;
	std::cout << "Logged out successfully!" << std::endl;
}

void System::addCollaboration(const MyString& name)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int index = allCollaborations.getSize();
	allCollaborations.pushBack(Collaboration(index, name, loggedInUser));
	std::cout << "Collaboration added successfully!" << std::endl;
}

void System::deleteCollaboration(const MyString& name)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int index = findCollaboration(name);

	if (index != -1)
	{
		if (allCollaborations[index].getCreator() == loggedInUser)
		{
			allCollaborations[index].deleteCollaboration();
			deleteTasksFromCollaboration(allCollaborations[index]);
			allCollaborations.popAt(index);
			std::cout << "Collaboration deleted!" << std::endl;
		}
		else
		{
			std::cout << "Don't have access to delete this collaboration" << std::endl;
		}
	}
	else
	{
		std::cout << "Collaboration with that name doesn't exist" << std::endl;
	}
}

void System::listCollaborations() const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int size = allCollaborations.getSize();

	for(int i = 0; i < size; i++)
	{
		const Collaboration& currCollab = allCollaborations[i];

		if(currCollab.userIsInCollaboration(*loggedInUser))
		{
			std::cout << currCollab.getName() << std::endl;
		}
	}
}

void System::addUser(const MyString& collaborationName, const MyString& username)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int indexCollab = findCollaboration(collaborationName);
	int indexUser = findUserIdx(username);

	if(indexCollab != -1 && indexUser != -1)
	{
		if(allCollaborations[indexCollab].getCreator() == loggedInUser)
		{
			allCollaborations[indexCollab].addUser(allUsers[indexUser]);
			std::cout << "User added successfully!" << std::endl;
		}
		else
		{
			std::cout << "You cannot add a user to a collaboration that you haven't created" << std::endl;
		}
	}
	else
	{
		std::cout << "Error. Collaboration or user doesn't exist in the system." << std::endl;
	}
}

void System::assignTask(const MyString& collaborationName, const MyString& username, const MyString& taskName,
						const std::tm& due_date, const MyString& desc)
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int collabIndex = findCollaboration(collaborationName);
	int assigneeIndex = findUserIdx(username);

	Collaboration& collaboration = allCollaborations[collabIndex];

	if(collabIndex != -1 && assigneeIndex != -1 &&  collaboration.userIsInCollaboration(*loggedInUser)&& collaboration.userIsInCollaboration(allUsers[assigneeIndex]))
	{
		int indexTask = allTasks.getSize();
		allTasks.pushBack(Task(taskName, due_date, desc, indexTask));
		collaboration.addTask(allUsers[assigneeIndex], allTasks[indexTask]);
		std::cout << "Task assigned successfully!" << std::endl;
	}
	else
	{
		std::cout << "Couldn't assign task!" << std::endl;
	}
}

void System::listTasks(const MyString& collaborationName) const
{
	if (!loggedInUser)
	{
		std::cout << "Please log in." << std::endl;
		return;
	}

	int index = findCollaboration(collaborationName);

	if(index != -1 && allCollaborations[index].userIsInCollaboration(*loggedInUser))
	{
		const Collaboration& collaboration = allCollaborations[index];
		collaboration.listTasks();
	}
	else
	{
		std::cout << "Couldn't find collaboration with that name" << std::endl;
	}
}

System::~System()
{
	saveDataToFile();
}
