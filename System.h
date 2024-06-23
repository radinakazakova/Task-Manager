#pragma once

#include "Task.h"
#include "User.h"
#include "Collaboration.h"
#include "Vector.hpp"
#include <ctime>
#include <fstream>

class System
{
	static const char filePath[];

	Vector<Task> allTasks;
	Vector<Collaboration> allCollaborations;
	Vector<User> allUsers;

	User* loggedInUser = nullptr;
	std::tm loggedInTime;

	int findUserIdx(const MyString& username) const;
	int findTask(int id) const;
	int findCollaboration(const MyString& name) const;
	void saveDataToFile() const;
	void saveDataToFile(std::ofstream& ofs) const;
	void loadDataFromFile(std::ifstream& ifs);
	void deleteTasksFromCollaboration(Collaboration& collaboration);
public:

	System() = default; 

	void loadDataFromFile();

	void registerUser(const MyString& username, const MyString& password);
	void loginUser(const MyString& username, const MyString& password);
	void addTask(const MyString& name, const std::tm& due_date, const MyString& description);
	void addTask(const MyString& name, const MyString& description);
	void updateTaskName(int id, const MyString& name);
	void startTask(int id);
	void updateTaskDescription(int id, const MyString& desc);
	void removeTaskFromDashboard(int id); 
	void addTaskToDashboard(int id);
	void deleteTask(int id);
	void getTask(const MyString& name) const;
	void getTask(int id) const;
	void listTasks(const std::tm& date) const;
	void listTasks()const;
	void listCompletedTasks() const;
	void listDashboard() const;
	void finishTask(int id);
	void logout(); 

	void addCollaboration(const MyString& name);
	void deleteCollaboration(const MyString& name);
	void listCollaborations() const;
	void addUser(const MyString& collaborationName, const MyString& username);
	void assignTask(const MyString& collaborationName, const MyString& username, const MyString& taskName, const std::tm& due_date, const MyString& desc);
	void listTasks(const MyString& collaborationName) const;

	~System();
};
