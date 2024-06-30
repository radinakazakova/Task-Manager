#include "Task.h"
#include <iomanip>
#include "Collaboration.h"

int Task::uniqueNumber = -1;
static constexpr int MAX_LENGTH = 100;

Task::Task()
{
	uniqueNumber++;
	id = uniqueNumber;
}

Task::Task(const MyString& name, const std::tm& due_date, const MyString& description)
	: name(name), description(description)
{
	uniqueNumber++;
	id = uniqueNumber;
	this->due_date.setValue(due_date);
}

Task::Task(const MyString& name, const MyString& description)
	: name(name), description(description)
{
	uniqueNumber++;
	id = uniqueNumber;
}

Task::Task(int id, const MyString& name, const Optional<std::tm>& due_date, TaskStatus status, const MyString& description)
	: id(id), name(name), due_date(due_date), status(status), description(description)
{
	if (id > uniqueNumber) {
		uniqueNumber = id;
	}
}

void Task::changeName(const MyString& name)
{
	this->name = name;
}

void Task::changeStatus(TaskStatus newStatus)
{
	status = newStatus;
}

void Task::changeDescription(const MyString& newDesc)
{
	description = newDesc;
}

int Task::getId() const
{
	return id;
}

TaskStatus Task::getStatus() const
{
	return status;
}

const std::tm& Task::getDueDate() const
{
	return due_date.getValue();
}

const MyString& Task::getName() const
{
	return name;
}

const MyString& Task::getDescription() const
{
	return description;
}

void Task::setInvalidId()
{
	id = -1;
}

bool Task::hasDate() const
{
	return due_date.hasValue();
}