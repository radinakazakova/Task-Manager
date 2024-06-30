#include "AuthCommand.h"
#include "Exceptions.h"

AuthCommand::AuthCommand(Session& context) : Command(context) {}

void AuthCommand::execute() {
	User* user = context.getLoggedInUser();

	if (!user)
	{
		throw AuthException("You are not logged in.");
	}

	executeWithUser(user);
}