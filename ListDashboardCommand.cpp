#include "ListDashboardCommand.h"
#include "CommandUtils.h"

ListDashboardCommand::ListDashboardCommand(Session& context) : AuthCommand(context) {}

void ListDashboardCommand::executeWithUser(User* user) {
	const auto& dashboard = user->getDashboard();

	for (size_t i = 0; i < dashboard.getSize(); i++) {
		if (dashboard[i]->getId() != -1)
		{
			CommandUtils::printTaskToConsole(dashboard[i]);
		}
	}
}