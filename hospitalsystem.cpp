#include "hospitalsystem.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "pharmacysystem.h"
#include <QDebug>

using namespace std;        // We start by using the standard namespace for convenience

// Need to create a singleton instance of the HospitalSystem class
HospitalSystem* HospitalSystem::instance = nullptr;

// This method will initialize the hospitals, we will create 5 hospitals based on close main hospital locations
void HospitalSystem::initializeHospitals() {
    const string HOSPITAL_NAMES[5] = {
        "Henry Ford Hospital",
        "Windsor Regional Hospital",
        "Amherstburg Hospital",
        "Tecumseh Hospital",
        "Chatham-Kent Health Alliance"
    };
    // We can start by clearing any existing hospitals if the system is already initialized
    for (auto hospital : hospitals) {
        delete hospital;
    }
    hospitals.clear();      
    // Next we will do a loop to create 5 hospitals based on the names we have above
    for (int clearCount = 0; clearCount < 5; clearCount++) {
        hospitals.push_back(new Hospital(to_string(clearCount + 1), HOSPITAL_NAMES[clearCount])); // Use push_back to add hospitals to the vector
    }
    // Initialize doctors after hospitals are created
    initializeDoctors();
    // Initialize nurses after hospitals and doctors are created
    initializeNurses();
}

// This method will initialize the doctors, we will create 50 doctors based on the names we have in the vector
void HospitalSystem::initializeDoctors() {
    // Clear existing doctors using a for loop and going through the map
    for (auto& clearPair : doctors) {
        delete clearPair.second;
    }
    doctors.clear();
    // We will use the srand function to seed the random number generator to ensure that we get different random numbers each time the program is run
    srand(time(nullptr));
    // Doctor names (50 total)
    vector<string> doctorNames = {
        "Dr. Smith", "Dr. Johnson", "Dr. Williams", "Dr. Brown", "Dr. Jones",
        "Dr. Miller", "Dr. Davis", "Dr. Garcia", "Dr. Rodriguez", "Dr. Wilson",
        "Dr. Martinez", "Dr. Anderson", "Dr. Taylor", "Dr. Thomas", "Dr. Hernandez",
        "Dr. Moore", "Dr. Jackson", "Dr. Martin", "Dr. Lee", "Dr. Perez",
        "Dr. Thompson", "Dr. White", "Dr. Harris", "Dr. Sanchez", "Dr. Clark",
        "Dr. Ramirez", "Dr. Lewis", "Dr. Robinson", "Dr. Walker", "Dr. Young",
        "Dr. Allen", "Dr. King", "Dr. Wright", "Dr. Scott", "Dr. Torres",
        "Dr. Nguyen", "Dr. Hill", "Dr. Flores", "Dr. Green", "Dr. Adams",
        "Dr. Nelson", "Dr. Baker", "Dr. Hall", "Dr. Rivera", "Dr. Campbell",
        "Dr. Nafiz", "Dr. Carter", "Dr. Ahmad", "Dr. Yacoub", "Dr. Cam"
    };

    int doctorIndex = 0;    // We will use this index to keep track of the doctor names
    int numHospitals = hospitals.size();    // Call the size method to get the number of hospitals and store it in a variable

    // We start by adding 3 doctors per hospital (guaranteed minimum to meet the requirements)
    for (int docCount = 0; docCount < numHospitals; ++docCount) {
        for (int innerCount = 0; innerCount < 3; ++innerCount) {
            string doctorID = "D" + to_string(doctorIndex + 1); // For the doctor ID we will use the letter D and the index + 1
            Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[docCount]->getHospitalID());  // We will create a new doctor object and pass the ID, name, and hospital ID to the constructor
            doctors[doctorID] = doctor; // Need to add the doctor to the map using the doctor ID as the key
            hospitals[docCount]->addDoctor(doctor); // We will add the doctor to the hospital using the addDoctor method
            ++doctorIndex;  // Increment the doctor index to get the next doctor name
        }
    }
    // Next we will randomly assign the rest of the doctors to any hospital (our groups choice)
    while (doctorIndex < 50) { // Generate doctors until we reach 50
        int randomHospitalIndex = rand() % numHospitals;
        string doctorID = "D" + to_string(doctorIndex + 1); // Give the rest the same ID format
        Doctor* doctor = new Doctor(doctorID, doctorNames[doctorIndex], hospitals[randomHospitalIndex]->getHospitalID());   // Create a new doctor object and pass the ID, name, and hospital ID to the constructor
        doctors[doctorID] = doctor; // Add the doctor to the map using the doctor ID as the key
        hospitals[randomHospitalIndex]->addDoctor(doctor);  // Add the doctor to the hospital using the addDoctor method
        ++doctorIndex;
    }
}

