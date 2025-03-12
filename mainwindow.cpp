#include "mainwindow.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>

using namespace std;

vector<Pharmacy*> MainWindow::pharmacies;

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

    // Additional patient detail fields
    addressInput = new QLineEdit(this);
    addressInput->setPlaceholderText("Enter Patient Address");
    formLayout->addRow("Address:", addressInput);
    
    emergencyInput = new QLineEdit(this);
    emergencyInput->setPlaceholderText("Enter Emergency Contact");
    formLayout->addRow("Emergency Contact:", emergencyInput);
    
    dobInput = new QLineEdit(this);
    dobInput->setPlaceholderText("Enter Date of Birth (YYYY-MM-DD)");
    formLayout->addRow("Date of Birth:", dobInput);

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

    // Additional doctor detail fields
    specializationInput = new QLineEdit(this);
    specializationInput->setPlaceholderText("Enter Doctor Specialization");
    formLayout->addRow("Specialization:", specializationInput);
    
    // Additional nurse detail fields
    nurseSpecializationInput = new QLineEdit(this);
    nurseSpecializationInput->setPlaceholderText("Enter Nurse Specialization");
    formLayout->addRow("Nurse Specialization:", nurseSpecializationInput);
    
    // Add a second combo box for destination hospital (for doctor transfers)
    destinationHospitalComboBox = new QComboBox(this);
    destinationHospitalComboBox->addItem("Windsor Health Centre");                        // index 0
    destinationHospitalComboBox->addItem("Windsor Regional Hospital");                    // index 1
    destinationHospitalComboBox->addItem("Windsor Regional Hospital - Metropolitan Campus"); // index 2
    destinationHospitalComboBox->addItem("Hotel-Dieu Grace Healthcare");                  // index 3
    destinationHospitalComboBox->addItem("Windsor Cardiac Centre");                       // index 4
    formLayout->addRow("Destination Hospital:", destinationHospitalComboBox);

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
    displayPharmacyStatusButton = new QPushButton("Display Pharmacy Status", this);
    authorizeDischargeButton = new QPushButton("Authorize Discharge", this);
    displayPatientButton = new QPushButton("Display Patient Details", this);
    printBillButton = new QPushButton("Print Patient Bill", this);
    displayDoctorButton = new QPushButton("Display Doctor Details", this);
    transferDoctorButton = new QPushButton("Transfer Doctor", this);
    displayNurseButton = new QPushButton("Display Nurse Details", this);
    transferNurseButton = new QPushButton("Transfer Nurse", this);

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
    mainLayout->addWidget(displayPharmacyStatusButton);
    mainLayout->addWidget(authorizeDischargeButton);
    mainLayout->addWidget(displayPatientButton);
    mainLayout->addWidget(printBillButton);
    mainLayout->addWidget(displayDoctorButton);
    mainLayout->addWidget(transferDoctorButton);
    mainLayout->addWidget(displayNurseButton);
    mainLayout->addWidget(transferNurseButton);
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
    connect(displayPharmacyStatusButton, &QPushButton::clicked, this, &MainWindow::displayPharmacyStatus);
    connect(authorizeDischargeButton, &QPushButton::clicked, this, &MainWindow::authorizeDischarge);
    connect(displayPatientButton, &QPushButton::clicked, this, &MainWindow::displayPatientDetails);
    connect(printBillButton, &QPushButton::clicked, this, &MainWindow::printPatientBill);
    connect(displayDoctorButton, &QPushButton::clicked, this, &MainWindow::displayDoctorDetails);
    connect(transferDoctorButton, &QPushButton::clicked, this, &MainWindow::transferDoctor);
    connect(displayNurseButton, &QPushButton::clicked, this, &MainWindow::displayNurseDetails);
    connect(transferNurseButton, &QPushButton::clicked, this, &MainWindow::transferNurse);

    // Initialize system components
    initializeHospitals();
    initializePharmacies();
    initializeDoctors();
    initializeNurses();
}

vector<Pharmacy*>& MainWindow::getPharmacies() {
    return pharmacies;
}

void MainWindow::initializeHospitals() {
    const string HOSPITAL_NAMES[5] = {
        "Windsor Health Centre",
        "Windsor Regional Hospital",
        "Windsor Regional Hospital - Metropolitan Campus",
        "Hotel-Dieu Grace Healthcare",
        "Windsor Cardiac Centre"
    };
    
    const string LOCATIONS[5] = {
        "1030 Ouellette Ave, Windsor",
        "1995 Lens Ave, Windsor",
        "1995 Lens Ave, Metropolitan Building",
        "1453 Prince Rd, Windsor",
        "2135 Richmond St, Windsor"
    };

    for (int i = 0; i < 5; i++) {
        hospitals.push_back(new Hospital(to_string(i + 1), HOSPITAL_NAMES[i], LOCATIONS[i]));
    }
}

