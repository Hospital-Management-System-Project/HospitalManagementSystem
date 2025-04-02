#ifndef NURSE_H
#define NURSE_H

#include <string>
#include <vector>
using namespace std;    // Using standard namespace for simplicity

// We created a Nurse class to manage nurse information and their assigned patients
class Nurse {
// Private member variables to store nurse information
private:
    string nurseID;
    string nurseName;
    string hospitalID;
    vector<string> patientIDs;  // Vector to store IDs of assigned patients

// Public member functions to manage nurse information
public:
    Nurse(string id, string name, string hospID);

    // Getters for nurse information
    string getNurseID() const;
    string getNurseName() const;
    string getHospitalID() const;
    vector<string> getPatientIDs() const;   // Vector of patient IDs that the nurse is assigned to
    string getFullDescription() const;

    // Setters for setting the hospital ID for the nurse
    void setHospitalID(string hosID);

    // Patient Assignment and Removal for the nurse
    bool assignPatient(string patientID);
    void removePatient(string patientID);
};

#endif // NURSE_H