// This method will initialize the nurses, we will create 60 nurses based on the names we have in the vector
void HospitalSystem::initializeNurses() {
    // Start by clearing any existing nurses
    for (auto& clearPair : nurses) {
        delete clearPair.second;
    }
    nurses.clear();
    srand(time(nullptr)); // Seed the random number generator for randomness of assigning nurses
    // Nurse names (60 total)
    vector<string> nurseNames = {
        "Nurse Adams", "Nurse Baker", "Nurse Clark", "Nurse Davis", "Nurse Evans",
        "Nurse Foster", "Nurse Green", "Nurse Harris", "Nurse Ivy", "Nurse Jones",
        "Nurse Klein", "Nurse Lewis", "Nurse Moore", "Nurse Nelson", "Nurse Owens",
        "Nurse Peters", "Nurse Quinn", "Nurse Roberts", "Nurse Smith", "Nurse Thomas",
        "Nurse Underwood", "Nurse Vegas", "Nurse Wolfe", "Nurse Xu", "Nurse Young",
        "Nurse Zane", "Nurse Allen", "Nurse Brooks", "Nurse Carter", "Nurse Diaz",
        "Nurse Ellis", "Nurse Flores", "Nurse Graham", "Nurse Hunt", "Nurse Irwin",
        "Nurse Jenkins", "Nurse Knight", "Nurse Long", "Nurse Miller", "Nurse Neal",
        "Nurse Ortiz", "Nurse Patel", "Nurse Quinnie", "Nurse Ramirez", "Nurse Stone",
        "Nurse Tran", "Nurse Usman", "Nurse Valentine", "Nurse Washington", "Nurse Xiong",
        "Nurse Yates", "Nurse Zahra", "Nurse Bell", "Nurse Chang", "Nurse Dalton",
        "Nurse Edmonds", "Nurse Fong", "Nurse Garcia", "Nurse Harper", "Nurse Nafizz"
    };

    int nurseIndex = 0;                     // We will use this index to keep track of the nurse names
    int numHospitals = hospitals.size();    // Call the size method to get the number of hospitals and store it in a variable

    // We started with a loop to add 5 nurses per hospital (guaranteed minimum for the requirements)
    for (int nurseCount = 0; nurseCount < numHospitals; ++nurseCount) {
        for (int innerCount = 0; innerCount < 5; ++innerCount) {
            string nurseID = "N" + to_string(nurseIndex + 1);   // For the nurse ID we will use the letter N and the index + 1
            Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[nurseCount]->getHospitalID());  // Create a new nurse object and pass the ID, name, and hospital ID to the constructor
            nurses[nurseID] = nurse;    // Add the nurse to the map using the nurse ID as the key
            hospitals[nurseCount]->addNurse(nurse); // Add the nurse to the hospital using the addNurse method
            ++nurseIndex;
        }
    }
    // Next we assigned the rest of the nurses randomly to any hospital (our groups choice)
    while (nurseIndex < 60) {
        int randomHospitalIndex = rand() % numHospitals;    // Generate a random number between 0 and the number of hospitals to assign the nurse to
        string nurseID = "N" + to_string(nurseIndex + 1);   // Give the rest the same ID format
        Nurse* nurse = new Nurse(nurseID, nurseNames[nurseIndex], hospitals[randomHospitalIndex]->getHospitalID());
        nurses[nurseID] = nurse;
        hospitals[randomHospitalIndex]->addNurse(nurse);
        ++nurseIndex;
    }
}