void MainWindow::initializePharmacies() {
    const string PHARMACY_LOCATIONS[20] = {
        "300 Tecumseh Rd E, Windsor",
        "7720 Tecumseh Rd E, Windsor",
        "500 Manning Rd, Tecumseh",
        "13278 Tecumseh Rd E, Tecumseh",
        "11990 Tecumseh Rd E, Tecumseh",
        "4115 Walker Rd, Windsor",
        "3175 Howard Ave, Windsor",
        "1760 Huron Church Rd, Windsor",
        "2080 Wyandotte St W, Windsor",
        "1675 Wyandotte St W, Windsor",
        "3850 Dougall Ave, Windsor",
        "3100 Howard Ave, Windsor",
        "5050 Tecumseh Rd E, Windsor",
        "9200 Tecumseh Rd E, Windsor",
        "1499 Provincial Rd, Windsor",
        "500 Erie St S, Leamington",
        "215 Talbot St W, Leamington",
        "24 Oak St W, Leamington",
        "33 Princess St, Leamington",
        "1425 Huron Church Rd, Windsor"
    };
    
    for (int i = 0; i < 20; i++) {
        pharmacies.push_back(new Pharmacy(to_string(i + 1), "Pharmacy " + to_string(i + 1), PHARMACY_LOCATIONS[i]));
        
        // Add some common medications to each pharmacy
        pharmacies[i]->addMedication("Acetaminophen");
        pharmacies[i]->addMedication("Ibuprofen");
        pharmacies[i]->addMedication("Amoxicillin");
        pharmacies[i]->addMedication("Atorvastatin");
        pharmacies[i]->addMedication("Lisinopril");
    }
}

void MainWindow::initializeDoctors() {
    // Create 50 doctors with at least 3 per hospital
    const string SPECIALIZATIONS[10] = {
        "General Practice",
        "Cardiology",
        "Neurology",
        "Orthopedics",
        "Pediatrics",
        "Obstetrics",
        "Oncology",
        "Dermatology",
        "Internal Medicine",
        "Emergency Medicine"
    };

    for (int i = 0; i < 50; i++) {
        string doctorID = "D" + to_string(i + 1);
        string name = "Doctor " + to_string(i + 1);
        
        // Assign to hospitals to ensure minimum 3 per hospital
        int hospitalIndex = i % 5; // Distribute evenly among 5 hospitals
        string hospitalID = hospitals[hospitalIndex]->hospitalID;
        
        // Assign a random specialization
        string specialization = SPECIALIZATIONS[i % 10];
        
        Doctor* doctor = new Doctor(doctorID, name, hospitalID, specialization);
        doctors[doctorID] = doctor;
        hospitals[hospitalIndex]->addDoctor(doctor);
    }
    
    // Verify we have at least 3 doctors per hospital
    for (auto& hospital : hospitals) {
        if (hospital->doctors.size() < 3) {
            statusDisplay->append(QString("Warning: Hospital %1 has fewer than 3 doctors.")
                               .arg(QString::fromStdString(hospital->name)));
        }
    }
}

void MainWindow::initializeNurses() {
    // Create 60 nurses with at least 5 per hospital
    const string NURSE_SPECIALIZATIONS[6] = {
        "General Care",
        "Intensive Care",
        "Pediatric",
        "Surgical",
        "Emergency",
        "Geriatric"
    };
    
    for (int i = 0; i < 60; i++) {
        string nurseID = "N" + to_string(i + 1);
        string name = "Nurse " + to_string(i + 1);
        
        // Assign to hospitals to ensure minimum 5 per hospital
        int hospitalIndex = i % 5; // Distribute evenly among 5 hospitals
        string hospitalID = hospitals[hospitalIndex]->hospitalID;
        
        // Assign a specialization
        string specialization = NURSE_SPECIALIZATIONS[i % 6];
        
        Nurse* nurse = new Nurse(nurseID, name, hospitalID, specialization);
        nurses[nurseID] = nurse;
        hospitals[hospitalIndex]->addNurse(nurse);
    }
    
    // Verify we have at least 5 nurses per hospital
    verifyNurseDistribution();
}

