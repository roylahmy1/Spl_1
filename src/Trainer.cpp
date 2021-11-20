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
    // remove orderpair by creating copy, and then removing original
    std::vector<OrderPair> orderListCopy;
    int counter=getOrders().size();
    for(int i=0; i<counter; i++)
    {
        if(orderList[i].first != id)
        {
            orderListCopy.push_back(orderList[i]);
        }
    }
    // overwrite original orderList
    orderList.clear();
    for (int i = 0; i <orderListCopy.size(); ++i) {
        orderList.push_back(orderListCopy[i]);
    }
    // don't delete the customer as it may be copied to other trainer
    // delete customer;
}



Customer* Trainer::getCustomer(int id){
    int index = getCustomerIndex(id);
    if(customersList.size() > index && index >= 0){
        return customersList[index];
    }
    return nullptr;
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
    if(!found){
        return -1;
    }
    return counter - 1;
}
std::vector<Customer*>& Trainer::getCustomers(){
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders(){
    return orderList;
}
// get orders by customer id
std::vector<OrderPair> Trainer::getOrders(int id){
    std::vector<OrderPair> pairs;
    for (int i = 0; i < orderList.size(); ++i) {
        if(orderList[i].first == id){
            pairs.push_back(orderList[i]);
        }
    }
    return pairs;
}
void Trainer::insert(std::vector<OrderPair> pairs){
    for (int i = 0; i < pairs.size(); ++i) {
        orderList.push_back(pairs[i]);
    }
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
    for (int i = customersList.size() - 1; i >= 0 ; --i) {
        delete customersList[i];
        customersList.pop_back();
    }
    orderList.clear();
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