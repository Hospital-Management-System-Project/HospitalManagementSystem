#ifndef DOCTOR_H
#define DOCTOR_H

#include <string>
#include <vector>

using namespace std;

class Doctor {
private:
    string doctorID;
    string name;
    string hospitalID;
    vector<string> primaryPatients;   // IDs of patients for whom this doctor is primary
    vector<string> secondaryPatients; // IDs of patients for whom this doctor is secondary
    
    static const int MAX_PRIMARY_PATIENTS = 10; // Arbitrary limit, can be adjusted

public:
    Doctor(string id, string n, string hospID);
    
    // Patient management methods
    bool addPrimaryPatient(const string& patientID);
    bool addSecondaryPatient(const string& patientID);
    bool removePrimaryPatient(const string& patientID);
    bool removeSecondaryPatient(const string& patientID);
    
    // Query methods
    bool isPrimaryPatient(const string& patientID) const;
    bool isSecondaryPatient(const string& patientID) const;
    bool isPatient(const string& patientID) const;
    
    // Getters and setters
    void setHospitalID(const string& hospID);
    string getDoctorID() const;
    string getName() const;
    string getHospitalID() const;
    vector<string> getPrimaryPatients() const;
    vector<string> getSecondaryPatients() const;
    int getPrimaryPatientCount() const;
    int getSecondaryPatientCount() const;
};

#endif // DOCTOR_H
