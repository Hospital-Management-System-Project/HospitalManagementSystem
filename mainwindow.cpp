#include "mainwindow.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGroupBox>
#include <QLabel>
#include <QTabWidget>
#include <QTimer>
#include <QTime>
#include <QIcon>
#include "hospitalsystem.h"
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Get the hospital system instance
    hospitalSystem = HospitalSystem::getInstance();
    
    // Set the window icon
    QIcon icon(":/images/hospital_icon.png");
    if (!icon.isNull()) {
        setWindowIcon(icon);
        QApplication::setWindowIcon(icon); // Also set application-wide icon
    } else {
        qDebug("Failed to load icon image");
    }
    
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
    doctorIDInput->setPlaceholderText("Enter Primary Doctor ID");
    formLayout->addRow("Primary Doctor ID:", doctorIDInput);

    nurseIDInput = new QLineEdit(this);
    nurseIDInput->setPlaceholderText("Enter Nurse ID");
    formLayout->addRow("Nurse ID:", nurseIDInput);
    
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
    
    doctorPatientAssignmentIDInput = new QLineEdit(this);
    doctorPatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    doctorFormLayout->addRow("Patient ID:", doctorPatientAssignmentIDInput);
    
    // Add the List All Patients button before doctor assignment buttons
    QPushButton* doctorListPatientsButton = new QPushButton("List All Patients", this);
    QPushButton* nurseListPatientsButton = new QPushButton("List All Patients", this);

    // Add assignment buttons in their own layout
    QHBoxLayout* doctorButtonLayout = new QHBoxLayout();
    QPushButton* assignDoctorButton = new QPushButton("Assign Doctor", this);
    QPushButton* setPrimaryDoctorButton = new QPushButton("Set as Primary Doctor", this);
    doctorButtonLayout->addWidget(assignDoctorButton);
    doctorButtonLayout->addWidget(setPrimaryDoctorButton);
    
    // Add the form layout and buttons to the main layout
    doctorPatientLayout->addLayout(doctorFormLayout);
    doctorPatientLayout->addWidget(doctorListPatientsButton); // Place list button above doctor assignment buttons
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

    doctorPatientLayout->addWidget(dischargeGroupBox);
    doctorPatientTab->setLayout(doctorPatientLayout);

    // ===== NURSE-PATIENT TAB =====

    QWidget* nursePatientTab = new QWidget(this);
    QVBoxLayout* nursePatientLayout = new QVBoxLayout(nursePatientTab);

    QFormLayout* nurseFormLayout = new QFormLayout();
    
    nurseAssignmentIDInput = new QLineEdit(this);
    nurseAssignmentIDInput->setPlaceholderText("Enter Nurse ID");
    nurseFormLayout->addRow("Nurse ID:", nurseAssignmentIDInput);

    nursePatientAssignmentIDInput = new QLineEdit(this);
    nursePatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    nurseFormLayout->addRow("Patient ID:", nursePatientAssignmentIDInput);

    // Add assignment buttons in their own layout
    QHBoxLayout* nurseButtonLayout = new QHBoxLayout();
    QPushButton* assignNurseButton = new QPushButton("Assign Nurse", this);
    nurseButtonLayout->addWidget(assignNurseButton);
    

    nursePatientLayout->addLayout(nurseFormLayout);
    nursePatientLayout->addLayout(nurseButtonLayout);
    nursePatientTab->setLayout(nursePatientLayout);
    nursePatientLayout->addWidget(nurseListPatientsButton); // Place list button above doctor assignment buttons
    
    // ===== BILLING TAB =====
    QWidget* billingTab = new QWidget(this);
    QVBoxLayout* billingLayout = new QVBoxLayout(billingTab);

    // Patient ID input
    QFormLayout* billingFormLayout = new QFormLayout();
    billingPatientIDInput = new QLineEdit(this);
    billingPatientIDInput->setPlaceholderText("Enter Patient ID");
    billingFormLayout->addRow("Patient ID:", billingPatientIDInput);

    // Add current bill display
    currentBillLabel = new QLabel("$0.00", this);
    currentBillLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    billingFormLayout->addRow("Current Bill:", currentBillLabel);
    
    // Add payment amount input
    paymentAmountInput = new QDoubleSpinBox(this);
    paymentAmountInput->setRange(0.0, 10000.0);
    paymentAmountInput->setPrefix("$");
    paymentAmountInput->setValue(0.0);
    billingFormLayout->addRow("Payment Amount:", paymentAmountInput);

    // Add button layout for multiple buttons
    QHBoxLayout* billingButtonLayout = new QHBoxLayout();
    QPushButton* calculateBillButton = new QPushButton("Calculate Bill", this);
    QPushButton* collectPaymentButton = new QPushButton("Collect Payment", this);
    QPushButton* billingReportButton = new QPushButton("Generate Billing Report", this);
    QPushButton* pharmacyBillingButton = new QPushButton("Generate Pharmacy Billing", this);
    
    billingButtonLayout->addWidget(calculateBillButton);
    billingButtonLayout->addWidget(collectPaymentButton);
    
    // Add all the components to the main billing layout
    billingLayout->addLayout(billingFormLayout);
    billingLayout->addLayout(billingButtonLayout);
    billingLayout->addWidget(billingReportButton);
    billingLayout->addWidget(pharmacyBillingButton);
    
    billingTab->setLayout(billingLayout);

    // Connect the buttons
    connect(calculateBillButton, &QPushButton::clicked, this, &MainWindow::calculateBill);
    connect(collectPaymentButton, &QPushButton::clicked, this, &MainWindow::collectPayment);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(pharmacyBillingButton, &QPushButton::clicked, this, &MainWindow::showPharmacyBillingReport);
    
        // ===== DRUG DELIVERY TAB =====
    QWidget* drugDeliveryTab = new QWidget(this);
    QVBoxLayout* drugDeliveryLayout = new QVBoxLayout(drugDeliveryTab);

    QFormLayout* drugFormLayout = new QFormLayout();

    drugHospitalComboBox = new QComboBox(this);
    auto allHospitals = hospitalSystem->getAllHospitals();
    for (auto hospital : allHospitals) {
        drugHospitalComboBox->addItem(QString::fromStdString(hospital->hospitalName));
    }
    drugFormLayout->addRow("Select Hospital:", drugHospitalComboBox);

    drugPharmacyComboBox = new QComboBox(this);
    auto allPharmacies = PharmacySystem::getInstance()->getAllPharmacies();
    for (auto pharmacy : allPharmacies) {
        drugPharmacyComboBox->addItem(
            QString::fromStdString(pharmacy->parmacyName + " (" + pharmacy->pharmacyID + ")")
            );
    }
    drugFormLayout->addRow("Select Pharmacy:", drugPharmacyComboBox);

    drugComboBox = new QComboBox(this);
    auto allDrugs = PharmacySystem::getInstance()->getAllDrugs();
    for (auto &d : allDrugs) {
        drugComboBox->addItem(QString::fromStdString(d.drugName));
    }
    drugFormLayout->addRow("Select Drug:", drugComboBox);

    QPushButton* requestDeliveryButton = new QPushButton("Request Delivery", this);
    drugFormLayout->addRow("", requestDeliveryButton);

    drugDeliveryLayout->addLayout(drugFormLayout);
    drugDeliveryTab->setLayout(drugDeliveryLayout);

    connect(requestDeliveryButton, &QPushButton::clicked, this, &MainWindow::requestDrugDelivery);

    // Add the tabs to the tab widget
    tabWidget->addTab(patientTab, "Patient Management");
    tabWidget->addTab(doctorPatientTab, "Doctor-Patient");
    tabWidget->addTab(nursePatientTab, "Nurse-Patient");
    tabWidget->addTab(billingTab, "Billing");
    tabWidget->addTab(drugDeliveryTab, "Drug Delivery");
    
    // Create status display
    statusDisplay = new QTextEdit(this);
    statusDisplay->setReadOnly(true);

    // Add widgets to the main layout
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(statusDisplay);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital Management System");

    resize(900, 700);

    // Connect buttons to slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(relocatePatientButton, &QPushButton::clicked, this, &MainWindow::relocatePatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &MainWindow::dischargePatient);
    connect(displayStatusButton, &QPushButton::clicked, this, &MainWindow::displayHospitalStatus);
    connect(displayPharmacyButton, &QPushButton::clicked, this, &MainWindow::displayPharmacyStatus);
    connect(viewPatientDetailsButton, &QPushButton::clicked, this, &MainWindow::viewPatientDetails);
    connect(assignDoctorButton, &QPushButton::clicked, this, &MainWindow::assignDoctorToPatient);
    connect(setPrimaryDoctorButton, &QPushButton::clicked, this, [this]() {
        // Lambda to call assignDoctorToPatient with isPrimary=true
        this->assignDoctorToPatient(true);
    });
    connect(requestDischargeButton, &QPushButton::clicked, this, &MainWindow::requestPatientDischarge);
    connect(assignNurseButton, &QPushButton::clicked, this, &MainWindow::assignNurseToPatient);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(doctorListPatientsButton, &QPushButton::clicked, this, &MainWindow::listAllPatients);
    connect(nurseListPatientsButton, &QPushButton::clicked, this, &MainWindow::listAllPatients);

    // Set up timer for daily updates
    dayUpdateTimer = new QTimer(this);
    connect(dayUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDayCounter);
    dayUpdateTimer->start(86400000); // 24 hours in milliseconds
    hospitalSystem->updateAllPatientDays(); // Initial update    
    QTime currentTime = QTime::currentTime();
    statusDisplay->append("Current Time: " + currentTime.toString());
    statusDisplay->append("System initialized. Daily updates will occur every 24 hours.");
    statusDisplay->append("Welcome to the Hospital Management System!");
    statusDisplay->append("---------------------------------------------");

    QPushButton* clearDisplayButton = new QPushButton("Clear Display", this);
    clearDisplayButton->setStyleSheet(R"(
        QPushButton {
            background-color: #e74c3c;
            color: white;
            padding: 6px 12px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #c0392b;
        }
    )");
    mainLayout->addWidget(clearDisplayButton);
    connect(clearDisplayButton, &QPushButton::clicked, this, &MainWindow::clearStatusDisplay);

    qApp->setStyleSheet(R"(
    QWidget {
            background-color: #2b2b2b;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }

        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit {
            background-color: #3c3f41;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 4px;
        }

        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            padding: 6px 12px;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #357ABD;
        }

        QLabel {
            font-weight: bold;
        }

        QTabWidget::pane {
            border: 1px solid #555;
            padding: 6px;
        }

        QTabBar::tab {
            background: #3a3a3a;
            padding: 8px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background: #4a90e2;
            color: white;
        }
    )");
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
    return doctor->getHospitalID() == hospital->hospitalID;
}