void MainWindow::verifyNurseDistribution() {
    for (auto& hospital : hospitals) {
        if (hospital->nurses.size() < 5) {
            statusDisplay->append(QString("Warning: Hospital %1 has fewer than 5 nurses.")
                               .arg(QString::fromStdString(hospital->name)));
        }
    }
}

void MainWindow::addPatient() {
    string patientID = patientIDInput->text().toStdString();
    string name = nameInput->text().toStdString();
    string phone = phoneInput->text().toStdString();
    string disease = diseaseInput->text().toStdString();
    string treatment = treatmentInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    string address = addressInput->text().toStdString();
    string emergency = emergencyInput->text().toStdString();
    string dob = dobInput->text().toStdString();

    // Convert 0-based index from combo box to 0-based
    int hospitalIndex = hospitalComboBox->currentIndex();

    // Sanity check
    if (hospitalIndex < 0 || hospitalIndex >= hospitals.size()) {
        statusDisplay->append("Invalid hospital selection.");
        return;
    }

    // Create patient with enhanced constructor if fields are provided
    Patient* patient;
    if (!address.empty() || !emergency.empty() || !dob.empty()) {
        patient = new Patient(name, phone, address, emergency, dob, disease, treatment, doctorID);
        // Auto-generated ID
        patientID = patient->getPatientID();
    } else {
        // If patientID is not provided, generate one
        if (patientID.empty()) {
            patientID = Patient::generateUniqueID();
        }
        patient = new Patient(patientID, name, phone, disease, treatment, doctorID);
    }

    if (hospitals[hospitalIndex]->admitPatient(patient)) {
        patients[patientID] = patient;
        statusDisplay->append(QString("Patient admitted successfully with ID: %1")
                             .arg(QString::fromStdString(patientID)));
                             
        // Clear input fields
        patientIDInput->clear();
        nameInput->clear();
        phoneInput->clear();
        diseaseInput->clear();
        treatmentInput->clear();
        addressInput->clear();
        emergencyInput->clear();
        dobInput->clear();
    } else {
        statusDisplay->append("Hospital is full. Cannot admit patient.");
        delete patient;
    }
}

