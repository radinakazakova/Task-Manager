#pragma once
#include "MyString.h"
#include "Pair.hpp"
#include "Task.h"
#include "User.h"
#include "Vector.hpp"
#include "DynamicArrayOfPointers.hpp"

class Collaboration
{
	static int uniqueId;
	int index = -1; //invalid collab
	int id = -1; //invalid collab

	MyString name;
	User* creator = nullptr;
	Vector<Pair<Task*, User*>> tasks; // task, assignee
	DynamicArrayOfPointers<User> users;

public:
	Collaboration();
	Collaboration(int index, const MyString& name, User* creator);

	const MyString& getName() const;
	const User* getCreator() const;
	int getIndex() const;
	Task* getTaskFromIndex(int index) const;
	size_t getTasksCount() const;
	int getId() const;

	void setCreator(User* creator);
	void setInvalidId();

	void serialize(std::ofstream& ofs) const;
	void deserialize(std::ifstream& ifs);

	bool userIsInCollaboration(User& user) const;
	void deleteCollaboration();
	void addDeserializedTask(User& assignee, Task& taskToAdd);
	void addDeserializedUser(User& user);
	void addUser(User& userToAdd);
	void addTask(User& assignee, Task& taskToAdd);
	void listTasks() const;
};
