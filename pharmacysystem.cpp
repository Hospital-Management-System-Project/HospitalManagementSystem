#include "pharmacysystem.h"
#include "drugs.h"
#include <sstream>

using namespace std;

// Initialize the static instance pointer
PharmacySystem* PharmacySystem::instance = nullptr;

void PharmacySystem::initializePharmacies() {
    // Clear existing pharmacies if any
    for (auto pharmacy : pharmacies) {
        delete pharmacy;
    }
    pharmacies.clear();
    
    // Pharmacy names - examples from Windsor, ON area
    const string PHARMACY_NAMES[20] = {
        "Shoppers Drug Mart", "Rexall Pharmacy", "Walmart Pharmacy", 
        "Medical Arts Pharmacy", "Loblaw Pharmacy", "Erie Pharmacy",
        "LaSalle Pharmacy", "Tecumseh Pharmacy", "Riverside Pharmacy",
        "University Pharmacy", "Downtown Pharmacy", "Essex Pharmacy",
        "Kingsville Pharmacy", "Leamington Pharmacy", "Belle River Pharmacy",
        "Walkerville Pharmacy", "South Windsor Pharmacy", "East End Pharmacy",
        "West Side Pharmacy", "County Pharmacy"
    };
    
    // Create 20 pharmacies with unique IDs
    for (int i = 0; i < 20; i++) {
        string id = "P" + to_string(i + 1);
        pharmacies.push_back(new Pharmacy(id, PHARMACY_NAMES[i]));
    }
}

void PharmacySystem::initializeDrugs() {
    drugList.clear();
    drugList.push_back(drug("Aspirin", 10.0));
    drugList.push_back(drug("Ibuprofen", 12.5));
    drugList.push_back(drug("Acetaminophen", 8.0));
    drugList.push_back(drug("Amoxicillin", 25.0));
    drugList.push_back(drug("Insulin", 50.0));
}

Pharmacy* PharmacySystem::getPharmacy(int index) {
    if (index >= 0 && static_cast<size_t>(index) < pharmacies.size()) {
        return pharmacies[index];
    }
    return nullptr;
}

vector<Pharmacy*> PharmacySystem::getAllPharmacies() {
    return pharmacies;
}

string PharmacySystem::getPharmacyStatus() {
    stringstream status;
    
    status << "=== Pharmacy System Status ===\n\n";
    
    for (auto pharmacy : pharmacies) {
        status << "Pharmacy: " << pharmacy->getPharmacyName() << " (ID: " << pharmacy->getPharmacyID() << ")\n";
        status << "Pending Payments: $" << pharmacy->getPendingPayments() << "\n\n";
    }
    
    return status.str();
}

PharmacySystem::~PharmacySystem() {
    for (auto pharmacy : pharmacies) {
        delete pharmacy;
    }
    pharmacies.clear();
}