void MainWindow::relocatePatient() {
    string patientID = patientIDInput->text().toStdString();

    // Convert 0-based index from combo box to 1-based
    int newHospitalIndex = hospitalComboBox->currentIndex();

    if (newHospitalIndex < 0 || newHospitalIndex >= hospitals.size()) {
        statusDisplay->append("Invalid hospital selection.");
        return;
    }

    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }

    Patient* patient = patients[patientID];
    Hospital* currentHospital = nullptr;
    
    // Find which hospital currently has this patient
    for (auto& hospital : hospitals) {
        if (hospital->hasPatient(patient)) {
            currentHospital = hospital;
            break;
        }
    }

    if (!currentHospital) {
        statusDisplay->append("Patient not found in any hospital.");
        return;
    }
    
    Hospital* newHospital = hospitals[newHospitalIndex];
    
    // Don't relocate to the same hospital
    if (currentHospital->hospitalID == newHospital->hospitalID) {
        statusDisplay->append("Patient is already at this hospital location.");
        return;
    }

    if (currentHospital->relocatePatient(patient, newHospital)) {
        statusDisplay->append(QString("Patient relocated successfully from %1 to %2.")
                             .arg(QString::fromStdString(currentHospital->name))
                             .arg(QString::fromStdString(newHospital->name)));
    } else {
        statusDisplay->append("Failed to relocate patient. Destination hospital may be full.");
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
        if (hospital->hasPatient(patient)) {
            // Check if patient is authorized for discharge
            if (!patient->getReadyForDischarge()) {
                statusDisplay->append("Patient has not been authorized for discharge by primary doctor.");
                return;
            }
            
            // Calculate final bill before discharge
            double finalBill = hospital->calculatePatientBill(patient);
            
            if (hospital->dischargePatient(patient)) {
                statusDisplay->append(QString("Patient %1 discharged successfully.")
                                     .arg(QString::fromStdString(patientID)));
                statusDisplay->append(QString("Final bill: $%1").arg(finalBill));
                
                // Remove from our tracking and free memory
                patients.erase(patientID);
                delete patient;
            } else {
                statusDisplay->append("Failed to discharge patient. Primary doctor authorization required.");
            }
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
    
    // Check if nurse can accept more patients
    if (!nurse->canAcceptMorePatients()) {
        statusDisplay->append("Nurse already has the maximum number of patients (2).");
        return;
    }
    
    // Find the hospital where the patient is admitted
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == patient->getHospitalID()) {
            if (hospital->assignNurseToPatient(nurse, patient)) {
                statusDisplay->append(QString("Nurse %1 assigned to patient %2.")
                                     .arg(QString::fromStdString(nurseID))
                                     .arg(QString::fromStdString(patientID)));
            } else {
                statusDisplay->append("Failed to assign nurse to patient.");
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
    
    if (pharmacyIndex < 0 || pharmacyIndex >= pharmacies.size()) {
        statusDisplay->append("Invalid pharmacy selection.");
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
                                     .arg(QString::fromStdString(pharmacy->getName())));
                statusDisplay->append(QString("Bill of $%1 sent to %2.")
                                     .arg(cost)
                                     .arg(QString::fromStdString(hospital->name)));
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
    
    if (pharmacyIndex < 0 || pharmacyIndex >= pharmacies.size() ||
        hospitalIndex < 0 || hospitalIndex >= hospitals.size()) {
        statusDisplay->append("Invalid selection.");
        return;
    }
    
    Pharmacy* pharmacy = pharmacies[pharmacyIndex];
    Hospital* hospital = hospitals[hospitalIndex];
    
    // Get the bill amount before payment
    double billAmount = pharmacy->getBillForHospital(hospital->hospitalID);
    
    hospital->payPharmacyBill(pharmacy->getPharmacyID());
    
    statusDisplay->append(QString("%1 paid bill of $%2 to %3.")
                         .arg(QString::fromStdString(hospital->name))
                         .arg(billAmount)
                         .arg(QString::fromStdString(pharmacy->getName())));
}

void MainWindow::updatePatientDays() {
    for (auto& hospital : hospitals) {
        hospital->updatePatientStay();
    }
    statusDisplay->append("Patient days updated for all hospitals.");
}

void MainWindow::displayHospitalStatus() {
    statusDisplay->clear();
    statusDisplay->append("=== HOSPITAL SYSTEM STATUS ===");
    
    int totalPatients = 0;
    for (auto& hospital : hospitals) {
        statusDisplay->append(QString::fromStdString("\nHospital: " + hospital->name));
        statusDisplay->append(QString::fromStdString("Location: " + hospital->location));
        statusDisplay->append(QString::fromStdString("Patients Admitted: " + to_string(hospital->patients.size())));
        statusDisplay->append(QString::fromStdString("Doctors: " + to_string(hospital->doctors.size())));
        statusDisplay->append(QString::fromStdString("Nurses: " + to_string(hospital->nurses.size())));
        
        // Display capacity information
        statusDisplay->append(QString::fromStdString("Capacity: " + to_string(hospital->patients.size()) + 
                                                    "/" + to_string(hospital->getCapacity())));
        
        // Display hospital ID for unique identification
        statusDisplay->append(QString::fromStdString("Hospital ID: " + hospital->hospitalID));
        
        // Add a separator for better readability
        statusDisplay->append("----------------------------------------");
        
        totalPatients += hospital->patients.size();
    }

    // Display overall system status
    statusDisplay->append(QString("\nTOTAL SYSTEM STATUS:"));
    statusDisplay->append(QString("Total patients across all locations: %1/%2")
                              .arg(totalPatients)
                              .arg(Hospital::getTotalCapacity()));
}

void MainWindow::displayPharmacyStatus() {
    statusDisplay->clear();
    statusDisplay->append("=== PHARMACY SYSTEM STATUS ===");
    
    for (auto& pharmacy : pharmacies) {
        statusDisplay->append(QString::fromStdString("\nPharmacy: " + pharmacy->getName()));
        statusDisplay->append(QString::fromStdString("ID: " + pharmacy->getPharmacyID()));
        statusDisplay->append(QString::fromStdString("Location: " + pharmacy->getLocation()));
        
        // Display billing information
        statusDisplay->append("Outstanding Bills:");
        map<string, double> bills = pharmacy->getAllBills();
        double totalBilled = 0.0;
        
        for (auto& bill : bills) {
            if (bill.second > 0) {
                for (auto& hospital : hospitals) {
                    if (hospital->hospitalID == bill.first) {
                        statusDisplay->append(QString("  %1: $%2")
                                             .arg(QString::fromStdString(hospital->name))
                                             .arg(bill.second));
                        totalBilled += bill.second;
                        break;
                    }
                }
            }
        }
        
        statusDisplay->append(QString("Total Outstanding: $%1").arg(totalBilled));
        statusDisplay->append("----------------------------------------");
    }
}

void MainWindow::authorizeDischarge() {
    string patientID = patientIDInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();

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
    
    // Doctor must be the patient's primary doctor
    if (doctor->isPrimaryPatient(patientID)) {
        patient->markReadyForDischarge(true);
        statusDisplay->append(QString("Doctor %1 authorized discharge for patient %2.")
                             .arg(QString::fromStdString(doctorID))
                             .arg(QString::fromStdString(patientID)));
    } else {
        statusDisplay->append("Only the primary doctor can authorize discharge.");
    }
}

void MainWindow::displayPatientDetails() {
    string patientID = patientIDInput->text().toStdString();

    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }

    Patient* patient = patients[patientID];
    statusDisplay->clear();
    statusDisplay->append(QString::fromStdString(patient->getFullDetails()));
}

void MainWindow::printPatientBill() {
    string patientID = patientIDInput->text().toStdString();

    if (patients.find(patientID) == patients.end()) {
        statusDisplay->append("Patient not found.");
        return;
    }

    Patient* patient = patients[patientID];
    
    // Find the hospital
    for (auto& hospital : hospitals) {
        if (hospital->hasPatient(patient)) {
            double bill = hospital->calculatePatientBill(patient);
            
            // Format current date and time
            QDateTime currentTime = QDateTime::currentDateTime();
            QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
            
            statusDisplay->clear();
            statusDisplay->append("==========================================");
            statusDisplay->append("            PATIENT BILL");
            statusDisplay->append("==========================================");
            statusDisplay->append(QString("Date: %1").arg(formattedTime));
            statusDisplay->append(QString("Patient ID: %1").arg(QString::fromStdString(patient->getPatientID())));
            statusDisplay->append(QString("Patient Name: %1").arg(QString::fromStdString(patient->getName())));
            statusDisplay->append(QString("Hospital: %1").arg(QString::fromStdString(hospital->name)));
            statusDisplay->append(QString("Days Admitted: %1").arg(patient->getDaysAdmitted()));
            statusDisplay->append(QString("Daily Rate: $%1").arg(hospital->getDailyRate()));
            statusDisplay->append("------------------------------------------");
            statusDisplay->append(QString("TOTAL DUE: $%1").arg(bill));
            statusDisplay->append("==========================================");
            statusDisplay->append("Thank you for choosing our hospital!");
            statusDisplay->append("==========================================");
            
            // Offer to save bill as a file
            bool saveBill = QMessageBox::question(this, "Save Bill", 
                                                 "Would you like to save this bill as a text file?",
                                                 QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
            if (saveBill) {
                QString fileName = QFileDialog::getSaveFileName(this, "Save Bill", 
                                                              QString::fromStdString("bill_" + patient->getPatientID() + ".txt"),
                                                              "Text Files (*.txt)");
                if (!fileName.isEmpty()) {
                    QFile file(fileName);
                    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream stream(&file);
                        stream << statusDisplay->toPlainText();
                        file.close();
                        statusDisplay->append("\nBill saved to file: " + fileName);
                    } else {
                        statusDisplay->append("\nFailed to save bill to file.");
                    }
                }
            }
            
            return;
        }
    }
    
    statusDisplay->append("Patient not found in any hospital.");
}

void MainWindow::displayDoctorDetails() {
    string doctorID = doctorIDInput->text().toStdString();

    if (doctors.find(doctorID) == doctors.end()) {
        statusDisplay->append("Doctor not found.");
        return;
    }

    Doctor* doctor = doctors[doctorID];
    statusDisplay->clear();
    statusDisplay->append(QString::fromStdString(doctor->getDetails()));
    
    // Display hospital information
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == doctor->getHospitalID()) {
            statusDisplay->append(QString("Assigned Hospital: %1")
                               .arg(QString::fromStdString(hospital->name)));
            break;
        }
    }
}

