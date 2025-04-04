// We have included the necessary libraries for the GUI from Qt framework documentation
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
#include <QDebug> // We used this to help debug the code
#include <QSplitter>
#include <QFrame>

// This is the constructor for the MainWindow class that initializes the UI and connects signals and slots
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Get the hospital system instance
    hospitalSystem = HospitalSystem::getInstance();
    
    // Set the window icon for when the user has the application open (FAB icon)
    QIcon icon(":/images/hospital_icon.png");
    if (!icon.isNull()) {       // Check if the icon is valid
        setWindowIcon(icon);    // Set the window icon for the main window
        QApplication::setWindowIcon(icon); // Also set application-wide icon
    } else {
        qDebug("Failed To Load Icon Image! Check File Path.");
    }
    // Need to start by creating the main window
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10); // This sets the margins for the main layout

    // Then we created a splitter to allow users to resize the proportion between tabs and status display
    QSplitter* mainSplitter = new QSplitter(Qt::Vertical);
    mainSplitter->setChildrenCollapsible(false);

    // Next we created a frame to hold the status display
    tabWidget = new QTabWidget();
    tabWidget->setDocumentMode(true);
    tabWidget->setTabPosition(QTabWidget::North);

    // ===== PATIENT MANAGEMENT TAB =====
    QWidget* patientTab = new QWidget();    // This will be the main widget for the patient management tab
    QVBoxLayout* patientLayout = new QVBoxLayout(patientTab);
    patientLayout->setSpacing(15);  // Set spacing between widgets in the layout
    
    // Create patient info group
    QGroupBox* patientInfoGroup = new QGroupBox("Patient Information");
    QHBoxLayout* patientInfoLayout = new QHBoxLayout();
    
    // For the left column for basic patient details, we will use a form layout
    QFormLayout* patientBasicFormLayout = new QFormLayout();
    patientBasicFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    patientBasicFormLayout->setSpacing(8);
    patientIDInput = new QLineEdit();   // This will be the input field for the patient ID
    patientIDInput->setPlaceholderText("Enter Patient ID");
    patientBasicFormLayout->addRow("Patient ID:", patientIDInput);  // Add the row to the form layout 
    // Next we will add the name and phone number input fields
    nameInput = new QLineEdit();
    nameInput->setPlaceholderText("Enter Patient Name");
    patientBasicFormLayout->addRow("Name:", nameInput);
    phoneInput = new QLineEdit();
    phoneInput->setPlaceholderText("Enter Phone Number");
    patientBasicFormLayout->addRow("Phone:", phoneInput);

    // Next we created the right column for the medical details
    QFormLayout* patientMedicalFormLayout = new QFormLayout();
    patientMedicalFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    patientMedicalFormLayout->setSpacing(8);    // Set spacing between widgets in the layout
    // Add the medical details input fields
    diseaseInput = new QLineEdit();
    diseaseInput->setPlaceholderText("Enter Disease");
    patientMedicalFormLayout->addRow("Diagnosis:", diseaseInput);
    // Add treatment input
    treatmentInput = new QLineEdit();
    treatmentInput->setPlaceholderText("Enter Treatment");
    patientMedicalFormLayout->addRow("Treatment:", treatmentInput);
    // Add daily rate input
    dailyRateInput = new QDoubleSpinBox();
    dailyRateInput->setRange(50.0, 1000.0); // We just picked the range of 50 to 1000
    dailyRateInput->setValue(100.0);
    dailyRateInput->setPrefix("$");
    patientMedicalFormLayout->addRow("Daily Rate:", dailyRateInput);
    // Add both columns to the patient info layout
    patientInfoLayout->addLayout(patientBasicFormLayout);
    patientInfoLayout->addLayout(patientMedicalFormLayout);
    patientInfoGroup->setLayout(patientInfoLayout);

    // Then we created the patient assignments group so we can assign the doctor and nurse to the patient
    QGroupBox* patientAssignGroup = new QGroupBox("Staff Assignments");
    QFormLayout* patientAssignFormLayout = new QFormLayout();   // This will be the layout for the staff assignments
    patientAssignFormLayout->setSpacing(8);                     // Set spacing between widgets in the layout
    // Add the doctor and nurse ID input fields, grab and store the IDs
    doctorIDInput = new QLineEdit();
    doctorIDInput->setPlaceholderText("Enter Primary Doctor ID");
    patientAssignFormLayout->addRow("Primary Doctor ID:", doctorIDInput);
    nurseIDInput = new QLineEdit();
    nurseIDInput->setPlaceholderText("Enter Nurse ID");
    patientAssignFormLayout->addRow("Nurse ID:", nurseIDInput);
    patientAssignGroup->setLayout(patientAssignFormLayout); // Set the layout for the group box

    // Next is the hospital assignment group
    QGroupBox* hospitalAssignGroup = new QGroupBox("Hospital Assignment");
    QFormLayout* hospitalFormLayout = new QFormLayout();    // This will be the layout for the hospital assignment
    hospitalFormLayout->setSpacing(8);                      // We picked to set the spacing to 8 for this layout
    // Within the hospital assignment group, we created two combo boxes for hospital selection
    hospitalComboBox = new QComboBox();
    for (auto hospitalAdmit : hospitalSystem->getAllHospitals()) {  // For loop through all hospitals
        hospitalComboBox->addItem(QString::fromStdString(hospitalAdmit->getHospitalName())); // In the loop we added the hospital names to the combo box
    }
    hospitalFormLayout->addRow("Admit to Hospital:", hospitalComboBox); // Add hospital selection combo box for initial patient admission
    // Need to add another combo box for the hospital relocation
    relocateHospitalComboBox = new QComboBox();
    for (auto hospitalRelocate : hospitalSystem->getAllHospitals()) {   // For loop through all hospitals
        relocateHospitalComboBox->addItem(QString::fromStdString(hospitalRelocate->getHospitalName()));   // In the loop we added the hospital names to the combo box
    }
    hospitalFormLayout->addRow("Relocate to Hospital:", relocateHospitalComboBox);   // Add hospital selection combo box for patient relocation
    hospitalAssignGroup->setLayout(hospitalFormLayout);   // Set the layout for the group box

    // After these definitions, we created a horizontal layout to hold the two assignment groups side by side
    QHBoxLayout* assignmentsLayout = new QHBoxLayout();
    assignmentsLayout->addWidget(patientAssignGroup);
    assignmentsLayout->addWidget(hospitalAssignGroup);
    
    // We then created actions group which will hold the buttons for patient management
    QGroupBox* patientActionsGroup = new QGroupBox("Patient Actions");
    QVBoxLayout* patientActionsLayout = new QVBoxLayout();  // This will be the layout for the patient actions

    // Patient management buttons in horizontal layout
    QHBoxLayout* mainPatientButtonLayout = new QHBoxLayout();
    QPushButton* addPatientButton = new QPushButton("Add Patient");
    addPatientButton->setIcon(QIcon::fromTheme("list-add"));    // list-add icon is from the Qt library
    addPatientButton->setMinimumHeight(35);

    // Next was the relocation button, including the icon and the minimum height
    QPushButton* relocatePatientButton = new QPushButton("Relocate Patient");
    relocatePatientButton->setIcon(QIcon::fromTheme("edit-copy"));  // edit-copy icon is from the Qt library
    relocatePatientButton->setMinimumHeight(35);
    
    // Next was the discharge button, including the icon and the minimum height
    QPushButton* dischargePatientButton = new QPushButton("Discharge Patient");
    dischargePatientButton->setIcon(QIcon::fromTheme("edit-delete"));  // edit-delete icon is from the Qt library
    dischargePatientButton->setMinimumHeight(35);   // This will set the minimum height for the button
    
    // For this we are creating a button to view the patient details, when clicked it will show the details of the patient
    QPushButton* viewPatientDetailsButton = new QPushButton("View Patient Details");
    viewPatientDetailsButton->setIcon(QIcon::fromTheme("document-properties")); // document-properties icon is from the Qt library
    viewPatientDetailsButton->setMinimumHeight(35);
    
    // For the mainPatientButtonLayout we added the buttons to the layout
    mainPatientButtonLayout->addWidget(addPatientButton);
    mainPatientButtonLayout->addWidget(relocatePatientButton);
    mainPatientButtonLayout->addWidget(dischargePatientButton);
    mainPatientButtonLayout->addWidget(viewPatientDetailsButton);
    
    // Next are the system action buttons which we organized into a horizontal layout
    QHBoxLayout* systemButtonLayout = new QHBoxLayout();
    QPushButton* displayStatusButton = new QPushButton("Display All Hospital Status");
    displayStatusButton->setIcon(QIcon::fromTheme("view-list"));    // view-list icon is from the Qt library
    displayStatusButton->setMinimumHeight(35);  // Made the display all hospital status button the same size as the others
    QPushButton* displayPharmacyButton = new QPushButton("Display Pharmacy Status"); // This button will show the pharmacy status    
    displayPharmacyButton->setIcon(QIcon::fromTheme("view-list"));  // view-list icon is from the Qt library
    displayPharmacyButton->setMinimumHeight(35);
    
    // Adjust layout to expand "Display Selected Hospital" button
    selectedHospitalComboBox = new QComboBox();
    for (auto hospitalSelected : hospitalSystem->getAllHospitals()) {   // For loop through all hospitals
        selectedHospitalComboBox->addItem(QString::fromStdString(hospitalSelected->getHospitalName())); // In the loop we added the hospital names to the combo box
    }
    // This combo box will be used to select the hospital for the display button
    QPushButton* displaySelectedHospitalButton = new QPushButton("Display Selected Hospital");
    displaySelectedHospitalButton->setIcon(QIcon::fromTheme("view-list"));
    displaySelectedHospitalButton->setMinimumHeight(35);
    displaySelectedHospitalButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);   // This will set the size policy for the button to expand

    // Next for each of the buttons we add them to the system button layout
    systemButtonLayout->addWidget(selectedHospitalComboBox);
    systemButtonLayout->addWidget(displaySelectedHospitalButton);
    systemButtonLayout->addWidget(displayStatusButton);
    systemButtonLayout->addWidget(displayPharmacyButton);
    // We also add the mainPatientButtonLayout to the patient actions layout
    patientActionsLayout->addLayout(mainPatientButtonLayout);
    patientActionsLayout->addSpacing(5);
    patientActionsLayout->addLayout(systemButtonLayout);
    patientActionsGroup->setLayout(patientActionsLayout);    
    // Add all patient management widgets to the layout
    patientLayout->addWidget(patientInfoGroup);
    patientLayout->addLayout(assignmentsLayout);
    patientLayout->addWidget(patientActionsGroup);
    patientLayout->addStretch(1); // Add stretch to ensure the bottom text display gets remaining space
    patientTab->setLayout(patientLayout);   // This will set the layout for the patient tab

    // ===== DOCTOR MANAGEMENT TAB =====
    // Using existing updated doctor tab layout
    QWidget* doctorTab = new QWidget();
    QVBoxLayout* doctorLayout = new QVBoxLayout(doctorTab);    
    doctorLayout->setSpacing(15);
    // We created a group box for doctor information entry
    QGroupBox* doctorInfoGroup = new QGroupBox("Doctor Information");
    QFormLayout* docManagementFormLayout = new QFormLayout(doctorInfoGroup);    // This will be the layout for the doctor information
    docManagementFormLayout->setSpacing(8);
    // Add the doctor ID and name input fields
    docManageIDInput = new QLineEdit();
    docManageIDInput->setPlaceholderText("Enter Doctor ID");
    docManagementFormLayout->addRow("Doctor ID:", docManageIDInput);
    doctorNameInput = new QLineEdit();
    doctorNameInput->setPlaceholderText("Enter Doctor Name");
    docManagementFormLayout->addRow("Name:", doctorNameInput); 

    // We then wanted to create a group box for hospital assignments
    QGroupBox* docHospitalAssignGroup = new QGroupBox("Hospital Assignment");
    QFormLayout* hospitalAssignLayout = new QFormLayout(docHospitalAssignGroup);
    hospitalAssignLayout->setSpacing(8);
    // Hospital selection for adding or reassigning a doctor
    docHospitalComboBox = new QComboBox();
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        docHospitalComboBox->addItem(QString::fromStdString(hospital->getHospitalName()));
    }
    hospitalAssignLayout->addRow("Assign to Hospital:", docHospitalComboBox);
    // Hospital selection for reassigning a doctor
    docChangeHospitalComboBox = new QComboBox();
    for (auto hospitalRelocate : hospitalSystem->getAllHospitals()) {
        docChangeHospitalComboBox->addItem(QString::fromStdString(hospitalRelocate->getHospitalName()));
    }
    hospitalAssignLayout->addRow("Reassign to Hospital:", docChangeHospitalComboBox);   // This will add the hospital selection combo box for doctor relocation
    
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
    QPushButton* listAllDoctorsButton = new QPushButton("List All Doctors");    // This is for the list all doctors button
    listAllDoctorsButton->setIcon(QIcon::fromTheme("view-list"));   // view-list icon is from the Qt library
    listAllDoctorsButton->setMinimumHeight(40);
    actionLayout->addWidget(listAllDoctorsButton);      // This will add the list all doctors button to the action layout
    QHBoxLayout* docButtonLayout = new QHBoxLayout();   // Add horizontal layout for doctor action buttons
    
    // We then added each of the buttons to the layout with the icons
    QPushButton* addDoctorButton = new QPushButton("Add Doctor");
    addDoctorButton->setIcon(QIcon::fromTheme("list-add"));
    QPushButton* relocateDoctorButton = new QPushButton("Relocate Doctor");
    relocateDoctorButton->setIcon(QIcon::fromTheme("edit-copy"));
    QPushButton* removeDoctorButton = new QPushButton("Remove Doctor");
    removeDoctorButton->setIcon(QIcon::fromTheme("edit-delete"));
    QPushButton* viewDoctorDetailsButton = new QPushButton("View Doctor Details");
    viewDoctorDetailsButton->setIcon(QIcon::fromTheme("document-properties"));
    
    // We then made sure that for each of the buttons we set the size policy to expanding and the minimum height to 35
    QList<QPushButton*> buttons = {addDoctorButton, relocateDoctorButton, removeDoctorButton, viewDoctorDetailsButton};
    for (QPushButton* button : buttons) {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        button->setMinimumHeight(35);
        docButtonLayout->addWidget(button);
    }
    // Add the button layout to the actions group
    actionLayout->addLayout(docButtonLayout);
    // Add the actions group to the main doctor layout
    doctorLayout->addWidget(actionGroup);
    // We then put a stretch to ensure the bottom text display gets the rest of the spacing
    doctorLayout->addStretch(1);
    // Add all components to the main doctor layout
    doctorTab->setLayout(doctorLayout);

    // ===== NURSE MANAGEMENT TAB =====
    // Next we created the nurse management tab
    QWidget* nurseTab = new QWidget();
    QVBoxLayout* nurseLayout = new QVBoxLayout(nurseTab);
    nurseLayout->setSpacing(15);
    // Create a group box for nurse information
    QGroupBox* nurseInfoGroup = new QGroupBox("Nurse Information");
    QFormLayout* nurseManagementFormLayout = new QFormLayout(nurseInfoGroup);   // This will be the layout for the nurse information
    nurseManagementFormLayout->setSpacing(8);   // We set the spacing to 8 for this layout
    // Add the nurse ID and name input fields
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
    // For loop through all hospitals and add them to the combo boxes
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        QString name = QString::fromStdString(hospital->getHospitalName());
        nurseHospitalComboBox->addItem(name);
        nurseChangeHospitalComboBox->addItem(name);
    }
    nurseHospitalLayout->addRow("Assign to Hospital:", nurseHospitalComboBox);
    nurseHospitalLayout->addRow("Reassign to Hospital:", nurseChangeHospitalComboBox);

    // We added a horizontal layout to hold the two group boxes side by side
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
    // We added a horizontal layout to hold the two group boxes side by side
    QHBoxLayout* nurseManagementButtonLayout = new QHBoxLayout();
    
    // We then added each of the buttons to the layout with the icons
    QPushButton* addNurseButton = new QPushButton("Add Nurse");
    addNurseButton->setIcon(QIcon::fromTheme("list-add"));
    QPushButton* relocateNurseButton = new QPushButton("Relocate Nurse");
    relocateNurseButton->setIcon(QIcon::fromTheme("edit-copy"));
    QPushButton* removeNurseButton = new QPushButton("Remove Nurse");
    removeNurseButton->setIcon(QIcon::fromTheme("edit-delete"));
    QPushButton* viewNurseDetailsButton = new QPushButton("View Nurse Details");
    viewNurseDetailsButton->setIcon(QIcon::fromTheme("document-properties"));

    // We made sure that for each of the buttons we set the size policy to expanding and the minimum height to 35
    QList<QPushButton*> nurseButtons = {addNurseButton, relocateNurseButton, removeNurseButton, viewNurseDetailsButton};
    for (QPushButton* button : nurseButtons) {
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        button->setMinimumHeight(35);
        nurseManagementButtonLayout->addWidget(button);
    }
    // Add the button layout to the actions group
    nurseActionLayout->addLayout(nurseManagementButtonLayout);
    // Add all components to the main nurse layout
    nurseLayout->addLayout(topNurseGroupsLayout);
    nurseLayout->addWidget(nurseActionGroup);
    nurseLayout->addStretch(1);
    // Add the nurse tab to the main layout
    nurseTab->setLayout(nurseLayout);
    
    // ===== DOCTOR-PATIENT TAB =====
    // Next we created the doctor-patient tab
    QWidget* doctorPatientTab = new QWidget();
    QVBoxLayout* doctorPatientLayout = new QVBoxLayout(doctorPatientTab);
    doctorPatientLayout->setSpacing(15);
    // Next we created a group box for doctor-patient assignment
    QGroupBox* doctorAssignmentGroup = new QGroupBox("Doctor-Patient Assignment");
    QFormLayout* doctorFormLayout = new QFormLayout(doctorAssignmentGroup);
    doctorFormLayout->setSpacing(8);
    // Add the doctor and patient ID input fields
    doctorAssignmentIDInput = new QLineEdit();
    doctorAssignmentIDInput->setPlaceholderText("Enter Doctor ID");
    doctorFormLayout->addRow("Doctor ID:", doctorAssignmentIDInput);
    doctorPatientAssignmentIDInput = new QLineEdit();
    doctorPatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    doctorFormLayout->addRow("Patient ID:", doctorPatientAssignmentIDInput);
    
    // We then created a group box for doctor-patient actions
    QGroupBox* doctorPatientActionGroup = new QGroupBox("Actions");
    QVBoxLayout* doctorPatientActionLayout = new QVBoxLayout(doctorPatientActionGroup);
    // Add the List All Patients button
    QPushButton* doctorListPatientsButton = new QPushButton("List All Patients");
    doctorListPatientsButton->setIcon(QIcon::fromTheme("view-list"));
    doctorListPatientsButton->setMinimumHeight(40);
    doctorPatientActionLayout->addWidget(doctorListPatientsButton);
    
    // The next part is to create the action buttons for doctor-patient assignment
    QHBoxLayout* doctorButtonLayout = new QHBoxLayout();
    QPushButton* assignDoctorButton = new QPushButton("Assign Doctor");
    assignDoctorButton->setIcon(QIcon::fromTheme("list-add"));
    assignDoctorButton->setMinimumHeight(35);
    QPushButton* setPrimaryDoctorButton = new QPushButton("Set as Primary Doctor");
    setPrimaryDoctorButton->setIcon(QIcon::fromTheme("emblem-important"));
    setPrimaryDoctorButton->setMinimumHeight(35);
    
    // Next we added the buttons to the layout
    doctorButtonLayout->addWidget(assignDoctorButton);
    doctorButtonLayout->addWidget(setPrimaryDoctorButton);
    doctorPatientActionLayout->addLayout(doctorButtonLayout);
    
    // Discharge request section
    QGroupBox* dischargeGroupBox = new QGroupBox("Request Patient Discharge");
    QFormLayout* dischargeFormLayout = new QFormLayout();
    dischargeFormLayout->setSpacing(8);
    // Add the doctor and patient ID input fields for discharge
    doctorDischargeIDInput = new QLineEdit();
    doctorDischargeIDInput->setPlaceholderText("Doctor requesting discharge");
    dischargeFormLayout->addRow("Doctor ID:", doctorDischargeIDInput);
    // Add the patient ID input field for discharge
    patientDischargeIDInput = new QLineEdit();
    patientDischargeIDInput->setPlaceholderText("Patient to discharge");
    dischargeFormLayout->addRow("Patient ID:", patientDischargeIDInput);
    
    // Add the discharge request button
    QPushButton* requestDischargeButton = new QPushButton("Request Discharge");
    requestDischargeButton->setIcon(QIcon::fromTheme("edit-delete"));
    requestDischargeButton->setMinimumHeight(35);
    dischargeFormLayout->addRow("", requestDischargeButton);
    dischargeGroupBox->setLayout(dischargeFormLayout);

    // We then included all of these aspects into the main layout
    doctorPatientLayout->addWidget(doctorAssignmentGroup);
    doctorPatientLayout->addWidget(doctorPatientActionGroup);
    doctorPatientLayout->addWidget(dischargeGroupBox);
    doctorPatientLayout->addStretch(1);
    // Add the doctor-patient tab to the main layout
    doctorPatientTab->setLayout(doctorPatientLayout);

    // ===== NURSE-PATIENT TAB =====
    // Next we created the nurse-patient tab
    QWidget* nursePatientTab = new QWidget();
    QVBoxLayout* nursePatientLayout = new QVBoxLayout(nursePatientTab);
    nursePatientLayout->setSpacing(15);
    // We also made sure to create a group box for nurse-patient assignment the same as above
    QGroupBox* nurseAssignmentGroup = new QGroupBox("Nurse-Patient Assignment");
    QFormLayout* nurseFormLayout = new QFormLayout(nurseAssignmentGroup);
    nurseFormLayout->setSpacing(8);
    
    // Add the nurse and patient ID input fields
    nurseAssignmentIDInput = new QLineEdit();
    nurseAssignmentIDInput->setPlaceholderText("Enter Nurse ID");
    nurseFormLayout->addRow("Nurse ID:", nurseAssignmentIDInput);
    nursePatientAssignmentIDInput = new QLineEdit();
    nursePatientAssignmentIDInput->setPlaceholderText("Enter Patient ID");
    nurseFormLayout->addRow("Patient ID:", nursePatientAssignmentIDInput);
    // We also made sure to create a group box for nurse-patient assignment the same as above
    QGroupBox* nursePatientActionGroup = new QGroupBox("Actions");
    QVBoxLayout* nursePatientActionLayout = new QVBoxLayout(nursePatientActionGroup);
    
    // Add the List All Patients button
    QPushButton* nurseListPatientsButton = new QPushButton("List All Patients");
    nurseListPatientsButton->setIcon(QIcon::fromTheme("view-list"));
    nurseListPatientsButton->setMinimumHeight(40);
    nursePatientActionLayout->addWidget(nurseListPatientsButton);

    // The next part is to create the action button for nurse-patient assignment
    QPushButton* assignNurseButton = new QPushButton("Assign Nurse");
    assignNurseButton->setIcon(QIcon::fromTheme("list-add"));
    assignNurseButton->setMinimumHeight(35);
    nursePatientActionLayout->addWidget(assignNurseButton);

    // We then included all of these aspects into the main layout
    nursePatientLayout->addWidget(nurseAssignmentGroup);
    nursePatientLayout->addWidget(nursePatientActionGroup);
    nursePatientLayout->addStretch(1);
    // Add the nurse-patient tab to the main layout
    nursePatientTab->setLayout(nursePatientLayout);
    
    // ===== BILLING TAB =====
    // Next we created the billing tab
    QWidget* billingTab = new QWidget();
    QVBoxLayout* billingLayout = new QVBoxLayout(billingTab);
    billingLayout->setSpacing(15);
    // Create a group box for billing information
    QGroupBox* billingInfoGroup = new QGroupBox("Billing Information");
    QFormLayout* billingFormLayout = new QFormLayout(billingInfoGroup);
    billingFormLayout->setSpacing(8);
    
    // Add the patient ID input field for billing
    billingPatientIDInput = new QLineEdit();
    billingPatientIDInput->setPlaceholderText("Enter Patient ID");
    billingFormLayout->addRow("Patient ID:", billingPatientIDInput);
    // Next we added a label to display the current bill amount
    currentBillLabel = new QLabel("$0.00");
    currentBillLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    billingFormLayout->addRow("Current Bill:", currentBillLabel);
    
    // Add payment amount input using a double spin box
    paymentAmountInput = new QDoubleSpinBox();
    paymentAmountInput->setRange(0.0, 10000.0);  // We set the range from 0 to 10000
    paymentAmountInput->setPrefix("$"); // This will add a prefix to the input field
    paymentAmountInput->setValue(0.0);
    billingFormLayout->addRow("Payment Amount:", paymentAmountInput);

    // We then created a group box for billing actions
    QGroupBox* billingActionGroup = new QGroupBox("Billing Actions");
    QVBoxLayout* billingActionLayout = new QVBoxLayout(billingActionGroup);
    // For the individual patient billing actions we used a horizontal layout
    QHBoxLayout* billingButtonLayout = new QHBoxLayout();
    QPushButton* calculateBillButton = new QPushButton("Calculate Bill");
    calculateBillButton->setIcon(QIcon::fromTheme("accessories-calculator"));
    calculateBillButton->setMinimumHeight(35);
    
    // Next we created the collect payment button
    QPushButton* collectPaymentButton = new QPushButton("Collect Payment");
    collectPaymentButton->setIcon(QIcon::fromTheme("emblem-money"));
    collectPaymentButton->setMinimumHeight(35);
    // This code will add the buttons to the billing button layout
    billingButtonLayout->addWidget(calculateBillButton);
    billingButtonLayout->addWidget(collectPaymentButton);
    
    // Next we created the system-wide billing action buttons
    QPushButton* billingReportButton = new QPushButton("Generate Billing Report");
    billingReportButton->setIcon(QIcon::fromTheme("office-chart"));
    billingReportButton->setMinimumHeight(35);
    // We also added a button for pharmacy billing reports
    QPushButton* pharmacyBillingButton = new QPushButton("Generate Pharmacy Billing");
    pharmacyBillingButton->setIcon(QIcon::fromTheme("office-chart"));
    pharmacyBillingButton->setMinimumHeight(35);
    
    // We then added the button layouts to the billing action layout
    billingActionLayout->addLayout(billingButtonLayout);
    billingActionLayout->addWidget(billingReportButton);
    billingActionLayout->addWidget(pharmacyBillingButton);
    
    // Add all components to the main billing layout
    billingLayout->addWidget(billingInfoGroup);
    billingLayout->addWidget(billingActionGroup);
    billingLayout->addStretch(1);
    // Add the billing tab to the main layout
    billingTab->setLayout(billingLayout);

    // We then needed to connect the buttons to their respective slots
    connect(calculateBillButton, &QPushButton::clicked, this, &MainWindow::calculateBill);
    connect(collectPaymentButton, &QPushButton::clicked, this, &MainWindow::collectPayment);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(pharmacyBillingButton, &QPushButton::clicked, this, &MainWindow::showPharmacyBillingReport);
    
    // ===== DRUG DELIVERY TAB =====
    // Next is to create the drug delivery tab which will be used to request drug deliveries
    QWidget* drugDeliveryTab = new QWidget();   // We created a new widget for the drug delivery tab
    QVBoxLayout* drugDeliveryLayout = new QVBoxLayout(drugDeliveryTab); // This will be the layout for the drug delivery tab
    drugDeliveryLayout->setSpacing(15);   // Set spacing between widgets in the layout

    // Create drug delivery form group which will have the drug delivery request tab
    QGroupBox* drugDeliveryGroup = new QGroupBox("Drug Delivery Request");
    QFormLayout* drugFormLayout = new QFormLayout(drugDeliveryGroup);
    drugFormLayout->setSpacing(8);
    // Add the hospital and pharmacy selection combo boxes
    drugHospitalComboBox = new QComboBox();
    auto allHospitals = hospitalSystem->getAllHospitals();
    // For loop through all hospitals and add them to the combo box
    for (auto hospitalDestination : allHospitals) {
        drugHospitalComboBox->addItem(QString::fromStdString(hospitalDestination->getHospitalName()));
    }
    drugFormLayout->addRow("Destination Hospital:", drugHospitalComboBox);  // Add hospital selection combo box for drug delivery
    // Add the pharmacy selection combo box so we can select the pharmacy for drug delivery
    drugPharmacyComboBox = new QComboBox();
    auto allPharmacies = PharmacySystem::getInstance()->getAllPharmacies(); // This variable will hold the pharmacy information
    // For loop through all pharmacies and add them to the combo box
    for (auto pharmacy : allPharmacies) {
        drugPharmacyComboBox->addItem(
            QString::fromStdString(pharmacy->getPharmacyName() + " (" + pharmacy->getPharmacyID() + ")"));
    }
    drugFormLayout->addRow("Source Pharmacy:", drugPharmacyComboBox);   // Add pharmacy selection combo box for drug delivery
    drugComboBox = new QComboBox();
    auto allDrugs = PharmacySystem::getInstance()->getAllDrugs();
    // For loop through all drugs and add them to the combo box so we can select the drug for delivery
    for (auto &drugsPicked : allDrugs) {
        drugComboBox->addItem(QString::fromStdString(drugsPicked.getDrugName()));
    }
    drugFormLayout->addRow("Select Drug:", drugComboBox);   // Add drug selection combo box for drug delivery
    // Next we added the request delivery button which will be used to request the drug delivery
    QPushButton* requestDeliveryButton = new QPushButton("Request Delivery");
    requestDeliveryButton->setIcon(QIcon::fromTheme("mail-send"));
    requestDeliveryButton->setMinimumHeight(35);
    drugFormLayout->addRow("", requestDeliveryButton);
    // Add the drug delivery group to the main layout
    drugDeliveryLayout->addWidget(drugDeliveryGroup);
    drugDeliveryLayout->addStretch(1);
    // Add the drug delivery tab to the main layout
    drugDeliveryTab->setLayout(drugDeliveryLayout);
    // Then we finished by connecting the request delivery button to the slot
    connect(requestDeliveryButton, &QPushButton::clicked, this, &MainWindow::requestDrugDelivery);

    // Add the tabs to the tab widget
    tabWidget->addTab(patientTab, "Patient Management");
    tabWidget->addTab(doctorTab, "Doctor Management");
    tabWidget->addTab(nurseTab, "Nurse Management");
    tabWidget->addTab(doctorPatientTab, "Doctor-Patient");
    tabWidget->addTab(nursePatientTab, "Nurse-Patient");
    tabWidget->addTab(billingTab, "Billing");
    tabWidget->addTab(drugDeliveryTab, "Drug Delivery");
    
    //=== SPLITTER AND STATUS DISPLAY ===
    // We needed to add the status display to the main layout and set it to read-only
    statusDisplay = new QTextEdit();
    statusDisplay->setReadOnly(true);
    // Set minimum heights for both main components
    tabWidget->setMinimumHeight(300);
    statusDisplay->setMinimumHeight(150);
    // Add the tab widget and status display to the splitter
    mainSplitter->addWidget(tabWidget);
    mainSplitter->addWidget(statusDisplay);
    // Set initial sizes for splitter (70% tabs, 30% status)
    QList<int> applicationSizes;
    applicationSizes << 700 << 300; 
    mainSplitter->setSizes(applicationSizes);

    // We thought it was a good idea to create a clear button for the status display
    QPushButton* clearDisplayButton = new QPushButton("Clear Display");
    clearDisplayButton->setStyleSheet(R"(QPushButton {
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
    // Then we connected the clear button to the clearStatusDisplay slot
    connect(clearDisplayButton, &QPushButton::clicked, this, &MainWindow::clearStatusDisplay);

    // Next thing to do is to add the main layout to the central widget
    mainLayout->addWidget(mainSplitter, 1);
    mainLayout->addWidget(clearDisplayButton);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Hospital Management System");   // We set the window title to Hospital Management System
    resize(1000, 800);  // Set the initial window size

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
        this->assignDoctorToPatient(true);  // This will set the doctor as primary
    });
    connect(requestDischargeButton, &QPushButton::clicked, this, &MainWindow::requestPatientDischarge);
    connect(assignNurseButton, &QPushButton::clicked, this, &MainWindow::assignNurseToPatient);
    connect(billingReportButton, &QPushButton::clicked, this, &MainWindow::showBillingReport);
    connect(doctorListPatientsButton, &QPushButton::clicked, this, &MainWindow::listAllPatients);
    connect(nurseListPatientsButton, &QPushButton::clicked, this, &MainWindow::listAllPatients);
    connect(displaySelectedHospitalButton, &QPushButton::clicked, this, &MainWindow::displaySelectedHospitalStatus);
    connect(listAllDoctorsButton, &QPushButton::clicked, this, &MainWindow::listAllDoctors);
    connect(listAllNursesButton, &QPushButton::clicked, this, &MainWindow::listAllNurses);

    // The requirements want us to bill the patient for every day they are in the hospital so we made a timer
    dayUpdateTimer = new QTimer(this);
    connect(dayUpdateTimer, &QTimer::timeout, this, &MainWindow::updateDayCounter);
    dayUpdateTimer->start(86400000); // 24 hours in milliseconds for daily updates
    hospitalSystem->updateAllPatientDays(); // Initial update    
    // Set up timer for time updates
    timeUpdateTimer = new QTimer(this);
    connect(timeUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCurrentTime);
    timeUpdateTimer->start(1000); // Update every second
    updateCurrentTime(); // Initial time update

    //=== INITIAL STATUS DISPLAY ===
    statusDisplay->append("System initialized. Daily updates will occur every 24 hours.");
    statusDisplay->append("Welcome to the Hospital Management System!");
    statusDisplay->append("----------------------------------------------------");
    // With our understanding of CSS we made some adjustments to the stylesheet to make it look more modern
    // Set the basic style for the entire application
    qApp->setStyleSheet(R"(QWidget {
            background-color: #2b2b2b;
            color: #f0f0f0;
            font-family: 'Segoe UI', sans-serif;
            font-size: 13px;
        }
        /* Style for input controls with dark background and light text */
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox, QTextEdit {
            background-color: #3c3f41;
            color: #f0f0f0;
            border: 1px solid #555;
            border-radius: 4px;
            padding: 4px;
            selection-background-color: #4a90e2;
            selection-color: #ffffff;
        }
        /* Highlight input controls when they have focus */
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #4a90e2;
        }
        /* Dim input controls when they are disabled */
        QLineEdit:disabled, QComboBox:disabled, QSpinBox:disabled, QDoubleSpinBox:disabled {
            background-color: #323232;
            color: #8a8a8a;
        }
        /* Style for the dropdown button in combo boxes */
        QComboBox::drop-down {
            border: none;
        }
        /* The arrow icon for combo boxes */
        QComboBox::down-arrow {
            width: 0;
            height: 0;
            border: none;
        }
        /* The list view for combo boxes */
        QComboBox QAbstractItemView {
            background-color: #3c3f41;
            color: #f0f0f0;
            border: 1px solid #555;
        }
        /* The up and down buttons for spin boxes */
        QSpinBox::up-button, QDoubleSpinBox::up-button,
        QSpinBox::down-button, QDoubleSpinBox::down-button {
        }
        /* Style for push buttons with blue background and white text */
        QPushButton {
            background-color: #4a90e2;
            color: white;
            border: none;
            padding: 6px 12px;
            border-radius: 4px;
        }
        /* The hover and pressed states for buttons */
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
        /* Style for labels with bold text */
        QLabel {
            color: #f0f0f0;
            font-weight: bold;
        }
        /* We added a style for the content area of tab widgets */
        QTabWidget::pane {
            border: 1px solid #555;
            padding: 6px;
        }
        /* We added a style for the tab bar */
        QTabBar::tab {
            background: #3a3a3a;
            color: #f0f0f0;
            padding: 8px;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            margin-right: 2px;
        }
        /* Next is a style for the selected tab */
        QTabBar::tab:selected {
            background: #4a90e2;
            color: white;
        }
        /* Style for group boxes with titled borders */
        QGroupBox {
            border: 1px solid #555;
            border-radius: 5px;
            margin-top: 1ex;
            padding-top: 10px;
            color: #f0f0f0;
        }
        /* Style for group box titles with blue highlighting */
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 5px;
            font-weight: bold;
            color: #4a90e2;
        }
        /* Style for splitter handles with subtle gray color */
        QSplitter::handle {
            background-color: #555;
            height: 2px;
        }
        /* Style for horizontal line separators */
        QFrame[frameShape="4"] { /* HLine */
            background-color: #555;
            height: 1px;
        }
        /* Style for text edit areas with slightly different background */
        QTextEdit {
            background-color: #2d2d2d;
            color: #f0f0f0;
            border: 1px solid #444;
            selection-background-color: #4a90e2;
            selection-color: #ffffff;
        }
        /* Style for vertical scrollbar with minimal design */
        QScrollBar:vertical {
            border: none;
            background: #2b2b2b;
            width: 10px;
            margin: 0px;
        }
        /* Style for scrollbar handles with rounded corners */
        QScrollBar::handle:vertical {
            background: #555;
            min-height: 20px;
            border-radius: 5px;
        }
        /* Hide scrollbar arrow buttons */
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        /* Style for horizontal scrollbar with minimal design */
        QScrollBar:horizontal {
            border: none;
            background: #2b2b2b;
            height: 10px;
            margin: 0px;
        }
        /* Style for horizontal scrollbar handles */
        QScrollBar::handle:horizontal {
            background: #555;
            min-width: 20px;
            border-radius: 5px;
        }
        /* Hide horizontal scrollbar arrow buttons */
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }
        /* Style for header sections in table views */
        QHeaderView::section {
            background-color: #3a3a3a;
            color: #f0f0f0;
            padding: 5px;
            border: 1px solid #555;
        }
        /* Style for tooltips with dark background */
        QToolTip {
            background-color: #2b2b2b;
            color: #f0f0f0;
            border: 1px solid #555;
            padding: 3px;
        }
        /* Style for dropdown lists in combo boxes */
        QComboBox QAbstractItemView {
            background-color: #3c3f41;
            color: #f0f0f0;
            border: 1px solid #555;
        }
        /* Use custom image for combo box dropdown arrow */
        QComboBox::down-arrow {
            image: url(:/images/down.png);
            width: 12px;
            height: 12px;
        }
        /* Style for combo box dropdown button area */
        QComboBox::drop-down {
            border: none;
            width: 20px;
            subcontrol-position: center right;
            subcontrol-origin: padding;
            padding-right: 5px;
            background-color: transparent;
        }
        /* Style for spinbox up buttons with custom image */
        QSpinBox::up-button, QDoubleSpinBox::up-button {
            image: url(:/images/up.png);
            width: 8px;
            height: 8px;
            subcontrol-position: top right;
            subcontrol-origin: margin;
            margin: 2px;
            padding: 4px;
            border: none;
            background-color: transparent;
        }
        /* Style for spinbox down buttons with custom image */
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            image: url(:/images/down.png);
            width: 12px;
            height: 12px;
            subcontrol-position: bottom right;
            subcontrol-origin: margin;
            margin: 2px;
            padding: 2px;
            border: none;
            background-color: transparent;
        }
        /* Repeated button styles (merged with above styles) */
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
        /* Enhanced tab pane styling with no top border */
        QTabWidget::pane {
            border: 1px solid #444;
            border-top-width: 0px;
            border-radius: 0 0 4px 4px;
            padding: 10px;
            background-color: #2d2d2d;
            top: -1px;
        }
        /* Style for the tab bar container */
        QTabBar {
            background-color: transparent;
            margin-top: 10px; /* Add margin to move tabs down */
        }
        /* Position the tab bar within the tab widget */
        QTabWidget::tab-bar {
            alignment: left;
            background: transparent;
            margin-top: 8px; /* Add margin to move tabs down */
        }
        /* Enhanced styling for individual tabs with fixed width */
        QTabBar::tab {
            background: #323232;
            color: #b8b8b8;
            padding: 12px 20px;
            border: 1px solid #444;
            border-bottom: none;
            border-top-left-radius: 6px;
            border-top-right-radius: 6px;
            min-width: 130px;
            font-weight: bold;
            margin-right: 3px;
        }
        /* Hover effect for tabs */
        QTabBar::tab:hover {
            background-color: #3a3a3a;
            color: #ffffff;
        }
        /* Style for the currently selected tab with shadow effect */
        QTabBar::tab:selected {
            background: #4a90e2;
            color: white;
            border-bottom: none;
            margin-bottom: -1px;
        }
    )");
}

