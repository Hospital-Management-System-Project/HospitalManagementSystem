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
#include <QScrollBar>
#include <QDebug>
#include <QSplitter>
#include <QFrame>

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
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Create a splitter to allow users to resize the proportion between tabs and status display
    QSplitter* mainSplitter = new QSplitter(Qt::Vertical);
    mainSplitter->setChildrenCollapsible(false);
    
    // Create a tab widget for different functionalities with enhanced styling
    tabWidget = new QTabWidget();
    tabWidget->setDocumentMode(true);
    tabWidget->setTabPosition(QTabWidget::North);
    
    // ===== PATIENT MANAGEMENT TAB =====
    QWidget* patientTab = new QWidget();
    QVBoxLayout* patientLayout = new QVBoxLayout(patientTab);
    patientLayout->setSpacing(15);
    
    // Create patient info group
    QGroupBox* patientInfoGroup = new QGroupBox("Patient Information");
    QHBoxLayout* patientInfoLayout = new QHBoxLayout();
    
    // Left column for basic patient details
    QFormLayout* patientBasicFormLayout = new QFormLayout();
    patientBasicFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    patientBasicFormLayout->setSpacing(8);
    
    patientIDInput = new QLineEdit();
    patientIDInput->setPlaceholderText("Enter Patient ID");
    patientBasicFormLayout->addRow("Patient ID:", patientIDInput);

    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter Patient Name");
    patientBasicFormLayout->addRow("Name:", nameInput);

    phoneInput = new QLineEdit();
    phoneInput->setPlaceholderText("Enter Phone Number");
    patientBasicFormLayout->addRow("Phone:", phoneInput);
    
    // Right column for medical details
    QFormLayout* patientMedicalFormLayout = new QFormLayout();
    patientMedicalFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    patientMedicalFormLayout->setSpacing(8);

    diseaseInput = new QLineEdit();
    diseaseInput->setPlaceholderText("Enter Disease");
    patientMedicalFormLayout->addRow("Diagnosis:", diseaseInput);

    treatmentInput = new QLineEdit();
    treatmentInput->setPlaceholderText("Enter Treatment");
    patientMedicalFormLayout->addRow("Treatment:", treatmentInput);
    
    // Add daily rate input
    dailyRateInput = new QDoubleSpinBox();
    dailyRateInput->setRange(50.0, 1000.0);
    dailyRateInput->setValue(100.0);
    dailyRateInput->setPrefix("$");
    patientMedicalFormLayout->addRow("Daily Rate:", dailyRateInput);

    // Add both columns to the patient info layout
    patientInfoLayout->addLayout(patientBasicFormLayout);
    patientInfoLayout->addLayout(patientMedicalFormLayout);
    patientInfoGroup->setLayout(patientInfoLayout);
    
    // Create assignments group
    QGroupBox* patientAssignGroup = new QGroupBox("Staff Assignments");
    QFormLayout* patientAssignFormLayout = new QFormLayout();
    patientAssignFormLayout->setSpacing(8);
    
    doctorIDInput = new QLineEdit();
    doctorIDInput->setPlaceholderText("Enter Primary Doctor ID");
    patientAssignFormLayout->addRow("Primary Doctor ID:", doctorIDInput);

    nurseIDInput = new QLineEdit();
    nurseIDInput->setPlaceholderText("Enter Nurse ID");
    patientAssignFormLayout->addRow("Nurse ID:", nurseIDInput);
    
    patientAssignGroup->setLayout(patientAssignFormLayout);
    
    // Create hospital assignment group
    QGroupBox* hospitalAssignGroup = new QGroupBox("Hospital Assignment");
    QFormLayout* hospitalFormLayout = new QFormLayout();
    hospitalFormLayout->setSpacing(8);
    
    // Hospital selection for admitting patients
    hospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        hospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    hospitalFormLayout->addRow("Admit to Hospital:", hospitalComboBox);
    
    // Add a second combo box for relocation
    relocateHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        relocateHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    hospitalFormLayout->addRow("Relocate to Hospital:", relocateHospitalComboBox);
    hospitalAssignGroup->setLayout(hospitalFormLayout);
    
    // Create horizontal layout for the two assignment groups
    QHBoxLayout* assignmentsLayout = new QHBoxLayout();
    assignmentsLayout->addWidget(patientAssignGroup);
    assignmentsLayout->addWidget(hospitalAssignGroup);
    
    // Create actions group
    QGroupBox* patientActionsGroup = new QGroupBox("Patient Actions");
    QVBoxLayout* patientActionsLayout = new QVBoxLayout();

    // Patient management buttons in horizontal layout
    QHBoxLayout* mainPatientButtonLayout = new QHBoxLayout();
    QPushButton* addPatientButton = new QPushButton("Add Patient");
    addPatientButton->setIcon(QIcon::fromTheme("list-add"));
    addPatientButton->setMinimumHeight(35);
    
    QPushButton* relocatePatientButton = new QPushButton("Relocate Patient");
    relocatePatientButton->setIcon(QIcon::fromTheme("edit-copy"));
    relocatePatientButton->setMinimumHeight(35);
    
    QPushButton* dischargePatientButton = new QPushButton("Discharge Patient");
    dischargePatientButton->setIcon(QIcon::fromTheme("edit-delete"));
    dischargePatientButton->setMinimumHeight(35);
    
    QPushButton* viewPatientDetailsButton = new QPushButton("View Patient Details");
    viewPatientDetailsButton->setIcon(QIcon::fromTheme("document-properties"));
    viewPatientDetailsButton->setMinimumHeight(35);
    
    mainPatientButtonLayout->addWidget(addPatientButton);
    mainPatientButtonLayout->addWidget(relocatePatientButton);
    mainPatientButtonLayout->addWidget(dischargePatientButton);
    mainPatientButtonLayout->addWidget(viewPatientDetailsButton);
    
    // System action buttons in horizontal layout
    QHBoxLayout* systemButtonLayout = new QHBoxLayout();
    QPushButton* displayStatusButton = new QPushButton("Display All Hospital Status");
    displayStatusButton->setIcon(QIcon::fromTheme("view-list"));
    displayStatusButton->setMinimumHeight(35);
    
    QPushButton* displayPharmacyButton = new QPushButton("Display Pharmacy Status");
    displayPharmacyButton->setIcon(QIcon::fromTheme("view-list"));
    displayPharmacyButton->setMinimumHeight(35);
    
    // Adjust layout to expand "Display Selected Hospital" button
    selectedHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        selectedHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    QPushButton* displaySelectedHospitalButton = new QPushButton("Display Selected Hospital");
    displaySelectedHospitalButton->setIcon(QIcon::fromTheme("view-list"));
    displaySelectedHospitalButton->setMinimumHeight(35);
    displaySelectedHospitalButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    systemButtonLayout->addWidget(selectedHospitalComboBox);
    systemButtonLayout->addWidget(displaySelectedHospitalButton);
    systemButtonLayout->addWidget(displayStatusButton);
    systemButtonLayout->addWidget(displayPharmacyButton);
    
    patientActionsLayout->addLayout(mainPatientButtonLayout);
    patientActionsLayout->addSpacing(5);
    patientActionsLayout->addLayout(systemButtonLayout);
    patientActionsGroup->setLayout(patientActionsLayout);
    
    // Add all patient management widgets to the layout
    patientLayout->addWidget(patientInfoGroup);
    patientLayout->addLayout(assignmentsLayout);
    patientLayout->addWidget(patientActionsGroup);
    patientLayout->addStretch(1);
    
    patientTab->setLayout(patientLayout);

    // ===== DOCTOR MANAGEMENT TAB =====
    // Using existing updated doctor tab layout
    QWidget* doctorTab = new QWidget();
    QVBoxLayout* doctorLayout = new QVBoxLayout(doctorTab);    
    doctorLayout->setSpacing(15);

    // Create a group box for doctor information entry
    QGroupBox* doctorInfoGroup = new QGroupBox("Doctor Information");
    QFormLayout* docManagementFormLayout = new QFormLayout(doctorInfoGroup);
    docManagementFormLayout->setSpacing(8);

    docManageIDInput = new QLineEdit();
    docManageIDInput->setPlaceholderText("Enter Doctor ID");
    docManagementFormLayout->addRow("Doctor ID:", docManageIDInput);

    doctorNameInput = new QLineEdit();
    doctorNameInput->setPlaceholderText("Enter Doctor Name");
    docManagementFormLayout->addRow("Name:", doctorNameInput); 

    // Create a group box for hospital assignments
    QGroupBox* docHospitalAssignGroup = new QGroupBox("Hospital Assignment");
    QFormLayout* hospitalAssignLayout = new QFormLayout(docHospitalAssignGroup);
    hospitalAssignLayout->setSpacing(8);
    
    // Hospital selection for adding or reassigning a doctor
    docHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        docHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    hospitalAssignLayout->addRow("Assign to Hospital:", docHospitalComboBox);

    docChangeHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        docChangeHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    hospitalAssignLayout->addRow("Reassign to Hospital:", docChangeHospitalComboBox);
    
    // Create horizontal layout to hold the two group boxes side by side
    QHBoxLayout* topGroupsLayout = new QHBoxLayout();
    topGroupsLayout->addWidget(doctorInfoGroup);
    topGroupsLayout->addWidget(docHospitalAssignGroup);
    
    // Add the horizontal layout to the main doctor layout
    doctorLayout->addLayout(topGroupsLayout);
    
    // Create action buttons group
    QGroupBox* actionGroup = new QGroupBox("Actions");
    QVBoxLayout* actionLayout = new QVBoxLayout(actionGroup);
    
    // Add List All Doctors button at the top of actions
    QPushButton* listAllDoctorsButton = new QPushButton("List All Doctors");
    listAllDoctorsButton->setIcon(QIcon::fromTheme("view-list"));
    listAllDoctorsButton->setMinimumHeight(40);
    actionLayout->addWidget(listAllDoctorsButton);
    
    // Add horizontal layout for doctor action buttons
    QHBoxLayout* docButtonLayout = new QHBoxLayout();
    
    // Create styled buttons with appropriate sizing
    QPushButton* addDoctorButton = new QPushButton("Add Doctor");
    addDoctorButton->setIcon(QIcon::fromTheme("list-add"));
    
    QPushButton* relocateDoctorButton = new QPushButton("Relocate Doctor");
    relocateDoctorButton->setIcon(QIcon::fromTheme("edit-copy"));
    
    QPushButton* removeDoctorButton = new QPushButton("Remove Doctor");
    removeDoctorButton->setIcon(QIcon::fromTheme("edit-delete"));
    
    QPushButton* viewDoctorDetailsButton = new QPushButton("View Doctor Details");
    viewDoctorDetailsButton->setIcon(QIcon::fromTheme("document-properties"));
    
    // Give all buttons equal sizing policies
    QList<QPushButton*> buttons = {addDoctorButton, relocateDoctorButton, 
                                  removeDoctorButton, viewDoctorDetailsButton};
    for (QPushButton* button : buttons) {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        button->setMinimumHeight(35);
        docButtonLayout->addWidget(button);
    }
    
    // Add the button layout to the actions group
    actionLayout->addLayout(docButtonLayout);
    
    // Add the actions group to the main doctor layout
    doctorLayout->addWidget(actionGroup);
    
    // Add stretch to ensure the bottom text display gets remaining space
    doctorLayout->addStretch(1);
    
    doctorTab->setLayout(doctorLayout);

    // ===== NURSE MANAGEMENT TAB =====
    QWidget* nurseTab = new QWidget();
    QVBoxLayout* nurseLayout = new QVBoxLayout(nurseTab);
    nurseLayout->setSpacing(15);

    // Create a group box for nurse information
    QGroupBox* nurseInfoGroup = new QGroupBox("Nurse Information");
    QFormLayout* nurseManagementFormLayout = new QFormLayout(nurseInfoGroup);
    nurseManagementFormLayout->setSpacing(8);

    nurseManageIDInput = new QLineEdit();
    nurseManageIDInput->setPlaceholderText("Enter Nurse ID");
    nurseManagementFormLayout->addRow("Nurse ID:", nurseManageIDInput);

    nurseNameInput = new QLineEdit();
    nurseNameInput->setPlaceholderText("Enter Nurse Name");
    nurseManagementFormLayout->addRow("Name:", nurseNameInput);

    // Create a group box for hospital assignments
    QGroupBox* nurseHospitalGroup = new QGroupBox("Hospital Assignment");
    QFormLayout* nurseHospitalLayout = new QFormLayout(nurseHospitalGroup);
    nurseHospitalLayout->setSpacing(8);

    // Hospital selection
    nurseHospitalComboBox = new QComboBox();
    nurseChangeHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        QString name = QString::fromStdString(hospital->getHospitalName());
        nurseHospitalComboBox->addItem(name);
        nurseChangeHospitalComboBox->addItem(name);
    }
    nurseHospitalLayout->addRow("Assign to Hospital:", nurseHospitalComboBox);
    nurseHospitalLayout->addRow("Reassign to Hospital:", nurseChangeHospitalComboBox);

    // Create horizontal layout to hold the two group boxes side by side
    QHBoxLayout* topNurseGroupsLayout = new QHBoxLayout();
    topNurseGroupsLayout->addWidget(nurseInfoGroup);
    topNurseGroupsLayout->addWidget(nurseHospitalGroup);

    // Create action buttons group
    QGroupBox* nurseActionGroup = new QGroupBox("Actions");
    QVBoxLayout* nurseActionLayout = new QVBoxLayout(nurseActionGroup);

    // Add List All Nurses button at the top of actions
    QPushButton* listAllNursesButton = new QPushButton("List All Nurses");
    listAllNursesButton->setIcon(QIcon::fromTheme("view-list"));
    listAllNursesButton->setMinimumHeight(40);
    nurseActionLayout->addWidget(listAllNursesButton);

    // Add horizontal layout for nurse action buttons
    QHBoxLayout* nurseManagementButtonLayout = new QHBoxLayout();
    
    QPushButton* addNurseButton = new QPushButton("Add Nurse");
    addNurseButton->setIcon(QIcon::fromTheme("list-add"));
    
    QPushButton* relocateNurseButton = new QPushButton("Relocate Nurse");
    relocateNurseButton->setIcon(QIcon::fromTheme("edit-copy"));
    
    QPushButton* removeNurseButton = new QPushButton("Remove Nurse");
    removeNurseButton->setIcon(QIcon::fromTheme("edit-delete"));
    
    QPushButton* viewNurseDetailsButton = new QPushButton("View Nurse Details");
    viewNurseDetailsButton->setIcon(QIcon::fromTheme("document-properties"));

    // Give all buttons equal sizing policies
    QList<QPushButton*> nurseButtons = {addNurseButton, relocateNurseButton, 
                                       removeNurseButton, viewNurseDetailsButton};
    for (QPushButton* button : nurseButtons) {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        button->setMinimumHeight(35);
        nurseManagementButtonLayout->addWidget(button);
    }

    nurseActionLayout->addLayout(nurseManagementButtonLayout);

    // Add all components to the main nurse layout
    nurseLayout->addLayout(topNurseGroupsLayout);
    nurseLayout->addWidget(nurseActionGroup);
    nurseLayout->addStretch(1);
    
    nurseTab->setLayout(nurseLayout);
    
    // ===== DOCTOR-PATIENT TAB =====
    QWidget* doctorPatientTab = new QWidget();
    QVBoxLayout* doctorPatientLayout = new QVBoxLayout(doctorPatientTab);
    doctorPatientLayout->setSpacing(15);
    
    // Create assignment group
    QGroupBox* doctorAssignmentGroup = new QGroupBox("Doctor-Patient Assignment");
    QFormLayout* doctorFormLayout = new QFormLayout(doctorAssignmentGroup);
    doctorFormLayout->setSpacing(8);
    
    doctorAssignmentIDInput = new QLineEdit();
    doctorAssignmentIDInput->setPlaceholderText("Enter Doctor ID");
    doctorFormLayout->addRow("Doctor ID:", doctorAssignmentIDInput);
    
    doctorPatientAssignmentIDInput = new QLineEdit();
    doctorPatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    doctorFormLayout->addRow("Patient ID:", doctorPatientAssignmentIDInput);
    
    // Create action buttons group
    QGroupBox* doctorPatientActionGroup = new QGroupBox("Actions");
    QVBoxLayout* doctorPatientActionLayout = new QVBoxLayout(doctorPatientActionGroup);
    
    // Add the List All Patients button
    QPushButton* doctorListPatientsButton = new QPushButton("List All Patients");
    doctorListPatientsButton->setIcon(QIcon::fromTheme("view-list"));
    doctorListPatientsButton->setMinimumHeight(40);
    doctorPatientActionLayout->addWidget(doctorListPatientsButton);
    
    // Add assignment buttons in their own layout
    QHBoxLayout* doctorButtonLayout = new QHBoxLayout();
    QPushButton* assignDoctorButton = new QPushButton("Assign Doctor");
    assignDoctorButton->setIcon(QIcon::fromTheme("list-add"));
    assignDoctorButton->setMinimumHeight(35);
    
    QPushButton* setPrimaryDoctorButton = new QPushButton("Set as Primary Doctor");
    setPrimaryDoctorButton->setIcon(QIcon::fromTheme("emblem-important"));
    setPrimaryDoctorButton->setMinimumHeight(35);
    
    doctorButtonLayout->addWidget(assignDoctorButton);
    doctorButtonLayout->addWidget(setPrimaryDoctorButton);
    
    doctorPatientActionLayout->addLayout(doctorButtonLayout);
    
    // Discharge request section
    QGroupBox* dischargeGroupBox = new QGroupBox("Request Patient Discharge");
    QFormLayout* dischargeFormLayout = new QFormLayout();
    dischargeFormLayout->setSpacing(8);
    
    doctorDischargeIDInput = new QLineEdit();
    doctorDischargeIDInput->setPlaceholderText("Doctor requesting discharge");
    dischargeFormLayout->addRow("Doctor ID:", doctorDischargeIDInput);
    
    patientDischargeIDInput = new QLineEdit();
    patientDischargeIDInput->setPlaceholderText("Patient to discharge");
    dischargeFormLayout->addRow("Patient ID:", patientDischargeIDInput);
    
    QPushButton* requestDischargeButton = new QPushButton("Request Discharge");
    requestDischargeButton->setIcon(QIcon::fromTheme("edit-delete"));
    requestDischargeButton->setMinimumHeight(35);
    dischargeFormLayout->addRow("", requestDischargeButton);
    dischargeGroupBox->setLayout(dischargeFormLayout);

    // Add all components to the main layout
    doctorPatientLayout->addWidget(doctorAssignmentGroup);
    doctorPatientLayout->addWidget(doctorPatientActionGroup);
    doctorPatientLayout->addWidget(dischargeGroupBox);
    doctorPatientLayout->addStretch(1);
    
    doctorPatientTab->setLayout(doctorPatientLayout);

    // ===== NURSE-PATIENT TAB =====
    QWidget* nursePatientTab = new QWidget();
    QVBoxLayout* nursePatientLayout = new QVBoxLayout(nursePatientTab);
    nursePatientLayout->setSpacing(15);

    // Create assignment group
    QGroupBox* nurseAssignmentGroup = new QGroupBox("Nurse-Patient Assignment");
    QFormLayout* nurseFormLayout = new QFormLayout(nurseAssignmentGroup);
    nurseFormLayout->setSpacing(8);
    
    nurseAssignmentIDInput = new QLineEdit();
    nurseAssignmentIDInput->setPlaceholderText("Enter Nurse ID");
    nurseFormLayout->addRow("Nurse ID:", nurseAssignmentIDInput);

    nursePatientAssignmentIDInput = new QLineEdit();
    nursePatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    nurseFormLayout->addRow("Patient ID:", nursePatientAssignmentIDInput);

    // Create action buttons group
    QGroupBox* nursePatientActionGroup = new QGroupBox("Actions");
    QVBoxLayout* nursePatientActionLayout = new QVBoxLayout(nursePatientActionGroup);
    
    // Add the List All Patients button
    QPushButton* nurseListPatientsButton = new QPushButton("List All Patients");
    nurseListPatientsButton->setIcon(QIcon::fromTheme("view-list"));
    nurseListPatientsButton->setMinimumHeight(40);
    nursePatientActionLayout->addWidget(nurseListPatientsButton);
    
    // Add assignment buttons
    QPushButton* assignNurseButton = new QPushButton("Assign Nurse");
    assignNurseButton->setIcon(QIcon::fromTheme("list-add"));
    assignNurseButton->setMinimumHeight(35);
    nursePatientActionLayout->addWidget(assignNurseButton);

    // Add all components to the main layout
    nursePatientLayout->addWidget(nurseAssignmentGroup);
    nursePatientLayout->addWidget(nursePatientActionGroup);
    nursePatientLayout->addStretch(1);
    
    nursePatientTab->setLayout(nursePatientLayout);
    
    // ===== BILLING TAB =====
    QWidget* billingTab = new QWidget();
    QVBoxLayout* billingLayout = new QVBoxLayout(billingTab);
    billingLayout->setSpacing(15);

    // Create billing info group
    QGroupBox* billingInfoGroup = new QGroupBox("Billing Information");
    QFormLayout* billingFormLayout = new QFormLayout(billingInfoGroup);
    billingFormLayout->setSpacing(8);
    
    billingPatientIDInput = new QLineEdit();
    billingPatientIDInput->setPlaceholderText("Enter Patient ID");
    billingFormLayout->addRow("Patient ID:", billingPatientIDInput);

    // Add current bill display
    currentBillLabel = new QLabel("$0.00");
    currentBillLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    billingFormLayout->addRow("Current Bill:", currentBillLabel);
    
    // Add payment amount input
    paymentAmountInput = new QDoubleSpinBox();
    paymentAmountInput->setRange(0.0, 10000.0);
    paymentAmountInput->setPrefix("$");
    paymentAmountInput->setValue(0.0);
    billingFormLayout->addRow("Payment Amount:", paymentAmountInput);

    // Create billing actions group
    QGroupBox* billingActionGroup = new QGroupBox("Billing Actions");
    QVBoxLayout* billingActionLayout = new QVBoxLayout(billingActionGroup);

    // Individual patient billing actions
    QHBoxLayout* billingButtonLayout = new QHBoxLayout();
    QPushButton* calculateBillButton = new QPushButton("Calculate Bill");
    calculateBillButton->setIcon(QIcon::fromTheme("accessories-calculator"));
    calculateBillButton->setMinimumHeight(35);
    
    QPushButton* collectPaymentButton = new QPushButton("Collect Payment");
    collectPaymentButton->setIcon(QIcon::fromTheme("emblem-money"));
    collectPaymentButton->setMinimumHeight(35);
    
    billingButtonLayout->addWidget(calculateBillButton);
    billingButtonLayout->addWidget(collectPaymentButton);
    
    // System-wide billing actions
    QPushButton* billingReportButton = new QPushButton("Generate Billing Report");
    billingReportButton->setIcon(QIcon::fromTheme("office-chart"));
    billingReportButton->setMinimumHeight(35);
    
    QPushButton* pharmacyBillingButton = new QPushButton("Generate Pharmacy Billing");
    pharmacyBillingButton->setIcon(QIcon::fromTheme("office-chart"));
    pharmacyBillingButton->setMinimumHeight(35);
    
    billingActionLayout->addLayout(billingButtonLayout);
    billingActionLayout->addWidget(billingReportButton);
    billingActionLayout->addWidget(pharmacyBillingButton);
    
    // Add all components to the main billing layout
    billingLayout->addWidget(billingInfoGroup);
    billingLayout->addWidget(billingActionGroup);
    billingLayout->addStretch(1);
    
    billingTab->setLayout(billingLayout);

    // Connect the buttons
    connect(calculateBillButton, &QPushButton::clicked, this, &MainWindow::calculateBill);
    connect(collectPaymentButton, &QPushButton::clicked, this, &MainWindow::collectPayment);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(pharmacyBillingButton, &QPushButton::clicked, this, &MainWindow::showPharmacyBillingReport);
    
    // ===== DRUG DELIVERY TAB =====
    QWidget* drugDeliveryTab = new QWidget();
    QVBoxLayout* drugDeliveryLayout = new QVBoxLayout(drugDeliveryTab);
    drugDeliveryLayout->setSpacing(15);

    // Create drug delivery form group
    QGroupBox* drugDeliveryGroup = new QGroupBox("Drug Delivery Request");
    QFormLayout* drugFormLayout = new QFormLayout(drugDeliveryGroup);
    drugFormLayout->setSpacing(8);

    drugHospitalComboBox = new QComboBox();
    auto allHospitals = hospitalSystem->getAllHospitals();
    for (auto hospital : allHospitals) {
        drugHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    drugFormLayout->addRow("Destination Hospital:", drugHospitalComboBox);

    drugPharmacyComboBox = new QComboBox();
    auto allPharmacies = PharmacySystem::getInstance()->getAllPharmacies();
    for (auto pharmacy : allPharmacies) {
        drugPharmacyComboBox->addItem(
            QString::fromStdString(pharmacy->getPharmacyName() + " (" + pharmacy->getPharmacyID() + ")")
            );
    }
    drugFormLayout->addRow("Source Pharmacy:", drugPharmacyComboBox);

    drugComboBox = new QComboBox();
    auto allDrugs = PharmacySystem::getInstance()->getAllDrugs();
    for (auto &d : allDrugs) {
        drugComboBox->addItem(QString::fromStdString(d.getDrugName()));
    }
    drugFormLayout->addRow("Select Drug:", drugComboBox);

    QPushButton* requestDeliveryButton = new QPushButton("Request Delivery");
    requestDeliveryButton->setIcon(QIcon::fromTheme("mail-send"));
    requestDeliveryButton->setMinimumHeight(35);
    drugFormLayout->addRow("", requestDeliveryButton);

    // Add the drug delivery group to the main layout
    drugDeliveryLayout->addWidget(drugDeliveryGroup);
    drugDeliveryLayout->addStretch(1);
    
    drugDeliveryTab->setLayout(drugDeliveryLayout);

    connect(requestDeliveryButton, &QPushButton::clicked, this, &MainWindow::requestDrugDelivery);

    // Add the tabs to the tab widget
    tabWidget->addTab(patientTab, "Patient Management");
    tabWidget->addTab(doctorTab, "Doctor Management");
    tabWidget->addTab(nurseTab, "Nurse Management");
    tabWidget->addTab(doctorPatientTab, "Doctor-Patient");
    tabWidget->addTab(nursePatientTab, "Nurse-Patient");
    tabWidget->addTab(billingTab, "Billing");
    tabWidget->addTab(drugDeliveryTab, "Drug Delivery");
    
    // Create status display
    statusDisplay = new QTextEdit();
    statusDisplay->setReadOnly(true);
    
    // Set minimum heights for both main components
    tabWidget->setMinimumHeight(300);
    statusDisplay->setMinimumHeight(150);
    
    // Add the tab widget and status display to the splitter
    mainSplitter->addWidget(tabWidget);
    mainSplitter->addWidget(statusDisplay);
    
    // Set initial sizes for splitter (70% tabs, 30% status)
    QList<int> sizes;
    sizes << 700 << 300;
    mainSplitter->setSizes(sizes);
    
    // Add a horizontal line separator between the main areas
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // Create a clear button for the status display
    QPushButton* clearDisplayButton = new QPushButton("Clear Display");
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
    connect(clearDisplayButton, &QPushButton::clicked, this, &MainWindow::clearStatusDisplay);

    // Add widgets to the main layout
    mainLayout->addWidget(mainSplitter, 1);
    mainLayout->addWidget(line);
    mainLayout->addWidget(clearDisplayButton);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital Management System");

    resize(1000, 800);  // Slightly larger default size

    // Connect buttons to slots
    connect(addPatientButton, &QPushButton::clicked, this, &MainWindow::addPatient);
    connect(relocatePatientButton, &QPushButton::clicked, this, &MainWindow::relocatePatient);
    connect(dischargePatientButton, &QPushButton::clicked, this, &MainWindow::dischargePatient);
    connect(displayStatusButton, &QPushButton::clicked, this, &MainWindow::displayHospitalStatus);
    connect(displayPharmacyButton, &QPushButton::clicked, this, &MainWindow::displayPharmacyStatus);
    connect(viewPatientDetailsButton, &QPushButton::clicked, this, &MainWindow::viewPatientDetails);

    connect(addDoctorButton, &QPushButton::clicked, this, &MainWindow::addDoctor);
    connect(relocateDoctorButton, &QPushButton::clicked, this, &MainWindow::relocateDoctor);
    connect(removeDoctorButton, &QPushButton::clicked, this, &MainWindow::removeDoctor);
    connect(viewDoctorDetailsButton, &QPushButton::clicked, this, &MainWindow::viewDoctorDetails);

    connect(addNurseButton, &QPushButton::clicked, this, &MainWindow::addNurse);
    connect(relocateNurseButton, &QPushButton::clicked, this, &MainWindow::relocateNurse);
    connect(removeNurseButton, &QPushButton::clicked, this, &MainWindow::removeNurse);
    connect(viewNurseDetailsButton, &QPushButton::clicked, this, &MainWindow::viewNurseDetails);

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
    connect(displaySelectedHospitalButton, &QPushButton::clicked, this, &MainWindow::displaySelectedHospitalStatus);

    connect(listAllDoctorsButton, &QPushButton::clicked, this, &MainWindow::listAllDoctors);
    connect(listAllNursesButton, &QPushButton::clicked, this, &MainWindow::listAllNurses);

    // Set up timer for daily updates
    dayUpdateTimer = new QTimer(this);
    connect(dayUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDayCounter);
    dayUpdateTimer->start(86400000); // 24 hours in milliseconds
    hospitalSystem->updateAllPatientDays(); // Initial update    
    
    // Set up timer for time updates
    timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTime);
    timeUpdateTimer->start(1000); // Update every second
    
    updateCurrentTime(); // Initial time update
    
    statusDisplay->append("System initialized. Daily updates will occur every 24 hours.");
    statusDisplay->append("Welcome to the Hospital Management System!");
    statusDisplay->append("---------------------------------------------");

    // Update the style sheet to ensure no black text on dark background
    qApp->setStyleSheet(R"(
    QWidget {
            background-color: #2b2b2b;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }

        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit {
            background-color: #3c3f41;
            color: #f0f0f0;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 4px;
            selection-background-color: #4a90e2;
            selection-color: #ffffff;
        }
        
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #4a90e2;
        }
        
        QLineEdit:disabled, QComboBox:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled {
            background-color: #323232;
            color: #8a8a8a;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
            /* remove arrow or add alternative design */
        }

        QComboBox::down-arrow {
            width: 0;
            height: 0;
            border: none;
            /* hide or replace with custom icon/content */
        }
        
        QComboBox QAbstractItemView {
            background-color: #3c3f41;
            color: #f0f0f0;
            border: 1px solid #555;
        }

        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            /* remove default arrows or use alternative design */
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
        QPushButton:pressed {
            background-color: #2868A0;
        }
        QPushButton:disabled {
            background-color: #555555;
            color: #aaaaaa;
        }

        QLabel {
            color: #f0f0f0;
            font-weight: bold;
        }

        QTabWidget::pane {
            border: 1px solid #555;
            padding: 6px;
        }

        QTabBar::tab {
            background: #3a3a3a;
            color: #f0f0f0;
            padding: 8px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }

        QTabBar::tab:selected {
            background: #4a90e2;
            color: white;
        }
        
        QGroupBox {
            border: 1px solid #555;
            border-radius: 5px;
            margin-top: 1ex;
            padding-top: 10px;
            color: #f0f0f0;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 5px;
            font-weight: bold;
            color: #4a90e2;
        }
        
        QSplitter::handle {
            background-color: #555;
            height: 2px;
        }
        
        QFrame[frameShape="4"] { /* HLine */
            background-color: #555;
            height: 1px;
        }
        
        QTextEdit {
            background-color: #2d2d2d;
            color: #f0f0f0;
            border: 1px solid #444;
            selection-background-color: #4a90e2;
            selection-color: #ffffff;
        }
        
        QScrollBar:vertical {
            border: none;
            background: #2b2b2b;
            width: 10px;
            margin: 0px;
        }
        
        QScrollBar::handle:vertical {
            background: #555;
            min-height: 20px;
            border-radius: 5px;
        }
        
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        
        QScrollBar:horizontal {
            border: none;
            background: #2b2b2b;
            height: 10px;
            margin: 0px;
        }
        
        QScrollBar::handle:horizontal {
            background: #555;
            min-width: 20px;
            border-radius: 5px;
        }
        
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
        
        QHeaderView::section {
            background-color: #3a3a3a;
            color: #f0f0f0;
            padding: 5px;
            border: 1px solid #555;
        }
        
        QToolTip {
            background-color: #2b2b2b;
            color: #f0f0f0;
            border: 1px solid #555;
            padding: 3px;
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
    return doctor->getHospitalID() == hospital->getHospitalID();
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
    return nurse->getHospitalID() == hospital->getHospitalID();
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
    for (auto doctor : currentHospital->getDoctors()) {
        doctor->removePatient(patientID);
    }
    patient->getAttendingDoctorIDs().clear();
    patient->setPrimaryDoctorID("");

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
    statusDisplay->append("Hospital: " + QString::fromStdString(hospital->getHospitalName()));
    
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
    statusDisplay->append("Patient ID: " + QString::fromStdString(patient->getPatientID()));
    statusDisplay->append("Name: " + QString::fromStdString(patient->getPatientName()));
    statusDisplay->append("Days Admitted: " + QString::number(patient->getDaysAdmitted()));
    statusDisplay->append("Daily Rate: $" + QString::number(patient->getBillingRatePerDay(), 'f', 2));
    statusDisplay->append("Current Total: $" + QString::number(patient->calculateCurrentBill(), 'f', 2));
    statusDisplay->append("Status: " + QString::fromStdString(patient->getStatus()));
    statusDisplay->append("\nNote: Additional charges for medications and services may apply.");
}

void MainWindow::addDoctor() {

    string docID = docManageIDInput->text().toStdString();
    string docName = doctorNameInput->text().toStdString();
    int hospitalIndex = docHospitalComboBox->currentIndex();

    // Validate inputs
    if (docID.empty() || docName.empty()) {
        statusDisplay->append("Error: All fields must be filled out.");
        return;
    }
    
    // Check if doctor already exists
    if (hospitalSystem->findDoctor(docID)) {
        statusDisplay->append("Error: Doctor ID already exists.");
        return;
    }

    Doctor* doctor = new Doctor(docID, docName, hospitalSystem->getHospital(hospitalIndex)->getHospitalID());

    if (hospitalSystem->addDoctor(doctor, hospitalIndex)) {
        statusDisplay->append("Doctor added successfully to " + 
            docHospitalComboBox->currentText());        
        docManageIDInput->clear();
        doctorNameInput->clear();
    } else {
        statusDisplay->append("Error: There are already 50 doctors");
        delete doctor;
        return;
    }

}

void MainWindow::relocateDoctor() {
    
    string docID = docManageIDInput->text().toStdString();
    int newHospitalIndex = docChangeHospitalComboBox->currentIndex();

    if (docID.empty()) {
        statusDisplay->append("Error: Doctor ID must be provided.");
        return;
    }

    Doctor* doctor = hospitalSystem->findDoctor(docID);

    if (!doctor) {
        statusDisplay->append("Error: Doctor not found.");
        return;
    }    

    Hospital* currentHospital = hospitalSystem->findDoctorHospital(docID);
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);
    
    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital selection is invalid.");
        return;
    }

    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        statusDisplay->append("Error: Doctor is already in the hospital you selected.");
        return;        
    }

    if (!doctor->getPatientIDs().empty()) {
        statusDisplay->append("Error: Doctor has assigned patients and can't be moved. Either assign the patients to another doctor or discharge them when they're done.");
        return;          
    }

    if (hospitalSystem->relocateDoctor(docID, newHospitalIndex)) {
        statusDisplay->append("Doctor " + QString::fromStdString(docID) +
        " relocated to " + docChangeHospitalComboBox->currentText());        
    } else {
        statusDisplay->append("Error: Unexpected"); 
    }
}

