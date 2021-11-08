#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Customer.h"

//Forward declaration

BaseAction::BaseAction(){

}
ActionStatus BaseAction::getStatus() const{}
void BaseAction::complete(){}
void BaseAction::error(std::string errorMsg){}
std::string BaseAction::getErrorMsg() const{}


OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList): trainerId(id), customers(customersList){

}
void OpenTrainer::act(Studio &studio) {}
std::string OpenTrainer::toString() const { }


Order::Order(int id):BaseAction(),trainerId(id){ }
void Order::act(Studio &studio){}
std::string Order::toString() const{}


MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId){}
void MoveCustomer::act(Studio &studio){}
std::string MoveCustomer::toString() const{}


Close::Close(int id): BaseAction(), trainerId(id){}
void Close::act(Studio &studio){}
std::string Close::toString() const{}


CloseAll::CloseAll(): BaseAction(){
}
void CloseAll::act(Studio &studio){}
std::string CloseAll::toString() const{}


PrintWorkoutOptions::PrintWorkoutOptions(): BaseAction(){
}
void PrintWorkoutOptions::act(Studio &studio){}
std::string PrintWorkoutOptions::toString() const{}


PrintTrainerStatus::PrintTrainerStatus(int id): BaseAction(), trainerId(id){

}
void PrintTrainerStatus::act(Studio &studio){}
std::string PrintTrainerStatus::toString() const{}

PrintActionsLog::PrintActionsLog(): BaseAction(){

}
void PrintActionsLog::act(Studio &studio){}
std::string PrintActionsLog::toString() const{}

BackupStudio::BackupStudio(): BaseAction(){

}
void BackupStudio::act(Studio &studio){}
std::string BackupStudio::toString() const{}

RestoreStudio::RestoreStudio(): BaseAction(){

}
void RestoreStudio::act(Studio &studio){}
std::string RestoreStudio::toString() const{}
