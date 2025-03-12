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
  vector<string> patientIDs; // IDs of assigned patients
  
  static const int MAX_PATIENTS = 2; // Maximum of two patients per nurse

public:
  Nurse(string id, string n, string hospID);
  
  // Patient management
  bool assignPatient(const string& patientID);
  void removePatient(const string& patientID);
  bool isResponsibleFor(const string& patientID) const;
  int getPatientCount() const;
  
  // Getters and setters
  string getNurseID() const;
  string getName() const;
  string getHospitalID() const;
  void setHospitalID(const string& hospID);
  vector<string> getPatients() const;
};

#endif // NURSE_H