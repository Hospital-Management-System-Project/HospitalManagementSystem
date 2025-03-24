#include <QApplication>
#include "mainwindow.h"
#include "hospitalsystem.h"
#include "pharmacysystem.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    // Initialize the hospital system
    HospitalSystem* hospitalSystem = HospitalSystem::getInstance();
    hospitalSystem->initializeHospitals();
    
    // Initialize pharmacy system
    PharmacySystem* pharmacySystem = PharmacySystem::getInstance();
    pharmacySystem->initializePharmacies();
    
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
