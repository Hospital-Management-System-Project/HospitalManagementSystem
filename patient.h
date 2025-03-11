#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

using namespace std;

class Patient {
public:
  string patientID;
  string name;
  string phoneNumber;
  string disease;
  string treatment;
  int daysAdmitted;
  string primaryDoctorID;
  vector<string> attendingDoctorIDs;

  Patient(string id, string n, string phone, string d, string t, string docID);
  void addAttendingDoctor(string docID);
  void incrementDaysAdmitted();
};

#endif // PATIENT_H
