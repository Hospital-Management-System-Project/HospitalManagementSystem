#include "patient.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Patient::Patient(string id, string n, string phone, string d, string t, string docID) {
    patientID = id;
    name = n;
    phoneNumber = phone;
    disease = d;
    treatment = t;
    primaryDoctorID = docID;
    hospitalID = "";
    daysAdmitted = 0;
}

void Patient::setPrimaryDoctorID(const string& doctorID) {
    primaryDoctorID = doctorID;
}

void Patient::addAttendingDoctor(const string& doctorID) {
    if (!hasAttendingDoctor(doctorID)) {
        attendingDoctorIDs.push_back(doctorID);
    }
}

void Patient::removeAttendingDoctor(const string& doctorID) {
    auto it = find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID);
    if (it != attendingDoctorIDs.end()) {
        attendingDoctorIDs.erase(it);
    }
}

bool Patient::hasAttendingDoctor(const string& doctorID) const {
    return find(attendingDoctorIDs.begin(), attendingDoctorIDs.end(), doctorID) != attendingDoctorIDs.end();
}

void Patient::addNurse(const string& nurseID) {
    if (!hasNurse(nurseID)) {
        nurseIDs.push_back(nurseID);
    }
}

void Patient::removeNurse(const string& nurseID) {
    auto it = find(nurseIDs.begin(), nurseIDs.end(), nurseID);
    if (it != nurseIDs.end()) {
        nurseIDs.erase(it);
    }
}

bool Patient::hasNurse(const string& nurseID) const {
    return find(nurseIDs.begin(), nurseIDs.end(), nurseID) != nurseIDs.end();
}

void Patient::addMedication(const string& medication) {
    medications.push_back(medication);
}

void Patient::incrementDaysAdmitted() {
    daysAdmitted++;
}

string Patient::getPatientID() const {
    return patientID;
}

string Patient::getName() const {
    return name;
}

string Patient::getPhoneNumber() const {
    return phoneNumber;
}

string Patient::getDisease() const {
    return disease;
}

string Patient::getTreatment() const {
    return treatment;
}

string Patient::getHospitalID() const {
    return hospitalID;
}

string Patient::getPrimaryDoctorID() const {
    return primaryDoctorID;
}

vector<string> Patient::getAttendingDoctors() const {
    return attendingDoctorIDs;
}

vector<string> Patient::getNurses() const {
    return nurseIDs;
}

vector<string> Patient::getMedications() const {
    return medications;
}

int Patient::getDaysAdmitted() const {
    return daysAdmitted;
}

void Patient::setHospitalID(const string& hospID) {
    hospitalID = hospID;
}