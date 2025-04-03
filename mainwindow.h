#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// We are including the necessary libraries for the GUI from Qt framework documentation
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <QTimer>
#include <QLabel>
#include <vector>
#include <map>
#include <string>
#include "hospital.h"
#include "patient.h"
#include "hospitalsystem.h"
#include "pharmacysystem.h"

// This is the main window class for the hospital management system
// It inherits from QMainWindow and contains all the necessary UI elements and slots for functionality
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);  // This is the constructor for the MainWindow class that initializes the UI and connects signals and slots

// Private slots are used to define the slots that will be connected to signals emitted by UI elements
private slots:
    // Patient management slots
    void addPatient();
    void relocatePatient();
    void dischargePatient();
    void displayHospitalStatus();
    void displayPharmacyStatus();
    void viewPatientDetails();
    void viewPatientBillingHistory();
    void displaySelectedHospitalStatus(); 
    void listAllPatients();
    void clearStatusDisplay();

    // Doctor management slots
    void addDoctor();
    void relocateDoctor();
    void removeDoctor();
    void viewDoctorDetails();
    void listAllDoctors();

    // Nurse management slots
    void addNurse();
    void relocateNurse();
    void removeNurse();
    void viewNurseDetails();
    void listAllNurses();

    // Doctor-patient slots and Nurse-patient slots
    void assignDoctorToPatient(bool isPrimary = false);
    void assignNurseToPatient();

    // Billing slots
    void requestPatientDischarge();
    void calculateBill();
    void collectPayment();
    void showBillingReport();
    void updateDayCounter();
    void updateCurrentTime(); 
    
    // Drug management slots
    void requestDrugDelivery();
    void showPharmacyBillingReport();    

private:
    // Patient management tab
    QLineEdit* patientIDInput;
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* diseaseInput;
    QLineEdit* treatmentInput;
    QLineEdit* doctorIDInput;
    QLineEdit* nurseIDInput;
    QDoubleSpinBox* dailyRateInput;
    QComboBox* hospitalComboBox;
    QComboBox* relocateHospitalComboBox;

    // Doctor management tab
    QLineEdit* docManageIDInput;
    QLineEdit* doctorNameInput;
    QComboBox* docHospitalComboBox;
    QComboBox* docChangeHospitalComboBox;

    // Nurse management tab
    QLineEdit* nurseManageIDInput;
    QLineEdit* nurseNameInput;
    QComboBox* nurseHospitalComboBox;
    QComboBox* nurseChangeHospitalComboBox;

    // Doctor-patient tab
    QLineEdit* doctorAssignmentIDInput;
    QLineEdit* doctorPatientAssignmentIDInput;
    QLineEdit* nursePatientAssignmentIDInput;
    QLineEdit* doctorDischargeIDInput;
    QLineEdit* patientDischargeIDInput;

    //Nurse-patient tab
    QLineEdit* nurseAssignmentIDInput;
    
    // Billing tab
    QLineEdit* billingPatientIDInput;
    QLabel* currentBillLabel;
    QDoubleSpinBox* paymentAmountInput;

    //Drug tab
    QComboBox* drugHospitalComboBox;
    QComboBox* drugPharmacyComboBox;
    QComboBox* drugComboBox;
    
    // Status display
    QTextEdit* statusDisplay;
    
    // Tabs
    QTabWidget* tabWidget;
    
    // System access
    HospitalSystem* hospitalSystem;
    
    // Timer for daily updates
    QTimer* dayUpdateTimer;
    
    // Timer for time updates
    QTimer* timeUpdateTimer;
    
    // Helper method to check if a doctor works at a specific hospital
    bool isDoctorInHospital(const string& doctorID, int hospitalIndex);
    bool isNurseInHospital(const string& nurseID, int hospitalIndex);
    QComboBox* selectedHospitalComboBox; // New combo box for hospital selection
};

#endif // MAINWINDOW_H