#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include <map>
#include "patient.h"
#include "doctor.h"
#include "nurse.h"

using namespace std;

// Forward declaration
class Pharmacy;

class Hospital {
public:
    string hospitalID;
    string hospitalName;
    vector<Patient*> patients;
    vector<Doctor*> doctors;
    vector<Nurse*> nurses;
    map<string, double> pharmacyBills; // Maps billID to amount
    double totalPatientBillings;
    double collectedPayments;
    map<string, double> patientPayments; // Track payments per patient
    
    Hospital(string id, string n);
    bool admitPatient(Patient* patient);
    bool relocatePatient(Patient* patient, Hospital* newHospital);
    void dischargePatient(Patient* patient);
    void addDoctor(Doctor* doctor);
    void addNurse(Nurse* nurse);
    
    // Pharmacy-related methods
    double getPaidPharmacyBills() const;
    double getPendingPharmacyBills() const;
    
    // Patient billing methods
    void updatePatientDays(); // Call this daily to increment days
    double getTotalPatientBills() const;
    bool collectPaymentFromPatient(string patientID, double amount);
    double getPatientRemainingBalance(string patientID) const;
    
    // Doctor-patient relationship
    bool assignDoctorToPatient(string doctorID, string patientID);
    bool requestPatientDischarge(string doctorID, string patientID);
    
    // Nurse-patient relationship
    bool assignNurseToPatient(string nurseID, string patientID);
    
    // Reporting
    string getPatientBillingReport() const;
};

#endif // HOSPITAL_H
