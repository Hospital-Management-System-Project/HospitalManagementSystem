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

private:
    // Data structures
    vector<Hospital*> hospitals;
    vector<Pharmacy*> pharmacies;
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
    
    // Initialization methods
    void initializeHospitals();
    void initializePharmacies();
    void initializeDoctors();
    void initializeNurses();
};

#endif // MAINWINDOW_H
