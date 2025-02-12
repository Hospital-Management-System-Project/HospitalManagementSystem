#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <string>
#include <vector>
#include "patient.h"
#include "doctor.h"
#include "nurse.h"

class Hospital {
public:
    std::string hospitalID;
    std::string name;
    std::vector<Patient*> patients;
    std::vector<Doctor*> doctors;
    std::vector<Nurse*> nurses;

    Hospital(std::string id, std::string n);
    bool admitPatient(Patient* patient);
    bool relocatePatient(Patient* patient, Hospital* newHospital);
    void dischargePatient(Patient* patient);
    void addDoctor(Doctor* doctor);
    void addNurse(Nurse* nurse);
};

#endif // HOSPITAL_H