bool MainWindow::isNurseInHospital(const string& nurseID, int hospitalIndex) {
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        return false;
    }
    
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        return false;
    }
    
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        return false;
    }
    
    // Check if the nurse's hospital ID matches the selected hospital's ID
    return nurse->hospitalID == hospital->hospitalID;
}

void MainWindow::addPatient() {
    string patientID = patientIDInput->text().toStdString();
    string name = nameInput->text().toStdString();
    string phone = phoneInput->text().toStdString();
    string disease = diseaseInput->text().toStdString();
    string treatment = treatmentInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    string nurseID = nurseIDInput->text().toStdString();
    double dailyRate = dailyRateInput->value();
    int hospitalIndex = hospitalComboBox->currentIndex();
    
    // Validate inputs
    if (patientID.empty() || name.empty() || phone.empty() || 
        disease.empty() || treatment.empty() || doctorID.empty() || nurseID.empty()) {
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

    if (!isNurseInHospital(nurseID, hospitalIndex)) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(nurseID) + 
                              " does not work at " + hospitalComboBox->currentText() + 
                              ". Please choose a nurse from this hospital.");
        return;        
    }
    
    Patient* patient = new Patient(patientID, name, phone, disease, treatment, doctorID, nurseID);
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
        nurseIDInput->clear();
    } else {
        statusDisplay->append("Hospital is full. Cannot admit patient.");
        delete patient;
    }
}

