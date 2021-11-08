#include "../include/Studio.h"
#include <vector>
#include <string>
#include "../include/Workout.h"
#include "../include/Trainer.h"
#include "../include/Action.h"

Studio::Studio() {

}
Studio::Studio(const std::string &configFilePath){

}
void Studio::start(){

}
int Studio::getNumOfTrainers() const{

}
Trainer* Studio::getTrainer(int tid){

}
const std::vector<BaseAction*>& Studio::getActionsLog() const {

} // Return a reference to the history of actions
std::vector<Workout>& Studio::getWorkoutOptions() {

}
