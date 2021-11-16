#include "../include/Studio.h"
#include <iostream>

using namespace std;

Studio* backup = nullptr;

int main(int argc, char** argv){

    std::cout << "Test!!";

    if(argc!=2){
        std::cout << "usage: studio <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Studio studio(configurationFile);
    studio.start();

    //
    Customer* customer = new HeavyMuscleCustomer("Yoni", 2);
    vector<int> res = customer->order(studio.getWorkoutOptions());

    //
    for (int i = 0; i <res.size(); ++i) {
        cout << res[i];
    }

    //
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    }
    return 0;
}