void MainWindow::removeDoctor() {
    string docID = docManageIDInput->text().toStdString();

    if (docID.empty()) {
        statusDisplay->append("Error: Doctor ID must be provided.");
        return;
    }

    Doctor* doctor = hospitalSystem->findDoctor(docID);
    if (!doctor) {
        statusDisplay->append("Error: Doctor Not Found or Already Removed.");
        return;
    }

    Hospital* currentHospital = hospitalSystem->findDoctorHospital(docID);
    if (!currentHospital) {
        statusDisplay->append("Error: Cannot Determine Doctor's Hospital.");
        return;
    }

    // Check explicitly here for the minimum doctor requirement
    if (currentHospital->getDoctors().size() <= 3) {
        statusDisplay->append("Error: Cannot Remove Doctor. Hospital Must Maintain At Least 3 Doctors!");
        return;
    }

    if (!doctor->getPatientIDs().empty()) {
        statusDisplay->append("Error: Doctor Has Assigned Patients and Can't Be Removed. Either Assign the Patients to Another Doctor or Discharge Them When They're Done.");
        return;          
    }

    try {
        if (hospitalSystem->removeDoctor(docID)) {
            statusDisplay->append("Doctor " + QString::fromStdString(docID) +
            " Removed From System.");            
        } else {
            statusDisplay->append("Error: Could Not Remove Doctor!");
        }
    } catch (const std::runtime_error& e) {
        // Display the specific exception message
        statusDisplay->append("Error: " + QString(e.what()));
    } catch (...) {
        statusDisplay->append("Error: An Unexpected Error Occurred!");
    }
}

