#pragma once
#include "MyString.h"
#include "Optional.hpp"
#include <ctime>

enum class TaskStatus
{
	ON_HOLD,
	IN_PROCESS,
	DONE,
	OVERDUE
};

class Task
{
private:
	static int uniqueNumber;

	int id = -1;
	MyString name;
	Optional<std::tm> due_date;
	TaskStatus status = TaskStatus::ON_HOLD;
	MyString description;

public:
	Task();
	Task(const MyString& name, const MyString& description);
	Task(const MyString& name, const std::tm& due_date, const MyString& description);
	Task(int id, const MyString& name, const Optional<std::tm>& due_date, TaskStatus status, const MyString& description);

	void changeName(const MyString& name);
	void changeStatus(TaskStatus newStatus);
	void changeDescription(const MyString& newDesc);
	void setInvalidId();

	int getId() const;
	TaskStatus getStatus() const;
	const std::tm& getDueDate() const;
	const MyString& getName() const;
	const MyString& getDescription() const;
	bool hasDate() const;
};