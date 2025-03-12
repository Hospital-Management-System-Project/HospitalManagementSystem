#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

using namespace std;

class Patient {
private:
  string patientID;
  string name;
  string phoneNumber;
  string disease;
  string treatment;
  string hospitalID;
  string primaryDoctorID;
  vector<string> attendingDoctorIDs;
  vector<string> nurseIDs;
  vector<string> medications;
  int daysAdmitted;

public:
  Patient(string id, string n, string phone, string d, string t, string docID);
  
  // Doctor management
  void setPrimaryDoctorID(const string& doctorID);
  void addAttendingDoctor(const string& doctorID);
  void removeAttendingDoctor(const string& doctorID);
  bool hasAttendingDoctor(const string& doctorID) const;
  
  // Nurse management
  void addNurse(const string& nurseID);
  void removeNurse(const string& nurseID);
  bool hasNurse(const string& nurseID) const;
  
  // Medication management
  void addMedication(const string& medication);
  
  // Hospital stay
  void incrementDaysAdmitted();
  
  // Getters
  string getPatientID() const;
  string getName() const;
  string getPhoneNumber() const;
  string getDisease() const;
  string getTreatment() const;
  string getHospitalID() const;
  string getPrimaryDoctorID() const;
  vector<string> getAttendingDoctors() const;
  vector<string> getNurses() const;
  vector<string> getMedications() const;
  int getDaysAdmitted() const;
  
  // Setters
  void setHospitalID(const string& hospID);
};

#endif // PATIENT_H