#ifndef HOSPITAL_H
#define HOSPITAL_H

#include "doctor.h"
#include "nurse.h"
#include "patient.h"
#include <string>
#include <vector>

using namespace std;

class Hospital {
public:
  string hospitalID;
  string name;
  vector<Patient *> patients;
  vector<Doctor *> doctors;
  vector<Nurse *> nurses;

  Hospital(string id, string n);
  bool admitPatient(Patient *patient);
  bool relocatePatient(Patient *patient, Hospital *newHospital);
  void dischargePatient(Patient *patient);
  void addDoctor(Doctor *doctor);
  void addNurse(Nurse *nurse);
};

#endif // HOSPITAL_H
