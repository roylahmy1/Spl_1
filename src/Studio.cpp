#include "../include/Studio.h"
#include <vector>
#include <string>
#include <fstream>

Studio::Studio() {
    // default values
    open = false;
}
Studio::Studio(const std::string &configFilePath){
    // default values
    open = false;

    // read text file
    const char comment = '#';
    std::string line;
    std::ifstream conFile(configFilePath);

    // create vector of the 3 properties data
    std::vector<std::string> properties;

    // loop lines and separate them to correct property
    while (getline (conFile, line)) {
        // insert line if not empty or comment
        if(!line.empty() && line.find(comment) == std::string::npos){
            properties.push_back(line);
        }
    }

    /// parse vector to properties

    // create trainers
    int numberOfTrainers = stringToInt(properties[0]); // not dynamic_cast since we want exception if null
    std::string capacityOfTrainers = properties[1]; // not dynamic_cast since we want exception if null
    for (int i = 0; i < numberOfTrainers; ++i) {
        // get the capacity
        int separatorIndex = capacityOfTrainers.find(',');
        int capacityOfTrainer = 0;
        if (separatorIndex != std::string::npos) {
            capacityOfTrainer = stringToInt(capacityOfTrainers.substr(0, separatorIndex - 1));
            capacityOfTrainers = capacityOfTrainers.substr(separatorIndex + 1, capacityOfTrainers.length() - (separatorIndex + 1));
        }
        else{
            capacityOfTrainer = stringToInt(capacityOfTrainers);
        }
        // create trainer with capacity
        trainers.push_back(new Trainer(capacityOfTrainer));
    }

    // create workouts
    for (int i = 2; i < properties.size(); ++i) {

        // 2 of the ',' that separate the values of th3e workout
        int separatorIndex1 = capacityOfTrainers.find(',');
        int separatorIndex2 = capacityOfTrainers.find(',', separatorIndex1 + 1);

        // find name, type and price of each workout
        std::string name = properties[i].substr(0, separatorIndex1);
        std::string str_type = properties[i].substr(separatorIndex1 + 1, separatorIndex2 - (separatorIndex1 + 1));
        int price =  stringToInt(properties[i].substr(separatorIndex2 + 1, properties[i].length() - (separatorIndex2 + 1)));

        // convert type from string to enum
        WorkoutType* enum_type;
        if (!str_type.compare("Anaerobic")) {
            *enum_type = ANAEROBIC;
        }
        else if (!str_type.compare("Cardio")) {
            *enum_type = CARDIO;
        }
        else if (!str_type.compare("Mixed")) {
            *enum_type = MIXED;
        }
        else{
            throw "config syntax error: Invalid type, row " + (i + 1);
        }

        //
        workout_options.push_back(Workout(i - 2, name, price, *enum_type));

        // clean (copies were sent and thus the values are useless)
        delete enum_type;
        enum_type = nullptr;
    }

    // Close the file
    conFile.close();
}
void Studio::start(){
    open = true;
}
int Studio::getNumOfTrainers() const{
    return trainers.size();
}
Trainer* Studio::getTrainer(int tid){
    return trainers[tid];
}
// Return a reference to the history of actions
const std::vector<BaseAction*>& Studio::getActionsLog() const {
    return actionsLog;
}
std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
}

// util functions
int Studio::stringToInt(const std::string &str) {
    return std::stoi(str);
}