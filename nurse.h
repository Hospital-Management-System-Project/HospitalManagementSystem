#ifndef NURSE_H
#define NURSE_H

#include <string>
#include <vector>

using namespace std;

class Nurse {
public:
    string nurseID;
    string nurseName;
    string hospitalID;
    vector<string> patientIDs;

    Nurse(string id, string name, string hospID);
    bool assignPatient(string patientID);
    void removePatient(string patientID);
};

#endif // NURSE_H