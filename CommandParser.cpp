#include "CommandParser.h"
#include <stdexcept>
#include <iomanip>

namespace
{
	constexpr int MAX_SIZE = 100;
	constexpr char dateFormat[] = "%Y-%m-%d %H:%M:%S";

	bool isNumber(char ch)
	{
		return ch >= '0' && ch <= '9';
	}

	int charToInt(const char* str)
	{
		if (str == nullptr || *str == '\0')
		{
			throw std::invalid_argument("Expected a number, but input was empty.");
		}

		int result = 0;
		const char* ptr = str;

		while (*ptr != '\0')
		{
			if (*ptr < '0' || *ptr > '9')
			{
				throw std::invalid_argument("Input string contains non-digit characters.");
			}

			int digit = *ptr - '0';
			result = result * 10 + digit;
			++ptr;
		}

		return result;
	}

	using CommandCreator = Command * (*)(Session&, std::stringstream&);

	Command* createRegister(Session& ctx, std::stringstream& ss) {
		MyString username, password;
		ss >> username >> password;
		return new RegisterCommand(ctx, username, password);
	}

	Command* createLogin(Session& ctx, std::stringstream& ss) {
		MyString username, password;
		ss >> username >> password;
		return new LoginCommand(ctx, username, password);
	}

	Command* createLogout(Session& ctx, std::stringstream& ss) {
		return new LogoutCommand(ctx);
	}

	Command* createAddTask(Session& ctx, std::stringstream& ss) {
		MyString name;
		char nextArg[MAX_SIZE];

		ss >> name;
		ss >> std::ws;
		ss.getline(nextArg, MAX_SIZE);

		if (isNumber(nextArg[0]))
		{
			std::tm tm = {};
			std::stringstream sstream(nextArg);

			sstream >> std::get_time(&tm, dateFormat);

			if (!sstream.fail())
			{
				char description[MAX_SIZE];
				sstream >> std::ws;
				sstream.getline(description, MAX_SIZE);

				return new AddTaskCommand(ctx, name, tm, description);
			}
			else
			{
				throw std::invalid_argument("Invalid date format. Expected: YYYY-MM-DD HH:MM:SS");
			}
		}

		return new AddTaskCommand(ctx, name, nextArg);
	}

	Command* createUpdateTaskName(Session& ctx, std::stringstream& ss) {
		int id;
		MyString name;
		ss >> id >> name;
		return new UpdateTaskNameCommand(ctx, id, name);
	}

	Command* createUpdateTaskDescription(Session& ctx, std::stringstream& ss) {
		int id;
		char desc[MAX_SIZE];
		ss >> id;
		ss >> std::ws;
		ss.getline(desc, MAX_SIZE);
		return new UpdateTaskDescriptionCommand(ctx, id, desc);
	}

	Command* createStartTask(Session& ctx, std::stringstream& ss) {
		int id;
		ss >> id;
		return new StartTaskCommand(ctx, id);
	}

	Command* createFinishTask(Session& ctx, std::stringstream& ss) {
		int id;
		ss >> id;
		return new FinishTaskCommand(ctx, id);
	}

	Command* createDeleteTask(Session& ctx, std::stringstream& ss) {
		int id;
		ss >> id;
		return new DeleteTaskCommand(ctx, id);
	}

	Command* createAddTaskToDashboard(Session& ctx, std::stringstream& ss) {
		int id;
		ss >> id;
		return new AddTaskToDashboardCommand(ctx, id);
	}

	Command* createRemoveTaskFromDashboard(Session& ctx, std::stringstream& ss) {
		int id;
		ss >> id;
		return new RemoveTaskFromDashboardCommand(ctx, id);
	}

	Command* createGetTask(Session& ctx, std::stringstream& ss) {
		MyString arg;
		ss >> arg;

		if (isNumber(arg[0])) {
			int id = charToInt(arg.c_str());
			return new GetTaskCommand(ctx, id);
		}
		return new GetTaskCommand(ctx, arg);
	}