// We created a method to admit a patient to a hospital given the patient object and the hospital index
bool HospitalSystem::admitPatient(Patient* patient, int hospitalIndex) {
    // First we need to check and make sure the hospital index is valid
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Next we need to see if the patient is already in the system 
    if (hospitals[hospitalIndex]->admitPatient(patient)) {
        patients[patient->getPatientID()] = patient;
        string assignedNurseID = patient->getAttendingNursesIDs().at(0);    // Get the first nurse ID from the patient's list of assigned nurses
        findNurse(assignedNurseID)->assignPatient(patient->getPatientID()); // Assign the patient to the nurse
        findDoctor(patient->getPrimaryDoctorID())->addPatient(patient->getPatientID()); // Add the patient to the primary doctor's list of patients
        return true;
    }
    return false;   // If the hospital couldn't admit the patient, return false
}

// This method will relocate a patient from one hospital to another
bool HospitalSystem::relocatePatient(string patientID, int newHospitalIndex) {
    // We will do the same check of seeing if the hospital index is valid
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Check if the patient exists in the system
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }
    // Check if the patient is already in the system and see if they are in the hospital
    Hospital* currentHospital = findPatientHospital(patientID);
    if (!currentHospital) {
        return false;
    }
    // Get the new hospital object at the specified index
    Hospital* newHospital = hospitals[newHospitalIndex];
    // Verify patient is not already in the target hospital because we don't want to add them again
    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        return false; // Already in this hospital
    }
    // Store the patient's data before clearing relationships so we can restore them if needed
    vector<string> oldDoctorIDs = patient->getAttendingDoctorIDs();
    vector<string> oldNurseIDs = patient->getAttendingNursesIDs();
    string oldPrimaryDoctorID = patient->getPrimaryDoctorID();

    // This method can be tricky, so we will use a try-catch block to handle any exceptions
    try {
        // We start by unassigning all doctors from the patient in the old hospital
        // Handle primary doctor
        if (!oldPrimaryDoctorID.empty()) {  // Check if the primary doctor ID is not empty
            Doctor* primaryDoctor = findDoctor(oldPrimaryDoctorID); 
            if (primaryDoctor) {    // Find the primary doctor in the system
                primaryDoctor->removePatient(patientID);    // Remove the patient from the primary doctor's list
            }
        }
        // Handle attending doctors
        for (const string& docID : oldDoctorIDs) {  // Loop through the list of attending doctors
            Doctor* doctor = findDoctor(docID);     // Find the doctor in the system
            if (doctor) {                           // If the doctor exists, remove the patient from their list
                doctor->removePatient(patientID);
            }
        }
        // Next we will unassign all nurses from the patient in the old hospital
        for (const string& nurseID : oldNurseIDs) { // Loop through the list of attending nurses
            Nurse* nurse = findNurse(nurseID);      // Find the nurse in the system
            if (nurse) {                            // If the nurse exists, remove the patient from their list
                nurse->removePatient(patientID);
            }
        }
        // Thirdly, we will clear the patient's staff assignments - temporarily, we'll restore appropriate ones later
        patient->getAttendingDoctorIDs().clear();
        patient->getAttendingNursesIDs().clear();
        patient->setPrimaryDoctorID("");

        // Then we need to remove the patient from current hospital
        if (!currentHospital->removePatient(patient)) {
            throw std::runtime_error("Failed To Remove Patient From Current Hospital!");
        }
        // Next we will add patient to new hospital
        if (!newHospital->admitPatient(patient)) {
            // If new hospital couldn't admit, re-add to original hospital
            currentHospital->admitPatient(patient);
            // Restore old relationships
            patient->setPrimaryDoctorID(oldPrimaryDoctorID);
            if (!oldPrimaryDoctorID.empty()) {
                Doctor* primaryDoctor = findDoctor(oldPrimaryDoctorID);
                if (primaryDoctor) {
                    primaryDoctor->addPatient(patientID);
                }
            }
            // Restore attending doctors and nurses
            for (const string& docID : oldDoctorIDs) {
                patient->addAttendingDoctor(docID);
                Doctor* doctor = findDoctor(docID);
                if (doctor) {
                    doctor->addPatient(patientID);
                }
            }
            for (const string& nurseID : oldNurseIDs) {
                patient->addAttendingNurse(nurseID);
                Nurse* nurse = findNurse(nurseID);
                if (nurse) {
                    nurse->assignPatient(patientID);
                }
            }
            throw std::runtime_error("Failed To Admit Patient To New Hospital!");
        }
        return true;    // Patient has been successfully relocated
    }
    catch (const std::exception& e) {   // Catch specific exceptions
        throw std::runtime_error("Error During Patient Relocation: " + string(e.what()));
    }
    catch (...) {   // Catch any other unexpected exceptions
        throw std::runtime_error("Unknown Error During Patient Relocation!!!");
    }
}