// After the styling we needed to add a helper method to check if a doctor or nurse belongs to a specific hospital
bool MainWindow::isDoctorInHospital(const string& doctorID, int hospitalIndex) {
    // We first check if the hospital index is valid
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        return false;
    }
    // Then we check if the doctor ID is valid
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {  // If the doctor is not found, return false
        return false;
    }
    // Get the hospital object using the index and check if it is valid
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        return false;
    }
    // Check if the doctor's hospital ID matches the selected hospital's ID
    return doctor->getHospitalID() == hospital->getHospitalID();
}

// Next is a bool method for checking if a nurse belongs to a specific hospital
bool MainWindow::isNurseInHospital(const string& nurseID, int hospitalIndex) {
    // We always want to first check if the hospital index is valid
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        return false;   // If the index is invalid, return false
    }
    // Then we check if the nurse ID is valid
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        return false;   // If the nurse is not found, return false
    }
    // Get the hospital object using the index and check if it is valid
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        return false;   // If the hospital is not found, return false
    }
    // Check if the nurse's hospital ID matches the selected hospital's ID
    return nurse->getHospitalID() == hospital->getHospitalID();
}

// Next we created the addPatient method which will be used to add a patient to the system for MainWindow
void MainWindow::addPatient() {
    // We started by getting the input values from the UI
    string patientID = patientIDInput->text().toStdString();
    string name = nameInput->text().toStdString();
    string phone = phoneInput->text().toStdString();
    string disease = diseaseInput->text().toStdString();
    string treatment = treatmentInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    string nurseID = nurseIDInput->text().toStdString();
    double dailyRate = dailyRateInput->value();
    int hospitalIndex = hospitalComboBox->currentIndex();
    
    // We then wanted to check and make sure all fields are filled out 
    if (patientID.empty() || name.empty() || phone.empty() || disease.empty() || treatment.empty() || doctorID.empty() || nurseID.empty()) {
        statusDisplay->append("Error: All Fields Must Be Filled Out!");
        return;
    }
    // We also needed to check if the patient ID is valid
    if (hospitalSystem->findPatient(patientID)) {
        statusDisplay->append("Error: Patient ID Already Exists!");
        return;
    }
    // Check if the doctor works at the selected hospital
    if (!isDoctorInHospital(doctorID, hospitalIndex)) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(doctorID) + " Does Not Work At " + hospitalComboBox->currentText() + "! Please Choose A Doctor From This Hospital.");
        return;
    }
    // Check if the nurse works at the selected hospital
    if (!isNurseInHospital(nurseID, hospitalIndex)) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(nurseID) + " Does Not Work At " + hospitalComboBox->currentText() + "! Please Choose A Nurse From This Hospital.");
        return;        
    }
    
    // Next we need to create a new patient object
    Patient* patient = new Patient(patientID, name, phone, disease, treatment, doctorID, nurseID);
    patient->setDailyRate(dailyRate); // Make sure this actually sets the rate
    if (hospitalSystem->admitPatient(patient, hospitalIndex)) {
        statusDisplay->append("Patient Admitted Successfully To " + hospitalComboBox->currentText() + "!");
        statusDisplay->append("Welcome " + QString::fromStdString(name) + " to " + hospitalComboBox->currentText() + "!");
        statusDisplay->append("Daily Billing Rate Set To: $" + QString::number(dailyRate, 'f', 2));
        statusDisplay->append("Initial Bill Amount: $" + QString::number(dailyRate, 'f', 2) + " (charged immediately)");
        // Clear input fields to prevent repeated attempts with the same data and reset for new patients
        patientIDInput->clear();
        nameInput->clear();
        phoneInput->clear();
        diseaseInput->clear();
        treatmentInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
    } else {
        statusDisplay->append("Hospital is Full! Sorry But We Cannot Admit The Patient!");
        delete patient;
    }
}

