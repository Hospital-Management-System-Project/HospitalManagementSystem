#ifndef NURSE_H
#define NURSE_H

#include <string>
#include <vector>

class Nurse {
public:
    std::string nurseID;
    std::string name;
    std::string hospitalID;
    std::vector<std::string> patientIDs;

    Nurse(std::string id, std::string n, std::string hospID);
    bool assignPatient(std::string patientID);
    void removePatient(std::string patientID);
};

#endif // NURSE_H