// This method will discharge a patient from the hospital
bool HospitalSystem::dischargePatient(string patientID) {
    Patient* patient = findPatient(patientID);  // Call the findPatient method to see if the patient exists in the system
    if (!patient) { // Patient doesn't exist or has already been discharged
        return false;
    }
    if (!patient->isDischargeRequested()) {
        // The doctor never requested discharge, so do not allow it
        return false;
    }
    // Otherwise, proceed with discharge
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return false;
    }
    // Actually remove the patient from the hospital
    hospital->dischargePatient(patient);
    patients.erase(patientID);
    delete patient;
    return true;
}

// This method will add a doctor to the hospital given the doctor object and the hospital index
bool HospitalSystem::addDoctor(Doctor* doctor, int hospitalIndex) {
    // Validate hospital index
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Check if there are more than 50 doctors since we have a limit of 50 doctors
    if (doctors.size() >= 50) {
        return false;
    }
    Hospital* hospitalToAssign = getHospital(hospitalIndex);    // Get the hospital object at the specified index
    hospitalToAssign->addDoctor(doctor);                        // Add the doctor to the hospital using the addDoctor method
    doctors.insert({doctor->getDoctorID(), doctor});            // Add the doctor to the map using the doctor ID as the key
    return true;
}

// This method will relocate a doctor from one hospital to another
bool HospitalSystem::relocateDoctor(string doctorID, int newHospitalIndex) {
    // Validate hospital index
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Check if the doctor exists in the system
    Doctor* doctor = findDoctor(doctorID);
    if (!doctor) {
        return false;
    }
    // Check if the doctor is already in the system and see if they are in the hospital
    Hospital* currentHospital = findDoctorHospital(doctorID);
    if (!currentHospital) {
        return false;
    }
    return currentHospital->relocateDoctor(doctor, hospitals[newHospitalIndex]);
}

// The method will remove the doctor from the system given the doctor ID
bool HospitalSystem::removeDoctor(string doctorID) {
    Doctor* doctor = findDoctor(doctorID);  // Call the findDoctor method to see if the doctor exists in the system
    if (!doctor) {
        // Doctor doesn't exist or has already been removed
        return false;
    }    
    // Check if the doctor is already in the system and see if they are in the hospital
    Hospital* currentHospital = findDoctorHospital(doctorID);
    if (!currentHospital) {
        return false;
    }
    // Check if the hospital has enough doctors before removing
    if (currentHospital->getDoctors().size() <= 3) {
        throw std::runtime_error("Cannot Remove Doctor: Hospital Must Maintain At Least 3 Doctors!");
    }
    // Check if the doctor has patients
    if (!doctor->getPatientIDs().empty()) {
        throw std::runtime_error("Cannot Remove Doctor With Assigned Patients!");
    }
    // We created a try-catch block to handle the removal of the doctor
    try {
        // Remove the doctor from the hospital
        currentHospital->removeDoctor(doctor);
        // Also remove the doctor from the system's map
        doctors.erase(doctorID);
        delete doctor;
        return true;
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Error During Doctor Removal: " + string(e.what()));
    } catch (...) {
        // Catch any other exceptions
        return false;
    }
}