void MainWindow::viewDoctorDetails() {
    string doctorID = docManageIDInput->text().toStdString();
    
    if (doctorID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to view details.");
        return;
    }
    
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {
        statusDisplay->append("Doctor not found.");
        return;
    }
    
    // Get the hospital information to display
    Hospital* hospital = hospitalSystem->findDoctorHospital(doctorID);
    if (!hospital) {
        statusDisplay->append("Error: Cannot locate patient's hospital.");
        return;
    }

    // Display patient details
    statusDisplay->append(QString::fromStdString(doctor->getFullDescription()));
    
    // Display hospital information with patient details
    statusDisplay->append("Working in Hospital: " + QString::fromStdString(hospital->getHospitalName()));
    statusDisplay->append("\n");
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
    
    if (doctor->getHospitalID() != patientHospital->getHospitalID()) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(doctorID) + 
                             " does not work at " + QString::fromStdString(patientHospital->getHospitalName()) + 
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

    if (nurse->getHospitalID() != patientHospital->getHospitalID()) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(nurseID) + 
                             " does not work at " + QString::fromStdString(patientHospital->getHospitalName()) + 
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
            std::vector<Bill> bills = pharmacy->getBillsForHospital(hospital->getHospitalID());
            for (const auto& bill : bills) {
                if (!bill.paid) {
                    hospitalTotals[hospital->getHospitalName()] += bill.amount;
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
    
    // Display a notification about the update
    statusDisplay->append("Daily update completed. Patient days incremented.");
    
    // Check for any rate increases and display them
    for (auto& pair : hospitalSystem->getAllPatients()) {
        Patient* patient = pair.second;
        if (patient->getDaysAdmitted() % 3 == 0) { // Match the condition in incrementDaysAdmitted
            statusDisplay->append("Rate increase applied for patient " + 
                                QString::fromStdString(patient->getPatientID()) + 
                                ". New daily rate: $" + 
                                QString::number(patient->getBillingRatePerDay(), 'f', 2));
        }
    }
    
    // If the current tab is the billing tab, update the displayed bill
    if (tabWidget->currentIndex() == 3) { // Billing tab is index 3
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
        Hospital* hospital = hospitalSystem->findPatientHospital(patient->getPatientID());
        
        if (hospital) {
            patientsByHospital[hospital->getHospitalName()].push_back(patient);
        }
    }
    
    // Display patients grouped by hospital
    for (const auto& pair : patientsByHospital) {
        statusDisplay->append("\n--- " + QString::fromStdString(pair.first) + " ---");
        
        for (const Patient* patient : pair.second) {
            statusDisplay->append("Patient ID: " + QString::fromStdString(patient->getPatientID()));
            statusDisplay->append("Name: " + QString::fromStdString(patient->getPatientName()));
            statusDisplay->append("Disease: " + QString::fromStdString(patient->getDisease()));
            statusDisplay->append("Days Admitted: " + QString::number(patient->getDaysAdmitted()));
            statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->getPrimaryDoctorID()));
            
            // Show attending nurses if any
            if (!patient->getAttendingNursesIDs().empty()) {
                QString attendingNurses = "Assigned Nurses: ";
                for (size_t i = 0; i < patient->getAttendingNursesIDs().size(); i++) {
                    attendingNurses += QString::fromStdString(patient->getAttendingNursesIDs()[i]);
                    if (i < patient->getAttendingNursesIDs().size() - 1) {
                        attendingNurses += ", ";
                    }
                }
                statusDisplay->append(attendingNurses);
            }
            
            // Show attending doctors if any
            if (!patient->getAttendingDoctorIDs().empty()) {
                QString attendingDocs = "Attending Doctors: ";
                for (size_t i = 0; i < patient->getAttendingDoctorIDs().size(); i++) {
                    attendingDocs += QString::fromStdString(patient->getAttendingDoctorIDs()[i]);
                    if (i < patient->getAttendingDoctorIDs().size() - 1) {
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
    std::string billID = pharmacy->billHospitalForDrug(hospital->getHospitalID(), selectedDrug.getDrugName(), selectedDrug.getPrice());

    statusDisplay->append("Drug \"" + QString::fromStdString(selectedDrug.getDrugName()) +
                          "\" billed to " + QString::fromStdString(hospital->getHospitalName()) +
                          " for $" + QString::number(selectedDrug.getPrice(), 'f', 2) +
                          " (Bill ID: " + QString::fromStdString(billID) + ")");
}

void MainWindow::clearStatusDisplay() {
    statusDisplay->clear();
}

void MainWindow::updateCurrentTime() {
    QTime currentTime = QTime::currentTime();
    QString timeText = "Current Time: " + currentTime.toString("h:mm:ss AP");

    QTextCursor cursor = statusDisplay->textCursor();
    int scrollPos = statusDisplay->verticalScrollBar()->value(); // Save scroll position

    QString text = statusDisplay->toPlainText();
    QStringList lines = text.split("\n");

    bool timeLineUpdated = false;
    for (int i = 0; i < lines.size(); ++i) {
        if (lines[i].startsWith("Current Time:")) {
            lines[i] = timeText;
            timeLineUpdated = true;
            break;
        }
    }

    if (!timeLineUpdated) {
        lines.prepend(timeText);
    }

    statusDisplay->blockSignals(true); // prevent triggering anything while updating
    statusDisplay->setPlainText(lines.join("\n"));
    statusDisplay->blockSignals(false);

    statusDisplay->verticalScrollBar()->setValue(scrollPos); // Restore scroll position
}

void MainWindow::displaySelectedHospitalStatus() {
    int hospitalIndex = selectedHospitalComboBox->currentIndex();
    
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        statusDisplay->append("Error: Invalid hospital selection.");
        return;
    }
    
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        statusDisplay->append("Error: Hospital not found.");
        return;
    }
    
    statusDisplay->clear();
    statusDisplay->append("=== SELECTED HOSPITAL STATUS ===\n");
    
    // Display hospital overview
    statusDisplay->append("Hospital: " + QString::fromStdString(hospital->getHospitalName()));
    statusDisplay->append("ID: " + QString::fromStdString(hospital->getHospitalID()));
    statusDisplay->append("Patients Admitted: " + QString::number(hospital->getPatients().size()) + "/20");
    statusDisplay->append("Doctors: " + QString::number(hospital->getDoctors().size()));
    statusDisplay->append("Nurses: " + QString::number(hospital->getNurses().size()));
    
    // Add detailed doctor information
    if (!hospital->getDoctors().empty()) {
        statusDisplay->append("\n--- Doctor Details ---");
        for (auto doctor : hospital->getDoctors()) {
            statusDisplay->append("ID: " + QString::fromStdString(doctor->getDoctorID()) +
                                 ", Name: " + QString::fromStdString(doctor->getDoctorName()) +
                                 ", Patients: " + QString::number(doctor->getPatientIDs().size()));
        }
        statusDisplay->append("");  // Changed from "\n" to "" for single line space
    }
    
    // Add detailed nurse information
    if (!hospital->getNurses().empty()) {
        statusDisplay->append("--- Nurse Details ---");
        for (auto nurse : hospital->getNurses()) {
            statusDisplay->append("ID: " + QString::fromStdString(nurse->getNurseID()) +
                                 ", Name: " + QString::fromStdString(nurse->getNurseName()) +
                                 ", Patients: " + QString::number(nurse->getPatientIDs().size()) + "/2");
        }
        statusDisplay->append("");  // Changed from "\n" to "" for single line space
    }
    
    // Display patient information with improved formatting
    if (!hospital->getPatients().empty()) {
        statusDisplay->append("=================== PATIENT DETAILS ===================");  // Removed "\n"
        
        for (auto patient : hospital->getPatients()) {
            statusDisplay->append("PATIENT INFORMATION");
            statusDisplay->append("Name:   \t\t" + QString::fromStdString(patient->getPatientName()));
            statusDisplay->append("ID:   \t\t" + QString::fromStdString(patient->getPatientID()));
            statusDisplay->append("Phone:   \t\t" + QString::fromStdString(patient->getPhoneNumber()));
            statusDisplay->append("Days Admitted:\t " + QString::number(patient->getDaysAdmitted()));
            statusDisplay->append("");
            
            statusDisplay->append("MEDICAL INFORMATION");
            statusDisplay->append("Diagnosis:      " + QString::fromStdString(patient->getDisease()));
            statusDisplay->append("Treatment:      " + QString::fromStdString(patient->getTreatment()));
            statusDisplay->append("");
            
            statusDisplay->append("MEDICAL STAFF");
            statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->getPrimaryDoctorID()));
            
            if (!patient->getAttendingDoctorIDs().empty()) {
                statusDisplay->append("Attending Doctors:");
                for (auto& docID : patient->getAttendingDoctorIDs()) {
                    statusDisplay->append("  • " + QString::fromStdString(docID));
                }
            }
            
            if (!patient->getAttendingNursesIDs().empty()) {
                statusDisplay->append("Attending Nurses:");
                for (auto& nurseID : patient->getAttendingNursesIDs()) {
                    statusDisplay->append("  • " + QString::fromStdString(nurseID));
                }
            }
            
            statusDisplay->append("======================================================\n");
        }
    } else {
        statusDisplay->append("No Patients Currently Admitted To This Hospital!");
    }
}

void MainWindow::addNurse() {
    string nurseID = nurseManageIDInput->text().toStdString();
    string nurseName = nurseNameInput->text().toStdString();
    int hospitalIndex = nurseHospitalComboBox->currentIndex();

    if (nurseID.empty() || nurseName.empty()) {
        statusDisplay->append("Error: All fields must be filled out.");
        return;
    }

    if (hospitalSystem->findNurse(nurseID)) {
        statusDisplay->append("Error: Nurse ID already exists.");
        return;
    }

    Nurse* nurse = new Nurse(nurseID, nurseName, hospitalSystem->getHospital(hospitalIndex)->getHospitalID());

    if (hospitalSystem->addNurse(nurse, hospitalIndex)) {
        statusDisplay->append("Nurse added successfully to " + nurseHospitalComboBox->currentText());
        nurseManageIDInput->clear();
        nurseNameInput->clear();
    } else {
        statusDisplay->append("Error: There are already 20 nurses in this hospital.");
        delete nurse;
    }
}

void MainWindow::relocateNurse() {
    string nurseID = nurseManageIDInput->text().toStdString();
    int newHospitalIndex = nurseChangeHospitalComboBox->currentIndex();

    if (nurseID.empty()) {
        statusDisplay->append("Error: Nurse ID must be provided.");
        return;
    }

    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Error: Nurse not found.");
        return;
    }

    Hospital* currentHospital = hospitalSystem->findNurseHospital(nurseID);
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);

    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital selection is invalid.");
        return;
    }

    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        statusDisplay->append("Error: Nurse is already in the selected hospital.");
        return;
    }

    if (!nurse->getPatientIDs().empty()) {
        statusDisplay->append("Error: Nurse is assigned to patients. Reassign or discharge them first.");
        return;
    }

    if (hospitalSystem->relocateNurse(nurseID, newHospitalIndex)) {
        statusDisplay->append("Nurse " + QString::fromStdString(nurseID) +
                              " relocated to " + nurseChangeHospitalComboBox->currentText());
    } else {
        statusDisplay->append("Error: Could not relocate nurse.");
    }
}