// Next we created the relocatePatient method for the button in MainWindow which will be used to relocate a patient to a different hospital
void MainWindow::relocatePatient() {
    // We started by getting the input values from the UI
    string patientID = patientIDInput->text().toStdString();
    string newPrimaryDoctorID = doctorIDInput->text().toStdString();
    string newNurseID = nurseIDInput->text().toStdString();
    int newHospitalIndex = relocateHospitalComboBox->currentIndex();
    // We then wanted to check and make sure all fields are filled out
    if (patientID.empty() || newPrimaryDoctorID.empty() || newNurseID.empty()) {
        statusDisplay->append("Error: Patient ID, New Primary Doctor ID, and New Nurse ID Must Be Provided!");
        return;
    }
    // Next we need to check and see if the patient ID is valid
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Error: Patient Not Found!");
        // If not valid, clear the input fields to prevent repeated attempts with the same data
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
        return;
    }

    // Next we are going to grab the current hospital and the new hospital the patient is being relocated to/has entered
    Hospital* currentHospital = hospitalSystem->findPatientHospital(patientID);
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);

    // We want to do a quick check to see if the current hospital and new hospital are valid
    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital Entered Is Not Valid!");
        return;
    }
    // Also we should see if patient is already in the target hospital
    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        statusDisplay->append("Error: Patient " + QString::fromStdString(patientID) + " Is Already Admitted To " + relocateHospitalComboBox->currentText());
        // Clear the input fields to prevent repeated attempts with the same data
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
        return;
    }
    // Check if trying to relocate to a hospital with the same doctor ID and nurse ID
    if (patient->getPrimaryDoctorID() == newPrimaryDoctorID && 
        !patient->getAttendingNursesIDs().empty() && 
        patient->getAttendingNursesIDs()[0] == newNurseID) {
        statusDisplay->append("Error: Cannot Relocate Patient With The Same Primary Doctor And Nurse. Please Select Different Staff!");
        // Clear the input fields to prevent repeated attempts with the same data
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
        return;
    }
    // Check if doctor works at new hospital
    if (!isDoctorInHospital(newPrimaryDoctorID, newHospitalIndex)) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(newPrimaryDoctorID) + " Does Not Work At " + relocateHospitalComboBox->currentText() + "!");
        return;
    }
    // Check if nurse works at new hospital
    if (!isNurseInHospital(newNurseID, newHospitalIndex)) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(newNurseID) + " Does Not Work At " + relocateHospitalComboBox->currentText() + "!");
        return;
    }

    // After these checks, we want to grab the nurse and doctor objects
    Doctor* newDoctor = hospitalSystem->findDoctor(newPrimaryDoctorID);
    Nurse* newNurse = hospitalSystem->findNurse(newNurseID);
    // Next we want to check if the doctor and nurse objects are valid
    if (!newDoctor || !newNurse) {
        statusDisplay->append("Error: Could Not Find The Specified Doctor Or Nurse!");
        return;
    }
    // As the requirements outline, we need to check if the nurse already has the maximum number of patients
    if (newNurse->getPatientIDs().size() >= 2) {
        // Check if the nurse is already assigned to this patient (could happen with a new relocation)
        bool alreadyAssigned = false;
        // Loop through the nurse's patients to see if the patient is already assigned
        for (const string& pid : newNurse->getPatientIDs()) {
            if (pid == patientID) {
                alreadyAssigned = true; // Patient is already assigned to this nurse
                break;
            }
        }
        // If the nurse already has 2 patients and the patient is not already assigned, show an error message
        if (!alreadyAssigned) {
            statusDisplay->append("Error: Nurse " + QString::fromStdString(newNurseID) + " Already Has The Maximum Number Of Patients (2)!");
            return;
        }
    }
    // We wanted to wrap the entire operation in a try-catch block so we could try to perform relocation
    try {
        // We first will try to relocate the patient to the new hospital
        bool relocationSuccess = hospitalSystem->relocatePatient(patientID, newHospitalIndex);
        // We need to do a check to see if the relocation was successful
        if (!relocationSuccess) {
            statusDisplay->append("Failed To Relocate Patient!");
            return;
        }
        // Only proceed with staff assignments if relocation succeeded
        // We then added the doctor-patient association 
        newDoctor->addPatient(patientID);
        patient->setPrimaryDoctorID(newPrimaryDoctorID);
        // We then added the nurse-patient association 
        newNurse->assignPatient(patientID);
        patient->addAttendingNurse(newNurseID);
    
        // If the relocation and assignments were successful, we will show a success message
        statusDisplay->append("Patient " + QString::fromStdString(patientID) + " Relocated To " + relocateHospitalComboBox->currentText() +
                            " And Assigned To Doctor " + QString::fromStdString(newPrimaryDoctorID) + " And Nurse " + QString::fromStdString(newNurseID));
        // Clear the input fields after successful relocation
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
    } catch (const std::exception& e) {
        statusDisplay->append("Error During Relocation: " + QString(e.what()));
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
    } catch (...) {
        statusDisplay->append("An Unexpected Error Occurred During Patient Relocation.");
        patientIDInput->clear();
        doctorIDInput->clear();
        nurseIDInput->clear();
    }
}