// The function will add a nurse to the hospital given the nurse object and the hospital index
bool HospitalSystem::addNurse(Nurse* nurse, int hospitalIndex) {
    // Validate hospital index 
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Check if there are more than 60 nurses since we have a limit of 60 nurses
    if (nurses.size() >= 60) {
        return false;
    }
    Hospital* hospitalToAssign = getHospital(hospitalIndex);
    hospitalToAssign->addNurse(nurse);
    nurses.insert({nurse->getNurseID(), nurse});
    return true;    
}

// This method will relocate a nurse from one hospital to another
bool HospitalSystem::relocateNurse(const std::string& nurseID, int newHospitalIndex) {
    // Validate hospital index to make sure that it is a correct index
    if (newHospitalIndex < 0 || static_cast<size_t>(newHospitalIndex) >= hospitals.size()) {
        return false;
    }
    // Check if the nurse exists in the system
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false;
    }
    // Check if the nurse is already in the system and see if they are in the hospital
    Hospital* currentHospital = findNurseHospital(nurseID);
    if (!currentHospital) {
        return false;
    }
    // Check if nurse has patients
    if (!nurse->getPatientIDs().empty()) {
        return false; // Cannot relocate nurse with patients
    }
    // Remove from current hospital's nurse list
    auto& currentNurses = currentHospital->getNurses();
    currentNurses.erase(remove(currentNurses.begin(), currentNurses.end(), nurse), currentNurses.end());
    // Update nurse's hospital ID
    nurse->setHospitalID(hospitals[newHospitalIndex]->getHospitalID());
    // Add to new hospital
    hospitals[newHospitalIndex]->addNurse(nurse);
    return true;
}

// This method will remove the nurse from the system given the nurse ID
bool HospitalSystem::removeNurse(const std::string& nurseID) {
    // Check if the nurse exists in the system
    Nurse* removeNurse = findNurse(nurseID);
    if (!removeNurse) {
        return false;
    }
    // Check if the nurse is already in the system and see if they are in the hospital
    Hospital* currentHospital = findNurseHospital(nurseID);
    if (!currentHospital) {
        return false;
    }
    // Check if removing this nurse would leave fewer than 3 nurses in the hospital
    if (currentHospital->getNurses().size() <= 3) {
        return false; // We must keep at least 3 nurses
    }
    // Check if nurse has patients
    if (!removeNurse->getPatientIDs().empty()) {
        return false; // Cannot remove nurse with assigned patients
    }
    
    // Remove from current hospital's nurse list
    auto& currentNurses = const_cast<vector<Nurse*>&>(currentHospital->getNurses());
    currentNurses.erase(remove(currentNurses.begin(), currentNurses.end(), removeNurse), currentNurses.end());
    // Remove from nurses map and delete
    nurses.erase(nurseID);
    delete removeNurse;
    return true;
}

// Getters for the HospitalSystem class
Hospital* HospitalSystem::getHospital(int index) {
    if (index >= 0 && static_cast<size_t>(index) < hospitals.size()) {
        return hospitals[index];
    }
    return nullptr;
}

vector<Hospital*> HospitalSystem::getAllHospitals() {
    return hospitals;
}

map<string, Patient*>& HospitalSystem::getAllPatients() {
    return patients;
}

// This method will return a patient object based on the provided ID
Patient* HospitalSystem::findPatient(string patientID) {
    auto it = patients.find(patientID); // Find the patient in the map using the patient ID
    if (it != patients.end()) { // If the patient is found, return the pointer to the patient object
        return it->second;
    } else {
        return nullptr;
    }
}

