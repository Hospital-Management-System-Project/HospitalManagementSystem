#include "mainwindow.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QTimer>
#include "hospitalsystem.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Get the hospital system instance
    hospitalSystem = HospitalSystem::getInstance();
    
    // Set up UI
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Create a tab widget for different functionalities
    tabWidget = new QTabWidget(this);
    
    // ===== PATIENT MANAGEMENT TAB =====
    QWidget* patientTab = new QWidget(this);
    QVBoxLayout* patientLayout = new QVBoxLayout(patientTab);
    
    // Create a form layout for input fields
    QFormLayout* formLayout = new QFormLayout();

    patientIDInput = new QLineEdit(this);
    patientIDInput->setPlaceholderText("Enter Patient ID");
    formLayout->addRow("Patient ID:", patientIDInput);

    nameInput = new QLineEdit(this);
    nameInput->setPlaceholderText("Enter Patient Name");
    formLayout->addRow("Name:", nameInput);

    phoneInput = new QLineEdit(this);
    phoneInput->setPlaceholderText("Enter Phone Number");
    formLayout->addRow("Phone:", phoneInput);

    diseaseInput = new QLineEdit(this);
    diseaseInput->setPlaceholderText("Enter Disease");
    formLayout->addRow("Disease:", diseaseInput);

    treatmentInput = new QLineEdit(this);
    treatmentInput->setPlaceholderText("Enter Treatment");
    formLayout->addRow("Treatment:", treatmentInput);

    doctorIDInput = new QLineEdit(this);
    doctorIDInput->setPlaceholderText("Enter Doctor ID");
    formLayout->addRow("Doctor ID:", doctorIDInput);
    
    // Add daily rate input
    dailyRateInput = new QDoubleSpinBox(this);
    dailyRateInput->setRange(50.0, 1000.0);
    dailyRateInput->setValue(100.0);
    dailyRateInput->setPrefix("$");
    formLayout->addRow("Daily Rate:", dailyRateInput);

    // Hospital selection for admitting patients
    hospitalComboBox = new QComboBox(this);
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        hospitalComboBox->addItem(QString::fromStdString(hospital->hospitalName));
    }
    formLayout->addRow("Admit to Hospital:", hospitalComboBox);
    
    // Add a second combo box for relocation
    relocateHospitalComboBox = new QComboBox(this);
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        relocateHospitalComboBox->addItem(QString::fromStdString(hospital->hospitalName));
    }
    formLayout->addRow("Relocate to Hospital:", relocateHospitalComboBox);

    // Add buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* addPatientButton = new QPushButton("Add Patient", this);
    QPushButton* relocatePatientButton = new QPushButton("Relocate Patient", this);
    QPushButton* dischargePatientButton = new QPushButton("Discharge Patient", this);
    QPushButton* viewPatientDetailsButton = new QPushButton("View Patient Details", this);
    QPushButton* displayStatusButton = new QPushButton("Display Hospital Status", this);
    QPushButton* displayPharmacyButton = new QPushButton("Display Pharmacy Status", this);
    
    buttonLayout->addWidget(addPatientButton);
    buttonLayout->addWidget(relocatePatientButton);
    buttonLayout->addWidget(dischargePatientButton);
    buttonLayout->addWidget(viewPatientDetailsButton);
    buttonLayout->addWidget(displayStatusButton);
    buttonLayout->addWidget(displayPharmacyButton);

    // Add patient management widgets to the layout
    patientLayout->addLayout(formLayout);
    patientLayout->addLayout(buttonLayout);
    patientTab->setLayout(patientLayout);
    
    // ===== DOCTOR-PATIENT TAB =====
    QWidget* doctorPatientTab = new QWidget(this);
    QVBoxLayout* doctorPatientLayout = new QVBoxLayout(doctorPatientTab);
    
    QFormLayout* doctorFormLayout = new QFormLayout();
    
    doctorAssignmentIDInput = new QLineEdit(this);
    doctorAssignmentIDInput->setPlaceholderText("Enter Doctor ID");
    doctorFormLayout->addRow("Doctor ID:", doctorAssignmentIDInput);
    
    patientAssignmentIDInput = new QLineEdit(this);
    patientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    doctorFormLayout->addRow("Patient ID:", patientAssignmentIDInput);
    
    // Add the List All Patients button before doctor assignment buttons
    QPushButton* listPatientsButton = new QPushButton("List All Patients", this);
    
    // Add assignment buttons in their own layout
    QHBoxLayout* doctorButtonLayout = new QHBoxLayout();
    QPushButton* assignDoctorButton = new QPushButton("Assign Doctor", this);
    QPushButton* setPrimaryDoctorButton = new QPushButton("Set as Primary Doctor", this);
    doctorButtonLayout->addWidget(assignDoctorButton);
    doctorButtonLayout->addWidget(setPrimaryDoctorButton);
    
    // Add the form layout and buttons to the main layout
    doctorPatientLayout->addLayout(doctorFormLayout);
    doctorPatientLayout->addWidget(listPatientsButton); // Place list button above doctor assignment buttons
    doctorPatientLayout->addLayout(doctorButtonLayout);
    
    // Discharge request section
    QGroupBox* dischargeGroupBox = new QGroupBox("Request Patient Discharge", this);
    QFormLayout* dischargeFormLayout = new QFormLayout();
    
    doctorDischargeIDInput = new QLineEdit(this);
    doctorDischargeIDInput->setPlaceholderText("Doctor requesting discharge");
    dischargeFormLayout->addRow("Doctor ID:", doctorDischargeIDInput);
    
    patientDischargeIDInput = new QLineEdit(this);
    patientDischargeIDInput->setPlaceholderText("Patient to discharge");
    dischargeFormLayout->addRow("Patient ID:", patientDischargeIDInput);
    
    QPushButton* requestDischargeButton = new QPushButton("Request Discharge", this);
    dischargeFormLayout->addRow("", requestDischargeButton);
    dischargeGroupBox->setLayout(dischargeFormLayout);
    
    doctorPatientLayout->addLayout(doctorFormLayout);
    doctorPatientLayout->addLayout(doctorButtonLayout);
    doctorPatientLayout->addWidget(dischargeGroupBox);
    doctorPatientTab->setLayout(doctorPatientLayout);
    
    // ===== BILLING TAB =====
    QWidget* billingTab = new QWidget(this);
    QVBoxLayout* billingLayout = new QVBoxLayout(billingTab);
    
    QFormLayout* billingFormLayout = new QFormLayout();
    
    billingPatientIDInput = new QLineEdit(this);
    billingPatientIDInput->setPlaceholderText("Enter Patient ID");
    billingFormLayout->addRow("Patient ID:", billingPatientIDInput);
    
    currentBillLabel = new QLabel("$0.00", this);
    billingFormLayout->addRow("Current Bill:", currentBillLabel);
    
    paymentAmountInput = new QDoubleSpinBox(this);
    paymentAmountInput->setRange(0.0, 10000.0);
    paymentAmountInput->setPrefix("$");
    billingFormLayout->addRow("Payment Amount:", paymentAmountInput);
    
    QHBoxLayout* billingButtonLayout = new QHBoxLayout();
    QPushButton* calculateBillButton = new QPushButton("Calculate Bill", this);
    QPushButton* collectPaymentButton = new QPushButton("Collect Payment", this);
    QPushButton* viewBillingButton = new QPushButton("View Billing History", this);
    QPushButton* billingReportButton = new QPushButton("Billing Report", this);
    
    billingButtonLayout->addWidget(calculateBillButton);
    billingButtonLayout->addWidget(collectPaymentButton);
    billingButtonLayout->addWidget(viewBillingButton);
    billingButtonLayout->addWidget(billingReportButton);
    
    billingLayout->addLayout(billingFormLayout);
    billingLayout->addLayout(billingButtonLayout);
    billingTab->setLayout(billingLayout);
    
    // Add the tabs to the tab widget
    tabWidget->addTab(patientTab, "Patient Management");
    tabWidget->addTab(doctorPatientTab, "Doctor-Patient");
    tabWidget->addTab(billingTab, "Billing");
    
    // Add status display
    statusDisplay = new QTextEdit(this);
    statusDisplay->setReadOnly(true);

    // Add widgets to the main layout
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(statusDisplay);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital Management System");
    resize(800, 600);

    // Connect buttons to slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(relocatePatientButton, &QPushButton::clicked, this, &MainWindow::relocatePatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &MainWindow::dischargePatient);
    connect(displayStatusButton, &QPushButton::clicked, this, &MainWindow::displayHospitalStatus);
    connect(displayPharmacyButton, &QPushButton::clicked, this, &MainWindow::displayPharmacyStatus);
    connect(viewPatientDetailsButton, &QPushButton::clicked, this, &MainWindow::viewPatientDetails);
    connect(viewBillingButton, &QPushButton::clicked, this, &MainWindow::viewPatientBillingHistory);
    connect(assignDoctorButton, &QPushButton::clicked, this, &MainWindow::assignDoctorToPatient);
    connect(setPrimaryDoctorButton, &QPushButton::clicked, this, [this]() {
        // Lambda to call assignDoctorToPatient with isPrimary=true
        this->assignDoctorToPatient(true);
    });
    connect(requestDischargeButton, &QPushButton::clicked, this, &MainWindow::requestPatientDischarge);
    connect(calculateBillButton, &QPushButton::clicked, this, &MainWindow::calculateBill);
    connect(collectPaymentButton, &QPushButton::clicked, this, &MainWindow::collectPayment);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(listPatientsButton, &QPushButton::clicked, this, &MainWindow::listAllPatients);
    
    // Set up timer for daily updates
    dayUpdateTimer = new QTimer(this);
    connect(dayUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDayCounter);
    dayUpdateTimer->start(60000); // Update every minute (simulation)
}

