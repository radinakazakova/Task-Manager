#include "Collaboration.h"

int Collaboration::uniqueId = -1;

Collaboration::Collaboration()
{
	++uniqueId;
	id = uniqueId;
}

Collaboration::Collaboration(int index, const MyString& name, User* creator): index(index), name(name), creator(creator)
{
	++uniqueId;
	id = uniqueId;
	users.addAtFirstFreeIndex(*creator);
	
}



const MyString& Collaboration::getName() const
{
	return name;
}

const User* Collaboration::getCreator() const
{
	return creator;
}

int Collaboration::getIndex() const
{
	return index;
}

Task* Collaboration::getTaskFromIndex(int index) const
{
	return tasks[index].getFirst();
}


size_t Collaboration::getTasksCount() const
{
	return tasks.getSize();
}

void Collaboration::setCreator(User* creator)
{
	this->creator = creator;
}

void Collaboration::serialize(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&index), sizeof(index));
	ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

	size_t nameSize = name.getLength();
	ofs.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
	ofs.write(name.c_str(), nameSize);

	int indexCreator = creator->getIndex();
	ofs.write(reinterpret_cast<const char*>(&indexCreator), sizeof(indexCreator));

	size_t tasksSize = tasks.getSize();
	ofs.write(reinterpret_cast<const char*>(&tasksSize), sizeof(tasksSize));
	for(size_t i = 0; i < tasksSize; ++i)
	{
		int first = tasks[i].getFirst()->getIndex();
		int second = tasks[i].getSecond()->getIndex();
		ofs.write(reinterpret_cast<const char*>(&first), sizeof(first));
		ofs.write(reinterpret_cast<const char*>(&second), sizeof(second));
	}

	size_t usersSize = users.getSize();
	ofs.write(reinterpret_cast<const char*>(&usersSize), sizeof(usersSize));
	for(int i = 0; i < usersSize; i++)
	{
		int currUserIndex = users[i].getIndex();
		ofs.write(reinterpret_cast<const char*>(&currUserIndex), sizeof(currUserIndex));
	}
}

void Collaboration::deserialize(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&index), sizeof(index));
	ifs.read(reinterpret_cast<char*>(&id), sizeof(id));

	size_t nameSize;
	ifs.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	MyString tempName(nameSize);
	ifs.read(&tempName[0], nameSize);
	name = tempName;

}


bool Collaboration::userIsInCollaboration(User& user) const
{
	int size = users.getSize();

	for(int i = 0; i < size; i++)
	{
		if(&users[i] == &user)
		{
			return true;
		}
	}

	return false;
}

void Collaboration::deleteCollaboration()
{
	int size = users.getSize();

	for(int i = 0; i < size; i++)
	{
		int tasksCount = tasks.getSize();

		for(int j = 0 ; j < tasksCount; j++)
		{
			users[i].deleteTask(tasks[j].getFirst()->getId());
		}
	}
}

void Collaboration::addDeserializedTask(User& assignee, Task& taskToAdd)
{
	tasks.pushBack(Pair<Task*, User*>(&taskToAdd, &assignee));
}

void Collaboration::addDeserializedUser(User& user)
{
	users.addAtFirstFreeIndex(user);
}

void Collaboration::addUser(User& userToAdd)
{
	users.addAtFirstFreeIndex(userToAdd);

	int tasksCount = tasks.getSize();

	for(int i = 0; i < tasksCount; i++)
	{
		userToAdd.addTask(*tasks[i].getFirst());
	}
}

void Collaboration::addTask(User& assignee, Task& taskToAdd)
{
	tasks.pushBack(Pair<Task*, User*>(&taskToAdd, &assignee));

	int size = users.getSize();

	for(int i = 0; i < size; i++)
	{
		users[i].addTask(taskToAdd);
	}
}

void Collaboration::listTasks() const
{
	int size = tasks.getSize();

	for(int i = 0; i < size; i++)
	{
		tasks[i].getFirst()->printTask();
		std::cout << "Assignee: " << tasks[i].getSecond()->getUsername() << std::endl;
		std::cout << "\n";
	}
}
