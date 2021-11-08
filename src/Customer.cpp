#include "../include/Customer.h"
#include <vector>
#include <string>
#include "../include/Workout.h"

Customer::Customer(std::string c_name, int c_id):id(c_id), name(c_name){

};
std::string Customer::getName() const{

};
int Customer::getId() const{

};


SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name, id){

};
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options){

};
std::string SweatyCustomer::toString() const{

};

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name, id){

};
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options){

};
std::string CheapCustomer::toString() const{

};

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id){

};
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options){

};
std::string HeavyMuscleCustomer::toString() const{

};

FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name, id){

};
std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options){

};
std::string FullBodyCustomer::toString() const{

};
