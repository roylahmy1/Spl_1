#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Studio.h"
#include "../include/Customer.h"

//Forward declaration

BaseAction::BaseAction(){

}
ActionStatus BaseAction::getStatus() const{
    return status;
}
void BaseAction::complete(){

}
void BaseAction::error(std::string errorMsg){

}
std::string BaseAction::getErrorMsg() const{
    return errorMsg;
}



OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList){}
void OpenTrainer::act(Studio &studio) {
    // throw errors
    if (studio.getNumOfTrainers() <= trainerId) {
        error("Workout session does not exist or is already open");
    }
    else if (studio.getTrainer(trainerId)->isOpen()) {
        error("Workout session does not exist or is already open");
    }
    //
    else{
        std::string output="";
        Trainer* trainer = studio.getTrainer(trainerId);
        output+="open"+trainerId;
        for(int i=0; i<customers.size(); i++)
        {
            trainer->addCustomer(customers[i]);
            output+=customers[i]->toString();
        }
        trainer->openTrainer();
    }
}
std::string OpenTrainer::toString() const
{

}


Order::Order(int id):BaseAction(),trainerId(id){ }
void Order::act(Studio &studio){
    // throw errors
    if (studio.getNumOfTrainers() <= trainerId) {
        error("Workout session does not exist or is already open");
    }
    //
    else{

        Trainer* trainer = studio.getTrainer(trainerId);
        std::vector<Customer *> customers = trainer->getCustomers();
        //
        for (int i = 0; i <customers.size(); ++i) {
            std::vector<int> customer_workout_ids = customers[i]->order(studio.getWorkoutOptions());
            trainer->order(customers[i]->getId(), customer_workout_ids, studio.getWorkoutOptions());
        }
        //
    }
}
std::string Order::toString() const{}


MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId){}
void MoveCustomer::act(Studio &studio)
{
    if(studio.getNumOfTrainers()>=srcTrainer || studio.getNumOfTrainers()>=dstTrainer)
    {
        std::cout << "Cannot move customer";
    }
    Trainer* trainersrc = studio.getTrainer(srcTrainer);
    Trainer* trainerdst= studio.getTrainer(dstTrainer);
    if(!trainersrc->isOpen() ||!trainerdst->isOpen())
    {
        std::cout << "Cannot move customer";
    }
    else
    {
        std::vector<Customer *> customers = trainersrc->getCustomers();
        bool find = false;
        for (int i = 0; i < customers.size(); i++)
        {
            if (customers[i]->getId() == id) {
                find = true;
                break;
            }
            if (!find) {
                std::cout << "Cannot move customer";
            }
            if (trainerdst->getCapacity() == trainerdst->getCustomers().size()) {
                std::cout << "Cannot move customer";
            }
            else {
                Customer *remove = trainersrc->getCustomer(id);
                trainersrc->removeCustomer(id);
                trainerdst->addCustomer(remove);
            }
        }
    }
}
std::string MoveCustomer::toString() const{}


Close::Close(int id): BaseAction(), trainerId(id){}
void Close::act(Studio &studio)
{
    if(trainerId>=studio.getNumOfTrainers()){
        std::cout << "Trainer does not exist or is not open";
    }
    Trainer* trainer=studio.getTrainer(trainerId);
    if(!trainer->isOpen())
    {
        std::cout << "Trainer does not exist or is not open";
    }
    else {
        trainer->setSalary();
        for (int i = 0; trainer->getCustomers().size(); i++) {
            trainer->removeCustomer(trainer->getCustomers()[i]->getId());
        }
        trainer->closeTrainer();
        std::cout << "Trainer"+std::to_string(trainerId)+"closed. salary"+std::to_string(trainer->getSalary())+"NIS";

    }
}
std::string Close::toString() const{}


CloseAll::CloseAll(): BaseAction(){
}
void CloseAll::act(Studio &studio){
    Trainer* trainer;
    for(int i=0; i<studio.getNumOfTrainers(); i++)
    {
        for(int j=0; j<studio.getTrainer(i)->getCustomers().size();j++)
        {
            trainer=studio.getTrainer(i);
            trainer->removeCustomer(trainer->getCustomers()[j]->getId());
        }
        std::cout << "Trainer"+std::to_string(i)+"closed. salary"+std::to_string(trainer->getSalary())+"NIS";
    }
}
std::string CloseAll::toString() const{}


PrintWorkoutOptions::PrintWorkoutOptions(): BaseAction(){
}
void PrintWorkoutOptions::act(Studio &studio)
{
    std::vector<Workout> workout=studio.getWorkoutOptions();
    for(int i=0; i<workout.size();i++)
    {
        std::cout<<workout[i].getName()+", "+std::to_string(workout[i].getType())+", "+std::to_string(workout[i].getPrice());
    }
}
std::string PrintWorkoutOptions::toString() const{}


PrintTrainerStatus::PrintTrainerStatus(int id): BaseAction(), trainerId(id){
}
void PrintTrainerStatus::act(Studio &studio)
{
    Trainer* trainer=studio.getTrainer(trainerId);
    if(!trainer->isOpen())
    {
        std::cout<<"Trainer "+std::to_string(trainerId)+"status: closed";
    }
    else
    {
        std::cout<<"Trainer "+std::to_string(trainerId)+"status: open";
        std::cout<<"Customers:";
        for(int i=0; i<trainer->getCustomers().size(); i++)
        {
            Customer* customer=trainer->getCustomers()[i];
            std::cout<<std::to_string(customer->getId())+customer->getName();
        }
        std::cout<<"Orders:";
        for(int i=0; i<trainer->getOrders().size();i++)
        {
            std::cout<<trainer->getOrders()[i].second.getName()+" "+std::to_string(trainer->getOrders()[i].second.getPrice())+"NIS "+std::to_string(trainer->getOrders()[i].first);
        }
        std::cout<<"Current Trainer's salary:"+std::to_string(trainer->getSalary());
    }
}
std::string PrintTrainerStatus::toString() const{}

PrintActionsLog::PrintActionsLog(): BaseAction(){

}
void PrintActionsLog::act(Studio &studio){
    //i coudnt finish
}
std::string PrintActionsLog::toString() const{}

BackupStudio::BackupStudio(): BaseAction(){

}
void BackupStudio::act(Studio &studio){}
std::string BackupStudio::toString() const{}

RestoreStudio::RestoreStudio(): BaseAction(){

}
void RestoreStudio::act(Studio &studio){}
std::string RestoreStudio::toString() const{}