// Add this helper method to check if a doctor belongs to a specific hospital
bool MainWindow::isDoctorInHospital(const string& doctorID, int hospitalIndex) {
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        return false;
    }
    
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {
        return false;
    }
    
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        return false;
    }
    
    // Check if the doctor's hospital ID matches the selected hospital's ID
    return doctor->hospitalID == hospital->hospitalID;
}

void MainWindow::addPatient() {
    string patientID = patientIDInput->text().toStdString();
    string name = nameInput->text().toStdString();
    string phone = phoneInput->text().toStdString();
    string disease = diseaseInput->text().toStdString();
    string treatment = treatmentInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    double dailyRate = dailyRateInput->value();
    int hospitalIndex = hospitalComboBox->currentIndex();
    
    // Validate inputs
    if (patientID.empty() || name.empty() || phone.empty() || 
        disease.empty() || treatment.empty() || doctorID.empty()) {
        statusDisplay->append("Error: All fields must be filled out.");
        return;
    }
    
    // Check if patient already exists
    if (hospitalSystem->findPatient(patientID)) {
        statusDisplay->append("Error: Patient ID already exists.");
        return;
    }
    
    // Check if the doctor works at the selected hospital
    if (!isDoctorInHospital(doctorID, hospitalIndex)) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(doctorID) + 
                              " does not work at " + hospitalComboBox->currentText() + 
                              ". Please choose a doctor from this hospital.");
        return;
    }
    
    Patient* patient = new Patient(patientID, name, phone, disease, treatment, doctorID);
    patient->setDailyRate(dailyRate); // Make sure this actually sets the rate
    
    if (hospitalSystem->admitPatient(patient, hospitalIndex)) {
        statusDisplay->append("Patient admitted successfully to " + 
                             hospitalComboBox->currentText());
        statusDisplay->append("Daily billing rate set to: $" + QString::number(dailyRate, 'f', 2));
        statusDisplay->append("Initial bill: $" + QString::number(dailyRate, 'f', 2) + " (charged immediately)");
        
        // Clear input fields
        patientIDInput->clear();
        nameInput->clear();
        phoneInput->clear();
        diseaseInput->clear();
        treatmentInput->clear();
        doctorIDInput->clear();
    } else {
        statusDisplay->append("Hospital is full. Cannot admit patient.");
        delete patient;
    }
}

