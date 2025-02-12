#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>

class Patient
{
public:
    std::string patientID;
    std::string name;
    std::string phoneNumber;
    std::string disease;
    std::string treatment;
    int daysAdmitted;
    std::string primaryDoctorID;
    std::vector<std::string> attendingDoctorIDs;

    Patient(std::string id,
            std::string n,
            std::string phone,
            std::string d,
            std::string t,
            std::string docID);
    void addAttendingDoctor(std::string docID);
    void incrementDaysAdmitted();
};

#endif // PATIENT_H
