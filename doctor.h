#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>

class Doctor {
public:
    std::string doctorID;
    std::string name;
    std::string hospitalID;

    Doctor(std::string id, std::string n, std::string hospID);
};

#endif // DOCTOR_H
