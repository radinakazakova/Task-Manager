#pragma once
#include "Vector.hpp"
#include "User.h"
#include "MyString.h"

class UserRepository {
private:
    Vector<User*> users;

public:
    UserRepository() = default;

    ~UserRepository() {
        size_t size = users.getSize();
        for (size_t i = 0; i < size; ++i) {
            delete users[i];
        }
    }

    UserRepository(const UserRepository&) = delete;
    UserRepository& operator=(const UserRepository&) = delete;

    void addUser(User* user) {
        users.pushBack(user);
    }

    User* findByUsername(const MyString& username) const {
        size_t size = users.getSize();

        for (size_t i = 0; i < size; ++i) {
            if (users[i]->getUsername() == username) {
                return users[i];
            }
        }

        return nullptr;
    }

    const Vector<User*>& getAllUsers() const {
        return users;
    }
};