// The function will return the hospital object where the patient is located
Hospital* HospitalSystem::findPatientHospital(string patientID) {
    Patient* patient = findPatient(patientID);  // Grab the patient object using the findPatient method and pass the ID
    if (!patient) { 
        return nullptr;
    }
    // Loop through all hospitals to find the one where the patient is located
    for (auto hospital : hospitals) {
        auto it = find(hospital->getPatients().begin(), hospital->getPatients().end(), patient);
        if (it != hospital->getPatients().end()) {  // If the patient is found in the hospital's list of patients
            return hospital;
        }
    }
    return nullptr;
}

// Need to create a method to find the hospital where the doctor is located by passing the doctor ID
Hospital* HospitalSystem::findDoctorHospital(string doctorID) {
    Doctor* doctor = findDoctor(doctorID);  // Grab the doctor object using the findDoctor method and pass the ID
    if (!doctor) {
        return nullptr; // return null if the doctor is not found
    }
    // Loop through all hospitals to find the one where the doctor is located
    for (auto hospital: hospitals) {
        for (Doctor* doc: hospital->getDoctors()) {
            if (doctor->getDoctorID() == doc->getDoctorID()) {
                return hospital;
            }
        } 
    }
    return nullptr; // return null if the hospital is not found
}

// Need to create a method to find the hospital where the nurse is located by passing the nurse ID
Hospital* HospitalSystem::findNurseHospital(string nurseID) {
    Nurse* nurse = findNurse(nurseID);  // Grab the nurse object using the findNurse method and pass the ID
    if (!nurse) {   
        return nullptr; // return null if the nurse is not found
    }
    // Loop through all hospitals to find the one where the nurse is located
    for (auto hospital : hospitals) {
        for (Nurse* nurseCount : hospital->getNurses()) {
            if (nurse->getNurseID() == nurseCount->getNurseID()) {
                return hospital;
            }
        } 
    }
    return nullptr; // return null if the hospital is not found
}

// This will return a doctor object based on the provided ID
Doctor* HospitalSystem::findDoctor(string doctorID) {
    auto it = doctors.find(doctorID);
    if (it != doctors.end()) {  // Check if the doctor is found in the map
        return it->second;
    } else {
        return nullptr; // Return null if the doctor is not found
    }
}

// This will return a nurse object based on the provided ID
Nurse* HospitalSystem::findNurse(string nurseID) {
    auto it = nurses.find(nurseID);
    if (it != nurses.end()) {   // Check if the nurse is found in the map
        return it->second;
    }
    return nullptr;
}

// This method will return the remaining balance of the patient
void HospitalSystem::updateAllPatientDays() {
    for (auto hospital : hospitals) {
        hospital->updatePatientDays();  // Go through all hospitals and call the updatePatientDays method
    }
}

// This method will collect payment from the patient
bool HospitalSystem::collectPatientPayment(string patientID, double amount) {
    Patient* patient = findPatient(patientID);  // Grab the patient object using the findPatient method and pass the ID
    if (!patient) {     // Check if the patient exists in the system
        return false;
    }
    // Check if the patient is already in the system and see if they are in the hospital
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {
        return false;
    }
    return hospital->collectPaymentFromPatient(patientID, amount);
}

