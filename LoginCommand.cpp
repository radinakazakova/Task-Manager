#include "LoginCommand.h"
#include "Exceptions.h"
#include <iostream>
#include <ctime>

LoginCommand::LoginCommand(Session& context, const MyString& username, const MyString& password)
	: Command(context), username(username), password(password) {
}

void LoginCommand::execute() {
	if (context.getLoggedInUser() != nullptr) {
		throw AuthException("You are already logged in. Please logout first to log into another account.");
	}

	UserRepository& userRepo = context.getUserRepository();
	User* user = userRepo.findByUsername(username);

	if (user != nullptr && user->checkPassword(password)) {
		std::time_t currTime = std::time(nullptr);
		std::tm loggedInTime;
		localtime_s(&loggedInTime, &currTime);

		context.login(user, loggedInTime);
		user->syncTasksAndDashboard(loggedInTime);

		std::cout << "Login successful" << std::endl;
	}
	else {
		throw AuthException("Login unsuccessful. Invalid credentials.");
	}
}