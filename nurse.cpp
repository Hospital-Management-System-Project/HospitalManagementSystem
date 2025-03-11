#include "nurse.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

Nurse::Nurse(std::string id, std::string n, std::string hospID){
    nurseID = id;
    name = n;
    hospitalID = hospID;
}

bool Nurse::assignPatient(std::string patientID) {
  if (patientIDs.size() < 2) {
    patientIDs.push_back(patientID);
    return true;
  }
  return false;
}

void Nurse::removePatient(std::string patientID) {
  // Use std::remove and erase to remove the patient from the nurse's list
  patientIDs.erase(std::remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
}