void MainWindow::transferDoctor() {
    string doctorID = doctorIDInput->text().toStdString();
    int destinationIndex = destinationHospitalComboBox->currentIndex();
    
    if (doctors.find(doctorID) == doctors.end()) {
        statusDisplay->append("Doctor not found.");
        return;
    }
    
    if (destinationIndex < 0 || destinationIndex >= hospitals.size()) {
        statusDisplay->append("Invalid destination hospital.");
        return;
    }
    
    Doctor* doctor = doctors[doctorID];
    Hospital* destinationHospital = hospitals[destinationIndex];
    
    // Check if the doctor is already at this hospital
    if (doctor->getHospitalID() == destinationHospital->hospitalID) {
        statusDisplay->append("Doctor is already assigned to this hospital.");
        return;
    }
    
    // Find current hospital
    Hospital* sourceHospital = nullptr;
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == doctor->getHospitalID()) {
            sourceHospital = hospital;
            break;
        }
    }
    
    if (!sourceHospital) {
        statusDisplay->append("Error: Doctor's current hospital not found.");
        return;
    }
    
    // Check if removing this doctor would leave fewer than 3 doctors at source hospital
    if (sourceHospital->doctors.size() <= 3) {
        statusDisplay->append(QString("Cannot transfer doctor. Hospital %1 must have at least 3 doctors.")
                           .arg(QString::fromStdString(sourceHospital->name)));
        return;
    }
    
    // Remove doctor from source hospital
    sourceHospital->doctors.erase(
        remove(sourceHospital->doctors.begin(), sourceHospital->doctors.end(), doctor),
        sourceHospital->doctors.end());
    
    // Add doctor to destination hospital
    destinationHospital->doctors.push_back(doctor);
    doctor->setHospitalID(destinationHospital->hospitalID);
    
    statusDisplay->append(QString("Doctor %1 transferred from %2 to %3.")
                       .arg(QString::fromStdString(doctorID))
                       .arg(QString::fromStdString(sourceHospital->name))
                       .arg(QString::fromStdString(destinationHospital->name)));
}