void MainWindow::relocatePatient() {
    string patientID = patientIDInput->text().toStdString();
    
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to relocate.");
        return;
    }

    int newHospitalIndex = relocateHospitalComboBox->currentIndex();
    
    if (hospitalSystem->relocatePatient(patientID, newHospitalIndex)) {
        statusDisplay->append("Patient " + QString::fromStdString(patientID) + 
                             " relocated successfully to " + 
                             relocateHospitalComboBox->currentText());
    } else {
        statusDisplay->append("Failed to relocate patient. Either patient not found or destination hospital is full.");
    }
}

void MainWindow::dischargePatient() {
    string patientID = patientIDInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to discharge.");
        return;
    }
    if (hospitalSystem->dischargePatient(patientID)) {
        statusDisplay->append("Patient " + QString::fromStdString(patientID) + 
                             " discharged successfully.");
    } else {
        statusDisplay->append("Patient not found.");
    }
}

void MainWindow::displayHospitalStatus() {
    statusDisplay->clear();
    statusDisplay->append("=== HOSPITAL SYSTEM STATUS ===\n");
    statusDisplay->append(QString::fromStdString(hospitalSystem->getHospitalStatus()));
}

void MainWindow::displayPharmacyStatus() {
    statusDisplay->clear();
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    statusDisplay->append(QString::fromStdString(pharmacySystem->getPharmacyStatus()));
}

