#include <QApplication>
#include "mainwindow.h"
#include "hospitalsystem.h"
#include "pharmacysystem.h"

int main(int argc, char* argv[]) {
    // This will initialize the Qt application
    QApplication app(argc, argv);
    HospitalSystem* hospitalSystem = HospitalSystem::getInstance(); // Initialize the hospital system
    hospitalSystem->initializeHospitals();  // Initialize hospitals

    // Sample patient 1
    Patient* p1 = new Patient("P001", "Alice Johnson", "555-1234", "Flu", "Rest and fluids", "D1", "N1");
    p1->setDailyRate(150.0);
    hospitalSystem->admitPatient(p1, 0); // Hospital 0: Henry Ford Hospital

    // Sample patient 2
    Patient* p2 = new Patient("P002", "Bob Smith", "555-5678", "Pneumonia", "Antibiotics", "D3", "N6");
    p2->setDailyRate(200.0);
    hospitalSystem->admitPatient(p2, 0); // Hospital 1: Windsor Regional Hospital

    // Sample patient 3
    Patient* p3 = new Patient("P003", "Carla Diaz", "555-9012", "Broken Arm", "Surgery and cast", "D7", "N11");
    p3->setDailyRate(180.0);
    hospitalSystem->admitPatient(p3, 2); // Hospital 2: Amherstburg Hospital

    // Sample patient 4
    Patient* p4 = new Patient("P004", "David Lee", "555-3456", "Migraine", "Pain management", "D10", "N16");
    p4->setDailyRate(120.0);
    hospitalSystem->admitPatient(p4, 3); // Hospital 3: Tecumseh Hospital

    // Sample patient 5
    Patient* p5 = new Patient("P005", "Eva Nguyen", "555-7890", "Appendicitis", "Appendectomy", "D13", "N21");
    p5->setDailyRate(220.0);
    hospitalSystem->admitPatient(p5, 4); // Hospital 4: Chatham-Kent Health Alliance
    
    // Initialize pharmacy system
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    pharmacySystem->initializePharmacies();
    pharmacySystem->initializeDrugs();
    
    MainWindow mainWindow;  // This creates the main window
    mainWindow.show();      // Show the main window
    return app.exec();      // This starts the Qt event loop
}