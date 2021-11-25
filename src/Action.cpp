#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Studio.h"
#include "../include/Customer.h"

extern Studio* backup;
using namespace std;

//Forward declaration

BaseAction::BaseAction() {

}
ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::complete() {
    status = COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
    cout << errorMsg;
}
// copy this baseAction values into another
BaseAction* BaseAction::baseCopy(BaseAction* copyTo) {
    copyTo->status = status;
    copyTo->errorMsg = errorMsg;
    return  copyTo;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
std::string BaseAction::statusToString() const {
    if (status == ERROR){
        return "Error: " + errorMsg;
    }
    else if (status == COMPLETED){
        return "Completed";
    }
    else{
        throw "UinitValue";
    }
}
BaseAction::~BaseAction() {} // virtual destructor so compiler doesn't create it's own default static bind destructor

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id) {
    for (std::size_t i = 0; i < customersList.size(); ++i) {
        customers.push_back(customersList[i]);
    }
}
void OpenTrainer::act(Studio &studio) {
    // throw errors
    if (studio.getNumOfTrainers() <= trainerId) {
        error("Workout session does not exist or is already open");
    } else if (studio.getTrainer(trainerId)->isOpen()) {
        error("Workout session does not exist or is already open");
    } else if (customers.size() > studio.getTrainer(trainerId)->getCapacity()) {
        error("Workout session does not exist or is already open");
    }
    //
    else {
        std::string output = "";
        Trainer *trainer = studio.getTrainer(trainerId);
        output += "open" + trainerId;
        for (std::size_t i = 0; i < customers.size(); i++) {
            // give trainer it's own clones of the customers
            trainer->addCustomer(customers[i]->clone());
            output += customers[i]->toString();
        }
        trainer->openTrainer();
        complete();
    }
}
std::string OpenTrainer::toString() const {
    std::string output = "open " + std::to_string(trainerId) + " ";
    for (std::size_t i = 0; i < customers.size(); ++i) {
        output += customers[i]->toString() + " ";
    }
    return output;
}
BaseAction* OpenTrainer::clone() {
    std::vector<Customer *> customersCopy;
    for (std::size_t i = 0; i < customers.size(); ++i) {
        customersCopy.push_back(customers[i]->clone());
    }
    // instead of actually using the pointer, deep copy them
    return baseCopy(new OpenTrainer(trainerId, customersCopy));;
}

//// ************************************************************
//// **** RULE OF 5 for OpenTrainer ****
OpenTrainer::~OpenTrainer() {
    clean();
}
OpenTrainer::OpenTrainer(const OpenTrainer &other): trainerId(other.trainerId) {
    copy(other);
}
OpenTrainer &OpenTrainer::operator=(const OpenTrainer &other) {
    if(this != &other){
        clean();
        copy(other);
    }
    return *this;
}
OpenTrainer::OpenTrainer(OpenTrainer &&other): trainerId(other.trainerId) {
    copy(other);
    other.clean();
}
OpenTrainer &OpenTrainer::operator=(OpenTrainer &&other) {
    if(this != &other){
        clean();
        copy(other);
        other.clean();
    }
    return *this;
}

void OpenTrainer::copy(const OpenTrainer &other) {
    // cloning customers, by virtual clone method
    for (std::size_t i = 0; i < other.customers.size(); ++i) {
        customers.push_back(other.customers[i]->clone());
    }
}
void OpenTrainer::clean(){
    // clean customers + clean order pairs
    int size =  customers.size();
    for (int i = size - 1; i >= 0; --i) {
        Customer* customer = customers[i];
        customers.pop_back();
        delete customer;
    }
    customers.clear();
}

//// *****baseCopy(*******************************************************



Order::Order(int id) : BaseAction(), trainerId(id) {}
void Order::act(Studio &studio) {
    // throw errors
    if (studio.getNumOfTrainers() <= trainerId) {
        error("Trainer is not exist or is not open");
    }
    else if(!studio.getTrainer(trainerId)->isOpen())
    {
        error("Trainer is not exist or is not open");
    }
        //
    else {

        Trainer *trainer = studio.getTrainer(trainerId);
        // clear orders to prevent duplicates
        trainer->getOrders().clear();
        std::vector<Customer *> customers = trainer->getCustomers();
        //
        for (std::size_t i = 0; i < customers.size(); ++i) {
            std::vector<int> customer_workout_ids = customers[i]->order(studio.getWorkoutOptions());
            trainer->order(customers[i]->getId(), customer_workout_ids, studio.getWorkoutOptions());
        }
        //
        complete();
    }
}
std::string Order::toString() const {
    std::string output = "order " + std::to_string(trainerId) + " ";
    return output;
}
BaseAction* Order::clone() {
    return baseCopy(new Order(trainerId));
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId)
        : BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId) {}
void MoveCustomer::act(Studio &studio) {
    if (studio.getNumOfTrainers() <= srcTrainer || studio.getNumOfTrainers() <= dstTrainer) {
        error("Cannot move customer");
    }
    else {
        Trainer *trainersrc = studio.getTrainer(srcTrainer);
        Trainer *trainerdst = studio.getTrainer(dstTrainer);
        if (!trainersrc->isOpen() || !trainerdst->isOpen() || trainerdst->getCustomers().size() >= trainerdst->getCapacity()) {
            error("Cannot move customer");
        } else {
            //  std::vector<Customer *> customers = trainersrc->getCustomers();
            Customer *customer = trainersrc->getCustomer(id);
            if (!customer) {
                error("Cannot move customer");
            }
            else if (trainerdst->getCapacity() == trainerdst->getCustomers().size()) {
                error("Cannot move customer");
            } else {
                Customer *remove = trainersrc->getCustomer(id);
                trainerdst->addCustomer(remove);
                trainerdst->insert(trainersrc->getOrders(id));
                trainersrc->removeCustomer(id);
                complete();
            }
        }
    }
}
std::string MoveCustomer::toString() const {
    std::string output = "move " + std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " ";
    return output;
}
BaseAction* MoveCustomer::clone() {
    return baseCopy(new MoveCustomer(srcTrainer, dstTrainer, id));
}

