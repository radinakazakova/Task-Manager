#include "Task.h"
#include <iomanip>

int Task::uniqueNumber = -1;

void Task::printStatus(const TaskStatus& stat) const
{
	switch (stat)
	{
	case TaskStatus::ON_HOLD: 
		std::cout << "ON HOLD";
		break;

	case TaskStatus::DONE:
		std::cout << "DONE";
		break;

	case TaskStatus::IN_PROCESS: 
		std::cout << "IN PROCESS";
		break;

	case TaskStatus::OVERDUE: 
		std::cout << "OVERDUE";
		break;
	}
}

Task::Task()
{
	uniqueNumber++;
	id = uniqueNumber;
}

Task::Task(const MyString& name,const std::tm& due_date, const MyString& description, int index): index(index), name(name), description(description)
{
	uniqueNumber++;
	id = uniqueNumber;
	this->due_date.setValue(due_date);
}

Task::Task(const MyString& name, const MyString& description, int index): index(index), name(name) , description(description) //Optional default -> value = nullptr
{
	uniqueNumber++;
	id = uniqueNumber;
} 

//Task* Task::clone() const
//{
//	return new Task(*this);
//}

void Task::changeName(const MyString& name)
{
	this->name = name;
}

void Task::changeStatus(const TaskStatus& newStatus)
{
	status = newStatus;
}

void Task::changeDescription(const MyString& newDesc)
{
	description = newDesc;
}

void Task::printTask() const
{
	std::cout << "Task name: " << name << std::endl;
	std::cout << "Task ID: " << id << std::endl;
	std::cout << "Due date: " << std::put_time(&due_date.getValue(), "%Y-%m-%d %H:%M:%S") << std::endl;
	std::cout << "Status: ";
	printStatus(status);
	std::cout << "\n";
	std::cout << "Task desc: " << description << std::endl;

}

void Task::serialize(std::ofstream& ofs) const
{
	ofs.write(reinterpret_cast<const char*>(&index), sizeof(index));
	ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

	int nameSize = name.getLength();
	ofs.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
	ofs.write(name.c_str(), nameSize);

	bool dateHasValue;
	if(due_date.hasValue())
	{
		dateHasValue = true;

		ofs.write(reinterpret_cast<const char*>(&dateHasValue), sizeof(dateHasValue));
		ofs.write(reinterpret_cast<const char*>(&due_date.getValue()), sizeof(std::tm));
	}
	else
	{
		dateHasValue = false;
		ofs.write(reinterpret_cast<const char*>(&dateHasValue), sizeof(dateHasValue));
	}

	int castStatus = status;
	ofs.write(reinterpret_cast<const char*>(&castStatus), sizeof(castStatus));

	int descSize = description.getLength();
	ofs.write(reinterpret_cast<const char*>(&descSize), sizeof(descSize));
	ofs.write(description.c_str(), descSize);
}

void Task::deserialize(std::ifstream& ifs)
{
	ifs.read(reinterpret_cast<char*>(&index), sizeof(index));
	ifs.read(reinterpret_cast<char*>(&id), sizeof(id));

	int nameSize;
	ifs.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
	MyString tempName(nameSize);
	ifs.read(&tempName[0], nameSize);
	name = tempName;

	bool dateHasValue;
	ifs.read(reinterpret_cast<char*>(&dateHasValue), sizeof(dateHasValue));

	if(dateHasValue)
	{
		std::tm date;
		ifs.read(reinterpret_cast<char*>(&date), sizeof(std::tm));
		due_date.setValue(date);
	}
	//else default Optional will be nullptr

	int castStatus;
	ifs.read(reinterpret_cast<char*>(&castStatus), sizeof(castStatus));
	status = static_cast<TaskStatus>(castStatus);

	int descSize;
	ifs.read(reinterpret_cast<char*>(&descSize), sizeof(descSize));
	MyString tempDesc(descSize);
	ifs.read(&tempDesc[0], descSize);
	description = tempDesc;
}

int Task::getId() const
{
	return id;
}

int Task::getIndex() const
{
	return index;
}

const TaskStatus& Task::getStatus() const
{
	return status;
}

const std::tm& Task::getDueDate() const
{
	return due_date.getValue();
}

const MyString& Task::getName() const
{
	return name;
}
