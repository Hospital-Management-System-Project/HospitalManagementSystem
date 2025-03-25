#ifndef PATIENT_H
#define PATIENT_H

#include <ctime>
#include <string>
#include <vector>

using namespace std;

class Patient {
public:
  string patientID;
  string patientName;
  string phoneNumber;
  string disease;
  string treatment;
  int daysAdmitted;
  string primaryDoctorID;
  vector<string> attendingDoctorIDs;
  bool discharged;
  bool dischargeRequested;
  double billingRatePerDay;
  time_t admissionDate;

  Patient(string id, string name, string phone, string patientDisease, string treatmentType, string docID);

  void addAttendingDoctor(string docID);
  void incrementDaysAdmitted();
  double calculateCurrentBill() const;
  void setDailyRate(double rate);
  bool canBeDischargedBy(string doctorID) const;
  void markAsDischargedBy(string doctorID);
  string getAdmissionDateString() const;
  int getDaysAdmitted() const;
  string getStatus() const;
  string getFullDescription() const; // New method to get comprehensive patient details
};

#endif // PATIENT_H
