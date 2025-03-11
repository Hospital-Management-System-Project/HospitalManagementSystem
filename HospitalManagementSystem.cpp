#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Constants
const int MAX_HOSPITALS = 5;
const int MAX_PHARMACIES = 20;
const int MAX_DOCTORS = 50;
const int MAX_NURSES = 60;
const int MAX_PATIENTS_PER_HOSPITAL = 20;

// Hospital Names
const string HOSPITAL_NAMES[MAX_HOSPITALS] = {
    "Windsor Health Centre",
    "Windsor Regional Hospital",
    "Windsor Regional Hospital - Metropolitan Campus",
    "Hotel-Dieu Grace Healthcare",
    "Windsor Cardiac Centre"
};

// Class Definitions
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

    Patient(string id, string n, string phone, string d, string t, string docID)
        : patientID(id), name(n), phoneNumber(phone), disease(d), treatment(t), primaryDoctorID(docID), daysAdmitted(0) {}

    void addAttendingDoctor(string docID) {
        attendingDoctorIDs.push_back(docID);
    }

    void incrementDaysAdmitted() {
        daysAdmitted++;
    }
};

class Doctor {
public:
    string doctorID;
    string name;
    string hospitalID;

    Doctor(string id, string n, string hospID)
        : doctorID(id), name(n), hospitalID(hospID) {}
};

class Nurse {
public:
    string nurseID;
    string name;
    string hospitalID;
    vector<string> patientIDs;

    Nurse(string id, string n, string hospID)
        : nurseID(id), name(n), hospitalID(hospID) {}

    bool assignPatient(string patientID) {
        if (patientIDs.size() < 2) {
            patientIDs.push_back(patientID);
            return true;
        }
        return false;
    }

    void removePatient(string patientID) {
        patientIDs.erase(remove(patientIDs.begin(), patientIDs.end(), patientID), patientIDs.end());
    }
};

class Pharmacy {
public:
    string pharmacyID;
    string name;

    Pharmacy(string id, string n)
        : pharmacyID(id), name(n) {}
};

class Hospital {
public:
    string hospitalID;
    string name;
    vector<Patient*> patients;
    vector<Doctor*> doctors;
    vector<Nurse*> nurses;

    Hospital(string id, string n)
        : hospitalID(id), name(n) {}

    bool admitPatient(Patient* patient) {
        if (patients.size() < MAX_PATIENTS_PER_HOSPITAL) {
            patients.push_back(patient);
            return true;
        }
        return false;
    }

    bool relocatePatient(Patient* patient, Hospital* newHospital) {
        if (newHospital->admitPatient(patient)) {
            patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
            return true;
        }
        return false;
    }

    void dischargePatient(Patient* patient) {
        patients.erase(remove(patients.begin(), patients.end(), patient), patients.end());
    }

    void addDoctor(Doctor* doctor) {
        doctors.push_back(doctor);
    }

    void addNurse(Nurse* nurse) {
        nurses.push_back(nurse);
    }
};

// Global Data
vector<Hospital*> hospitals;
vector<Pharmacy*> pharmacies;
map<string, Patient*> patients;
map<string, Doctor*> doctors;
map<string, Nurse*> nurses;

// Function Prototypes
void initializeHospitals();
void initializePharmacies();
void addPatient();
void relocatePatient();
void dischargePatient();
void displayHospitalStatus();

int main() {
    initializeHospitals();
    initializePharmacies();

    int choice;
    do {
        cout << "\nHospital Management System\n";
        cout << "1. Add Patient\n";
        cout << "2. Relocate Patient\n";
        cout << "3. Discharge Patient\n";
        cout << "4. Display Hospital Status\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                relocatePatient();
                break;
            case 3:
                dischargePatient();
                break;
            case 4:
                displayHospitalStatus();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    // Cleanup
    for (auto& hospital : hospitals) delete hospital;
    for (auto& pharmacy : pharmacies) delete pharmacy;
    for (auto& patient : patients) delete patient.second;
    for (auto& doctor : doctors) delete doctor.second;
    for (auto& nurse : nurses) delete nurse.second;

    return 0;
}

void initializeHospitals() {
    for (int i = 0; i < MAX_HOSPITALS; i++) {
        hospitals.push_back(new Hospital(to_string(i + 1), HOSPITAL_NAMES[i]));
    }
}

void initializePharmacies() {
    for (int i = 0; i < MAX_PHARMACIES; i++) {
        pharmacies.push_back(new Pharmacy(to_string(i + 1), "Pharmacy " + to_string(i + 1)));
    }
}

void addPatient() {
    string patientID, name, phone, disease, treatment, doctorID;
    int hospitalIndex;

    cout << "Enter Patient ID: ";
    cin >> patientID;
    cout << "Enter Patient Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Phone Number: ";
    cin >> phone;
    cout << "Enter Disease: ";
    cin.ignore();
    getline(cin, disease);
    cout << "Enter Treatment: ";
    getline(cin, treatment);
    cout << "Enter Primary Doctor ID: ";
    cin >> doctorID;
    cout << "Select Hospital (1-5): ";
    cin >> hospitalIndex;

    if (hospitalIndex < 1 || hospitalIndex > MAX_HOSPITALS) {
        cout << "Invalid hospital selection.\n";
        return;
    }

    Patient* patient = new Patient(patientID, name, phone, disease, treatment, doctorID);
    if (hospitals[hospitalIndex - 1]->admitPatient(patient)) {
        patients[patientID] = patient;
        cout << "Patient admitted successfully.\n";
    } else {
        cout << "Hospital is full. Cannot admit patient.\n";
        delete patient;
    }
}

void relocatePatient() {
    string patientID;
    int newHospitalIndex;

    cout << "Enter Patient ID: ";
    cin >> patientID;
    cout << "Select New Hospital (1-5): ";
    cin >> newHospitalIndex;

    if (newHospitalIndex < 1 || newHospitalIndex > MAX_HOSPITALS) {
        cout << "Invalid hospital selection.\n";
        return;
    }

    if (patients.find(patientID) == patients.end()) {
        cout << "Patient not found.\n";
        return;
    }

    Patient* patient = patients[patientID];
    Hospital* currentHospital = nullptr;
    for (auto& hospital : hospitals) {
        if (find(hospital->patients.begin(), hospital->patients.end(), patient) != hospital->patients.end()) {
            currentHospital = hospital;
            break;
        }
    }

    if (currentHospital && currentHospital->relocatePatient(patient, hospitals[newHospitalIndex - 1])) {
        cout << "Patient relocated successfully.\n";
    } else {
        cout << "Failed to relocate patient.\n";
    }
}

void dischargePatient() {
    string patientID;

    cout << "Enter Patient ID: ";
    cin >> patientID;

    if (patients.find(patientID) == patients.end()) {
        cout << "Patient not found.\n";
        return;
    }

    Patient* patient = patients[patientID];
    for (auto& hospital : hospitals) {
        if (find(hospital->patients.begin(), hospital->patients.end(), patient) != hospital->patients.end()) {
            hospital->dischargePatient(patient);
            patients.erase(patientID);
            delete patient;
            cout << "Patient discharged successfully.\n";
            return;
        }
    }
    cout << "Patient not found in any hospital.\n";
}

void displayHospitalStatus() {
    for (auto& hospital : hospitals) {
        cout << "\nHospital: " << hospital->name << "\n";
        cout << "Patients Admitted: " << hospital->patients.size() << "\n";
        cout << "Doctors: " << hospital->doctors.size() << "\n";
        cout << "Nurses: " << hospital->nurses.size() << "\n";
    }
}