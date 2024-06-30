#pragma once
#include "Session.h"
#include "MyString.h"
#include <fstream>
#include <exception>

class FileStorageManager {
private:
    const char* filePath;

    static void writeString(std::ofstream& ofs, const MyString& str);
    static MyString readString(std::ifstream& ifs);

    void saveTask(std::ofstream& ofs, const Task* task) const;
    Task* loadTask(std::ifstream& ifs) const;

    void saveUser(std::ofstream& ofs, const User* user) const;
    User* loadUser(std::ifstream& ifs, Session& context) const;

    void saveCollaboration(std::ofstream& ofs, const Collaboration* collaboration) const;
    Collaboration* loadCollaboration(std::ifstream& ifs, Session& context) const;

public:
    FileStorageManager(const char* path = "users.dat");

    void save(const Session& context) const;
    void load(Session& context) const;
};