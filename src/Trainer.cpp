#include "../include/Trainer.h"
#include <vector>
#include "../include/Customer.h"
#include "../include/Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity) {}
int Trainer::getCapacity() const{
    return capacity;
}
void Trainer::addCustomer(Customer* customer){
    customersList.push_back(customer);
}
void Trainer::removeCustomer(int id){
    customersList.erase(customersList.begin() + getCustomerIndex(id));
}
Customer* Trainer::getCustomer(int id){
    return customersList[getCustomerIndex(id)];
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
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options){
    //
    for(int i=0; i<workout_options.size(); i++)
    {
        OrderPair *a=new OrderPair(customer_id,workout_options[i]);
      //  orderList.push_back(a);
    }


    //
}
void Trainer::openTrainer(){
    open = true;
}
void Trainer::closeTrainer(){
    open = false;
}
int Trainer::getSalary(){
    int salary = 0;
    for (int i = 0; i < orderList.size(); ++i) {
        salary += orderList[i].second.getPrice();
    }
    return salary;
}
bool Trainer::isOpen(){
    return  open;
}