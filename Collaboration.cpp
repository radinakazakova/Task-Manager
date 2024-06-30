#include "Collaboration.h"
#include "Exceptions.h"

int Collaboration::uniqueId = -1;

Collaboration::Collaboration(const MyString& name, User* creator) : name(name), creator(creator) {
	++uniqueId;
	id = uniqueId;
	users.pushBack(creator);
}

Collaboration::Collaboration(const int id, const MyString& name, User* creator) : id(id), name(name), creator(creator) {
	users.pushBack(creator);
	if (id > uniqueId)
	{
		uniqueId = id;
	}
}

const MyString& Collaboration::getName() const
{
	return name;
}

User* Collaboration::getCreator() const
{
	return creator;
}

int Collaboration::getId() const
{
	return id;
}

void Collaboration::setInvalidId()
{
	id = -1;
}

const Vector<Pair<Task*, User*>>& Collaboration::getTasks() const
{
	return tasks;
}

const Vector<User*>& Collaboration::getUsers() const
{
	return users;
}

bool Collaboration::hasUser(const User* user) const {
	size_t size = users.getSize();

	for (size_t i = 0; i < size; i++) {
		if (users[i] == user)
		{
			return true;
		}
	}

	return false;
}

void Collaboration::tryAddUser(const User* caller, User* userToAdd) {
	if (!caller || !userToAdd)
	{
		throw InvalidStateException("Invalid user data.");
	}

	if (hasUser(userToAdd))
	{
		throw DuplicateException("User is already in this collaboration.");
	}

	if (this->creator != caller)
	{
		throw UnauthorizedException("Only the creator can add new, non-existing users.");
	}

	users.pushBack(userToAdd);

	size_t tSize = tasks.getSize();
	for (size_t i = 0; i < tSize; i++) {
		Task* t = tasks[i].getFirst();
		bool exists = t->hasDate() ? userToAdd->taskExists(t->getName(), t->getDueDate()) : userToAdd->taskExists(t->getName());

		if (!exists) {
			userToAdd->tryAddTask(t);
		}
	}
}

void Collaboration::tryAssignTask(const User* caller, User* assignee, Task* task) {
	if (!caller || !assignee || !task)
	{
		throw InvalidStateException("Invalid assignment data.");
	}

	if (!hasUser(caller) || !hasUser(assignee))
	{
		throw UnauthorizedException("Assignment failed. Verify collaboration membership.");
	}

	tasks.pushBack(Pair<Task*, User*>(task, assignee));

	size_t usersSize = users.getSize();
	for (size_t i = 0; i < usersSize; i++) {
		bool exists = task->hasDate() ? users[i]->taskExists(task->getName(), task->getDueDate()) : users[i]->taskExists(task->getName());

		if (!exists) {
			users[i]->tryAddTask(task);
		}
	}
}

void Collaboration::restoreUser(User* user) {
	if (user)
	{
		users.pushBack(user);
	}
}

void Collaboration::restoreTaskAssignment(User* assignee, Task* task) {
	if (assignee && task)
	{
		tasks.pushBack(Pair<Task*, User*>(task, assignee));
	}
}