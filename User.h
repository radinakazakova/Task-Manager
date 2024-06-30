#pragma once
#include "MyString.h"
#include "Task.h"
#include "Vector.hpp"
#include <ctime>

class User
{
private:
	MyString username;
	MyString password;

	Vector<Task*> tasks;
	Vector<Task*> dashboard;

public:
	User() = default;
	User(const MyString& name, const MyString& pass);

	MyString getUsername() const;
	const MyString& getPassword() const;
	bool checkPassword(const MyString& pass) const;

	const Vector<Task*>& getTasks() const;
	const Vector<Task*>& getDashboard() const;
	Task* getTaskById(int id) const;

	void tryAddTask(Task* task);
	void tryDeleteTask(int id);
	void tryAddTaskToDashboard(Task* task);
	void tryRemoveTaskFromDashboard(int id);

	bool isTaskInDashboard(const Task* task) const;
	bool taskExists(const MyString& name) const;
	bool taskExists(const MyString& name, const std::tm& due_date) const;

	void syncTasksAndDashboard(const std::tm& currentTime);

	void restoreTask(Task* task);
	void restoreTaskToDashboard(Task* task);
};