// For the dischargePatient method that is executed when the user clicks the button in MainWindow will be used to discharge a patient
void MainWindow::dischargePatient() {
    // We will start by getting the input values from the UI for the patient ID and doctor ID
    string patientID = patientIDInput->text().toStdString();
    string doctorID = doctorIDInput->text().toStdString();
    // We will then check and make sure all fields are filled out
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID to Discharge!");
        return;
    }
    // We will also check if the patient ID is valid
    if (hospitalSystem->dischargePatient(patientID)) {
        statusDisplay->append("Patient " + QString::fromStdString(patientID) + " Discharged Successfully!");
    } else {
        statusDisplay->append("Patient Not Found, Sorry! Please Check The ID Again!");
    }
}

// We then created the displayHospitalStatus method which will be used to display the hospital status
void MainWindow::displayHospitalStatus() {
    statusDisplay->clear();
    statusDisplay->append("=== HOSPITAL SYSTEM STATUS ===\n");
    // This will get the hospital system instance and display the status
    statusDisplay->append(QString::fromStdString(hospitalSystem->getHospitalStatus()));
}

// We also wanted to add the displayPharmacyStatus method which will be used to display the pharmacy status
void MainWindow::displayPharmacyStatus() {
    statusDisplay->clear();
    // This will get the pharmacy system instance and display the status
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    if (!pharmacySystem) {
        statusDisplay->append("Error: Pharmacy System Not Initialized!");
        return;
    }
    // Display the pharmacy status using the pharmacy system instance and call the getPharmacyStatus method
    statusDisplay->append(QString::fromStdString(pharmacySystem->getPharmacyStatus()));
}

