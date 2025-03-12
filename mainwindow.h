#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <vector>
#include <map>
#include <string>
#include "hospital.h"
#include "patient.h"
#include "doctor.h"
#include "nurse.h"
#include "pharmacy.h"

using namespace std;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    
    // Provide access to pharmacies for Hospital class
    static vector<Pharmacy*>& getPharmacies();

private slots:
    void addPatient();
    void relocatePatient();
    void dischargePatient();
    void displayHospitalStatus();
    void assignDoctorToPatient();
    void assignNurseToPatient();
    void requestMedication();
    void payPharmacyBill();
    void updatePatientDays();
    void displayPharmacyStatus(); // New method to show pharmacy details
    void authorizeDischarge(); // New method to authorize discharge
    void displayPatientDetails(); // New method to show patient details
    void printPatientBill(); // New method to print patient bill
    void displayDoctorDetails(); // New method to show doctor details
    void transferDoctor(); // Transfer a doctor between hospitals
    void displayNurseDetails(); // New method to show nurse details
    void transferNurse(); // Transfer a nurse between hospitals

private:
    // Data structures
    static vector<Pharmacy*> pharmacies;  // Make static so Hospital can access
    vector<Hospital*> hospitals;
    map<string, Patient*> patients;
    map<string, Doctor*> doctors;
    map<string, Nurse*> nurses;
    
    // UI elements
    QLineEdit* patientIDInput;
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* diseaseInput;
    QLineEdit* treatmentInput;
    QLineEdit* doctorIDInput;
    QLineEdit* nurseIDInput;
    QLineEdit* medicationInput;
    QLineEdit* costInput;
    QComboBox* hospitalComboBox;
    QComboBox* pharmacyComboBox;
    QComboBox* doctorTypeComboBox;
    QTextEdit* statusDisplay;
    QPushButton* displayPharmacyStatusButton; // New button for pharmacy status
    
    // Additional UI elements for patient details
    QLineEdit* addressInput;
    QLineEdit* emergencyInput;
    QLineEdit* dobInput;
    QPushButton* authorizeDischargeButton;
    QPushButton* displayPatientButton;
    QPushButton* printBillButton;
    
    // Additional UI elements for doctor management
    QLineEdit* specializationInput;
    QPushButton* displayDoctorButton;
    QPushButton* transferDoctorButton;
    QComboBox* destinationHospitalComboBox;
    
    // Additional UI elements for nurse management
    QLineEdit* nurseSpecializationInput;
    QPushButton* displayNurseButton;
    QPushButton* transferNurseButton;
    QComboBox* nurseDestinationHospitalComboBox;
    
    // Initialization methods
    void initializeHospitals();
    void initializePharmacies();
    void initializeDoctors();
    void initializeNurses();
    
    // Helper function to check nurse counts at hospitals
    void verifyNurseDistribution();
};

#endif // MAINWINDOW_H
