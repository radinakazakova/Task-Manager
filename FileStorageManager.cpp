#include "FileStorageManager.h"

FileStorageManager::FileStorageManager(const char* path) : filePath(path) {}

void FileStorageManager::writeString(std::ofstream& ofs, const MyString& str) {
    int len = str.getLength();
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));

    if (len > 0) {
        ofs.write(str.c_str(), len);
    }
}

MyString FileStorageManager::readString(std::ifstream& ifs) {
    int len = 0;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len));

    if (len == 0)
    {
	    return MyString("");
    }

    char* buffer = new char[len + 1];
    ifs.read(buffer, len);
    buffer[len] = '\0';

    MyString result(buffer);
    delete[] buffer;

    return result;
}

void FileStorageManager::saveTask(std::ofstream& ofs, const Task* task) const {
    int id = task->getId();
    ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

    if (id != -1) {
        writeString(ofs, task->getName());

        bool dateHasValue = task->hasDate();
        ofs.write(reinterpret_cast<const char*>(&dateHasValue), sizeof(dateHasValue));
        if (dateHasValue) {
            ofs.write(reinterpret_cast<const char*>(&task->getDueDate()), sizeof(std::tm));
        }

        int castStatus = static_cast<int>(task->getStatus());
        ofs.write(reinterpret_cast<const char*>(&castStatus), sizeof(castStatus));

        writeString(ofs, task->getDescription());
    }
}

Task* FileStorageManager::loadTask(std::ifstream& ifs) const {
    int id;
    ifs.read(reinterpret_cast<char*>(&id), sizeof(id));

    if (id != -1) {
        MyString name = readString(ifs);

        bool dateHasValue = false;
        ifs.read(reinterpret_cast<char*>(&dateHasValue), sizeof(dateHasValue));

        Optional<std::tm> due_date;
        if (dateHasValue) {
            std::tm date;
            ifs.read(reinterpret_cast<char*>(&date), sizeof(std::tm));
            due_date.setValue(date);
        }

        int castStatus = 0;
        ifs.read(reinterpret_cast<char*>(&castStatus), sizeof(castStatus));
        TaskStatus status = static_cast<TaskStatus>(castStatus);

        MyString description = readString(ifs);

        return new Task(id, name, due_date, status, description);
    }

    Task* deletedTask = new Task();
    deletedTask->setInvalidId();
    return deletedTask;
}

void FileStorageManager::saveUser(std::ofstream& ofs, const User* user) const {
    writeString(ofs, user->getUsername());
    writeString(ofs, user->getPassword());

    const auto& tasks = user->getTasks();
    int tasksCount = static_cast<int>(tasks.getSize());
    ofs.write(reinterpret_cast<const char*>(&tasksCount), sizeof(tasksCount));
    for (int i = 0; i < tasksCount; i++) {
        int taskId = tasks[i]->getId();
        ofs.write(reinterpret_cast<const char*>(&taskId), sizeof(taskId));
    }

    const auto& dashboard = user->getDashboard();
    int dashboardCount = static_cast<int>(dashboard.getSize());
    ofs.write(reinterpret_cast<const char*>(&dashboardCount), sizeof(dashboardCount));
    for (int i = 0; i < dashboardCount; i++) {
        int taskId = dashboard[i]->getId();
        ofs.write(reinterpret_cast<const char*>(&taskId), sizeof(taskId));
    }
}

User* FileStorageManager::loadUser(std::ifstream& ifs, Session& context) const {
    MyString username = readString(ifs);
    MyString password = readString(ifs);

    User* newUser = new User(username, password);

    int tasksCount;
    ifs.read(reinterpret_cast<char*>(&tasksCount), sizeof(tasksCount));
    for (int i = 0; i < tasksCount; i++) {
        int taskId;
        ifs.read(reinterpret_cast<char*>(&taskId), sizeof(taskId));

        Task* task = context.getTaskRepo().findById(taskId);
        if (task && task->getId() != -1) {
            newUser->restoreTask(task);
        }
    }

    int dashboardCount;
    ifs.read(reinterpret_cast<char*>(&dashboardCount), sizeof(dashboardCount));
    for (int i = 0; i < dashboardCount; i++) {
        int taskId;
        ifs.read(reinterpret_cast<char*>(&taskId), sizeof(taskId));

        Task* task = context.getTaskRepo().findById(taskId);
        if (task && task->getId() != -1) {
            newUser->restoreTaskToDashboard(task);
        }
    }
    return newUser;
}

