#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"

// IMPLEMENT RULE OF 5 !!
class Studio{		
public:
	Studio();
    // read config file and convert to Trainers\workout_options\actionsLog vectors
    Studio(const std::string &configFilePath);
    virtual ~Studio();
    Studio(const Studio &other);
    Studio(Studio &&other);
    Studio& operator=(const Studio &other);
    Studio& operator=(Studio &&other);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    int stringToInt(const std::string &str);
    int getCustomerCounter();
    void setCustomerCounter(int c);
    // get actions
    BaseAction* getOpenAction(std::string &command);
    BaseAction* getOrderAction(std::string &command);
    BaseAction* getMoveAction(std::string &command);
    BaseAction* getCloseAction(std::string &command);
    BaseAction* getCloseAllAction();
    BaseAction* getPrintWorkoutOptionsAction();
    BaseAction* getStatusAction(std::string &command);
    BaseAction* getLogAction();
    BaseAction* getBackupAction();
    BaseAction* getRestoreAction();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    // our functions
    std::string trim(std::string str);
    std::string rtrim(std::string str);
    std::string ltrim(std::string str);
    static std::vector<std::string> split(const std::string& s, const std::string& delimiter);
    void clean();
    // customer counter for id management - every time new customers their id will be set using this counter;
    int customerCounter = 0;
};

#endif