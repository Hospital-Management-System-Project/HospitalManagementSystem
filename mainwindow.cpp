#include "mainwindow.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>

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

    // --- REPLACED QLineEdit FOR HOSPITAL INDEX WITH A QComboBox ---
    hospitalComboBox = new QComboBox(this);
    hospitalComboBox->addItem("Windsor Health Centre");                        // index 0
    hospitalComboBox->addItem("Windsor Regional Hospital");                    // index 1
    hospitalComboBox->addItem("Windsor Regional Hospital - Metropolitan Campus"); // index 2
    hospitalComboBox->addItem("Hotel-Dieu Grace Healthcare");                  // index 3
    hospitalComboBox->addItem("Windsor Cardiac Centre");                       // index 4
    formLayout->addRow("Select Hospital:", hospitalComboBox);

    // Add buttons
    QPushButton* addPatientButton = new QPushButton("Add Patient", this);
    QPushButton* relocatePatientButton = new QPushButton("Relocate Patient", this);
    QPushButton* dischargePatientButton = new QPushButton("Discharge Patient", this);
    QPushButton* displayStatusButton = new QPushButton("Display Hospital Status", this);

    // Add status display
    statusDisplay = new QTextEdit(this);
    statusDisplay->setReadOnly(true);

    // Add widgets to the main layout
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(addPatientButton);
    mainLayout->addWidget(relocatePatientButton);
    mainLayout->addWidget(dischargePatientButton);
    mainLayout->addWidget(displayStatusButton);
    mainLayout->addWidget(statusDisplay);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Connect buttons to slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(relocatePatientButton, &QPushButton::clicked, this, &MainWindow::relocatePatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &MainWindow::dischargePatient);
    connect(displayStatusButton, &QPushButton::clicked, this, &MainWindow::displayHospitalStatus);

    // Initialize hospitals
    initializeHospitals();
}

void MainWindow::initializeHospitals() {
    const std::string HOSPITAL_NAMES[5] = {
        "Windsor Health Centre",
        "Windsor Regional Hospital",
        "Windsor Regional Hospital - Metropolitan Campus",
        "Hotel-Dieu Grace Healthcare",
        "Windsor Cardiac Centre"
    };

    for (int i = 0; i < 5; i++) {
        hospitals.push_back(new Hospital(std::to_string(i + 1), HOSPITAL_NAMES[i]));
    }
}

void MainWindow::addPatient() {
    std::string patientID = patientIDInput->text().toStdString();
    std::string name = nameInput->text().toStdString();
    std::string phone = phoneInput->text().toStdString();
    std::string disease = diseaseInput->text().toStdString();
    std::string treatment = treatmentInput->text().toStdString();
    std::string doctorID = doctorIDInput->text().toStdString();

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
    std::string patientID = patientIDInput->text().toStdString();

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
    std::string patientID = patientIDInput->text().toStdString();

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

void MainWindow::displayHospitalStatus() {
    for (auto& hospital : hospitals) {
        statusDisplay->append(QString::fromStdString("\nHospital: " + hospital->name));
        statusDisplay->append(QString::fromStdString("Patients Admitted: " + std::to_string(hospital->patients.size())));
        statusDisplay->append(QString::fromStdString("Doctors: " + std::to_string(hospital->doctors.size())));
        statusDisplay->append(QString::fromStdString("Nurses: " + std::to_string(hospital->nurses.size())));
    }
}