// Add this new method to view individual patient details
void MainWindow::viewPatientDetails() {
    string patientID = patientIDInput->text().toStdString();
    
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to view details.");
        return;
    }
    
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Patient not found.");
        return;
    }
    
    Hospital* hospital = hospitalSystem->findPatientHospital(patientID);
    
    // Get the remaining balance
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    
    statusDisplay->clear();
    statusDisplay->append("=== PATIENT DETAILS ===");
    statusDisplay->append("Patient ID: " + QString::fromStdString(patient->patientID));
    statusDisplay->append("Name: " + QString::fromStdString(patient->patientName));
    statusDisplay->append("Phone Number: " + QString::fromStdString(patient->phoneNumber));
    statusDisplay->append("Disease: " + QString::fromStdString(patient->disease));
    statusDisplay->append("Treatment: " + QString::fromStdString(patient->treatment));
    statusDisplay->append("Days Admitted: " + QString::number(patient->daysAdmitted));
    statusDisplay->append("Admission Date: " + QString::fromStdString(patient->getAdmissionDateString()));
    statusDisplay->append("Status: " + QString::fromStdString(patient->getStatus()));
    
    // Show billing information prominently
    statusDisplay->append("\n=== BILLING INFORMATION ===");
    statusDisplay->append("Daily Rate: $" + QString::number(patient->billingRatePerDay, 'f', 2));
    
    if (patient->daysAdmitted == 0) {
        statusDisplay->append("Current Bill: $" + QString::number(patient->billingRatePerDay, 'f', 2) + " (Initial day charge)");
        statusDisplay->append("Remaining Balance: $" + QString::number(remainingBalance, 'f', 2));
        
        // Add explanation about billing
        statusDisplay->append("\nNote: Patients are charged the daily rate ($" + 
                             QString::number(patient->billingRatePerDay, 'f', 2) + 
                             ") immediately upon admission. Additional days will increase the bill.");
    } else {
        statusDisplay->append("Current Bill: $" + QString::number(patient->calculateCurrentBill(), 'f', 2));
        statusDisplay->append("Remaining Balance: $" + QString::number(remainingBalance, 'f', 2));
    }
    
    statusDisplay->append("\n=== HOSPITAL & DOCTOR INFO ===");
    statusDisplay->append("Hospital: " + QString::fromStdString(hospital ? hospital->hospitalName : "Unknown"));
    statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->primaryDoctorID));
    
    if (!patient->attendingDoctorIDs.empty()) {
        statusDisplay->append("\nAttending Doctors:");
        for (const auto& docID : patient->attendingDoctorIDs) {
            statusDisplay->append(" - " + QString::fromStdString(docID));
        }
    }
}

