#include "patient.h"

Patient::Patient(std::string id, std::string n, std::string phone, std::string d, std::string t, std::string docID)
    : patientID(id), name(n), phoneNumber(phone), disease(d), treatment(t), primaryDoctorID(docID), daysAdmitted(0) {}

void Patient::addAttendingDoctor(std::string docID) {
    attendingDoctorIDs.push_back(docID);
}

void Patient::incrementDaysAdmitted() {
    daysAdmitted++;
}
