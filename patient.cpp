#include "patient.h"
#include <string>
#include <vector>

using namespace std;

Patient::Patient(std::string id, std::string n, std::string phone, std::string d, std::string t, std::string docID) {
    patientID = id;
    name = n;
    phoneNumber = phone;
    disease = d;
    treatment = t;
    daysAdmitted = 0;
    primaryDoctorID = docID;
}

void Patient::addAttendingDoctor(std::string docID) {
    attendingDoctorIDs.push_back(docID);
}

void Patient::incrementDaysAdmitted() {
    daysAdmitted++;
}
