#include "AddUserCommand.h"

#include "Exceptions.h"

AddUserCommand::AddUserCommand(Session& context, const MyString& collaborationName, const MyString& username)
	: AuthCommand(context), collaborationName(collaborationName), username(username) {
}

void AddUserCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	UserRepository& userRepo = context.getUserRepository();

	Collaboration* collab = collabRepo.findByName(collaborationName);
	User* targetUser = userRepo.findByUsername(username);

	if (!collab) {
		throw NotFoundException("Collaboration not found.");
	}

	if (!targetUser) {
		throw NotFoundException("User not found.");
	}

	collab->tryAddUser(user, targetUser);
	std::cout << "User added to collaboration!" << std::endl;
}