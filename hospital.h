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
private:
    string hospitalID;
    string hospitalName;
    vector<Patient*> patients;
    vector<Doctor*> doctors;
    vector<Nurse*> nurses;
    map<string, double> pharmacyBills; // Maps billID to amount
    double totalPatientBillings;
    double collectedPayments;
    map<string, double> patientPayments; // Track payments per patient
    
public:
    Hospital(string id, string n);
    
    bool admitPatient(Patient* patient);
    bool relocatePatient(Patient* patient, Hospital* newHospital);
    void dischargePatient(Patient* patient);
    bool removePatient(Patient* patient);
    
    void addDoctor(Doctor* doctor);
    bool relocateDoctor(Doctor* doctor, Hospital* newHospital);
    void removeDoctor(Doctor* doctor);
    void addNurse(Nurse* nurse);
    void removeNurse(Nurse* nurse);

    // Getters and setters
    string getHospitalID() const;
    string getHospitalName() const;

    vector<Patient*> getPatients() const;
    vector<Doctor*> getDoctors() const;
    vector<Nurse*> getNurses()const;
    vector<Nurse*>& getNurses();
    
    // Patient billing methods
    void updatePatientDays(); // Call this daily to increment days
    double getTotalPatientBills() const;
    bool collectPaymentFromPatient(string patientID, double amount);
    double getPatientRemainingBalance(string patientID) const;
    
    // Doctor-patient relationship
    bool requestPatientDischarge(string doctorID, string patientID);
    
    // Reporting
    string getPatientBillingReport() const;
};

#endif // HOSPITAL_H