void MainWindow::displayNurseDetails() {
    string nurseID = nurseIDInput->text().toStdString();

    if (nurses.find(nurseID) == nurses.end()) {
        statusDisplay->append("Nurse not found.");
        return;
    }

    Nurse* nurse = nurses[nurseID];
    statusDisplay->clear();
    statusDisplay->append(QString::fromStdString(nurse->getDetails()));
    
    // Display hospital information
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == nurse->getHospitalID()) {
            statusDisplay->append(QString("Assigned Hospital: %1")
                               .arg(QString::fromStdString(hospital->name)));
            break;
        }
    }
    
    // Show patient assignment status
    statusDisplay->append(QString("Can accept more patients: %1")
                       .arg(nurse->canAcceptMorePatients() ? "Yes" : "No"));
}

void MainWindow::transferNurse() {
    string nurseID = nurseIDInput->text().toStdString();
    int destinationIndex = destinationHospitalComboBox->currentIndex();
    
    if (nurses.find(nurseID) == nurses.end()) {
        statusDisplay->append("Nurse not found.");
        return;
    }
    
    if (destinationIndex < 0 || destinationIndex >= hospitals.size()) {
        statusDisplay->append("Invalid destination hospital.");
        return;
    }
    
    Nurse* nurse = nurses[nurseID];
    Hospital* destinationHospital = hospitals[destinationIndex];
    
    // Check if the nurse is already at this hospital
    if (nurse->getHospitalID() == destinationHospital->hospitalID) {
        statusDisplay->append("Nurse is already assigned to this hospital.");
        return;
    }
    
    // Find current hospital
    Hospital* sourceHospital = nullptr;
    for (auto& hospital : hospitals) {
        if (hospital->hospitalID == nurse->getHospitalID()) {
            sourceHospital = hospital;
            break;
        }
    }
    
    if (!sourceHospital) {
        statusDisplay->append("Error: Nurse's current hospital not found.");
        return;
    }
    
    // Check if removing this nurse would leave fewer than 5 nurses at source hospital
    if (sourceHospital->nurses.size() <= 5) {
        statusDisplay->append(QString("Cannot transfer nurse. Hospital %1 must have at least 5 nurses.")
                           .arg(QString::fromStdString(sourceHospital->name)));
        return;
    }
    
    // Check if the nurse has assigned patients
    if (!nurse->getPatients().empty()) {
        statusDisplay->append("Cannot transfer nurse with assigned patients. Please reassign patients first.");
        return;
    }
    
    // Remove nurse from source hospital
    sourceHospital->nurses.erase(
        remove(sourceHospital->nurses.begin(), sourceHospital->nurses.end(), nurse),
        sourceHospital->nurses.end());
    
    // Add nurse to destination hospital
    destinationHospital->nurses.push_back(nurse);
    nurse->setHospitalID(destinationHospital->hospitalID);
    
    statusDisplay->append(QString("Nurse %1 transferred from %2 to %3.")
                       .arg(QString::fromStdString(nurseID))
                       .arg(QString::fromStdString(sourceHospital->name))
                       .arg(QString::fromStdString(destinationHospital->name)));
}
