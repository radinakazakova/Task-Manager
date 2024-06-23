#pragma once
#include "MyString.h"
#include "Task.h"
#include "DynamicArrayOfPointers.hpp"
#include "TimeManagement.h"
#include <ctime>
#include <fstream>

class User
{
	int index = -1;

	MyString username;
	MyString password;

	DynamicArrayOfPointers<Task> tasks;
	DynamicArrayOfPointers<Task> dashboard;

	int findTask(int id) const;
	int findTaskInDashboard(int id) const;

	int getSmallestIdFromName(const MyString& name);
	bool taskIsInDashboard(const Task& task) const;

public:
	User() = default;
	User(int index, const MyString& name, const MyString& pass);

	MyString getUsername() const;
	int getIndex() const;
	bool checkPassword(const MyString& pass) const;
	bool taskExists(const MyString& name, const std::tm& due_date) const;
	bool taskExists(const MyString& name) const;

	void serialize(std::ofstream& ofs) const;
	void deserialize(std::ifstream& ifs);

	void updateTaskStatus(const std::tm& loggedInTime);
	void updateDashboard(const std::tm& loggedInTime);
	void addTask(Task& task);
	void updateTaskName(int id, const MyString& name);
	void startTask(int id);
	void updateTaskDescription(int id, const MyString& desc);
	void removeTaskFromDashboard(int id);
	void addTaskToDashboard(int id);
	bool deleteTask(int id);
	void getTask(const MyString& name);
	void getTask(int id);
	void listTasks(const std::tm& date) const;
	void listTasks()const;
	void listCompletedTasks() const;
	void listDashboard() const;
	void finishTask(int id);
};
