#include "mainwindow.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Set up UI
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

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

    nurseIDInput = new QLineEdit(this);
    nurseIDInput->setPlaceholderText("Enter Nurse ID");
    formLayout->addRow("Nurse ID:", nurseIDInput);

    // Hospital selection
    hospitalComboBox = new QComboBox(this);
    hospitalComboBox->addItem("Windsor Health Centre");                        // index 0
    hospitalComboBox->addItem("Windsor Regional Hospital");                    // index 1
    hospitalComboBox->addItem("Windsor Regional Hospital - Metropolitan Campus"); // index 2
    hospitalComboBox->addItem("Hotel-Dieu Grace Healthcare");                  // index 3
    hospitalComboBox->addItem("Windsor Cardiac Centre");                       // index 4
    formLayout->addRow("Select Hospital:", hospitalComboBox);

    // Pharmacy selection
    pharmacyComboBox = new QComboBox(this);
    for (int i = 1; i <= 20; i++) {
        pharmacyComboBox->addItem("Pharmacy " + QString::number(i));
    }
    formLayout->addRow("Select Pharmacy:", pharmacyComboBox);

    // Doctor type selection
    doctorTypeComboBox = new QComboBox(this);
    doctorTypeComboBox->addItem("Primary Doctor");
    doctorTypeComboBox->addItem("Secondary Doctor");
    formLayout->addRow("Doctor Type:", doctorTypeComboBox);

    // Medication and cost inputs
    medicationInput = new QLineEdit(this);
    medicationInput->setPlaceholderText("Enter Medication Name");
    formLayout->addRow("Medication:", medicationInput);

    costInput = new QLineEdit(this);
    costInput->setPlaceholderText("Enter Cost");
    formLayout->addRow("Cost ($):", costInput);

    // Add buttons
    QPushButton* addPatientButton = new QPushButton("Add Patient", this);
    QPushButton* relocatePatientButton = new QPushButton("Relocate Patient", this);
    QPushButton* dischargePatientButton = new QPushButton("Discharge Patient", this);
    QPushButton* displayStatusButton = new QPushButton("Display Hospital Status", this);
    QPushButton* assignDoctorButton = new QPushButton("Assign Doctor", this);
    QPushButton* assignNurseButton = new QPushButton("Assign Nurse", this);
    QPushButton* requestMedicationButton = new QPushButton("Request Medication", this);
    QPushButton* payBillButton = new QPushButton("Pay Pharmacy Bill", this);
    QPushButton* updateDaysButton = new QPushButton("Update Patient Days", this);

    // Add status display
    statusDisplay = new QTextEdit(this);
    statusDisplay->setReadOnly(true);

    // Add widgets to the main layout
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(addPatientButton);
    mainLayout->addWidget(relocatePatientButton);
    mainLayout->addWidget(dischargePatientButton);
    mainLayout->addWidget(displayStatusButton);
    mainLayout->addWidget(assignDoctorButton);
    mainLayout->addWidget(assignNurseButton);
    mainLayout->addWidget(requestMedicationButton);
    mainLayout->addWidget(payBillButton);
    mainLayout->addWidget(updateDaysButton);
    mainLayout->addWidget(statusDisplay);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(relocatePatientButton, &QPushButton::clicked, this, &MainWindow::relocatePatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &MainWindow::dischargePatient);
    connect(displayStatusButton, &QPushButton::clicked, this, &MainWindow::displayHospitalStatus);
    connect(assignDoctorButton, &QPushButton::clicked, this, &MainWindow::assignDoctorToPatient);
    connect(assignNurseButton, &QPushButton::clicked, this, &MainWindow::assignNurseToPatient);
    connect(requestMedicationButton, &QPushButton::clicked, this, &MainWindow::requestMedication);
    connect(payBillButton, &QPushButton::clicked, this, &MainWindow::payPharmacyBill);
    connect(updateDaysButton, &QPushButton::clicked, this, &MainWindow::updatePatientDays);

    // Initialize system components
    initializeHospitals();
    initializePharmacies();
    initializeDoctors();
    initializeNurses();
}

void MainWindow::initializeHospitals() {
    const string HOSPITAL_NAMES[5] = {
        "Windsor Health Centre",
        "Windsor Regional Hospital",
        "Windsor Regional Hospital - Metropolitan Campus",
        "Hotel-Dieu Grace Healthcare",
        "Windsor Cardiac Centre"
    };

    for (int i = 0; i < 5; i++) {
        hospitals.push_back(new Hospital(to_string(i + 1), HOSPITAL_NAMES[i]));
    }
}

