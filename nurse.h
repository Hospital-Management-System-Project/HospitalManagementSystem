#ifndef NURSE_H
#define NURSE_H

#include <string>
#include <vector>

using namespace std;

class Nurse {
private:
  string nurseID;
  string name;
  string hospitalID;
  string specialization;  // Added: nurse specialization
  vector<string> patientIDs; // IDs of assigned patients
  
  static const int MAX_PATIENTS = 2; // Maximum of two patients per nurse

public:
  Nurse(string id, string n, string hospID);
  Nurse(string id, string n, string hospID, string spec); // Constructor with specialization
  
  // Patient management
  bool assignPatient(const string& patientID);
  void removePatient(const string& patientID);
  bool isResponsibleFor(const string& patientID) const;
  int getPatientCount() const;
  bool canAcceptMorePatients() const; // Check if nurse can take more patients
  
  // Getters and setters
  string getNurseID() const;
  string getName() const;
  string getHospitalID() const;
  string getSpecialization() const;
  void setHospitalID(const string& hospID);
  void setSpecialization(const string& spec);
  vector<string> getPatients() const;
  
  // Get detailed information about nurse
  string getDetails() const;
};

#endif // NURSE_H