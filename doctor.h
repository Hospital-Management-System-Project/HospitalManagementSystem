#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>

using namespace std;

class Doctor {
public:
    string doctorID;
    string doctorName;
    string hospitalID;
    vector<string> patientIDs; // Patients this doctor treats

    Doctor(string id, string doctorName, string hospID);
    
    // Patient management
    void addPatient(string patientID);
    void removePatient(string patientID);
    bool requestPatientDischarge(string patientID);
};

#endif // DOCTOR_H
