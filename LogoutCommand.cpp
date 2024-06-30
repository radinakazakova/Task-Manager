#include "LogoutCommand.h"
LogoutCommand::LogoutCommand(Session& context) : AuthCommand(context) {}

void LogoutCommand::executeWithUser(User* user) {
	context.logout();
	std::cout << "Logged out successfully!" << std::endl;
}