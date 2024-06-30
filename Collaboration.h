#pragma once
#include "MyString.h"
#include "Pair.hpp"
#include "Task.h"
#include "User.h"
#include "Vector.hpp"

class Collaboration
{
private:
	static int uniqueId;
	int id = -1;

	MyString name;
	User* creator = nullptr;

	Vector<Pair<Task*, User*>> tasks;
	Vector<User*> users;

public:
	Collaboration() = default;
	Collaboration(const MyString& name, User* creator);
	Collaboration(int id, const MyString& name, User* creator);

	const MyString& getName() const;
	User* getCreator() const;
	int getId() const;
	void setInvalidId();

	const Vector<Pair<Task*, User*>>& getTasks() const;
	const Vector<User*>& getUsers() const;

	bool hasUser(const User* user) const;
	void tryAddUser(const User* caller, User* userToAdd);
	void tryAssignTask(const User* caller, User* assignee, Task* task);

	void restoreUser(User* user);
	void restoreTaskAssignment(User* assignee, Task* task);
};