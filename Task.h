#pragma once

#include "MyString.h"
#include "Optional.hpp"
#include <ctime>
#include <fstream>

enum TaskStatus 
{
	ON_HOLD,
	IN_PROCESS,
	DONE,
	OVERDUE
};

class Task
{
	static int uniqueNumber;

	int index = -1; //invalid task
	int id = -1; //invalid task
	MyString name;
	Optional<std::tm> due_date;
	TaskStatus status = TaskStatus::ON_HOLD;
	MyString description;

	void printStatus(const TaskStatus& stat) const;
public:

	Task();
	Task(const MyString& name, const std::tm& due_date, const MyString& description, int index);
	Task(const MyString& name, const MyString& description, int index);

	void changeName(const MyString& name);
	void changeStatus(const TaskStatus& newStatus);
	void changeDescription(const MyString& newDesc);
	void printTask() const;

	void serialize(std::ofstream& ofs) const;
	void deserialize(std::ifstream& ifs);

	int getId() const;
	int getIndex() const;
	const TaskStatus& getStatus() const;
	const std::tm& getDueDate() const;
	const MyString& getName() const;
	void setInvalidId();
	bool hasDate() const;
};
