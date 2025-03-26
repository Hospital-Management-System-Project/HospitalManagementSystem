# Hospital Management System

## Project Overview

The Hospital Management System is a comprehensive Qt-based application designed to streamline the management of multiple hospitals, patients, doctors, and pharmacies. This system enables healthcare administrators to efficiently track patient care, manage billing, coordinate with pharmacies, and maintain medical records.

### Key Features

- **Patient Management**: Admit, relocate, and discharge patients across multiple hospitals
- **Doctor Assignment**: Assign primary and attending doctors to patients
- **Billing System**: Track patient charges, collect payments, and generate billing reports
- **Pharmacy Integration**: Order medications and track pharmacy bills
- **Hospital Network**: Manage multiple hospital locations with their respective staff and patients
- **Real-time Updates**: Daily patient status updates with automatic billing adjustments

## System Architecture

The system follows a Model-View-Controller (MVC) pattern with Qt providing the View and Controller components while custom C++ classes form the Model layer:

- **Core Entities**: Hospital, Patient, Doctor, Nurse, Pharmacy
- **System Controllers**: HospitalSystem, PharmacySystem
- **User Interface**: Qt-based MainWindow with tabbed interface

## Building and Running the Application

### Prerequisites
- Qt 5.x or newer
- C++ compiler compatible with your Qt version
- Make or equivalent build system

### Build Methods

#### Using Qt Creator
1. Open Qt Creator
2. Go to File > Open File or Project...
3. Navigate to `/Users/camvandy/CS/HospitalManagementSystem/`
4. Select `HospitalManagementSystem.pro` and click Open
5. Configure the project if prompted
6. Click the Build icon (hammer) or press Ctrl+B (Cmd+B on Mac)

#### Using Command Line
```bash
# Navigate to your project directory
cd /Users/.../HospitalManagementSystem

# Generate the Makefile from your .pro file
qmake HospitalManagementSystem.pro

# Compile the project
make

# Run the application
# On macOS
open HospitalManagementSystem.app
# OR on other platforms
./HospitalManagementSystem
```

## Testing Guide

### User Interface Navigation

The application is organized into four main tabs:
1. **Patient Management**: Add, relocate, and discharge patients
2. **Doctor-Patient**: Assign doctors and request patient discharges
3. **Billing**: Calculate bills, collect payments, and generate reports
4. **Drug Delivery**: Request drug deliveries from pharmacies

### Test Scenarios

#### 1. Patient Admission Process
- Fill in all patient fields (ID, name, phone, disease, treatment, primary doctor)
- Select a hospital from the dropdown
- Set a daily billing rate
- Click "Add Patient"
- Verify the confirmation message

#### 2. Hospital Status Monitoring
- Click "Display Hospital Status" to see all hospitals and their patients
- Review doctor and nurse assignments
- Check bed availability at each hospital

#### 3. Patient Management
- View detailed patient information using "View Patient Details"
- Relocate patients between hospitals
- Request and process patient discharges

#### 4. Doctor Assignment
- Go to the Doctor-Patient tab
- Assign doctors to patients (both primary and attending)
- Use "List All Patients" to verify assignments

#### 5. Billing Operations
- Calculate a patient's current bill
- Collect partial or full payments
- Generate comprehensive billing reports
- Track pharmacy billing for drugs

#### 6. Pharmacy Integration
- Order medications for patients
- Request drug deliveries from specific pharmacies
- Track and pay pharmacy bills

### Sample Test Data
- **Doctor IDs**: D1-D15 (three doctors per hospital)
- **Hospital Indices**: 0-4 (corresponding to the five hospitals)
- **Patient IDs**: P001-P005 (pre-loaded), create your own (e.g., P006)
- **Sample Drugs**: Aspirin, Ibuprofen, Acetaminophen, Amoxicillin, Insulin