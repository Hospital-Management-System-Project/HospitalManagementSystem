#include "nurse.h"
#include <algorithm>

using namespace std;

Nurse::Nurse(string id, string name, string hospID) {
  nurseID = id;
  nurseName = name;
  hospitalID = hospID;
}

bool Nurse::assignPatient(string patientID) {
  if (patientIDs.size() < 2) {
    patientIDs.push_back(patientID);
    return true;
  }
  return false;
}

void Nurse::removePatient(string patientID) {
  patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}
