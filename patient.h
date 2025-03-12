#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Patient {
private:
  string patientID;
  string name;
  string phoneNumber;
  string address;          // Added field: patient's address
  string emergencyContact; // Added field: emergency contact
  string dateOfBirth;      // Added field: date of birth
  string disease;
  string treatment;
  string hospitalID;
  string primaryDoctorID;
  vector<string> attendingDoctorIDs;
  vector<string> nurseIDs;
  vector<string> medications;
  int daysAdmitted;
  bool isReadyForDischarge; // Added field: tracks if doctor has authorized discharge
  time_t admissionDate;     // Added field: when patient was first admitted
  
  // Static member for unique ID generation
  static int nextPatientNumber;

public:
  Patient(string id, string n, string phone, string d, string t, string docID);
  
  // Enhanced constructor with more personal details
  Patient(string n, string phone, string addr, string emergency, string dob, 
          string d, string t, string docID);
  
  // Doctor management
  void setPrimaryDoctorID(const string& doctorID);
  void addAttendingDoctor(const string& doctorID);
  void removeAttendingDoctor(const string& doctorID);
  bool hasAttendingDoctor(const string& doctorID) const;
  
  // Nurse management
  void addNurse(const string& nurseID);
  void removeNurse(const string& nurseID);
  bool hasNurse(const string& nurseID) const;
  
  // Medication management
  void addMedication(const string& medication);
  
  // Hospital stay
  void incrementDaysAdmitted();
  double calculateBill(double dailyRate) const;
  
  // Discharge management
  void markReadyForDischarge(bool ready = true);
  bool getReadyForDischarge() const;
  
  // Static method for ID generation
  static string generateUniqueID();
  
  // Getters
  string getPatientID() const;
  string getName() const;
  string getPhoneNumber() const;
  string getAddress() const;
  string getEmergencyContact() const;
  string getDateOfBirth() const;
  string getDisease() const;
  string getTreatment() const;
  string getHospitalID() const;
  string getPrimaryDoctorID() const;
  vector<string> getAttendingDoctors() const;
  vector<string> getNurses() const;
  vector<string> getMedications() const;
  int getDaysAdmitted() const;
  time_t getAdmissionDate() const;
  
  // Setters
  void setHospitalID(const string& hospID);
  void setAddress(const string& addr);
  void setEmergencyContact(const string& contact);
  void setDateOfBirth(const string& dob);
  
  // Get full patient details as string
  string getFullDetails() const;
};

#endif // PATIENT_H