// The assignDoctorToPatient method will assign a doctor to a patient and check if the doctor is already assigned to the patient
bool HospitalSystem::assignDoctorToPatient(string doctorID, string patientID, bool isPrimary) {
    Doctor* doctor = findDoctor(doctorID); // Grab the doctor object using the findDoctor method and pass the ID
    if (!doctor) {
        return false;
    }
    // Check if the doctor is already assigned to the patient
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false;
    }
    // Find which hospital the doctor is in
    Hospital* doctorHospital = nullptr;
    for (auto hospital : hospitals) {
        for (auto doc : hospital->getDoctors()) {   // Loop through the hospital's doctors
            if (doc->getDoctorID() == doctorID) {   // If the doctor ID matches, we found the hospital
                doctorHospital = hospital;          
                break;
            }
        }
        if (doctorHospital) break;  // Break out of the loop if we found the hospital
    }
    if (!doctorHospital) {
        return false; // Doctor not found in any hospital
    }

    // Find which hospital the patient is in
    Hospital* patientHospital = findPatientHospital(patientID);
    if (!patientHospital) {
        return false; // Patient not found in any hospital
    }
    // Doctors can only be assigned to patients in the same hospital
    if (doctorHospital->getHospitalID() != patientHospital->getHospitalID()) {
        return false;
    }
    // Assign doctor to patient
    doctor->addPatient(patientID);
    // Set as primary if required
    if (isPrimary) {
        patient->setPrimaryDoctorID(doctorID);
    } else {
        patient->addAttendingDoctor(doctorID);
    }
    return true;
}

// The assignNurseToPatient method will assign a nurse to a patient and check if the nurse is already assigned to the patient
bool HospitalSystem::assignNurseToPatient(string nurseID, string patientID) {
    // Find the nurse in the system using their ID
    Nurse* nurse = findNurse(nurseID);
    if (!nurse) {
        return false; // Return false if nurse not found
    }
    // Find the patient in the system using their ID
    Patient* patient = findPatient(patientID);
    if (!patient) {
        return false; // Return false if patient not found
    }
    // Check if the nurse is already assigned to this patient
    if (std::find(nurse->getPatientIDs().begin(), nurse->getPatientIDs().end(), patientID) != nurse->getPatientIDs().end()) {
        return false; // Return false if nurse is already assigned to this patient
    }
    // The nurse class will handle capacity checks (max 2 patients per nurse)
    if (!nurse->assignPatient(patientID)) {
        return false; // Return false if the nurse can't accept more patients
    }
    // Add the nurse to the patient's list of attending nurses
    patient->addAttendingNurse(nurseID);
    return true; // Return true to indicate successful assignment
}

// This method will set the primary doctor for a patient
bool HospitalSystem::setPatientPrimaryDoctor(string patientID, string doctorID) {
    return assignDoctorToPatient(doctorID, patientID, true);    // returns true if the doctor was assigned successfully
}

// We created a method to request a patient discharge from the hospital
bool HospitalSystem::requestPatientDischarge(string doctorID, string patientID) {
    // Find which hospital the doctor is in
    for (auto hospital : hospitals) {
        for (auto doctor : hospital->getDoctors()) {
            if (doctor->getDoctorID() == doctorID) {    // Check if the doctor ID matches
                return hospital->requestPatientDischarge(doctorID, patientID);  // Call the hospital's requestPatientDischarge method to request the discharge
            }
        }
    }
    return false; // Means the doctor was not found
}

// The getPatientBillingReport method will generate a billing report for all patients in the system
string HospitalSystem::getPatientBillingReport() {
    stringstream billReport;
    for (auto hospital : hospitals) {
        billReport << hospital->getPatientBillingReport() << "\n\n";
    }
    // We formatted the report using str to make it easier to read
    return billReport.str();
}