void MainWindow::removeNurse() {
    string nurseID = nurseManageIDInput->text().toStdString();

    if (nurseID.empty()) {
        statusDisplay->append("Error: Nurse ID must be provided.");
        return;
    }

    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Error: Nurse not found or already removed.");
        return;
    }

    if (!nurse->getPatientIDs().empty()) {
        statusDisplay->append("Error: Nurse is assigned to patients and cannot be removed.");
        return;
    }

    Hospital* hospital = hospitalSystem->findNurseHospital(nurseID);
    if (!hospital) {
        statusDisplay->append("Error: Could not determine nurse's hospital.");
        return;
    }

    if (hospital->getNurses().size() <= 3) {
        statusDisplay->append("Error: Hospital must maintain at least 3 nurses.");
        return;
    }

    if (hospitalSystem->removeNurse(nurseID)) {
        statusDisplay->append("Nurse " + QString::fromStdString(nurseID) + " removed successfully.");
    } else {
        statusDisplay->append("Error: Failed to remove nurse.");
    }
}

void MainWindow::viewNurseDetails() {
    string nurseID = nurseManageIDInput->text().toStdString();

    if (nurseID.empty()) {
        statusDisplay->append("Error: Enter Nurse ID to view details.");
        return;
    }

    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Nurse not found.");
        return;
    }

    Hospital* hospital = hospitalSystem->findNurseHospital(nurseID);
    if (!hospital) {
        statusDisplay->append("Error: Cannot determine nurse's hospital.");
        return;
    }

    statusDisplay->append(QString::fromStdString(nurse->getFullDescription()));
    statusDisplay->append("Working in Hospital: " + QString::fromStdString(hospital->getHospitalName()));
}

