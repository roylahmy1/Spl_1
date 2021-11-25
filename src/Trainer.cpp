#include "../include/Trainer.h"
#include <vector>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Workout.h"

Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false), customersList(std::vector<Customer*>()),orderList(std::vector<OrderPair>()) {}
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
            orderListCopy.push_back({orderList[i].first, orderList[i].second});
        }
    }
    // overwrite original orderList
    orderList.clear();
    for (std::size_t i = 0; i <orderListCopy.size(); ++i) {
        orderList.push_back(orderListCopy[i]);
    }
    // don't delete the customer as it may be copied to other trainer
    // delete customer;
}
Customer* Trainer::getCustomer(int id){
    int index = getCustomerIndex(id);
    if((int)customersList.size() > index && index >= 0){
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
    while(counter < (int)customersList.size() && !found){
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
    for (std::size_t i = 0; i < orderList.size(); ++i) {
        if(orderList[i].first == id){
            pairs.push_back(orderList[i]);
        }
    }
    return pairs;
}
void Trainer::insert(std::vector<OrderPair> pairs){
    for (std::size_t i = 0; i < pairs.size(); ++i) {
        orderList.push_back(pairs[i]);
    }
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
{
   for(std::size_t j=0; j<workout_ids.size(); j++)
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
void Trainer::reset(){
    // use reset since is clears customer data, but is not "meant" for the destructor (even though it is the same), thus a different name was given.
    // clean customers + clean order pairs
    for (int i = customersList.size() - 1; i >= 0 ; --i) {
        delete customersList[i];
        customersList.pop_back();
    }
    orderList.clear();
}
int Trainer::getSalary(){
    int salary = 0;
    for (std::size_t i = 0; i < orderList.size(); ++i) {
        salary += orderList[i].second.getPrice();
    }
    return salary+oldsalary;
}
bool Trainer::isOpen(){
    return  open;
}

//// ************************************************************
//// **** RULE OF 5 ****
Trainer::~Trainer() {
    clean();
}
Trainer::Trainer(const Trainer &other): capacity(other.capacity), open(other.open), customersList(std::vector<Customer*>()),orderList(std::vector<OrderPair>()) {
    copy(other);
}
Trainer &Trainer::operator=(const Trainer &other) {
    if(this != &other){
        clean();
        copy(other);
    }
    return *this;
}
Trainer::Trainer(Trainer &&other): capacity(other.capacity), open(other.open), customersList(std::vector<Customer*>()),orderList(std::vector<OrderPair>()){
    copy(other);
    other.clean();
}
Trainer &Trainer::operator=(Trainer &&other) {
    if(this != &other){
        clean();
        copy(other);
        other.clean();
    }
    return *this;
}

void Trainer::copy(const Trainer &other) {
    capacity = other.capacity;
    oldsalary = other.oldsalary;
    open = other.open;
    // cloning customers, by virtual clone method
    for (std::size_t i = 0; i < other.customersList.size(); ++i) {
        customersList.push_back(other.customersList[i]->clone());
    }
    for (std::size_t i = 0; i < other.orderList.size(); ++i) {
        orderList.push_back({other.orderList[i].first, other.orderList[i].second});
    }
}
void Trainer::clean(){
    reset();
}
//// ************************************************************