void MainWindow::relocatePatient() {
    string patientID = patientIDInput->text().toStdString();
    string newPrimaryDoctorID = doctorIDInput->text().toStdString();
    int newHospitalIndex = relocateHospitalComboBox->currentIndex();

    if (patientID.empty() || newPrimaryDoctorID.empty()) {
        statusDisplay->append("Error: Patient ID and new Primary Doctor ID must be provided.");
        return;
    }

    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Error: Patient not found.");
        return;
    }

    Hospital* currentHospital = hospitalSystem->findPatientHospital(patientID);
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);

    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital selection is invalid.");
        return;
    }

    // Check if doctor works at new hospital
    if (!isDoctorInHospital(newPrimaryDoctorID, newHospitalIndex)) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(newPrimaryDoctorID) +
                              " does not work at " + relocateHospitalComboBox->currentText() + ".");
        return;
    }

    // Remove all doctor assignments from current hospital
    for (auto doctor : currentHospital->doctors) {
        doctor->removePatient(patientID);
    }
    patient->attendingDoctorIDs.clear();
    patient->primaryDoctorID = "";

    // Attempt to relocate the patient
    if (hospitalSystem->relocatePatient(patientID, newHospitalIndex)) {
        // Assign the new primary doctor
        if (hospitalSystem->setPatientPrimaryDoctor(patientID, newPrimaryDoctorID)) {
            statusDisplay->append("Patient " + QString::fromStdString(patientID) +
                                  " relocated to " + relocateHospitalComboBox->currentText() +
                                  " and assigned to Doctor " + QString::fromStdString(newPrimaryDoctorID));
        } else {
            statusDisplay->append("Patient relocated, but failed to assign new primary doctor.");
        }
    } else {
        statusDisplay->append("Failed to relocate patient. Destination hospital may be full.");
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
    
    // Get the hospital information to display
    Hospital* hospital = hospitalSystem->findPatientHospital(patientID);
    if (!hospital) {
        statusDisplay->append("Error: Cannot locate patient's hospital.");
        return;
    }
    
    // Display hospital information with patient details
    statusDisplay->append("Hospital: " + QString::fromStdString(hospital->hospitalName));
    
    // Get and display the remaining balance
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    statusDisplay->append("Remaining Balance: $" + QString::number(remainingBalance, 'f', 2));

    // Display patient details
    statusDisplay->append(QString::fromStdString(patient->getFullDescription()));
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
    string patientID = doctorPatientAssignmentIDInput->text().toStdString();
    
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
    
    if (doctor->getHospitalID() != patientHospital->hospitalID) {
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

void MainWindow::assignNurseToPatient() {
    string nurseID = nurseAssignmentIDInput->text().toStdString();
    string patientID = nursePatientAssignmentIDInput->text().toStdString();

    if (nurseID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Nurse ID and Patient ID are required.");
        return;
    }

    // Find which hospital the patient is in
    Hospital* patientHospital = hospitalSystem->findPatientHospital(patientID);
    if (!patientHospital) {
        statusDisplay->append("Error: Patient not found in any hospital.");
        return;
    }

    // Check if the doctor works at the patient's hospital
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Error: Nurse ID not found.");
        return;
    }    

    if (nurse->hospitalID != patientHospital->hospitalID) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(nurseID) + 
                             " does not work at " + QString::fromStdString(patientHospital->hospitalName) + 
                             " where the patient is admitted.");
        return;
    }

    // Continue with assignment
    // Explicitly provide the third parameter to resolve ambiguity
    if (hospitalSystem->assignNurseToPatient(nurseID, patientID)) {
        statusDisplay->append("Assigned nurse " + QString::fromStdString(nurseID) + 
                            " to patient " + QString::fromStdString(patientID));
    } else {
        statusDisplay->append("Failed to assign nurse. Check IDs and try again.");
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
                            QString::fromStdString(patientID) + " can now be discharged.");
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

