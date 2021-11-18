#include "../include/Studio.h"
#include <iostream>

using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv){


    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();

    // main loop for commands
    std::string command = "";
    std::string commandName = "";
    std::string commandArgs = "";
    while(command.compare("closeall")) {
        std::getline(std::cin, command);
        commandName = command.substr(0, command.find(' '));
        commandArgs = command.substr(command.find(' ') + 1,command.size() - command.find(' ') - 1);

        if (!commandName.compare("open")) {
            cout << "it is open!!\n";
            studio.getOpenAction(commandArgs)->act(studio);
        }
        if (!commandName.compare("order")) {
            cout << "it is order!!\n";
            studio.getOrderAction(commandArgs)->act(studio);
        }
        if (!commandName.compare("status")) {
            cout << "it is status!!\n";
            studio.getOrderAction(commandArgs)->act(studio);
        }
        if (!commandName.compare("move")) {
            cout << "it is move!!\n";
        }
        if (!commandName.compare("close")) {
            cout << "it is close!!\n";
        }
        if (!commandName.compare("backup")) {
            cout << "it is backup!!\n";
        }
        if (!commandName.compare("restore")) {
            cout << "it is restore!!\n";
        }
        cout << "\n";
        cout << "args: " + commandArgs;
        cout << "\n";
    }
    cout << "it is close!!";

    // clean
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}