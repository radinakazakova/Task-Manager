#include "ListCollaborationsCommand.h"

ListCollaborationsCommand::ListCollaborationsCommand(Session& context) : AuthCommand(context) {}

void ListCollaborationsCommand::executeWithUser(User* user) {
	CollaborationRepository& collabRepo = context.getCollaborationRepository();
	const auto& collabs = collabRepo.getAllCollaborations();

	for (size_t i = 0; i < collabs.getSize(); i++) {
		if (collabs[i]->getId() != -1 && collabs[i]->hasUser(user)) {
			std::cout << collabs[i]->getName() << std::endl;
		}
	}
}