# Hospital Management System - Testing Guide

## Prerequisites
- Qt 5.x or newer installed
- C++ compiler compatible with your Qt version

## Building the Project

### Using Qt Creator
1. Open Qt Creator
2. Go to File > Open File or Project...
3. Navigate to `/Users/camvandy/CS/HospitalManagementSystem/`
4. Select the project file (likely a .pro file) and click Open
5. Configure the project if prompted
6. Click the Build (hammer) icon or press Ctrl+B (Cmd+B on Mac)

# Navigate to your project directory
cd /Users/camvandy/CS/HospitalManagementSystem

# Generate the Makefile from your .pro file
qmake HospitalManagementSystem.pro

# Compile the project
make

# Run the application
# On macOS, the executable might be in a .app bundle
open HospitalManagementSystem.app
# OR if it's a direct executable
./HospitalManagementSystem


## Running the Application
- In Qt Creator: Click the Run (green play) button or press Ctrl+R (Cmd+R on Mac)
- From command line: Execute the binary created in the build directory

## Testing the System

### Test Plan

1. **Add Patients**
   - Fill in all patient fields (ID, name, phone, etc.)
   - Select a hospital from the dropdown
   - Click "Add Patient"
   - Verify confirmation message

2. **View Hospital Status**
   - Click "Display Hospital Status"
   - Verify the list shows hospitals and their patients

3. **View Patient Details**
   - Enter a patient ID
   - Click "View Patient Details"
   - Verify patient information is displayed correctly

4. **Doctor Assignment**
   - Go to Doctor-Patient tab
   - Enter a doctor ID (try D1, D2, etc.)
   - Enter a patient ID (one you created earlier)
   - Click "Assign Doctor" or "Set as Primary Doctor"
   - Verify confirmation message

5. **Billing Functions**
   - Go to the Billing tab
   - Enter a patient ID
   - Click "Calculate Bill"
   - Try entering a payment amount and click "Collect Payment"
   - View the billing report

6. **Discharge and Relocate**
   - Try relocating a patient to another hospital
   - Try discharging a patient through regular discharge or doctor request

## Sample Data
- **Doctor IDs**: D1, D2, D3, etc.
- **Hospital Indices**: 0-4 (corresponding to the 5 hospitals)
- **Patient IDs**: Create your own (e.g., P001, P002)

## Troubleshooting
- If patient can't be found, ensure you've entered the correct ID
- If doctors can't be assigned, ensure both doctor and patient exist and are in the same hospital
- The timer updates patient days every minute (simulated)