// This method will be used to view the details of a patient
void MainWindow::viewPatientDetails() {
    // We will start by getting the input values from the UI for the patient ID
    string patientID = patientIDInput->text().toStdString();
    // We will then check and make sure all fields are filled out
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID To View Details!");
    }
    // Next we will check if the patient ID is valid
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Patient Not Found!");    // If not valid, clear the input fields to prevent repeated attempts with the same data
        return;
    }
    // After this, we will grab the hospital object using the patient ID
    Hospital* hospital = hospitalSystem->findPatientHospital(patientID);
    if (!hospital) {
        statusDisplay->append("Error: Cannot Locate Patient's Hospital!");  // If the patient does not have a hospital, show an error message
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

// This method is going to provide the user with the ability to view the billing history of a patient
void MainWindow::viewPatientBillingHistory() {
    // We will start by getting the input values from the UI for the patient ID
    string patientID = billingPatientIDInput->text().toStdString();
    // Check to see if the patient ID is valid
    if (patientID.empty()) {
        statusDisplay->append("Error: Enter Patient ID To View Billing History");
        return;
    }
    // If the patient ID is valid, we will check if the patient exists in the system
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) { // If the patient is not found, show an error message
        statusDisplay->append("Patient Not Found!");
        return;
    }
    // We will then check if the patient has a hospital
    Hospital* hospital = hospitalSystem->findPatientHospital(patientID);
    if (!hospital) {
        statusDisplay->append("Error: Hospital Not Found For This Patient.");
        return;
    }
    // Display hospital information with patient details if the patient has a hospital and satisfies the conditions above
    statusDisplay->clear();
    statusDisplay->append("=== PATIENT BILLING HISTORY ===");
    statusDisplay->append("Patient ID: " + QString::fromStdString(patient->getPatientID()));
    statusDisplay->append("Name: " + QString::fromStdString(patient->getPatientName()));
    statusDisplay->append("Days Admitted: " + QString::number(patient->getDaysAdmitted()));
    statusDisplay->append("Daily Rate: $" + QString::number(patient->getBillingRatePerDay(), 'f', 2));
    statusDisplay->append("Current Total: $" + QString::number(patient->calculateCurrentBill(), 'f', 2));
    statusDisplay->append("Status: " + QString::fromStdString(patient->getStatus()));
    statusDisplay->append("\nHeads Up: Additional Charges For Medications and Services May Apply!");
}

// Next is the method that will handle when the user clicks the button to add a doctor in MainWindow
void MainWindow::addDoctor() {
    // We first need to get the input values from the UI for the doctor ID, name, and hospital
    string docID = docManageIDInput->text().toStdString();
    string docName = doctorNameInput->text().toStdString();
    int hospitalIndex = docHospitalComboBox->currentIndex();
    // Next we need too check if the doctor ID and name are valid
    if (docID.empty() || docName.empty()) {
        statusDisplay->append("Error: All Fields Must Be Filled Out. Check The ID and Name!");
        return;
    }
    // Check if doctor already exists
    if (hospitalSystem->findDoctor(docID)) {
        statusDisplay->append("Error: Doctor ID Already Exists! Check The List Of Doctors.");
        return;
    }
    // Check if the hospital index is valid then we create a new doctor object
    Doctor* doctor = new Doctor(docID, docName, hospitalSystem->getHospital(hospitalIndex)->getHospitalID());
    // If the doctor object is added successfully to the hospital system, we will show a success message
    if (hospitalSystem->addDoctor(doctor, hospitalIndex)) {
        statusDisplay->append("Doctor Added Successfully To " +  docHospitalComboBox->currentText());  
        statusDisplay->append("Doctor ID: " + QString::fromStdString(docID));
        statusDisplay->append("Doctor Name: " + QString::fromStdString(docName));
        // Clear input fields to prevent repeated attempts with the same data      
        docManageIDInput->clear();
        doctorNameInput->clear();
    } else {
        // If we already have 50 doctors in the system, we will show an error message
        statusDisplay->append("Error: There Are Already 50 Doctors In The System!");
        delete doctor;
        return;
    }

}

// Next is the method that will handle when the user clicks the button to relocate a doctor in MainWindow
void MainWindow::relocateDoctor() {
    // We first need to get the input values from the UI for the doctor ID and new hospital
    string docID = docManageIDInput->text().toStdString();
    int newHospitalIndex = docChangeHospitalComboBox->currentIndex();
    // Next we need to check if the doctor ID is valid and provided
    if (docID.empty()) {
        statusDisplay->append("Error: Doctor ID Must Be Provided");
        return;
    }
    // If it is then we will check if the doctor ID is valid by grabbing the doctor object and its id
    Doctor* doctor = hospitalSystem->findDoctor(docID);
    // We will check if the doctor object is valid
    if (!doctor) {
        statusDisplay->append("Error: Doctor Not Found!");
        return;
    }    
    // Next we need to grab the current hospital and the new hospital the doctor is being relocated to/has entered
    Hospital* currentHospital = hospitalSystem->findDoctorHospital(docID);
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);
    // We need to make sure the current hospital and new hospital are valid and are not the same
    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital Selection Is Invalid!");
        return;
    }
    // Check if the doctor is already in the target hospital
    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        statusDisplay->append("Error: Doctor Is Already In The Hospital You Selected!");
        return;        
    }
    // Make sure the patient ID is valid
    if (!doctor->getPatientIDs().empty()) {
        statusDisplay->append("Error: Doctor Has Assigned Patients and Can't Be Moved. Either Assign The Patients to Another Doctor or Discharge Them When They're Done!");
        return;          
    }
    // If the doctor is relocated successfully, we will show a success message
    if (hospitalSystem->relocateDoctor(docID, newHospitalIndex)) {
        statusDisplay->append("Doctor " + QString::fromStdString(docID) + " Relocated To " + docChangeHospitalComboBox->currentText());        
    } else {
        statusDisplay->append("Error: Unexpected"); 
    }
}