Close::Close(int id) : BaseAction(), trainerId(id) {}
void Close::act(Studio &studio) {
    if (trainerId >= studio.getNumOfTrainers()) {
        error("Trainer does not exist or is not open");
    }
    else {
        Trainer *trainer = studio.getTrainer(trainerId);
        if (!trainer->isOpen()) {
            error("Trainer does not exist or is not open");
        } else {
            trainer->setSalary();
            trainer->closeTrainer();
            std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " +
                         std::to_string(trainer->getSalary()) +
                         "NIS";
            complete();
        }
    }
}
std::string Close::toString() const {
    std::string output = "close " + std::to_string(trainerId) + " ";
    return output;
}
BaseAction* Close::clone() {
    return baseCopy(new Close(trainerId));
}

CloseAll::CloseAll() : BaseAction() {
}
void CloseAll::act(Studio &studio) {
    for (int i = 0; i < studio.getNumOfTrainers(); i++) {
       Trainer *trainer = studio.getTrainer(i);
       if(trainer->isOpen()){
           std::cout << "Trainer " + std::to_string(i) + " closed. salary " + std::to_string(trainer->getSalary()) + " NIS \n";
       }
    }
    complete();
}
std::string CloseAll::toString() const {
    std::string output = "closeall ";
    return output;
}
BaseAction* CloseAll::clone() {
    return baseCopy(new CloseAll());
}

PrintWorkoutOptions::PrintWorkoutOptions() : BaseAction() {
}
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workout = studio.getWorkoutOptions();
    std::string type;
    for (std::size_t i = 0; i < workout.size(); i++) {
        if(workout[i].getType()==ANAEROBIC){type="Anaerobic";}else if(workout[i].getType()==CARDIO){type="Cardio";}else type="Mixed";
        std::cout << workout[i].getName() + ", " + type + ", " +
                     std::to_string(workout[i].getPrice()) + "\n";
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {
    std::string output = "workout_options ";
    return output;
}
BaseAction* PrintWorkoutOptions::clone() {
    return baseCopy(new PrintWorkoutOptions());
}

PrintTrainerStatus::PrintTrainerStatus(int id) : BaseAction(), trainerId(id) {
}
void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (!trainer->isOpen()) {
        std::cout << "Trainer " + std::to_string(trainerId) + " status: closed";
    } else {
        std::cout << "Trainer " + std::to_string(trainerId) + " status: open \n";
        std::cout << "Customers: \n";
        for (std::size_t i = 0; i < trainer->getCustomers().size(); i++) {
            Customer *customer = trainer->getCustomers()[i];
            std::cout << std::to_string(customer->getId()) + " " + customer->getName() + "\n";
        }
        std::cout << "Orders:\n";
        for (std::size_t i = 0; i < trainer->getOrders().size(); i++) {
            std::cout << trainer->getOrders()[i].second.getName() + " " +
                         std::to_string(trainer->getOrders()[i].second.getPrice()) + "NIS " +
                         std::to_string(trainer->getOrders()[i].first) + "\n";
        }
        std::cout << "Current Trainer's salary: " + std::to_string(trainer->getSalary()) + "NIS\n";
        complete();
    }
}
std::string PrintTrainerStatus::toString() const {
    std::string output = "status " + std::to_string(trainerId) + " ";
    return output;
}
BaseAction* PrintTrainerStatus::clone() {
    return baseCopy(new PrintTrainerStatus(trainerId));
}

PrintActionsLog::PrintActionsLog() : BaseAction() {

}
void PrintActionsLog::act(Studio &studio) {
    for (std::size_t i = 0; i < studio.getActionsLog().size() - 1; ++i) { // don't print last since it is the current action, no need to display it
        BaseAction* action = studio.getActionsLog()[i];
        std::string output = action->toString();
        output += action->statusToString();
        output += "\n";
        cout << output;
    }
    complete();
}
std::string PrintActionsLog::toString() const {
    std::string output = "log ";
    return output;
}
BaseAction* PrintActionsLog::clone() {
    return baseCopy(new PrintActionsLog());
}


BackupStudio::BackupStudio() : BaseAction() {

}
void BackupStudio::act(Studio &studio) {
    backup = new Studio(studio);
    complete();
}
std::string BackupStudio::toString() const {
    std::string output = "backup ";
    return output;
}
BaseAction* BackupStudio::clone() {
    return baseCopy(new BackupStudio());
}

RestoreStudio::RestoreStudio() : BaseAction() {

}
void RestoreStudio::act(Studio &studio) {
    if (backup) {
        studio = *backup;
        complete();
    }
    else{
        error("No backup available");
    }
}
std::string RestoreStudio::toString() const {
    std::string output = "restore ";
    return output;
}
BaseAction* RestoreStudio::clone() {
    return baseCopy(new RestoreStudio());
}
