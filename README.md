# Hospital Management System

The Hospital Management System is a Qt-based C++ application for managing patients, staff, billing, and medication logistics across a network of hospitals. It offers a clean tabbed interface for efficient hospital administration, supporting tasks like patient tracking, doctor/nurse assignment, billing, and pharmacy coordination.

---

## Getting Started

### Prerequisites
- Qt 5.x or newer
- C++ compiler (e.g., GCC, Clang, or MSVC)
- `make` or a compatible build tool

### Build & Run

#### Option 1: Using Qt Creator
1. Open **Qt Creator**
2. Go to **File > Open File or Project...**
3. Select `HospitalManagementSystem.pro` from the project folder
4. Configure the project if prompted
5. Click **Build** and then **Run**

#### Option 2: Using Command Line
```bash
# Navigate to the project directory
cd /path/to/HospitalManagementSystem

# Generate Makefile
qmake HospitalManagementSystem.pro

# Compile the project
make

# Run the application
./HospitalManagementSystem         # Linux/macOS
open HospitalManagementSystem.app  # macOS
HospitalManagementSystem.exe       # Windows
```

---

## System Architecture

The system follows the Model-View-Controller (MVC) pattern:

- **Model**: Core logic in C++ classes like `Hospital`, `Patient`, `Doctor`, `Nurse`, `Pharmacy`, and `Drug`
- **View & Controller**: Qt-based GUI (`MainWindow`) with a tabbed interface
- **Controllers**: Singleton managers `HospitalSystem` and `PharmacySystem` coordinate global operations

---

### Core OOP Principles
- **Encapsulation**: All class attributes are private, accessed via getters/setters (e.g., `getDoctorID()`, `getHospitalID()`).
- **Composition**: Hospitals manage collections of `Patient`, `Doctor`, and `Nurse` objects; `Pharmacy` manages `Drug` and `Bill` objects.
- **Modular Classes**: Each class (e.g., `Patient`, `Doctor`, `Hospital`, `Pharmacy`) has a focused responsibility and minimal dependencies.
- **Singleton Pattern**: `HospitalSystem` and `PharmacySystem` are global managers used to coordinate application-wide logic from any part of the code.

---

## Main Features

### Patient Management
- Admit, relocate, discharge patients across hospitals
- View individual patient details and status

### Doctor Management
- View, add, remove, and relocate doctors
- Assign primary and attending doctors to patients
- Allow doctors to request discharges

### Nurse Management
- View, add, remove, and relocate nurses
- Assign nurses (max 2 patients per nurse)

### Billing System
- Daily charges update automatically
- Accept partial/full payments
- Generate reports for patients and hospitals

### Pharmacy Integration
- Request medications from pharmacies
- Track and pay pharmacy-related bills

### Hospital Network
- Manage staff/patient lists per hospital
- Monitor capacity and workload

### Real-Time Updates
- Automated daily updates for billing and patient stay tracking using Qt timers

---

## User Interface Overview

The interface is organized into the following tabs:

- **Patient Management**: Admit, relocate, discharge patients
- **Doctor Management**: Add, remove, view, or reassign doctors
- **Nurse Management**: Add, remove, view, or reassign nurses
- **Doctor-Patient**: Assign doctors, set primary doctors, request discharges
- **Nurse-Patient**: Assign nurses to patients
- **Billing**: View/calculate bills, collect payments, generate reports
- **Drug Delivery**: Order medications and track hospital pharmacy bills

---

## How to Test the Application

### 1. Add a Patient
- Fill in patient details
- Choose hospital, doctor, and nurse
- Set daily billing rate
- Click **"Add Patient"**

### 2. Assign a Doctor
- Go to the **Doctor-Patient** tab
- Enter doctor and patient IDs
- Click **"Assign Doctor"** or **"Set as Primary Doctor"**

### 3. Relocate a Patient
- Select a new hospital from dropdown
- Ensure doctor is valid at the destination
- Click **"Relocate Patient"**

### 4. View & Collect Payments
- Enter patient ID under **Billing**
- Click **"Calculate Bill"**
- Input amount and click **"Collect Payment"**

### 5. Request Drug Delivery
- Go to **Drug Delivery**
- Select hospital, pharmacy, and drug
- Click **"Request Delivery"** to generate a bill

### 6. Manage Staff
- Go to **Doctor Management** or **Nurse Management**
- Add new staff, relocate them between hospitals, or remove them by ID

---

## Sample Data

- **Doctor IDs**: `D1` to `D49` (auto-assigned across hospitals, 1 less than max to test add doctor functionality)
- **Nurse IDs**: `N1` to `N59` (auto-assigned across hospitals, 1 less than max to test add nurse functionality)
- **Patient IDs**:
  - `P001`: Alice Johnson (Hospital 0, Doctor D1, Nurse N1)
  - `P002`: Bob Smith (Hospital 0, Doctor D3, Nurse N6)
  - `P003`: Carla Diaz (Hospital 2, Doctor D7, Nurse N11)
  - `P004`: David Lee (Hospital 3, Doctor D10, Nurse N16)
  - `P005`: Eva Nguyen (Hospital 4, Doctor D13, Nurse N21)
  - You can add more through the UI
- **Hospitals**: Indexed `0` through `4`
- **Pharmacies**: `P1` to `P20`
- **Available Drugs**: Aspirin, Ibuprofen, Acetaminophen, Amoxicillin, Insulin

---