// We implemented the removeDoctor method which will be used to remove a doctor from the system when the user clicks the button 
void MainWindow::removeDoctor() {
    // We first need to get the input values from the UI for the doctor ID
    string docID = docManageIDInput->text().toStdString();
    // We are checking if the doctor ID is valid and provided
    if (docID.empty()) {
        statusDisplay->append("Error: Doctor ID Must Be Provided!");
        return;
    }
    // If it is then we will check if the doctor ID is valid by grabbing the doctor object and its id
    Doctor* doctor = hospitalSystem->findDoctor(docID);
    if (!doctor) {
        statusDisplay->append("Error: Doctor Not Found or Already Removed!");
        return;
    }
    // Next we need to grab the current hospital the doctor is in
    Hospital* currentHospital = hospitalSystem->findDoctorHospital(docID);
    if (!currentHospital) {
        statusDisplay->append("Error: Cannot Locate Doctor's Hospital!");
        return;
    }
    // Check for the minimum doctor requirement
    if (currentHospital->getDoctors().size() <= 3) {
        statusDisplay->append("Error: Cannot Remove Doctor. Hospital Must Maintain At Least 3 Doctors!");
        return;
    }
    // Check if the doctor has any patients assigned
    if (!doctor->getPatientIDs().empty()) {
        statusDisplay->append("Error: Doctor Has Assigned Patients and Can't Be Removed! Either Assign the Patients to Another Doctor or Discharge Them When They're Done!");
        return;          
    }
    // Next we added a try-catch block to handle any exceptions that may occur during the removal process
    try {
        // Check if the doctor is removed successfully
        if (hospitalSystem->removeDoctor(docID)) {
            statusDisplay->append("Doctor " + QString::fromStdString(docID) + " Removed From System!");            
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

// The next button is going to be used to view the details of a doctor in MainWindow
void MainWindow::viewDoctorDetails() {
    // We will start by getting the input values from the UI for the doctor ID
    string doctorID = docManageIDInput->text().toStdString();
    // We have a check to make sure he user has entered a doctor ID
    if (doctorID.empty()) {
        statusDisplay->append("Error: Enter Doctor ID To View Details!");
    }
    // We will try and find the doctor object using the doctor ID
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {
        statusDisplay->append("Doctor Not Found!");
        return;
    }
    // Get the hospital information to display
    Hospital* hospital = hospitalSystem->findDoctorHospital(doctorID);
    if (!hospital) {
        statusDisplay->append("Error: Cannot Locate Patient's Hospital");
        return;
    }
    // Display patient details
    statusDisplay->append(QString::fromStdString(doctor->getFullDescription()));
    // Display hospital information with patient details
    statusDisplay->append("Working in Hospital: " + QString::fromStdString(hospital->getHospitalName()));
    statusDisplay->append("\n");
}

// The next button is going to be used to assign a doctor to a patient in MainWindow
void MainWindow::assignDoctorToPatient(bool isPrimary) {
    // We will start by getting the input values from the UI for the doctor ID and patient ID
    string doctorID = doctorAssignmentIDInput->text().toStdString();
    string patientID = doctorPatientAssignmentIDInput->text().toStdString();
    // We will check and make sure all fields are filled out
    if (doctorID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Doctor ID And Patient ID Are Required");
        return;
    }
    // Find which hospital the patient is in
    Hospital* patientHospital = hospitalSystem->findPatientHospital(patientID);
    if (!patientHospital) {
        statusDisplay->append("Error: Patient Not Found In Any Hospital");
        return;
    }
    // Check if the doctor works at the patient's hospital
    Doctor* doctor = hospitalSystem->findDoctor(doctorID);
    if (!doctor) {
        statusDisplay->append("Error: Doctor ID Not Found");
        return;
    }
    // Check if the doctor works at the patient's hospital
    if (doctor->getHospitalID() != patientHospital->getHospitalID()) {
        statusDisplay->append("Error: Doctor " + QString::fromStdString(doctorID) + " Does Not Work At " + QString::fromStdString(patientHospital->getHospitalName()) + " Where The Patient is Admitted!");
        return;
    }
    // Continue with assignment
    if (isPrimary) {
        if (hospitalSystem->setPatientPrimaryDoctor(patientID, doctorID)) {
            statusDisplay->append("Set Doctor " + QString::fromStdString(doctorID) + " As Primary Doctor For Patient " + QString::fromStdString(patientID));
        } else {
            statusDisplay->append("Failed To Set Primary Doctor, Check IDs and Try Again!");
        }
    } else {
        // We also need to check if the doctor is already assigned to the patient
        if (hospitalSystem->assignDoctorToPatient(doctorID, patientID, false)) {
            statusDisplay->append("Assigned Doctor " + QString::fromStdString(doctorID) + " To Patient " + QString::fromStdString(patientID));
        } else {
            statusDisplay->append("Failed To Assign Doctor, Check IDs and Try Again!");
        }
    }
}

// The next button is going to be used to assign a nurse to a patient in MainWindow when the user clicks the button
void MainWindow::assignNurseToPatient() {
    // We will start by getting the input values from the UI for the nurse ID and patient ID
    string nurseID = nurseAssignmentIDInput->text().toStdString();
    string patientID = nursePatientAssignmentIDInput->text().toStdString();
    // We will check and make sure all fields are filled out, starting with the nurse ID and patient ID
    if (nurseID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Nurse ID And Patient ID Are Required!");
        return;
    }
    // Find which hospital the patient is in
    Hospital* patientHospital = hospitalSystem->findPatientHospital(patientID);
    if (!patientHospital) {
        statusDisplay->append("Error: Patient Not Found In Any Hospital");
        return;
    }
    // Check if the nurse works at the patient's hospital
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Error: Nurse ID Not Found");
        return;
    }    
    // Check if the nurse works at the patient's hospital and we will provide a message if the nurse does not work at the hospital
    if (nurse->getHospitalID() != patientHospital->getHospitalID()) {
        statusDisplay->append("Error: Nurse " + QString::fromStdString(nurseID) + " Does Not Work At " + QString::fromStdString(patientHospital->getHospitalName()) + " Where The Patient is Admitted!");
        return;
    }
    // We will try and assign the nurse to the patient and if it is successful
    if (hospitalSystem->assignNurseToPatient(nurseID, patientID)) {
        statusDisplay->append("Assigned Nurse " + QString::fromStdString(nurseID) + " To Patient " + QString::fromStdString(patientID));
    } else {
        statusDisplay->append("Failed To Assign Nurse. Check IDs And Try Again!");
    } 
}

// The next button is going to be used to request a patient discharge in MainWindow when the user clicks the button
void MainWindow::requestPatientDischarge() {
    // We will grab the doctor ID and patient ID from the input fields
    string doctorID = doctorDischargeIDInput->text().toStdString();
    string patientID = patientDischargeIDInput->text().toStdString();
    // We will check and make sure all fields are filled out
    if (doctorID.empty() || patientID.empty()) {
        statusDisplay->append("Error: Both Doctor ID And Patient ID Are Required");
        return;
    }
    // We will call the hospital system for the method requestPatientDischarge to try and request a discharge
    if (hospitalSystem->requestPatientDischarge(doctorID, patientID)) {
        statusDisplay->append("Discharge Request Approved! Patient " + QString::fromStdString(patientID) + " Can Now Be Discharged");
    } else {
        statusDisplay->append("Sorry, Discharge Request Denied! Either The Doctor Is Not Authorized " + QString("Or The Patient ID Is Invalid!"));
    }
}

// This method will be used when the user clicks the button to calculate the bill in MainWindow
void MainWindow::calculateBill() {
    // We will start by getting the input values from the UI for the patient ID
    string patientID = billingPatientIDInput->text().toStdString();
    // We will check and make sure all fields are filled out which will be the patient ID
    if (patientID.empty()) {
        statusDisplay->append("Error: Patient ID Is Required!");
        return;
    }
    
    // Check if the patient exists
    Patient* patient = hospitalSystem->findPatient(patientID);
    if (!patient) {
        statusDisplay->append("Error: Patient Not Found!");
        return;
    }
    
    // Get remaining balance instead of full bill
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    
    // If the balance is 0 but the patient exists, it's likely the billing record wasn't properly initialized
    // Let's calculate the current bill directly from the patient
    if (remainingBalance <= 0.01 && patient->getDaysAdmitted() > 0) {
        remainingBalance = patient->calculateCurrentBill();
        statusDisplay->append("Note: Initialized billing record for patient " + QString::fromStdString(patientID));
    }
    
    currentBillLabel->setText(QString("$%1").arg(remainingBalance, 0, 'f', 2));
    // Show the remaining balance in the status display
    statusDisplay->append("Remaining Balance For Patient " + QString::fromStdString(patientID) + " Is $" + 
                         QString::number(remainingBalance, 'f', 2));
}

// This method will be used when the user clicks the button to collect payment in MainWindow
void MainWindow::collectPayment() {
    // We need the patient ID and payment amount from the input fields
    string patientID = billingPatientIDInput->text().toStdString();
    double amount = paymentAmountInput->value();
    // We need to check to make sure the user has entered a patient ID
    if (patientID.empty()) {
        statusDisplay->append("Error: Patient ID Is Required!");
        return;
    }
    // For the payment, we must check if the amount is valid
    if (amount <= 0) {
        statusDisplay->append("Error: Payment Amount Must Be Greater Than Zero");
        return;
    }
    // Get the current remaining balance
    double remainingBalance = hospitalSystem->getPatientRemainingBalance(patientID);
    
    // Check if patient exists but balance is 0
    if (remainingBalance <= 0.01) {
        Patient* patient = hospitalSystem->findPatient(patientID);
        if (patient) {
            remainingBalance = patient->calculateCurrentBill();
            statusDisplay->append("Note: Initialized Billing Record For Patient " + QString::fromStdString(patientID));
        }
    }
    // Allow a small floating-point difference and make sure the amount is not greater than the remaining balance
    if (amount > remainingBalance + 0.01) {
        statusDisplay->append("Error: Payment Amount ($" + QString::number(amount, 'f', 2) + ") Exceeds Remaining Balance ($" + QString::number(remainingBalance, 'f', 2) + ")");
        return;
    }
    // If the amount is very close to the remaining balance, adjust it to be exact
    if (fabs(amount - remainingBalance) < 0.01) {
        amount = remainingBalance;
    }
    // Attempt to collect the payment and show the result
    if (hospitalSystem->collectPatientPayment(patientID, amount)) {
        statusDisplay->append("Payment of $" + QString::number(amount, 'f', 2) + " Collected From Patient " + QString::fromStdString(patientID));
        // Calculate new remaining balance
        double newBalance = remainingBalance - amount;
        if (newBalance < 0.01) newBalance = 0.0; // Avoid tiny remaining amounts
        // Update display with remaining amount
        statusDisplay->append("Remaining balance: $" + QString::number(newBalance, 'f', 2));
        // Update the bill label
        currentBillLabel->setText(QString("$%1").arg(newBalance, 0, 'f', 2));
        // Reset payment input
        paymentAmountInput->setValue(0.0);
        // If paid in full, we want to show a message
        if (newBalance < 0.01) {
            statusDisplay->append("Bill Paid In Full - Thank You!");
        }
    } else {
        statusDisplay->append("Payment Failed! Check Patient ID And Try Again");
    }
}

// This method will be used when the user clicks the button to show the billing report in MainWindow
void MainWindow::showBillingReport() {
    statusDisplay->clear(); // Clear the display before showing the report
    statusDisplay->append("=== HOSPITAL BILLING REPORT ===\n");
    statusDisplay->append(QString::fromStdString(hospitalSystem->getPatientBillingReport()));
}

// The user will be able to view the pharmacy billing report when they click the button in MainWindow
void MainWindow::showPharmacyBillingReport() {
    statusDisplay->clear(); // Clear the display before showing the report
    statusDisplay->append("=== PHARMACY BILLING REPORT ===\n");
    // Get the pharmacy system instance
    PharmacySystem* pharmSys = PharmacySystem::getInstance();
    // We need to grab both the pharmacy system and hospital system instances so we can get the billing report
    auto allPharmacies = pharmSys->getAllPharmacies();
    auto allHospitals = hospitalSystem->getAllHospitals();
    // We will create a map to store the total unpaid bills for each hospital
    std::map<std::string, double> hospitalTotals;
    // Next we will loop through all pharmacies and hospitals to get the billing report
    for (auto pharmacy : allPharmacies) {
        // Loop through all hospitals to get the bills for each hospital
        for (auto hospital : allHospitals) {
            // Get the bills for the hospital from the pharmacy and store them in a vector
            std::vector<Bill> bills = pharmacy->getBillsForHospital(hospital->getHospitalID());
            // Loop through the bills to calculate the total unpaid amount
            for (const auto& bill : bills) {
                if (!bill.paid) {   // Check if the bill is unpaid
                    hospitalTotals[hospital->getHospitalName()] += bill.amount; // Add to the total for this hospital
                }
            }
        }
    }
    // Show the result
    for (const auto& pair : hospitalTotals) {
        statusDisplay->append(QString::fromStdString(pair.first) + " Owes: $" + QString::number(pair.second, 'f', 2));
    }
    // If there are no unpaid bills, show a message
    if (hospitalTotals.empty()) {
        statusDisplay->append("All Pharmacy Bills Have Been Paid!");
    }
}

// This method will be used to update the day counter in MainWindow so that the system can simulate the passage of time for the billing system
void MainWindow::updateDayCounter() {
    // This is called periodically to simulate the passage of time
    hospitalSystem->updateAllPatientDays();
    // Display a notification about the update
    statusDisplay->append("Daily Update Completed! Patient Days Have Been Incremented");
    // Check for any rate increases and display them, using a loop to go through all patients
    for (auto& pair : hospitalSystem->getAllPatients()) {
        Patient* patient = pair.second; // Get the patient object
        // We will check if the patient is currently admitted and if the days admitted is a multiple of 3
        if (patient->getDaysAdmitted() % 3 == 0 && patient->getDaysAdmitted() > 0) {
            // Store the previous rate
            double oldRate = patient->getBillingRatePerDay();
            // Apply rate increase
            patient->applyRateIncrease(0.03); // We choose 3% as the rate increase
            // Show the new rate
            double newRate = patient->getBillingRatePerDay();
            // Only show the message if the rate actually changed
            if (newRate > oldRate) {
                statusDisplay->append("Rate Increase Applied For Patient " + QString::fromStdString(patient->getPatientID()) + ". Rate Changed From $" + QString::number(oldRate, 'f', 2) + " To $" + QString::number(newRate, 'f', 2));
            }
        }
    }
    // If the current tab is the billing tab, update the displayed bill
    if (tabWidget->currentIndex() == 4) { // Check if the current tab is the billing tab
        string patientID = billingPatientIDInput->text().toStdString();
        if (!patientID.empty()) {   // Check if the patient ID is not empty
            calculateBill();    // Call the calculateBill method to update the displayed bill
        }
    }
}

// We wanted to have a method to list all patients in the system
void MainWindow::listAllPatients() {
    // We will start by clearing the status display
    statusDisplay->clear();
    statusDisplay->append("=== CURRENT PATIENTS LIST ===");
    // We are gonna create a map to store all patients in the hospital system
    map<string, Patient*>& allPatients = hospitalSystem->getAllPatients();
    // If the map is empty, we will show a message since there are no patients
    if (allPatients.empty()) {
        statusDisplay->append("No Patients Currently Admitted!");
        return;
    }
    // Next we will create another map to store the patients by their hospital (grouping)
    map<string, vector<Patient*>> patientsByHospital;
    // We want to loop through all patients and group them by their hospital
    for (const auto& pair : allPatients) {
        Patient* patient = pair.second; // We are creating a pointer to the patient object
        if (!patient) continue; // Skip null patients for safety
        
        // Created a hospital object to get find the hospital of the patient using the patient ID
        Hospital* hospital = hospitalSystem->findPatientHospital(patient->getPatientID());
        // We will check if the hospital is valid and if it is then we push the patient into the vector
        if (hospital) {
            patientsByHospital[hospital->getHospitalName()].push_back(patient);
        } else {
            // Handle patients without a hospital assignment
            patientsByHospital["Unassigned"].push_back(patient);
        }
    }
    
    // Display patients grouped by hospital
    for (const auto& pair : patientsByHospital) {
        statusDisplay->append("\n--- " + QString::fromStdString(pair.first) + " ---");
        // Loop through the patients in the vector and display their details
        for (const Patient* patient : pair.second) {
            if (!patient) continue; // Skip null patients for safety
            
            statusDisplay->append("Patient ID: " + QString::fromStdString(patient->getPatientID()));
            statusDisplay->append("Name: " + QString::fromStdString(patient->getPatientName()));
            statusDisplay->append("Disease: " + QString::fromStdString(patient->getDisease()));
            statusDisplay->append("Days Admitted: " + QString::number(patient->getDaysAdmitted()));
            
            // Add null checks before accessing IDs
            if (!patient->getPrimaryDoctorID().empty()) {
                statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->getPrimaryDoctorID()));
            } else {
                statusDisplay->append("Primary Doctor: None assigned");
            }
            
            // Show attending nurses if any
            if (!patient->getAttendingNursesIDs().empty()) {
                QString attendingNurses = "Assigned Nurses: ";
                for (size_t nurseCount = 0; nurseCount < patient->getAttendingNursesIDs().size(); nurseCount++) {
                    attendingNurses += QString::fromStdString(patient->getAttendingNursesIDs()[nurseCount]);
                    if (nurseCount < patient->getAttendingNursesIDs().size() - 1) {   // Check if not the last nurse
                        attendingNurses += ", ";
                    }
                }
                statusDisplay->append(attendingNurses); // Show the list of assigned nurses
            } else {
                statusDisplay->append("Assigned Nurses: None");
            }
            
            // Show attending doctors if any
            if (!patient->getAttendingDoctorIDs().empty()) {
                QString attendingDocs = "Attending Doctors: ";
                for (size_t doctorCount = 0; doctorCount < patient->getAttendingDoctorIDs().size(); doctorCount++) {
                    attendingDocs += QString::fromStdString(patient->getAttendingDoctorIDs()[doctorCount]);
                    if (doctorCount < patient->getAttendingDoctorIDs().size() - 1) {    // Check if not the last doctor
                        attendingDocs += ", ";
                    }
                }
                statusDisplay->append(attendingDocs);   // Show the list of attending doctors
            } else {
                statusDisplay->append("Attending Doctors: None");
            }
            
            // Show a separator between patients
            statusDisplay->append("------------------------------------------");
        }
    }
    // Show count at the end
    statusDisplay->append("\nTotal Patients In All Hospitals: " + QString::number(allPatients.size()));
    statusDisplay->append("===================================================");
}