// The getHospitalStatus method will generate a status report for all hospitals in the system
string HospitalSystem::getHospitalStatus() {
    stringstream status; // Used to format the report
    // We will loop through all hospitals and get their status
    for (auto hospital : hospitals) {
        status << "Hospital: " << hospital->getHospitalName() << "\n";
        status << "Patients Admitted: " << hospital->getPatients().size() << "/20\n";
        status << "Doctors: " << hospital->getDoctors().size() << "\n";
        status << "Nurses: " << hospital->getNurses().size() << "\n\n";
        // Add detailed doctor information
        if (!hospital->getDoctors().empty()) {
            status << "--- Doctor Details ---" << endl;
            for (auto doctor : hospital->getDoctors()) {
                status << "ID: " << doctor->getDoctorID()
                << ", Name: " << doctor->getDoctorName()
                << ", Patients: " << doctor->getPatientIDs().size() << "\n";
            }
            status << "\n";
        }
        // Next we want to show the detailed outline for the nurses
        if (!hospital->getNurses().empty()) {
            status << "--- Nurse Details ---" << endl;
            for (auto nurse : hospital->getNurses()) {
                status << "ID: " << nurse->getNurseID()
                << ", Name: " << nurse->getNurseName()
                << ", Patients: " << nurse->getPatientIDs().size() << "/2\n";
            }
            status << "\n";
        }
        // We then displayed the patient information
        if (!hospital->getPatients().empty()) {
            status << "=================== PATIENT DETAILS ===================\n";
            for (auto patient : hospital->getPatients()) {
                status << "PATIENT INFORMATION\n";
                status << "Name:   \t\t" << patient->getPatientName() << "\n";
                status << "ID:   \t\t" << patient->getPatientID() << "\n";
                status << "Phone:   \t\t" << patient->getPhoneNumber() << "\n";
                status << "Days Admitted:\t " << patient->getDaysAdmitted() << "\n\n";
                status << "MEDICAL INFORMATION\n";
                status << "Diagnosis:      " << patient->getDisease() << "\n";
                status << "Treatment:      " << patient->getTreatment() << "\n\n";
                status << "MEDICAL STAFF\n";
                status << "Primary Doctor: " << patient->getPrimaryDoctorID() << "\n";
                // We will check if the patient has any doctors assigned to them and list them
                if (!patient->getAttendingDoctorIDs().empty()) {
                    status << "Attending Doctors:" << endl;
                    for (auto& docID : patient->getAttendingDoctorIDs()) {  // Loop through the list of attending doctors
                        status << "  • " << docID << endl;
                    }
                }
                // We will check if the patient has any nurses assigned to them and list them
                if (!patient->getAttendingNursesIDs().empty()) {
                    status << "Attending Nurses:" << endl;
                    for (auto& nurseID : patient->getAttendingNursesIDs()) { // Loop through the list of attending nurses
                        status << "  • " << nurseID << endl;
                    }
                }
                status << "========================================================\n";
            }
        }
    }
    return status.str();    // Return the formatted status report
}

// A method that will allow us to get the remaining balance of a patient
double HospitalSystem::getPatientRemainingBalance(string patientID) {
    Patient* patient = findPatient(patientID);  // Grab the id of the patient using the findPatient method
    if (!patient) { 
        return 0.0; // Return 0.0 if the patient is not found
    }
    
    // Calculate bill directly from patient if they exist (as a fallback)
    double directBill = patient->calculateCurrentBill();
    
    // Check if the patient is already in the system and see if they are in the hospital
    Hospital* hospital = findPatientHospital(patientID);
    if (!hospital) {    
        return directBill; // Return direct bill if hospital not found
    }
    // Check if the patient has a remaining balance at that hospital
    double hospitalBill = hospital->getPatientRemainingBalance(patientID);
    // If hospital shows no bill but the patient has days admitted, return the direct bill
    if (hospitalBill <= 0.01 && patient->getDaysAdmitted() > 0) {
        return directBill;
    }
    return hospitalBill;
}

// We have implemented the destructor for the HospitalSystem class since we are creating dynamic memory
HospitalSystem::~HospitalSystem() {
    // Delete all hospitals and their contained objects
    for (auto hospital : hospitals) {
        delete hospital;
    }
    hospitals.clear();  // Clear the vector of hospitals
    
    // Delete all patients
    for (auto& pair : patients) {
        delete pair.second;
    }
    patients.clear();
    
    // Delete all doctors
    for (auto& pair : doctors) {
        delete pair.second;
    }
    doctors.clear();
    
    // Delete all nurses
    for (auto& pair : nurses) {
        delete pair.second;
    }
    nurses.clear();
}

// Static method to properly release the singleton instance
void HospitalSystem::releaseInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}