#include "../include/Workout.h"
#include <string>

Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id), name(w_name), price(w_price), type(w_type)
{

};
int Workout::getId() const {  };
std::string Workout::getName() const {  };
int Workout::getPrice() const {  };
WorkoutType Workout::getType() const {  };