// When the user clicks the button to request a drug delivery in MainWindow it will call this method
void MainWindow::requestDrugDelivery() {
    // We are starting with the input values from the UI for the hospital, pharmacy, and drug
    int hospitalIndex = drugHospitalComboBox->currentIndex();
    int pharmacyIndex = drugPharmacyComboBox->currentIndex();
    int drugIndex = drugComboBox->currentIndex();
    // We will check and make sure all fields are filled out and are valid and greater than or equal to 0
    if (hospitalIndex < 0 || pharmacyIndex < 0 || drugIndex < 0) {
        statusDisplay->append("Error: Please Select A Hospital, Pharmacy, and Drug.");
        return;
    }
    // We will create an instance of the pharmacy system and get the hospital and pharmacy objects
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {    // Check if the hospital is valid
        statusDisplay->append("Error: Invalid Hospital Selection!");
        return;
    }
    // Get the pharmacy system instance and check if it is valid
    PharmacySystem* ps = PharmacySystem::getInstance();
    // Grab the pharmacy object using the pharmacy index
    Pharmacy* pharmacy = ps->getPharmacy(pharmacyIndex);
    if (!pharmacy) {    // Check if the pharmacy is valid
        statusDisplay->append("Error: Invalid Pharmacy Selection!");
        return;
    }
    drug selectedDrug = ps->getAllDrugs()[drugIndex];  // Changed Drug to drug to match the actual class name
    // Just bill the hospital for this drug
    std::string billID = pharmacy->billHospitalForDrug(hospital->getHospitalID(), selectedDrug.getDrugName(), selectedDrug.getPrice());
    // We will provide a message to the user that the drug has been billed to the hospital
    statusDisplay->append("Drug \"" + QString::fromStdString(selectedDrug.getDrugName()) + "\" Billed To " + QString::fromStdString(hospital->getHospitalName()) +
                          " For $" + QString::number(selectedDrug.getPrice(), 'f', 2) + " (Bill ID: " + QString::fromStdString(billID) + ")");
}

// When the user clicks the red clear button in MainWindow it will call this method
void MainWindow::clearStatusDisplay() {
    statusDisplay->clear();
}

// We need to update the current time in the status display so the user can see the current time for daily billing updates
void MainWindow::updateCurrentTime() {
    // We are using QTime to get the current time
    QTime currentTime = QTime::currentTime();
    // We will format the time to show it in a readable format
    QString timeText = "Current Time: " + currentTime.toString("h:mm:ss AP");
    // Next the QTextCursor is used to save the current scroll position
    QTextCursor cursor = statusDisplay->textCursor();
    int scrollPos = statusDisplay->verticalScrollBar()->value(); // Save scroll position
    // We will set the cursor to the end of the text display
    QString text = statusDisplay->toPlainText();
    QStringList lines = text.split("\n");
    // We will check if the current time line already exists in the text display
    bool timeLineUpdated = false;
    for (int timeUpdate = 0; timeUpdate < lines.size(); ++timeUpdate) {
        if (lines[timeUpdate].startsWith("Current Time:")) {// Check if the line starts with "Current Time:"
            // Update the existing line with the new time
            lines[timeUpdate] = timeText;
            timeLineUpdated = true;
            break;
        }
    }
    // If the time line was not updated, we will prepend the new time to the beginning of the text
    if (!timeLineUpdated) {
        lines.prepend(timeText);
    }
    // We will set the cursor to the end of the text display
    statusDisplay->blockSignals(true); // prevent triggering anything while updating
    statusDisplay->setPlainText(lines.join("\n"));
    statusDisplay->blockSignals(false); // re-enable signals
    statusDisplay->verticalScrollBar()->setValue(scrollPos); // Restore scroll position
}

