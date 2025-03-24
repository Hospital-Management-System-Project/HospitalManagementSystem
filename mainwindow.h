#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QTabWidget>
#include <vector>
#include <map>
#include <string>
#include "hospital.h"
#include "patient.h"
#include "hospitalsystem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void addPatient();
    void relocatePatient();
    void dischargePatient();
    void displayHospitalStatus();
    void viewPatientDetails(); // Add this slot
    void assignDoctorToPatient();
    void requestPatientDischarge();
    void calculateBill();
    void collectPayment();
    void showBillingReport();
    void updateDayCounter();

private:
    // Patient management tab
    QLineEdit* patientIDInput;
    QLineEdit* nameInput;
    QLineEdit* phoneInput;
    QLineEdit* diseaseInput;
    QLineEdit* treatmentInput;
    QLineEdit* doctorIDInput;
    QDoubleSpinBox* dailyRateInput;
    QComboBox* hospitalComboBox;
    QComboBox* relocateHospitalComboBox;
    
    // Doctor-patient tab
    QLineEdit* doctorAssignmentIDInput;
    QLineEdit* patientAssignmentIDInput;
    QLineEdit* doctorDischargeIDInput;
    QLineEdit* patientDischargeIDInput;
    
    // Billing tab
    QLineEdit* billingPatientIDInput;
    QLabel* currentBillLabel;
    QDoubleSpinBox* paymentAmountInput;
    
    // Status display
    QTextEdit* statusDisplay;
    
    // Tabs
    QTabWidget* tabWidget;
    
    // System access
    HospitalSystem* hospitalSystem;
    
    // Timer for daily updates
    QTimer* dayUpdateTimer;
};

#endif // MAINWINDOW_H
