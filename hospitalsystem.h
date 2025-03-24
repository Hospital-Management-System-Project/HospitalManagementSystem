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
    string getHospitalStatus();
    
    // Pharmacy-related methods
    string orderPrescription(int hospitalIndex, string patientID, string pharmacyID, string medication, double price);
    bool payPharmacyBill(int hospitalIndex, string pharmacyID, string billID);
    
    // Patient billing methods
    void updateAllPatientDays();
    double calculatePatientBill(string patientID);
    bool collectPatientPayment(string patientID, double amount);
    
    // Doctor-patient relations - remove this version as it's redundant
    // bool assignDoctorToPatient(string doctorID, string patientID);
    bool requestPatientDischarge(string doctorID, string patientID);
    
    // Enhanced reporting
    string getPatientBillingReport();
    
    // Doctor management
    void initializeDoctors();
    Doctor* findDoctor(string doctorID);
    bool assignDoctorToPatient(string doctorID, string patientID, bool isPrimary = false);
    bool setPatientPrimaryDoctor(string patientID, string doctorID);
    bool addDoctor(Doctor* doctor);
    
    // Nurse management
    void initializeNurses();
    Nurse* findNurse(string nurseID);
    bool assignNurseToPatient(string nurseID, string patientID);
    bool addNurse(Nurse* nurse);
};

#endif