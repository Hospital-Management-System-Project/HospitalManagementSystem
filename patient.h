#ifndef PATIENT_H
#define PATIENT_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Patient {
private:
  string patientID;
  string patientName;
  string phoneNumber;
  string disease;
  string treatment;
  int daysAdmitted;
  string primaryDoctorID;
  vector<string> attendingNursesIDs;
  vector<string> attendingDoctorIDs;
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
  string getPatientID() const;

  string getPatientName() const;

  string getDisease() const;

  int getDaysAdmitted() const;

  string getPrimaryDoctorID() const;
  void setPrimaryDoctorID(const string& docID);

  vector<string>& getAttendingDoctorIDs();
  const vector<string>& getAttendingDoctorIDs() const;
  
  vector<string>& getAttendingNursesIDs();
  const vector<string>& getAttendingNursesIDs() const;

  double getBillingRatePerDay() const;

  void setDischarged(bool status);

  bool isDischargeRequested() const;
  void setDischargeRequested(bool status);

  string getPhoneNumber() const;
  string getTreatment() const;
};
#endif