void MainWindow::viewPatientBillingHistory() {
    string patientID = billingPatientIDInput->text().toStdString();
    
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to view billing history.");
        return;
    }
    
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Patient not found.");
        return;
    }
    
    Hospital* hospital = hospitalSystem->findPatientHospital(patientID);
    if (!hospital) {
        statusDisplay->append("Hospital not found for this patient.");
        return;
    }
    
    statusDisplay->clear();
    statusDisplay->append("=== PATIENT BILLING HISTORY ===");
    statusDisplay->append("Patient ID: " + QString::fromStdString(patient->patientID));
    statusDisplay->append("Name: " + QString::fromStdString(patient->patientName));
    statusDisplay->append("Days Admitted: " + QString::number(patient->daysAdmitted));
    statusDisplay->append("Daily Rate: $" + QString::number(patient->billingRatePerDay, 'f', 2));
    statusDisplay->append("Current Total: $" + QString::number(patient->calculateCurrentBill(), 'f', 2));
    statusDisplay->append("Status: " + QString::fromStdString(patient->getStatus()));
    statusDisplay->append("\nNote: Additional charges for medications and services may apply.");
}

void MainWindow::assignDoctorToPatient(bool isPrimary) {
    string doctorID = doctorAssignmentIDInput->text().toStdString();
    string patientID = patientAssignmentIDInput->text().toStdString();
    
    if (doctorID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Doctor ID and Patient ID are required.");
        return;
    }
    
    // Find which hospital the patient is in
    Hospital* patientHospital = hospitalSystem->findPatientHospital(patientID);
    if (!patientHospital) {
        statusDisplay->append("Error: Patient not found in any hospital.");
        return;
    }
    
    // Check if the doctor works at the patient's hospital
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {
        statusDisplay->append("Error: Doctor ID not found.");
        return;
    }
    
    if (doctor->hospitalID != patientHospital->hospitalID) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(doctorID) + 
                             " does not work at " + QString::fromStdString(patientHospital->hospitalName) + 
                             " where the patient is admitted.");
        return;
    }
    
    // Continue with assignment
    if (isPrimary) {
        if (hospitalSystem->setPatientPrimaryDoctor(patientID, doctorID)) {
            statusDisplay->append("Set doctor " + QString::fromStdString(doctorID) + 
                                " as primary doctor for patient " + 
                                QString::fromStdString(patientID));
        } else {
            statusDisplay->append("Failed to set primary doctor. Check IDs and try again.");
        }
    } else {
        // Explicitly provide the third parameter to resolve ambiguity
        if (hospitalSystem->assignDoctorToPatient(doctorID, patientID, false)) {
            statusDisplay->append("Assigned doctor " + QString::fromStdString(doctorID) + 
                                " to patient " + QString::fromStdString(patientID));
        } else {
            statusDisplay->append("Failed to assign doctor. Check IDs and try again.");
        }
    }
}

void MainWindow::requestPatientDischarge() {
    string doctorID = doctorDischargeIDInput->text().toStdString();
    string patientID = patientDischargeIDInput->text().toStdString();
    
    if (doctorID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Doctor ID and Patient ID are required.");
        return;
    }
    
    if (hospitalSystem->requestPatientDischarge(doctorID, patientID)) {
        statusDisplay->append("Discharge request approved. Patient " + 
                            QString::fromStdString(patientID) + " is now discharged.");
    } else {
        statusDisplay->append("Discharge request denied. Either the doctor is not authorized " +
                            QString("or the patient ID is invalid."));
    }
}

void MainWindow::calculateBill() {
    string patientID = billingPatientIDInput->text().toStdString();
    
    if (patientID.empty()) {
        statusDisplay->append("Error: Patient ID is required.");
        return;
    }
    
    // Get remaining balance instead of full bill
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    currentBillLabel->setText(QString("$%1").arg(remainingBalance, 0, 'f', 2));
    
    statusDisplay->append("Remaining balance for patient " + QString::fromStdString(patientID) + 
                        " is $" + QString::number(remainingBalance, 'f', 2));
}

