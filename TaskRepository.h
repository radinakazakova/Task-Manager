#pragma once
#include "Vector.hpp"
#include "Task.h"

class TaskRepository {
private:
    Vector<Task*> tasks;

public:
    TaskRepository() = default;

    ~TaskRepository() {
        size_t size = tasks.getSize();
        for (size_t i = 0; i < size; ++i) {
            delete tasks[i];
        }
    }

    TaskRepository(const TaskRepository&) = delete;
    TaskRepository& operator=(const TaskRepository&) = delete;

    void addTask(Task* task) {
        tasks.pushBack(task);
    }

    Task* findById(int id) const {
        if (id == -1) {
            return nullptr;
        }

        size_t size = tasks.getSize();
        for (size_t i = 0; i < size; ++i) {
            if (tasks[i]->getId() == id) {
                return tasks[i];
            }
        }
        return nullptr;
    }

    const Vector<Task*>& getAllTasks() const {
        return tasks;
    }
};