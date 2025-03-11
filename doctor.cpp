#include "doctor.h"
#include <string>

using namespace std;

Doctor::Doctor(std::string id, std::string n, std::string hospID){
    doctorID = id;
    name = n;
    hospitalID = hospID;
}
