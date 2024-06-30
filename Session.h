#pragma once
#include "TaskRepository.h"
#include "UserRepository.h"
#include "CollaborationRepository.h"
#include "User.h"
#include <ctime>

class Session {
private:
    TaskRepository taskRepository;
    UserRepository userRepository;
    CollaborationRepository collabRepository;

    User* loggedInUser = nullptr;
    std::tm loggedInTime = {};

public:
    Session() = default;

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    TaskRepository& getTaskRepository()
    {
	    return taskRepository;
    }

    UserRepository& getUserRepository()
    {
	    return userRepository;
    }

    CollaborationRepository& getCollaborationRepository()
    {
	    return collabRepository;
    }

    const TaskRepository& getTaskRepo() const
    {
	    return taskRepository;
    }

    const UserRepository& getUserRepository() const
    {
	    return userRepository;
    }

    const CollaborationRepository& getCollaborationRepository() const
    {
	    return collabRepository;
    }

    User* getLoggedInUser() const
    {
	    return loggedInUser;
    }

    void login(User* user, const std::tm& time) {
        loggedInUser = user;
        loggedInTime = time;
    }

    void logout() {
        loggedInUser = nullptr;
        loggedInTime = {};
    }

    std::tm getLoggedInTime() const
    {
	    return loggedInTime;
    }
};