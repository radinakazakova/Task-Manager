#include "User.h"
#include "TimeManagement.h"
#include "Exceptions.h"

User::User(const MyString& name, const MyString& pass) : username(name), password(pass) {}

MyString User::getUsername() const
{
	return username;
}

const MyString& User::getPassword() const
{
	return password;
}

bool User::checkPassword(const MyString& pass) const
{
	return password == pass;
}

const Vector<Task*>& User::getTasks() const
{
	return tasks;
}

const Vector<Task*>& User::getDashboard() const
{
	return dashboard;
}

Task* User::getTaskById(int id) const {
	size_t size = tasks.getSize();

	for (size_t i = 0; i < size; ++i) {
		if (tasks[i]->getId() == id)
		{
			return tasks[i];
		}
	}

	return nullptr;
}

bool User::taskExists(const MyString& name) const {
	size_t size = tasks.getSize();

	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->getName() == name)
		{
			return true;
		}
	}

	return false;
}

bool User::taskExists(const MyString& name, const std::tm& due_date) const {
	size_t size = tasks.getSize();

	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->getId() != -1 && tasks[i]->hasDate() && tasks[i]->getName() == name) {
			if (TimeManagement::operator==(tasks[i]->getDueDate(), due_date))
			{
				return true;
			}
		}
	}

	return false;
}

bool User::isTaskInDashboard(const Task* task) const {
	size_t size = dashboard.getSize();

	for (size_t i = 0; i < size; i++) {
		if (dashboard[i] == task)
		{
			return true;
		}
	}

	return false;
}

void User::tryAddTask(Task* task) {
	if (!task || task->getId() == -1)
	{
		throw InvalidStateException("Cannot add an invalid task.");
	}

	if (task->hasDate() && taskExists(task->getName(), task->getDueDate()))
	{
		throw DuplicateException("Cannot add task. Task with this name and date already exists.");
	}

	if (!task->hasDate() && taskExists(task->getName()))
	{
		throw DuplicateException("Cannot add task. Task with this name already exists.");
	}

	tasks.pushBack(task);
}

void User::tryAddTaskToDashboard(Task* task) {
	if (!task || isTaskInDashboard(task))
	{
		throw InvalidStateException("Task is already in dashboard or does not exist.");
	}

	if (task->getStatus() == TaskStatus::DONE || task->getStatus() == TaskStatus::OVERDUE) {
		throw InvalidStateException("Failed to add task. Task is DONE or OVERDUE.");
	}

	dashboard.pushBack(task);
}

void User::tryRemoveTaskFromDashboard(const int id) {
	size_t size = dashboard.getSize();

	for (size_t i = 0; i < size; i++) {
		if (dashboard[i]->getId() == id) {
			dashboard.popAt(i);
			return;
		}
	}
}

void User::tryDeleteTask(int id) {
	size_t size = tasks.getSize();

	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->getId() == id) {
			tryRemoveTaskFromDashboard(id);

			tasks.popAt(i);
			return;
		}
	}

	throw NotFoundException("Task not found.");
}

void User::syncTasksAndDashboard(const std::tm& currentTime) {
	size_t size = tasks.getSize();
	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->hasDate() && tasks[i]->getStatus() != TaskStatus::DONE && TimeManagement::isBefore(tasks[i]->getDueDate(), currentTime)) {
			tasks[i]->changeStatus(TaskStatus::OVERDUE);
		}
	}

	for (size_t i = dashboard.getSize(); i > 0; i--) {
		Task* t = dashboard[i - 1];
		if (t->getStatus() == TaskStatus::OVERDUE || t->getStatus() == TaskStatus::DONE) {
			dashboard.popAt(i - 1);
		}
	}

	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->hasDate() && TimeManagement::isSameDay(tasks[i]->getDueDate(), currentTime)) {

			if (!isTaskInDashboard(tasks[i]) && tasks[i]->getStatus() != TaskStatus::DONE && tasks[i]->getStatus() != TaskStatus::OVERDUE) {
				tryAddTaskToDashboard(tasks[i]);
			}

		}
	}
}

void User::restoreTask(Task* task) {
	if (task) {
		tasks.pushBack(task);
	}
}

void User::restoreTaskToDashboard(Task* task) {
	if (task) {
		dashboard.pushBack(task);
	}
}