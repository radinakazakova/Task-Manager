#include "AddCollaborationCommand.h"
#include "Exceptions.h"

AddCollaborationCommand::AddCollaborationCommand(Session& context, const MyString& name) : AuthCommand(context), name(name) {}

void AddCollaborationCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();

	if (collabRepo.findByName(name) != nullptr) {
		throw DuplicateException("A collaboration with this name already exists!");
	}

	Collaboration* newCollab = new Collaboration(name, user);
	collabRepo.addCollaboration(newCollab);

	std::cout << "Collaboration added successfully!" << std::endl;
}