	Command* createListTasks(Session& ctx, std::stringstream& ss) {
		ss >> std::ws;
		if (ss.eof()) {
			return new ListTasksCommand(ctx);
		}

		char nextArg[MAX_SIZE];
		ss.getline(nextArg, MAX_SIZE);

		if (isNumber(nextArg[0])) {
			std::tm tm = {};
			std::stringstream sstream(nextArg);
			sstream >> std::get_time(&tm, dateFormat);

			if (!sstream.fail()) {
				return new ListTasksCommand(ctx, tm);
			}
			throw std::invalid_argument("Invalid date format.");
		}

		return new ListTasksCommand(ctx, nextArg);
	}

	Command* createListCompletedTasks(Session& ctx, std::stringstream& ss) {
		return new ListCompletedTasksCommand(ctx);
	}

	Command* createListDashboard(Session& ctx, std::stringstream& ss) {
		return new ListDashboardCommand(ctx);
	}

	Command* createAddCollaboration(Session& ctx, std::stringstream& ss) {
		MyString name;
		ss >> name;
		return new AddCollaborationCommand(ctx, name);
	}

	Command* createDeleteCollaboration(Session& ctx, std::stringstream& ss) {
		MyString name;
		ss >> name;
		return new DeleteCollaborationCommand(ctx, name);
	}

	Command* createListCollaborations(Session& ctx, std::stringstream& ss) {
		return new ListCollaborationsCommand(ctx);
	}

	Command* createAddUser(Session& ctx, std::stringstream& ss) {
		MyString collabName, userName;
		ss >> collabName >> userName;
		return new AddUserCommand(ctx, collabName, userName);
	}

	Command* createAssignTask(Session& ctx, std::stringstream& ss) {
		MyString collabName, username, name;
		char nextArg[MAX_SIZE];

		ss >> collabName >> username >> name;
		ss >> std::ws;
		ss.getline(nextArg, MAX_SIZE);

		if (isNumber(nextArg[0]))
		{
			std::stringstream sstream(nextArg);
			std::tm tm = {};

			sstream >> std::get_time(&tm, dateFormat);

			if (!sstream.fail()) {
				char description[MAX_SIZE];
				sstream >> std::ws;
				sstream.getline(description, MAX_SIZE);

				return new AssignTaskCommand(ctx, collabName, username, name, tm, description);
			}
			throw std::invalid_argument("Invalid due date format.");
		}

		return new AssignTaskCommand(ctx, collabName, username, name, nextArg);
	}

	struct CommandEntry {
		const char* name;
		CommandCreator creator;
	};

	const CommandEntry REGISTRY[] = {
		{"register", createRegister},
		{"login", createLogin},
		{"logout", createLogout},
		{"add-task", createAddTask},
		{"update-task-name", createUpdateTaskName},
		{"update-task-description", createUpdateTaskDescription},
		{"start-task", createStartTask},
		{"finish-task", createFinishTask},
		{"delete-task", createDeleteTask},
		{"add-task-to-dashboard", createAddTaskToDashboard},
		{"remove-task-from-dashboard", createRemoveTaskFromDashboard},
		{"get-task", createGetTask},
		{"list-tasks", createListTasks},
		{"list-completed-tasks", createListCompletedTasks},
		{"list-dashboard", createListDashboard},
		{"add-collaboration", createAddCollaboration},
		{"delete-collaboration", createDeleteCollaboration},
		{"list-collaborations", createListCollaborations},
		{"add-user", createAddUser},
		{"assign-task", createAssignTask}
	};

	constexpr size_t REGISTRY_SIZE = sizeof(REGISTRY) / sizeof(CommandEntry);
}

Command* CommandParser::parse(Session& context, const MyString& commandName, std::stringstream& ss)
{
	for (size_t i = 0; i < REGISTRY_SIZE; ++i) {
		if (commandName == REGISTRY[i].name) {
			return REGISTRY[i].creator(context, ss);
		}
	}

	return nullptr;
}