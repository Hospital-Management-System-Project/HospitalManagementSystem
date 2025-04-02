#ifndef PATIENT_H
#define PATIENT_H

#include <ctime>
#include <string>
#include <vector>

using namespace std; // Just using this for simplicity, so we dont have to use std:: everywhere

// Define the Patient class
class Patient {
private:  // Define
  string patientID;
  string patientName;
  string phoneNumber;
  string disease;
  string treatment;
  int daysAdmitted;
  string primaryDoctorID;
  vector<string> attendingNursesIDs;  // Vector of IDs of attending nurses
  vector<string> attendingDoctorIDs;  // Vector of IDs of attending doctors
  bool discharged;
  bool dischargeRequested;
  double billingRatePerDay;
  time_t admissionDate;

public:
  Patient(string id, string name, string phone, string patientDisease, string treatmentType, string docID, string nurseID);
  void addAttendingDoctor(string docID);
  bool addAttendingNurse(string nurseID);
  void incrementDaysAdmitted();
  double calculateCurrentBill() const;
  void setDailyRate(double rate);
  bool canBeDischargedBy(string doctorID) const;
  string getAdmissionDateString() const;
  string getStatus() const;
  string getFullDescription() const; 
  void applyRateIncrease(double percentage = 0.05);

  // Getters and Setters for Encapsulated Variables
  string getPatientID() const;  // Get patient ID
  string getPatientName() const;  // Get patient name
  string getDisease() const;  // Get disease
  int getDaysAdmitted() const;  // Get days admitted
  string getPrimaryDoctorID() const;  // Get primary doctor ID
  vector<string>& getAttendingDoctorIDs();  // Get vector of attending doctor IDs
  const vector<string>& getAttendingDoctorIDs() const;  // Get vector of attending doctor IDs
  vector<string>& getAttendingNursesIDs();  // Get vector of attending nurse IDs
  const vector<string>& getAttendingNursesIDs() const;  // Get vector of attending nurse IDs
  double getBillingRatePerDay() const;  // Get billing rate per day
  string getPhoneNumber() const;  // Get phone number
  string getTreatment() const;  // Get treatment

  void setPrimaryDoctorID(const string& docID); // Set primary doctor ID 
  void setDischarged(bool status);  // Set discharged status
  bool isDischargeRequested() const;  // Check if discharge is requested
  void setDischargeRequested(bool status);  // Set discharge requested status
};
#endif