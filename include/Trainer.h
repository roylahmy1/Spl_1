#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

// IMPLEMENT RULE OF 5 !!
class Trainer{
public:
    Trainer(int t_capacity);
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
    void setSalary();
    std::vector<OrderPair> getOrders(int id);
    void insert(std::vector<OrderPair> pairs);

    /// Rule of 5
    virtual ~Trainer();
    Trainer(const Trainer &other);
    Trainer(Trainer &&other);
    Trainer& operator=(const Trainer &other);
    Trainer& operator=(Trainer &&other);

private:
    int oldsalary;
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    // our methods
    int getCustomerIndex(int id);
    void clean();
    void reset();
    void copy(const Trainer &other);

};


#endif