void MainWindow::showPharmacyBillingReport() {
    statusDisplay->clear();
    statusDisplay->append("=== PHARMACY BILLING REPORT ===\n");

    PharmacySystem* ps = PharmacySystem::getInstance();
    auto allPharmacies = ps->getAllPharmacies();
    auto allHospitals = hospitalSystem->getAllHospitals();

    std::map<std::string, double> hospitalTotals;

    // Aggregate unpaid bills for each hospital
    for (auto pharmacy : allPharmacies) {
        for (auto hospital : allHospitals) {
            std::vector<Bill> bills = pharmacy->getBillsForHospital(hospital->hospitalID);
            for (const auto& bill : bills) {
                if (!bill.paid) {
                    hospitalTotals[hospital->hospitalName] += bill.amount;
                }
            }
        }
    }

    // Show the result
    for (const auto& pair : hospitalTotals) {
        statusDisplay->append(QString::fromStdString(pair.first) +
                              " owes: $" + QString::number(pair.second, 'f', 2));
    }

    if (hospitalTotals.empty()) {
        statusDisplay->append("All pharmacy bills have been paid!");
    }
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
    statusDisplay->append("=== CURRENT PATIENTS LIST ===");
    
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
            
            // Show attending nurses if any
            if (!patient->attendingNursesIDs.empty()) {
                QString attendingNurses = "Assigned Nurses: ";
                for (size_t i = 0; i < patient->attendingNursesIDs.size(); i++) {
                    attendingNurses += QString::fromStdString(patient->attendingNursesIDs[i]);
                    if (i < patient->attendingNursesIDs.size() - 1) {
                        attendingNurses += ", ";
                    }
                }
                statusDisplay->append(attendingNurses);
            }
            
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
            statusDisplay->append("-------------------------------");
        }
    }
    
    // Show count at the end
    statusDisplay->append("\nTotal Patients In All Hospitals: " + QString::number(allPatients.size()));
}