void FileStorageManager::saveCollaboration(std::ofstream& ofs, const Collaboration* collab) const {
    int id = collab->getId();
    ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

    if (id != -1) {
        writeString(ofs, collab->getName());
        writeString(ofs, collab->getCreator()->getUsername());

        const auto& tasks = collab->getTasks();
        int tasksSize = static_cast<int>(tasks.getSize());
        ofs.write(reinterpret_cast<const char*>(&tasksSize), sizeof(tasksSize));

        for (int i = 0; i < tasksSize; ++i) {
            int taskId = tasks[i].getFirst()->getId();
            MyString assigneeName = tasks[i].getSecond()->getUsername();

            ofs.write(reinterpret_cast<const char*>(&taskId), sizeof(taskId));
            writeString(ofs, assigneeName);
        }

        const auto& users = collab->getUsers();
        int usersSize = static_cast<int>(users.getSize());
        ofs.write(reinterpret_cast<const char*>(&usersSize), sizeof(usersSize));

        for (int i = 0; i < usersSize; i++) {
            writeString(ofs, users[i]->getUsername());
        }
    }
}

Collaboration* FileStorageManager::loadCollaboration(std::ifstream& ifs, Session& context) const {
    int id;
    ifs.read(reinterpret_cast<char*>(&id), sizeof(id));

    if (id != -1) {
        MyString name = readString(ifs);
        MyString creatorName = readString(ifs);

        User* creator = context.getUserRepository().findByUsername(creatorName);
        Collaboration* newCollab = new Collaboration(id, name, creator);

        int tasksCount;
        ifs.read(reinterpret_cast<char*>(&tasksCount), sizeof(tasksCount));
        for (int i = 0; i < tasksCount; i++) {
            int taskId;
            ifs.read(reinterpret_cast<char*>(&taskId), sizeof(taskId));
            MyString assigneeName = readString(ifs);

            Task* task = context.getTaskRepository().findById(taskId);
            User* assignee = context.getUserRepository().findByUsername(assigneeName);
            
        	newCollab->restoreTaskAssignment(assignee, task);
        }

        int usersCount;
        ifs.read(reinterpret_cast<char*>(&usersCount), sizeof(usersCount));
        for (int i = 0; i < usersCount; i++) {
            MyString username = readString(ifs);

            User* user = context.getUserRepository().findByUsername(username);
            if (user && user->getUsername() != creatorName) {
                newCollab->restoreUser(user);
            }
        }
        return newCollab;
    }

    Collaboration* deletedCollab = new Collaboration();
    deletedCollab->setInvalidId();
    return deletedCollab;
}

void FileStorageManager::save(const Session& context) const {
    std::ofstream ofs(filePath, std::ios::binary);
    if (!ofs.is_open())
    {
	    throw std::exception("Cannot open file for saving");
    }

    const auto& tasks = context.getTaskRepo().getAllTasks();
    int tasksCount = static_cast<int>(tasks.getSize());
    ofs.write(reinterpret_cast<const char*>(&tasksCount), sizeof(tasksCount));
    for (int i = 0; i < tasksCount; i++)
    {
	    saveTask(ofs, tasks[i]);
    }

    const auto& users = context.getUserRepository().getAllUsers();
    int usersCount = static_cast<int>(users.getSize());
    ofs.write(reinterpret_cast<const char*>(&usersCount), sizeof(usersCount));
    for (int i = 0; i < usersCount; i++)
    {
	    saveUser(ofs, users[i]);
    }

    const auto& collabs = context.getCollaborationRepository().getAllCollaborations();
    int collabsCount = static_cast<int>(collabs.getSize());
    ofs.write(reinterpret_cast<const char*>(&collabsCount), sizeof(collabsCount));
    for (int i = 0; i < collabsCount; i++)
    {
	    saveCollaboration(ofs, collabs[i]);
    }
}

void FileStorageManager::load(Session& context) const {
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs.is_open())
    {
	    return;
    }

    int tasksCount = 0;
    if (ifs.read(reinterpret_cast<char*>(&tasksCount), sizeof(tasksCount))) {
        for (int i = 0; i < tasksCount; i++) {
            context.getTaskRepository().addTask(loadTask(ifs));
        }

        int usersCount = 0;
        ifs.read(reinterpret_cast<char*>(&usersCount), sizeof(usersCount));
        for (int i = 0; i < usersCount; i++) {
            context.getUserRepository().addUser(loadUser(ifs, context));
        }

        int collabsCount = 0;
        ifs.read(reinterpret_cast<char*>(&collabsCount), sizeof(collabsCount));
        for (int i = 0; i < collabsCount; i++) {
            context.getCollaborationRepository().addCollaboration(loadCollaboration(ifs, context));
        }
    }
}