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
            capacityOfTrainer = stringToInt(capacityOfTrainers.substr(0, separatorIndex));
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
        int separatorIndex1 = properties[i].find(',');
        int separatorIndex2 = properties[i].find(',', separatorIndex1 + 1);

        // find name, type and price of each workout
        std::string name = trim(properties[i].substr(0, separatorIndex1));
        std::string str_type = trim(properties[i].substr(separatorIndex1 + 1, separatorIndex2 - (separatorIndex1 + 1)));
        int price =  stringToInt(properties[i].substr(separatorIndex2 + 1, properties[i].length() - (separatorIndex2 + 1)));

        // convert type from string to enum
        WorkoutType enum_type;
        if (!str_type.compare("Anaerobic")) {
            enum_type = ANAEROBIC;
        }
        else if (!str_type.compare("Cardio")) {
            enum_type = CARDIO;
        }
        else if (!str_type.compare("Mixed")) {
            enum_type = MIXED;
        }
        else{
            throw "config syntax error: Invalid type, row " + (i + 1);
        }

        //
        workout_options.push_back(Workout(i - 2, name, price, enum_type));

        // clean (copies were sent and thus the values are useless)
        //delete enum_type;
        //enum_type = nullptr;
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
std::string Studio::trim(std::string str) {
    str = rtrim(str);
    str = ltrim(str);
    return str;
}
std::string Studio::rtrim(std::string str) {
    int counter = str.size() - 1;
    while (!str.substr(counter, 1).compare(" ") && str.size() > 0){
        // if equal remove
        str = str.substr(0, counter);
        counter--;
    }
    return str;
}
std::string Studio::ltrim(std::string str) {
    int counter = 1;
    while (!str.substr(counter - 1, 1).compare(" ") && str.size() > 0){
        // if equal remove
        str = str.substr(counter, str.size() - counter);
        counter++;
    }
    return str;
}

int Studio::getCustomerCounter() {
    return customerCounter;
}
void Studio::setCustomerCounter(int c) {
    customerCounter = c;
}
std::vector<std::string> Studio::split(const std::string& s, const std::string& delimiter){
    std::vector<std::string> v;
    int s_pos = 0;
    int e_pos;
    while ((e_pos = s.find(delimiter, s_pos)) != std::string::npos) {
//        std::string segment = ;
//        std::string* segmentP = &segment;
        v.push_back(s.substr(s_pos, e_pos - s_pos));
        s_pos = e_pos + 1;
    }
    // another for the last
    //std::string segment = s.substr(s_pos, s.size() - s_pos);
    //std::string* segmentP = &segment;
    v.push_back(s.substr(s_pos, e_pos - s_pos));

    return v;
}

//// ************************************************************88
//// ************************************************************88
//// ************************************************************88
//// ************************************************************88
// get actions
//// ************************************************************88
//// ************************************************************88
//// ************************************************************88
//// ************************************************************88

BaseAction* Studio::getOpenAction(std::string &command) {
    std::vector<std::string> args = split(command, " ");
    std::vector<Customer*> customers;

    int trainerId = stringToInt(args[0]);
    // create the customers out of the given data
    for (int i = 1; i < args.size(); ++i) {
        // customer data = {name, type}
        std::vector<std::string> customerData = split(args[i], ",");
        std::string name = customerData[0];
        std::string type = customerData[1];
        int id = getCustomerCounter() + i;
        if (!type.compare("swt")){
            customers.push_back(new SweatyCustomer(name, id));
        }
        if (!type.compare("chp")){
            customers.push_back(new CheapCustomer(name, id));
        }
        if (!type.compare("mcl")){
            customers.push_back(new HeavyMuscleCustomer(name, id));
        }
        if (!type.compare("fbd")){
            customers.push_back(new FullBodyCustomer(name, id));
        }

        // clean
//        for (int i = 0; i < customerData.size(); ++i) {
//            delete customerData[i];
//        }
    }
    // start the action
    BaseAction* action = new OpenTrainer(trainerId, customers);
    actionsLog.push_back(action);

    // update the customer counter
    setCustomerCounter(getCustomerCounter() + args.size() - 1);

    // clean
//    for (int i = 0; i < args.size(); ++i) {
//        delete args[i];
//    }

    //
    return action;
}


BaseAction* Studio::getOrderAction(std::string &command) {
    std::vector<std::string> args = split(command, " ");

    int trainerId = stringToInt(args[0]);
    // start the action
    BaseAction* action = new Order(trainerId);
    actionsLog.push_back(action);

    return action;
}

BaseAction* Studio::getMoveAction(std::string &command){
    std::vector<std::string> args = split(command, " ");

    int originTrainerId = stringToInt(args[0]);
    int destTrainerId = stringToInt(args[1]);
    int customerId = stringToInt(args[2]);
    // start the action
    BaseAction* action = new MoveCustomer(originTrainerId, destTrainerId, customerId);
    actionsLog.push_back(action);

    return action;
}
BaseAction* Studio::getCloseAction(std::string &command) {
    std::vector<std::string> args = split(command, " ");

    int trainerId = stringToInt(args[0]);
    // start the action
    BaseAction* action = new Close(trainerId);
    actionsLog.push_back(action);

    return action;
}

BaseAction *Studio::getCloseAllAction() {
    BaseAction* action = new CloseAll();
    actionsLog.push_back(action);
    return action;
}
BaseAction *Studio::getPrintWorkoutOptionsAction() {
    BaseAction* action = new PrintWorkoutOptions();
    actionsLog.push_back(action);
    return action;
}

BaseAction *Studio::getStatusAction(std::string &command) {
    std::vector<std::string> args = split(command, " ");

    int trainerId = stringToInt(args[0]);
    // start the action
    BaseAction* action = new PrintTrainerStatus(trainerId);
    actionsLog.push_back(action);

    return action;
}
BaseAction *Studio::getLogAction() {
    BaseAction* action = new PrintActionsLog();
    actionsLog.push_back(action);
    return action;
}
BaseAction *Studio::getBackupAction() {
    BaseAction* action = new BackupStudio();
    actionsLog.push_back(action);
    return action;
}
BaseAction *Studio::getRestoreAction() {
    BaseAction* action = new RestoreStudio();
    actionsLog.push_back(action);
    return action;
}