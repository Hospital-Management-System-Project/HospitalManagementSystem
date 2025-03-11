#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>

using namespace std;

class Doctor {
public:
    string doctorID;
    string name;
    string hospitalID;

    Doctor(string id, string n, string hospID);
};

#endif // DOCTOR_H
