#include "../include/Customer.h"
#include <vector>
#include <string>
#include <algorithm>
#include "../include/Workout.h"

Customer::Customer(std::string c_name, int c_id) : id(c_id), name(c_name) {

};

std::string Customer::getName() const {
    return name;
};

int Customer::getId() const {
    return id;
};


SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {

};

std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> result;
    for (int i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == CARDIO)
            result.push_back(workout_options[i].getId());

    }
    return result;
};

std::string SweatyCustomer::toString() const {
    std::string s = getName() + "swt";
    return s;
};

CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id) {

};

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> result;
    int max = -1;
    for (int i = 0; i < workout_options.size(); i++) {
        if (max < workout_options[i].getPrice()) {
            max = workout_options[i].getPrice();
            if (result.size() == 0)
                result.push_back(workout_options[i].getId());
            else {
                result.pop_back();
                result.push_back(workout_options[i].getId());
            }
        }
        if (max == workout_options[i].getPrice()) {
            if (result[0] > workout_options[i].getId()) {
                result.pop_back();
                result.push_back(workout_options[i].getId());
            }
        }

    }
    return result;
};

std::string CheapCustomer::toString() const {
    std::string s = getName() + "chp";
    return s;
};

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {

};

std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<const Workout*> onlyAnarobic;
    std::vector<int> result;
    for (int i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == ANAEROBIC) {
            onlyAnarobic.push_back(&workout_options[i]);
        }
    }
    std::sort(onlyAnarobic.begin(), onlyAnarobic.end(), comparing); //need to implement rule of 5 before it will work
    for(int i=0; i<onlyAnarobic.size(); i++)
    {
        result.push_back(onlyAnarobic[i]->getId());
    }
    return result;
};

std::string HeavyMuscleCustomer::toString() const {
    std::string s = getName() + "mcl";
    return s;
};

bool HeavyMuscleCustomer::comparing(const Workout &a, const Workout &b) {
    return (b.getPrice() <= a.getPrice());
};

FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {

};

std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    int cheapcardio = -1;
    int expensivemixtype = -1;
    int cheapanaerobic = -1;
    std::vector<int> result;
    for (int i = 0; i < workout_options.size(); i++) {

        if (workout_options[i].getType() == CARDIO) {
            if (cheapcardio < 0 || cheapcardio > workout_options[i].getPrice()) {
                if (result.size() == 1) { result.pop_back(); }
                result.push_back(workout_options[i].getId());
                cheapcardio = workout_options[i].getPrice();

            }
            if (cheapcardio == workout_options[i].getPrice()) {
                if (result[0] > workout_options[i].getId()) {
                    result.pop_back();
                    result.push_back(workout_options[i].getId());
                }
            }

        }
    }
    for (int i = 0; i < workout_options.size(); i++) {

        if (workout_options[i].getType() == MIXED) {
            if (expensivemixtype < 0 || expensivemixtype < workout_options[i].getPrice()) {
                if (result.size() == 2) { result.pop_back(); }
                result.push_back(workout_options[i].getId());
                expensivemixtype = workout_options[i].getPrice();

            }
            if (expensivemixtype == workout_options[i].getPrice()) {
                if (result[1] > workout_options[i].getId()) {
                    result.pop_back();
                    result.push_back(workout_options[i].getId());
                }
            }
        }
    }
    for (int i = 0; i < workout_options.size(); i++) {

        if (workout_options[i].getType() == ANAEROBIC) {
            if (cheapanaerobic < 0 || cheapanaerobic > workout_options[i].getPrice()) {
                if (result.size() == 3) { result.pop_back(); }
                result.push_back(workout_options[i].getId());
                cheapanaerobic = workout_options[i].getPrice();

            }
            if (cheapanaerobic == workout_options[i].getPrice()) {
                if (result[2] > workout_options[i].getId()) {
                    result.pop_back();
                    result.push_back(workout_options[i].getId());
                }
            }
        }
    }
    return result;
};

std::string FullBodyCustomer::toString() const {
    std::string s = getName() + "fbd";
    return s;
};