void MainWindow::initializePharmacies() {
    for (int i = 0; i < 20; i++) {
        pharmacies.push_back(new Pharmacy(to_string(i + 1), "Pharmacy " + to_string(i + 1)));
    }
}

void MainWindow::initializeDoctors() {
    // Create 50 doctors with at least 3 per hospital
    for (int i = 0; i < 50; i++) {
        string doctorID = "D" + to_string(i + 1);
        string name = "Doctor " + to_string(i + 1);
        
        // Assign to hospitals to ensure minimum 3 per hospital
        int hospitalIndex = i % 5; // Distribute evenly among 5 hospitals
        string hospitalID = hospitals[hospitalIndex]->hospitalID;
        
        Doctor* doctor = new Doctor(doctorID, name, hospitalID);
        doctors[doctorID] = doctor;
        hospitals[hospitalIndex]->addDoctor(doctor);
    }
}

void MainWindow::initializeNurses() {
    // Create 60 nurses with at least 5 per hospital
    for (int i = 0; i < 60; i++) {
        string nurseID = "N" + to_string(i + 1);
        string name = "Nurse " + to_string(i + 1);
        
        // Assign to hospitals to ensure minimum 5 per hospital
        int hospitalIndex = i % 5; // Distribute evenly among 5 hospitals
        string hospitalID = hospitals[hospitalIndex]->hospitalID;
        
        Nurse* nurse = new Nurse(nurseID, name, hospitalID);
        nurses[nurseID] = nurse;
        hospitals[hospitalIndex]->addNurse(nurse);
    }
}

void MainWindow::addPatient() {
    string patientID = patientIDInput->text().toStdString();
    string name = nameInput->text().toStdString();
    string phone = phoneInput->text().toStdString();
    string disease = diseaseInput->text().toStdString();
    string treatment = treatmentInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();

    // Convert 0-based index from combo box to 1-based
    int hospitalIndex = hospitalComboBox->currentIndex() + 1;

    // Sanity check (not strictly needed since combo is always valid, but kept for safety)
    if (hospitalIndex < 1 || hospitalIndex > 5) {
        statusDisplay->append("Invalid hospital selection.");
        return;
    }

    Patient* patient = new Patient(patientID, name, phone, disease, treatment, doctorID);
    if (hospitals[hospitalIndex - 1]->admitPatient(patient)) {
        patients[patientID] = patient;
        statusDisplay->append("Patient admitted successfully.");
    } else {
        statusDisplay->append("Hospital is full. Cannot admit patient.");
        delete patient;
    }
}

void MainWindow::relocatePatient() {
    string patientID = patientIDInput->text().toStdString();

    // Convert 0-based index from combo box to 1-based
    int newHospitalIndex = hospitalComboBox->currentIndex() + 1;

    if (newHospitalIndex < 1 || newHospitalIndex > 5) {
        statusDisplay->append("Invalid hospital selection.");
        return;
    }

    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }

    Patient* patient = patients[patientID];
    Hospital* currentHospital = nullptr;
    for (auto& hospital : hospitals) {
        if (std::find(hospital->patients.begin(), hospital->patients.end(), patient) != hospital->patients.end()) {
            currentHospital = hospital;
            break;
        }
    }

    if (currentHospital && currentHospital->relocatePatient(patient, hospitals[newHospitalIndex - 1])) {
        statusDisplay->append("Patient relocated successfully.");
    } else {
        statusDisplay->append("Failed to relocate patient.");
    }
}

void MainWindow::dischargePatient() {
    string patientID = patientIDInput->text().toStdString();

    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }

    Patient* patient = patients[patientID];
    for (auto& hospital : hospitals) {
        if (std::find(hospital->patients.begin(), hospital->patients.end(), patient) != hospital->patients.end()) {
            hospital->dischargePatient(patient);
            patients.erase(patientID);
            delete patient;
            statusDisplay->append("Patient discharged successfully.");
            return;
        }
    }
    statusDisplay->append("Patient not found in any hospital.");
}

