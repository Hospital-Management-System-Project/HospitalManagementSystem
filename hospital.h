#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include <map>
#include "patient.h"
#include "doctor.h"
#include "nurse.h"

using namespace std;

class Hospital {
private:
    string hospitalID;
    string hospitalName;
    vector<Patient*> patients;              // List of patients in the hospital
    vector<Doctor*> doctors;                // List of doctors in the hospital       
    vector<Nurse*> nurses;                  // List of nurses in the hospital 
    map<string, double> pharmacyBills;      // Maps billID to amount
    double totalPatientBillings;            // Total billings for all patients
    double collectedPayments;               // Total payments collected 
    map<string, double> patientPayments;    // Track payments per patient
    
public:
    Hospital(string id, string n); // Constructor
    
    // Hospital management methods for bool
    bool admitPatient(Patient* patient);
    bool relocatePatient(Patient* patient, Hospital* newHospital);
    bool relocateDoctor(Doctor* doctor, Hospital* newHospital);
    bool removePatient(Patient* patient);

    // Hospital management methods for void
    void dischargePatient(Patient* patient);
    void addDoctor(Doctor* doctor);
    void removeDoctor(Doctor* doctor);
    void addNurse(Nurse* nurse);
    void removeNurse(Nurse* nurse);

    // Getters and setters
    string getHospitalID() const;
    string getHospitalName() const;

    // Getters for patients, doctors, and nurses
    vector<Patient*> getPatients() const;
    vector<Doctor*> getDoctors() const;
    vector<Nurse*> getNurses() const;
    vector<Nurse*>& getNurses();
    
    void updatePatientDays(); // This will be called daily to increment days for all patients
    double getTotalPatientBills() const;    // Total billings for all patients
    bool collectPaymentFromPatient(string patientID, double amount); // Collect payment from a patient
    double getPatientRemainingBalance(string patientID) const;  // Get remaining balance for a patient
    bool requestPatientDischarge(string doctorID, string patientID); // Request discharge for a patient by a doctor
    
    // This method will be called to generate a report of all patients and their billing status
    string getPatientBillingReport() const;
};

#endif // HOSPITAL_H