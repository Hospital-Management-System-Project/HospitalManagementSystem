#ifndef NURSE_H
#define NURSE_H

#include <string>
#include <vector>

using namespace std;

class Nurse {
private:
    string nurseID;
    string nurseName;
    string hospitalID;
    vector<string> patientIDs;

public:
    Nurse(string id, string name, string hospID);

    // Getters
    string getNurseID() const;
    string getNurseName() const;
    string getHospitalID() const;
    vector<string> getPatientIDs() const;

    // Patient assignment
    bool assignPatient(string patientID);
    void removePatient(string patientID);
};

#endif // NURSE_H