// We created a large method to display the selected hospital status in MainWindow when the display all button is clicked
void MainWindow::displaySelectedHospitalStatus() {
    // We will create an int variable to store the selected hospital index
    int hospitalIndex = selectedHospitalComboBox->currentIndex();
    // We will check if the hospital index is valid and greater than or equal to 0 and is a current index
    if (hospitalIndex < 0 || static_cast<size_t>(hospitalIndex) >= hospitalSystem->getAllHospitals().size()) {
        statusDisplay->append("Error: Invalid Hospital Selection!");
        return;
    }
    // We will create a hospital object to get the hospital information using the hospital index
    Hospital* hospital = hospitalSystem->getHospital(hospitalIndex);
    if (!hospital) {
        statusDisplay->append("Error: Hospital Not Found");
        return;
    }
    // We will clear the status display and show the selected hospital status
    statusDisplay->clear();
    statusDisplay->append("=== SELECTED HOSPITAL STATUS ===\n");
    // Display hospital overview with the hospital name and ID and the number of patients and staff
    statusDisplay->append("Hospital: " + QString::fromStdString(hospital->getHospitalName()));
    statusDisplay->append("ID: " + QString::fromStdString(hospital->getHospitalID()));
    statusDisplay->append("Patients Admitted: " + QString::number(hospital->getPatients().size()) + "/20");
    statusDisplay->append("Doctors: " + QString::number(hospital->getDoctors().size()));
    statusDisplay->append("Nurses: " + QString::number(hospital->getNurses().size()));

    // We will display the hospitals doctors as long as the hospital has doctors
    if (!hospital->getDoctors().empty()) {
        statusDisplay->append("\n------ Doctor Details ------");
        for (auto doctor : hospital->getDoctors()) {    // We will loop through the doctors and display their details
            statusDisplay->append("ID: " + QString::fromStdString(doctor->getDoctorID()) + ", Name: " + QString::fromStdString(doctor->getDoctorName()) + ", Patients: " + QString::number(doctor->getPatientIDs().size()));
        }
        statusDisplay->append("");
    }
    // Add detailed nurse information
    if (!hospital->getNurses().empty()) {
        statusDisplay->append("\n------ Nurse Details ------"); 
        for (auto nurse : hospital->getNurses()) {  // We will loop through the nurses and display their details
            statusDisplay->append("ID: " + QString::fromStdString(nurse->getNurseID()) + ", Name: " + QString::fromStdString(nurse->getNurseName()) + ", Patients: " + QString::number(nurse->getPatientIDs().size()) + "/2");
        }
        statusDisplay->append(""); 
    }
    // The next section will display the patients in the hospital as long as the hospital has patients
    if (!hospital->getPatients().empty()) {
        statusDisplay->append("=================== PATIENT DETAILS ===================");
        // We will loop through the patients and display their details
        for (auto patient : hospital->getPatients()) {
            statusDisplay->append("PATIENT INFORMATION");
            statusDisplay->append("Name:   \t\t" + QString::fromStdString(patient->getPatientName()));
            statusDisplay->append("ID:   \t\t" + QString::fromStdString(patient->getPatientID()));
            statusDisplay->append("Phone:   \t\t" + QString::fromStdString(patient->getPhoneNumber()));
            statusDisplay->append("Days Admitted:\t " + QString::number(patient->getDaysAdmitted()));
            statusDisplay->append("");
            // We will display the medical information of the patient
            statusDisplay->append("MEDICAL INFORMATION");
            statusDisplay->append("Diagnosis:      " + QString::fromStdString(patient->getDisease()));
            statusDisplay->append("Treatment:      " + QString::fromStdString(patient->getTreatment()));
            statusDisplay->append("");
            // Next we will display the medical staff information of the patient
            statusDisplay->append("MEDICAL STAFF");
            statusDisplay->append("Primary Doctor: " + QString::fromStdString(patient->getPrimaryDoctorID()));
            // Check if the patient has any assigned doctors or nurses
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

// The add nurse button on mainwindow will call this method when the user clicks it which will be used to add a nurse to the system
void MainWindow::addNurse() {
    // The method starts by grabbing the input values from the UI for the nurse ID, name, and hospital
    string nurseID = nurseManageIDInput->text().toStdString();
    string nurseName = nurseNameInput->text().toStdString();
    int hospitalIndex = nurseHospitalComboBox->currentIndex();
    // We will check and make sure all fields are filled out
    if (nurseID.empty() || nurseName.empty()) {
        statusDisplay->append("Error: All Fields Must Be Filled Out!");
        return;
    }
    // Need to check and see if the nurse ID is valid and if the nurse ID already exists in the system
    if (hospitalSystem->findNurse(nurseID)) {
        statusDisplay->append("Error: Nurse ID Already Exists!");
        return;
    }
    // If the nurse ID is valid, we will create a new nurse object using the input values and the hospital ID
    Nurse* nurse = new Nurse(nurseID, nurseName, hospitalSystem->getHospital(hospitalIndex)->getHospitalID());
    // We will check if the nurse object was created successfully using the nurse and hospital index and returned from addNurse
    if (hospitalSystem->addNurse(nurse, hospitalIndex)) {
        statusDisplay->append("Nurse Added Successfully To " + nurseHospitalComboBox->currentText());
        nurseManageIDInput->clear();
        nurseNameInput->clear();
    } else {    // Check if the hospital already has 60 nurses
        statusDisplay->append("Error: There Are Already 60 Nurses!");
        delete nurse;
    }
}

// This method will be used when the user clicks the button to relocate a nurse in MainWindow
void MainWindow::relocateNurse() {
    // We will start by getting the input values from the UI for the nurse ID and new hospital
    string nurseID = nurseManageIDInput->text().toStdString();
    int newHospitalIndex = nurseChangeHospitalComboBox->currentIndex();
    // We will check and make sure all fields are filled out
    if (nurseID.empty()) {
        statusDisplay->append("Error: Nurse ID Must Be Provided!");
        return;
    }
    // We will grab the nurse object in the hospital system using the Nurse ID and check if it is valid
    Nurse* nurse = hospitalSystem->findNurse(nurseID); 
    if (!nurse) { //Check if the nurse ID is valid and if the nurse ID already exists in the system
        statusDisplay->append("Error: Nurse Not Found");
        return;
    }
    // We will grab the nurse object in the hospital system using the Nurse ID for the current hospital
    Hospital* currentHospital = hospitalSystem->findNurseHospital(nurseID);
    // We will also grab the new hospital object in the hospital system using the new hospital index
    Hospital* newHospital = hospitalSystem->getHospital(newHospitalIndex);
    // Make sure the new hospital is valid and not the same as the current hospital
    if (!currentHospital || !newHospital) {
        statusDisplay->append("Error: Hospital Selection Is Invalid!");
        return;
    }
    // Check if the new hospital is the same as the current hospital
    if (currentHospital->getHospitalID() == newHospital->getHospitalID()) {
        statusDisplay->append("Error: Nurse is already in the selected hospital.");
        return;
    }
    // Check to make sure the nurse is not assigned to any patients since we cannot relocate a nurse that is assigned to patients
    if (!nurse->getPatientIDs().empty()) {
        statusDisplay->append("Error: Nurse Is Assigned To Patients. Reassign or Discharge Them First!");
        return;
    }
    // We will provide users with a message if the nurse was successfully relocated to the new hospital
    if (hospitalSystem->relocateNurse(nurseID, newHospitalIndex)) {
        statusDisplay->append("Nurse " + QString::fromStdString(nurseID) + " Relocated To " + nurseChangeHospitalComboBox->currentText());
    } else {
        statusDisplay->append("Error: Could Not Relocate Nurse!");
    }
}

// This method will be used when the user clicks the button to remove a nurse in MainWindow
void MainWindow::removeNurse() {
    // We first need to get the input values from the UI for the nurse ID
    string nurseID = nurseManageIDInput->text().toStdString();
    // Check if the nurse ID is valid and not empty
    if (nurseID.empty()) {
        statusDisplay->append("Error: Nurse ID Must Be Provided!");
        return;
    }
    // We will check if the nurse ID is valid and if the nurse ID already exists in the system
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {
        statusDisplay->append("Error: Nurse Not Found Or Already Removed!");
        return;
    }
    // Check if the nurse is assigned to any patients
    if (!nurse->getPatientIDs().empty()) {
        statusDisplay->append("Error: Nurse Is Assigned To Patients And Cannot Be Removed!");
        return;
    }
    // We are creating a hospital object to get the hospital information and find the hospital of the nurse
    Hospital* hospital = hospitalSystem->findNurseHospital(nurseID);
    if (!hospital) {    // Check if the hospital is valid for the nurse ID
        statusDisplay->append("Error: Could Not Determine Nurse's Hospital!");
        return;
    }
    // Check if the hospital has at least 3 nurses before removing one
    if (hospital->getNurses().size() <= 3) {
        statusDisplay->append("Error: Hospital Must Maintain At Least 3 Nurses!");
        return;
    }
    // We will check if the nurse ID is valid and if the nurse ID already exists in the system
    if (hospitalSystem->removeNurse(nurseID)) {
        statusDisplay->append("Nurse " + QString::fromStdString(nurseID) + " Removed Successfully!");
    } else {
        statusDisplay->append("Error: Failed To Remove Nurse");
    }
}

// We wanted to have a method to view the details of a nurse in MainWindow when the user clicks the button
void MainWindow::viewNurseDetails() {
    // We will allow the user to enter the nurse ID to view their details and we will grab that from the mainwindow
    string nurseID = nurseManageIDInput->text().toStdString();
    // We will check if the nurse ID is valid and not empty
    if (nurseID.empty()) {
        statusDisplay->append("Error: Enter Nurse ID To View Details!");
        return;
    }
    // Using the nurse ID, we will create a nurse object to get the nurse information
    Nurse* nurse = hospitalSystem->findNurse(nurseID);
    if (!nurse) {   // if the ID is not valid, we will show a message
        statusDisplay->append("Nurse Not Found!");
        return;
    }
    // We will use the nurse ID to create a hospital object to get the hospital information for the nurse
    Hospital* hospital = hospitalSystem->findNurseHospital(nurseID);
    if (!hospital) {    // Check if the hospital is valid for the nurse ID
        statusDisplay->append("Error: Could Not Determine Nurse's Hospital!");
        return;
    }
    // We will get the full description of the nurse and show it in the status display and the nurses current hospital
    statusDisplay->append(QString::fromStdString(nurse->getFullDescription()));
    statusDisplay->append("Working in Hospital: " + QString::fromStdString(hospital->getHospitalName()));
}

// Added a method to list all doctors in the system when the user clicks the button in MainWindow
void MainWindow::listAllDoctors() {
    statusDisplay->clear(); // Clear the display before showing the report
    statusDisplay->append("=== ALL DOCTORS IN SYSTEM ===");
    // We will create a map to store all doctors in the hospital system
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
    // Group doctors by hospital into a map so we can display them using the map
    map<string, vector<Doctor*>> doctorsByHospital;
    // We will loop threw all doctors and group them by their hospital
    for (const auto& pair : doctorMap) {
        Doctor* doctor = pair.second;
        Hospital* hospital = hospitalSystem->findDoctorHospital(doctor->getDoctorID());
        // Check if the hospital is valid
        if (hospital) {
            doctorsByHospital[hospital->getHospitalName()].push_back(doctor);
        }
    }
    // Loop through each hospital and display doctors grouped by each hospital
    for (const auto& pair : doctorsByHospital) {
        statusDisplay->append("\n==== " + QString::fromStdString(pair.first) + " ====");
        // Loop through the doctors in the vector and display their details
        for (const Doctor* doctor : pair.second) {
            statusDisplay->append("\nDoctor ID: " + QString::fromStdString(doctor->getDoctorID()));
            statusDisplay->append("Name: " + QString::fromStdString(doctor->getDoctorName()));
            statusDisplay->append("Patients: " + QString::number(doctor->getPatientIDs().size()));
            // Show patient IDs if any
            if (!doctor->getPatientIDs().empty()) {
                QString patientList = "Patient IDs: ";
                // Loop through the patient IDs and display them
                for (size_t patientID = 0; patientID < doctor->getPatientIDs().size(); patientID++) {
                    patientList += QString::fromStdString(doctor->getPatientIDs()[patientID]);
                    if (patientID < doctor->getPatientIDs().size() - 1) {   // Check if not the last patient
                        patientList += ", ";
                    }
                }
                statusDisplay->append(patientList);
            } else {
                statusDisplay->append("No Assigned Patients");
            }
            statusDisplay->append("----------------------------------------");
        }
    }
    // Show count at the end
    statusDisplay->append("\nTotal Doctors: " + QString::number(doctorMap.size()));
}

// This method will list all nurses in the system and display their details.
void MainWindow::listAllNurses() {
    statusDisplay->clear(); // Clear the display for the nurse list
    statusDisplay->append("==== ALL NURSES IN SYSTEM ====");
    // We will create a map to store all nurses in the hospital system
    map<string, Nurse*> nurseMap;
    // We will loop through all hospitals and get the nurses from each hospital
    for (auto hospital : hospitalSystem->getAllHospitals()) {
        for (auto nurse : hospital->getNurses()) {
            nurseMap[nurse->getNurseID()] = nurse;
        }
    }
    // Check if there are no nurses in the system
    if (nurseMap.empty()) {
        statusDisplay->append("No Nurses In The System!");
        return;
    }
    // We will create another map with the value of the nurse vector to store the nurses by their hospital
    map<string, vector<Nurse*>> nursesByHospital;
    // Loop through all nurses and group them by their hospital
    for (const auto& pair : nurseMap) {
        Nurse* nurse = pair.second;
        Hospital* hospital = hospitalSystem->findNurseHospital(nurse->getNurseID());
        // Check if the hospital is valid
        if (hospital) {
            // We will push the nurse into the vector of nurses for that hospital
            nursesByHospital[hospital->getHospitalName()].push_back(nurse);
        }
    }
    // Do another for loop to display the nurses grouped by their hospital
    for (const auto& pair : nursesByHospital) {
        statusDisplay->append("\n==== " + QString::fromStdString(pair.first) + " ====");
        // Another loop to go through the nurses in the vector and display their details
        for (const Nurse* nurse : pair.second) {
            statusDisplay->append("\nNurse ID: " + QString::fromStdString(nurse->getNurseID()));
            statusDisplay->append("Name: " + QString::fromStdString(nurse->getNurseName()));
            statusDisplay->append("Patients: " + QString::number(nurse->getPatientIDs().size()) + "/2");
            // We will check if the nurse has any assigned patients
            if (!nurse->getPatientIDs().empty()) {
                QString patientList = "Patient IDs: ";
                // Loop through the patient IDs and display them and add them to the list
                for (size_t nursePatientID = 0; nursePatientID < nurse->getPatientIDs().size(); nursePatientID++) {
                    patientList += QString::fromStdString(nurse->getPatientIDs()[nursePatientID]);  // Get the patient ID
                    if (nursePatientID < nurse->getPatientIDs().size() - 1) {   // Check if not the last patient
                        patientList += ", ";
                    }
                }
                statusDisplay->append(patientList); // Show the list of assigned patients and add them to the list
            } else {
                statusDisplay->append("No Assigned Patients");
            }
            statusDisplay->append("----------------------------------------");
        }
    }
    // Show count at the end
    statusDisplay->append("\nTotal Nurses: " + QString::number(nurseMap.size()));
}