void MainWindow::requestDrugDelivery() {
    int hospitalIndex = drugHospitalComboBox->currentIndex();
    int pharmacyIndex = drugPharmacyComboBox->currentIndex();
    int drugIndex = drugComboBox->currentIndex();

    if (hospitalIndex < 0 || pharmacyIndex < 0 || drugIndex < 0) {
        statusDisplay->append("Error: Please select a hospital, pharmacy, and drug.");
        return;
    }

    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        statusDisplay->append("Error: Invalid hospital selection.");
        return;
    }

    PharmacySystem* ps = PharmacySystem::getInstance();
    Pharmacy* pharmacy = ps->getPharmacy(pharmacyIndex);
    if (!pharmacy) {
        statusDisplay->append("Error: Invalid pharmacy selection.");
        return;
    }

    drug selectedDrug = ps->getAllDrugs()[drugIndex];  // Changed Drug to drug to match the actual class name

    // Just bill the hospital for this drug
    std::string billID = pharmacy->billHospitalForDrug(hospital->hospitalID, selectedDrug.drugName, selectedDrug.price);

    statusDisplay->append("Drug \"" + QString::fromStdString(selectedDrug.drugName) +
                          "\" billed to " + QString::fromStdString(hospital->hospitalName) +
                          " for $" + QString::number(selectedDrug.price, 'f', 2) +
                          " (Bill ID: " + QString::fromStdString(billID) + ")");
}

void MainWindow::clearStatusDisplay() {
    statusDisplay->clear();
}
