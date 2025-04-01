# Hospital Management System

The Hospital Management System is a Qt-based C++ application that allows administrators to manage multiple hospitals, patients, doctors, nurses, and pharmacies. It supports patient tracking, staff assignments, billing management, pharmacy coordination, and more through a user-friendly interface.

---

## Features

### Patient Management
- Admit, relocate, and discharge patients across multiple hospitals
- View individual patient information and medical status

### Doctor Assignment
- Assign both primary and attending doctors to patients
- Allow doctors to request patient discharges

### Nurse Assignment
- Assign nurses to patients with a limit of two patients per nurse

### Billing System
- Automatically track daily charges per patient
- Accept partial and full payments
- Generate billing reports for patients and hospitals

### Pharmacy Integration
- Request drug deliveries from specific pharmacies
- Track and pay pharmacy-related bills

### Hospital Network Management
- Manage multiple hospitals with individual staff and patient lists
- Monitor hospital capacity and status

### Real-Time Updates
- Daily updates for patient billing and admission duration

---

## System Architecture

The system is designed using the Model-View-Controller (MVC) pattern:

- **Model**: Core C++ classes (`Hospital`, `Patient`, `Doctor`, `Nurse`, `Pharmacy`)
- **View & Controller**: Qt-based GUI using `MainWindow` and tabbed navigation
- **Controllers**: Singleton managers (`HospitalSystem`, `PharmacySystem`) coordinate logic

---

## Building and Running

### Prerequisites
- Qt 5.x or newer
- C++ compiler (e.g., GCC, Clang, or MSVC)
- Make or equivalent build tool

### Build Instructions

#### Using Qt Creator
1. Open **Qt Creator**
2. Select **File > Open File or Project...**
3. Navigate to your project directory
4. Open `HospitalManagementSystem.pro`
5. Configure the project as prompted
6. Build and run the application

#### Using Command Line
```bash
# Navigate to the project directory
cd /path/to/HospitalManagementSystem

# Generate Makefile
qmake HospitalManagementSystem.pro

# Compile the project
make

# Run the application
./HospitalManagementSystem   # Also open HospitalManagementSystem.app for Mac Or 'HospitalManagementSystem.exe' on Windows
```

---

## User Guide

### Tab Overview
- **Patient Management**: Add, relocate, and discharge patients
- **Doctor Management**: View, add, remove, and relocate doctors
- **Nurse Management:**: View, add, remove, and relocate nurses
- **Doctor-Patient**: Assign doctors and handle discharge requests
- **Nurse-Patient**: Assign nurses to patients
- **Billing**: Calculate and collect payments, generate billing reports
- **Drug Delivery**: Order and track medication delivery from pharmacies

---

## Testing Scenarios

### 1. Add a Patient
- Fill out all patient fields
- Select a hospital and set a billing rate
- Click "Add Patient" and verify success message

### 2. Assign a Doctor
- Go to the Doctor-Patient tab
- Enter a doctor and patient ID
- Click "Assign Doctor" or "Set as Primary Doctor"

### 3. Relocate a Patient
- Select a different hospital from the dropdown
- Ensure a valid doctor is available at the new hospital
- Click "Relocate Patient"

### 4. Billing and Payments
- Use "Calculate Bill" to view current charges
- Enter payment amount and click "Collect Payment"
- Generate billing reports for detailed summaries

### 5. Request a Drug
- Go to the Drug Delivery tab
- Select hospital, pharmacy, and drug
- Click "Request Delivery" to create a pharmacy bill

---

## Sample Data

- **Doctor IDs**: `D1` to `D15`
- **Patient IDs**: `P001` to `P005` (sample), or create your own (e.g., `P006`)
- **Hospitals**: Indexes `0` to `4`
- **Sample Drugs**: Aspirin, Ibuprofen, Acetaminophen, Amoxicillin, Insulin
