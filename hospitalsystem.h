#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H

#include <vector>
#include <string>
#include <map>
#include "hospital.h"

using namespace std;    // We are using namespace std for convenience so we don't have to prefix std:: to every standard library object

// We are using a singleton pattern for the HospitalSystem class which will act as a central point for managing all hospitals, patients, doctors, and nurses
class HospitalSystem {
private:
    static HospitalSystem* instance;    // Singleton instance
    vector<Hospital*> hospitals;        // Created a vector to store all hospitals
    map<string, Patient*> patients;     // Implemented a map to track patients by ID
    map<string, Doctor*> doctors;       // Implemented a map to track doctors by ID
    map<string, Nurse*> nurses;         // Used a map to track nurses by ID
    
    // Created a private constructor for singleton 
    HospitalSystem() {}
    
    // Prevent copying and assignment to maintain singleton
    HospitalSystem(const HospitalSystem&) = delete;
    HospitalSystem& operator=(const HospitalSystem&) = delete;

public:
    // Destructor to properly release all resources
    ~HospitalSystem();
    
    // Static method to release the singleton instance - call at program termination
    static void releaseInstance();
    
    // Defined a static method to get the singleton instance for the HospitalSystem 
    static HospitalSystem* getInstance() {
        if (!instance) {    // Check if instance is null
            instance = new HospitalSystem();    // If null, create a new instance
        }
        return instance;    // Return the singleton instance
    }
    
    // Hospital management methods
    void initializeHospitals();
    bool admitPatient(Patient* patient, int hospitalIndex);
    bool relocatePatient(string patientID, int newHospitalIndex);
    bool dischargePatient(string patientID);
    bool requestPatientDischarge(string doctorID, string patientID);

    Hospital* getHospital(int index);                   // This method will return a hospital object at the specified index
    vector<Hospital*> getAllHospitals();                // This method will return a vector of all hospitals
    map<string, Patient*>& getAllPatients();            // This method will return a map of all patients
    Patient* findPatient(string patientID);             // This method will return a patient object based on the provided ID
    Hospital* findPatientHospital(string patientID);    // The function will return the hospital object where the patient is located
    Hospital* findDoctorHospital(string doctorID);      // The function will return the hospital object where the doctor is located
    Hospital* findNurseHospital(string nurseID);        // The function will return the hospital object where the nurse is located
    string getHospitalStatus();                         // The method will return a string containing the status of all hospitals
    
    // Patient billing methods
    void updateAllPatientDays();
    bool collectPatientPayment(string patientID, double amount);    // This method will collect payment from the patient
    double getPatientRemainingBalance(string patientID);            // This method will return the remaining balance of the patient
    string getPatientBillingReport();                               // The function will return a string containing the billing report of all patients
    
    // Doctor management
    void initializeDoctors();
    Doctor* findDoctor(string doctorID);                            // The function will return a doctor object based on the provided ID
    bool addDoctor(Doctor* doctor, int hospitalIndex);              // This method will add a doctor to the specified hospital     
    bool relocateDoctor(string doctorID, int newHospitalIndex);     // The boolean function will relocate the doctor to a new hospital and let us know if it was successful
    bool relocateNurse(const std::string& nurseID, int newHospitalIndex);   // The function will relocate the nurse to a new hospital and let us know if it was successful
    bool removeNurse(const std::string& nurseID);                   // The function will remove the nurse from the system
    bool removeDoctor(string doctorID);                             // The function will remove the doctor from the system                    
    bool assignDoctorToPatient(string doctorID, string patientID, bool isPrimary = false);  // We have a check to see if the doctor is primary or not
    bool setPatientPrimaryDoctor(string patientID, string doctorID);    // The function will set the primary doctor for the patient
    
    // Nurse management
    void initializeNurses();            // This method will initialize the requirement of 60 nurses
    Nurse* findNurse(string nurseID);   // The function will return a nurse object based on the provided ID
    bool addNurse(Nurse* nurse, int hospitalIndex); // This will add a nurse to the specified hospital
    bool assignNurseToPatient(string nurseID, string patientID);    // This method will assign a nurse to the patient
};

#endif