void MainWindow::collectPayment() {
    string patientID = billingPatientIDInput->text().toStdString();
    double amount = paymentAmountInput->value();
    
    if (patientID.empty()) {
        statusDisplay->append("Error: Patient ID is required.");
        return;
    }
    
    if (amount <= 0) {
        statusDisplay->append("Error: Payment amount must be greater than zero.");
        return;
    }
    
    // Get the current remaining balance
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    
    // Allow a small floating-point difference (0.01) for comparing doubles
    if (amount > remainingBalance + 0.01) {
        statusDisplay->append("Error: Payment amount ($" + QString::number(amount, 'f', 2) + 
                             ") exceeds remaining balance ($" + QString::number(remainingBalance, 'f', 2) + ")");
        return;
    }
    
    // If the amount is very close to the remaining balance, adjust it to be exact
    if (fabs(amount - remainingBalance) < 0.01) {
        amount = remainingBalance;
    }
    
    if (hospitalSystem->collectPatientPayment(patientID, amount)) {
        // Show payment collected
        statusDisplay->append("Payment of $" + QString::number(amount, 'f', 2) + 
                            " collected from patient " + QString::fromStdString(patientID));
        
        // Calculate new remaining balance
        double newBalance = remainingBalance - amount;
        if (newBalance < 0.01) newBalance = 0.0; // Avoid tiny remaining amounts
        
        // Update display with remaining amount
        statusDisplay->append("Remaining balance: $" + QString::number(newBalance, 'f', 2));
        
        // Update the bill label
        currentBillLabel->setText(QString("$%1").arg(newBalance, 0, 'f', 2));
        
        // Reset payment input
        paymentAmountInput->setValue(0.0);
        
        // If paid in full, show a congratulatory message
        if (newBalance < 0.01) {
            statusDisplay->append("Bill paid in full. Thank you!");
        }
    } else {
        statusDisplay->append("Payment failed. Check patient ID and try again.");
    }
}

void MainWindow::showBillingReport() {
    statusDisplay->clear();
    statusDisplay->append("=== HOSPITAL BILLING REPORT ===\n");
    statusDisplay->append(QString::fromStdString(hospitalSystem->getPatientBillingReport()));
}

void MainWindow::updateDayCounter() {
    // This is called periodically to simulate the passage of time
    hospitalSystem->updateAllPatientDays();
    
    // If the current tab is the billing tab, update the displayed bill
    if (tabWidget->currentIndex() == 2) { // Billing tab is index 2
        string patientID = billingPatientIDInput->text().toStdString();
        if (!patientID.empty()) {
            calculateBill();
        }
    }
}

// Implementation of the new method to list all patients
void MainWindow::listAllPatients() {
    statusDisplay->clear();
    statusDisplay->append("=== CURRENT PATIENTS LIST ===\n");
    
    map<string, Patient*>& allPatients = hospitalSystem->getAllPatients();
    
    if (allPatients.empty()) {
        statusDisplay->append("No patients currently admitted.");
        return;
    }
    
    // Group patients by hospital for better organization
    map<string, vector<Patient*>> patientsByHospital;
    
    for (const auto& pair : allPatients) {
        Patient* patient = pair.second;
        Hospital* hospital = hospitalSystem->findPatientHospital(patient->patientID);
        
        if (hospital) {
            patientsByHospital[hospital->hospitalName].push_back(patient);
        }
    }
    
    // Display patients grouped by hospital
    for (const auto& pair : patientsByHospital) {
        statusDisplay->append("\n--- " + QString::fromStdString(pair.first) + " ---");
        
        for (const Patient* patient : pair.second) {
            statusDisplay->append("Patient ID: " + QString::fromStdString(patient->patientID));
            statusDisplay->append("Name: " + QString::fromStdString(patient->patientName));
            statusDisplay->append("Disease: " + QString::fromStdString(patient->disease));
            statusDisplay->append("Days Admitted: " + QString::number(patient->daysAdmitted));
            statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->primaryDoctorID));
            
            // Show attending doctors if any
            if (!patient->attendingDoctorIDs.empty()) {
                QString attendingDocs = "Attending Doctors: ";
                for (size_t i = 0; i < patient->attendingDoctorIDs.size(); i++) {
                    attendingDocs += QString::fromStdString(patient->attendingDoctorIDs[i]);
                    if (i < patient->attendingDoctorIDs.size() - 1) {
                        attendingDocs += ", ";
                    }
                }
                statusDisplay->append(attendingDocs);
            }
            
            // Show a separator between patients
            statusDisplay->append("------------------------");
        }
    }
    
    // Show count at the end
    statusDisplay->append("\nTotal patients: " + QString::number(allPatients.size()));
}