void MainWindow::listAllDoctors() {
    statusDisplay->clear();
    statusDisplay->append("=== ALL DOCTORS IN SYSTEM ===");
    
    map<string, Doctor*> doctorMap;
    
    // Get all doctors from all hospitals
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        for (auto doctor : hospital->getDoctors()) {
            doctorMap[doctor->getDoctorID()] = doctor;
        }
    }
    // Check if there are no doctors in the system
    if (doctorMap.empty()) {
        statusDisplay->append("No Doctors In The System!");
        return;
    }
    
    // Group doctors by hospital for better organization
    map<string, vector<Doctor*>> doctorsByHospital;
    
    for (const auto& pair : doctorMap) {
        Doctor* doctor = pair.second;
        Hospital* hospital = hospitalSystem->findDoctorHospital(doctor->getDoctorID());
        // Check if the hospital is valid
        if (hospital) {
            doctorsByHospital[hospital->getHospitalName()].push_back(doctor);
        }
    }
    
    // Display doctors grouped by hospital
    for (const auto& pair : doctorsByHospital) {
        statusDisplay->append("\n=== " + QString::fromStdString(pair.first) + " ===");
        
        for (const Doctor* doctor : pair.second) {
            statusDisplay->append("\nDoctor ID: " + QString::fromStdString(doctor->getDoctorID()));
            statusDisplay->append("Name: " + QString::fromStdString(doctor->getDoctorName()));
            statusDisplay->append("Patients: " + QString::number(doctor->getPatientIDs().size()));
            
            // Show patient IDs if any
            if (!doctor->getPatientIDs().empty()) {
                QString patientList = "Patient IDs: ";
                for (size_t i = 0; i < doctor->getPatientIDs().size(); i++) {
                    patientList += QString::fromStdString(doctor->getPatientIDs()[i]);
                    if (i < doctor->getPatientIDs().size() - 1) {
                        patientList += ", ";
                    }
                }
                statusDisplay->append(patientList);
            } else {
                statusDisplay->append("No Assigned Patients");
            }
            statusDisplay->append("-----------------------------------");
        }
    }
    // Show count at the end
    statusDisplay->append("\nTotal Doctors: " + QString::number(doctorMap.size()));
}

