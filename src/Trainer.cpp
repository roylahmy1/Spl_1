#include "../include/Trainer.h"
#include <vector>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity) {}
int Trainer::getCapacity() const{
    return capacity;
}
void Trainer::addCustomer(Customer* customer){
    customersList.push_back(customer);
}
void Trainer::removeCustomer(int id) {
    int index =  getCustomerIndex(id);
    Customer* customer = getCustomer(id);
    customersList.erase(customersList.begin() + index);
    // remove orderpair


    delete customer;
}
Customer* Trainer::getCustomer(int id){
    return customersList[getCustomerIndex(id)];
}
void Trainer::setSalary()
{
    oldsalary=getSalary();
}
int Trainer::getCustomerIndex(int id) {
    int counter = 0;
    bool found = false;
    while(counter < customersList.size() && !found){
        if (customersList[counter]->getId() == id){
            found = true;
        }
        counter++;
    }
    return counter - 1;
}
std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
   for(int j=0; j<workout_ids.size(); j++)
   {
       orderList.push_back({customer_id, workout_options[workout_ids[j]]});
       std::cout << getCustomer(customer_id)->getName() + " Is Doing " + workout_options[workout_ids[j]].getName() << "\n";
   }
   //
}
void Trainer::openTrainer(){
    open = true;
}
void Trainer::closeTrainer(){
    open = false;
    // clean customers + clean order pairs
    reset();
    // update salary

}
void Trainer::clean(){
    reset();
}
void Trainer::reset(){
    // clean customers + clean order pairs
    for (int i = customersList.size(); i > 0 ; --i) {
        delete customersList[i];
        customersList.pop_back();
    }
    for (int i = orderList.size(); i > 0 ; --i) {
        orderList.pop_back();
    }
}

int Trainer::getSalary(){
    int salary = 0;
    for (int i = 0; i < orderList.size(); ++i) {
        salary += orderList[i].second.getPrice();
    }
    return salary+oldsalary;
}
bool Trainer::isOpen(){
    return  open;
}