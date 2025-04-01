#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>

using namespace std;

class Doctor {
private:
    string doctorID;
    string doctorName;
    string hospitalID;
    vector<string> patientIDs;

public:
    Doctor(string id, string doctorName, string hospID);

    // Getters
    string getDoctorID() const;
    string getDoctorName() const;
    string getHospitalID() const;
    vector<string> getPatientIDs() const;

    string getFullDescription() const;

    // Setters
    void setHospitalID(string hospID);

    // Patient management
    void addPatient(string patientID);
    void removePatient(string patientID);
    bool requestPatientDischarge(string patientID);
};

#endif // DOCTOR_H
