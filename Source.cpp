#include <iostream>
#include <sstream>
#include "Session.h"
#include "CommandParser.h"
#include "FileStorageManager.h"
#include "Exceptions.h"

int main()
{
    try
    {
        Session context;
        FileStorageManager storage("users.dat");

        storage.load(context);

        constexpr int MAX_SIZE = 100;
        while (true)
        {
            try
            {
                char line[MAX_SIZE];
                std::cout << "Enter command: ";
                std::cin.getline(line, MAX_SIZE);

                std::stringstream ss(line);
                char commandName[MAX_SIZE];
                ss.getline(commandName, MAX_SIZE, ' ');

                if (strcmp(commandName, "exit") == 0) {
                    break;
                }

                Command* command = CommandParser::parse(context, commandName, ss);

                if (command != nullptr) {
                    command->execute();
                    delete command;
                }
                else {
                    std::cout << "Invalid command syntax or unknown command." << std::endl;
                }
            }
            catch (const std::logic_error& e)
            {
                std::cout << "Action Failed: " << e.what() << std::endl;
            }
        }

        storage.save(context);
    }
    catch (const SystemException& e)
    {
        std::cout << "CRITICAL SYSTEM ERROR: " << e.what() << std::endl;
        std::cout << "The application will now safely terminate." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "UNEXPECTED FATAL ERROR: " << e.what() << std::endl;
    }

    return 0;
}