void MainWindow::assignDoctorToPatient() {
    string patientID = patientIDInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    bool isPrimary = doctorTypeComboBox->currentIndex() == 0; // Primary is index 0
    
    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }
    
    if (doctors.find(doctorID) == doctors.end()) {
        statusDisplay->append("Doctor not found.");
        return;
    }
    
    Patient* patient = patients[patientID];
    Doctor* doctor = doctors[doctorID];
    
    // Find the hospital where the patient is admitted
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == patient->getHospitalID()) {
            if (hospital->assignDoctorToPatient(doctor, patient, isPrimary)) {
                statusDisplay->append(QString("Doctor %1 assigned as %2 to patient %3.")
                                     .arg(QString::fromStdString(doctorID))
                                     .arg(isPrimary ? "primary" : "attending")
                                     .arg(QString::fromStdString(patientID)));
            } else {
                statusDisplay->append("Failed to assign doctor to patient.");
            }
            return;
        }
    }
    
    statusDisplay->append("Patient not found in any hospital.");
}

void MainWindow::assignNurseToPatient() {
    string patientID = patientIDInput->text().toStdString();
    string nurseID = nurseIDInput->text().toStdString();
    
    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }
    
    if (nurses.find(nurseID) == nurses.end()) {
        statusDisplay->append("Nurse not found.");
        return;
    }
    
    Patient* patient = patients[patientID];
    Nurse* nurse = nurses[nurseID];
    
    // Find the hospital where the patient is admitted
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == patient->getHospitalID()) {
            if (hospital->assignNurseToPatient(nurse, patient)) {
                statusDisplay->append(QString("Nurse %1 assigned to patient %2.")
                                     .arg(QString::fromStdString(nurseID))
                                     .arg(QString::fromStdString(patientID)));
            } else {
                statusDisplay->append("Failed to assign nurse to patient. Nurse may already have max patients (2).");
            }
            return;
        }
    }
    
    statusDisplay->append("Patient not found in any hospital.");
}

void MainWindow::requestMedication() {
    string patientID = patientIDInput->text().toStdString();
    string medication = medicationInput->text().toStdString();
    double cost = costInput->text().toDouble();
    int pharmacyIndex = pharmacyComboBox->currentIndex();
    
    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }
    
    Patient* patient = patients[patientID];
    Pharmacy* pharmacy = pharmacies[pharmacyIndex];
    
    // Find the hospital where the patient is admitted
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == patient->getHospitalID()) {
            if (hospital->requestMedication(pharmacy, patient, medication, cost)) {
                statusDisplay->append(QString("Medication %1 requested for patient %2 from pharmacy %3.")
                                     .arg(QString::fromStdString(medication))
                                     .arg(QString::fromStdString(patientID))
                                     .arg(QString::fromStdString(pharmacy->getPharmacyID())));
            } else {
                statusDisplay->append("Failed to request medication.");
            }
            return;
        }
    }
    
    statusDisplay->append("Patient not found in any hospital.");
}

void MainWindow::payPharmacyBill() {
    int pharmacyIndex = pharmacyComboBox->currentIndex();
    int hospitalIndex = hospitalComboBox->currentIndex();
    
    Pharmacy* pharmacy = pharmacies[pharmacyIndex];
    Hospital* hospital = hospitals[hospitalIndex];
    
    hospital->payPharmacyBill(pharmacy->getPharmacyID());
    statusDisplay->append(QString("%1 paid bill to pharmacy %2.")
                         .arg(QString::fromStdString(hospital->name))
                         .arg(QString::fromStdString(pharmacy->getPharmacyID())));
}

void MainWindow::updatePatientDays() {
    for (auto& hospital : hospitals) {
        hospital->updatePatientStay();
    }
    statusDisplay->append("Patient days updated for all hospitals.");
}

void MainWindow::displayHospitalStatus() {
    for (auto& hospital : hospitals) {
        statusDisplay->append(QString::fromStdString("\nHospital: " + hospital->name));
        statusDisplay->append(QString::fromStdString("Patients Admitted: " + to_string(hospital->patients.size())));
        statusDisplay->append(QString::fromStdString("Doctors: " + to_string(hospital->doctors.size())));
        statusDisplay->append(QString::fromStdString("Nurses: " + to_string(hospital->nurses.size())));
        
        // Display capacity information
        statusDisplay->append(QString::fromStdString("Capacity: " + to_string(hospital->patients.size()) + 
                                                    "/" + to_string(hospital->getCapacity())));
        
        // Add a separator for better readability
        statusDisplay->append("----------------------------------------");
    }
}
