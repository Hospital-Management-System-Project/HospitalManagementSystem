#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H

#include <vector>
#include <string>
#include <map>
#include "hospital.h"

using namespace std;

class HospitalSystem {
private:
    static HospitalSystem* instance;
    vector<Hospital*> hospitals;
    map<string, Patient*> patients;
    map<string, Doctor*> doctors; // Track doctors by ID
    map<string, Nurse*> nurses; // Track nurses by ID
    
    // Private constructor for singleton
    HospitalSystem() {}

public:
    static HospitalSystem* getInstance() {
        if (!instance) {
            instance = new HospitalSystem();
        }
        return instance;
    }
    
    void initializeHospitals();
    bool admitPatient(Patient* patient, int hospitalIndex);
    bool relocatePatient(string patientID, int newHospitalIndex);
    bool dischargePatient(string patientID);

    Hospital* getHospital(int index);
    vector<Hospital*> getAllHospitals();
    map<string, Patient*>& getAllPatients();
    Patient* findPatient(string patientID);
    Hospital* findPatientHospital(string patientID);
    Hospital* findDoctorHospital(string doctorID);
    string getHospitalStatus();
    
    // Patient billing methods
    void updateAllPatientDays();
    bool collectPatientPayment(string patientID, double amount);
    
    // Add a method to get remaining balance
    double getPatientRemainingBalance(string patientID);
    
    // Doctor-patient relations - remove this version as it's redundant
    // bool assignDoctorToPatient(string doctorID, string patientID);
    bool requestPatientDischarge(string doctorID, string patientID);
    
    // Enhanced reporting
    string getPatientBillingReport();
    
    // Doctor management
    void initializeDoctors();
    Doctor* findDoctor(string doctorID);
    bool addDoctor(Doctor* doctor, int hospitalIndex);
    bool relocateDoctor(string doctorID, int newHospitalIndex);
    bool removeDoctor(string doctorID);
    bool assignDoctorToPatient(string doctorID, string patientID, bool isPrimary = false);
    bool setPatientPrimaryDoctor(string patientID, string doctorID);
    
    // Nurse management
    void initializeNurses();
    Nurse* findNurse(string nurseID);
    bool assignNurseToPatient(string nurseID, string patientID);
};

#endif
