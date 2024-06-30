#include "RegisterCommand.h"

#include "Exceptions.h"

RegisterCommand::RegisterCommand(Session& context, const MyString& username, const MyString& password)
	: Command(context), username(username), password(password) {
}

void RegisterCommand::execute() {
	if (context.getLoggedInUser() != nullptr) {
		throw AuthException("You are already logged in. Please logout first to register a new account.");
	}

	if (username.getLength() == 0 || password.getLength() == 0) {
		throw InvalidStateException("Please input password and/or username.");
	}

	UserRepository& userRepo = context.getUserRepository();

	if (userRepo.findByUsername(username) == nullptr) {
		User* newUser = new User(username, password);
		userRepo.addUser(newUser);
		std::cout << "Successfully created account!" << std::endl;
	}
	else {
		throw DuplicateException("Username already taken.");
	}
}