void MainWindow::listAllNurses() {
    statusDisplay->clear();
    statusDisplay->append("=== ALL NURSES IN SYSTEM ===");
    
    map<string, Nurse*> nurseMap;
    
    // Get all nurses from all hospitals
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        for (auto nurse : hospital->getNurses()) {
            nurseMap[nurse->getNurseID()] = nurse;
        }
    }
    // Check if there are no nurses in the system
    if (nurseMap.empty()) {
        statusDisplay->append("No nurses in the system.");
        return;
    }
    
    // Group nurses by hospital for better organization
    map<string, vector<Nurse*>> nursesByHospital;
    
    for (const auto& pair : nurseMap) {
        Nurse* nurse = pair.second;
        Hospital* hospital = hospitalSystem->findNurseHospital(nurse->getNurseID());
        // Check if the hospital is valid
        if (hospital) {
            nursesByHospital[hospital->getHospitalName()].push_back(nurse);
        }
    }
    
    // Display nurses grouped by hospital
    for (const auto& pair : nursesByHospital) {
        statusDisplay->append("\n=== " + QString::fromStdString(pair.first) + " ===");
        
        for (const Nurse* nurse : pair.second) {
            statusDisplay->append("\nNurse ID: " + QString::fromStdString(nurse->getNurseID()));
            statusDisplay->append("Name: " + QString::fromStdString(nurse->getNurseName()));
            statusDisplay->append("Patients: " + QString::number(nurse->getPatientIDs().size()) + "/2");
            
            // Show patient IDs if any
            if (!nurse->getPatientIDs().empty()) {
                QString patientList = "Patient IDs: ";
                for (size_t i = 0; i < nurse->getPatientIDs().size(); i++) {
                    patientList += QString::fromStdString(nurse->getPatientIDs()[i]);
                    if (i < nurse->getPatientIDs().size() - 1) {
                        patientList += ", ";
                    }
                }
                statusDisplay->append(patientList);
            } else {
                statusDisplay->append("No Assigned Patients");
            }
            statusDisplay->append("-----------------------------------");
        }
    }
    // Show count at the end
    statusDisplay->append("\nTotal Nurses: " + QString::number(nurseMap.size()));
}