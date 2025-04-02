#include "pharmacysystem.h"
#include "drugs.h"
#include <sstream>

using namespace std;

// Initialize the static instance pointer which will be used to manage the singleton instance
PharmacySystem* PharmacySystem::instance = nullptr;

void PharmacySystem::initializePharmacies() {
    // Clear existing pharmacies if any
    for (auto pharmacy : pharmacies) {
        delete pharmacy;
    }
    pharmacies.clear();
    
    // We generated pharmacy names to meet the requirement of 20 pharmacies
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
        string id = "P" + to_string(i + 1); // We put the pharmacy ID in the format P1, P2, ..., P20
        pharmacies.push_back(new Pharmacy(id, PHARMACY_NAMES[i]));
    }
}

// We have a function to initialize the drugs
void PharmacySystem::initializeDrugs() {
    drugList.clear();   // Start by clearing the existing drug list
    // We added a couple drugs with their prices to the drug list vector using push_back
    drugList.push_back(drug("Aspirin", 10.0)); 
    drugList.push_back(drug("Ibuprofen", 12.5));
    drugList.push_back(drug("Acetaminophen", 8.0));
    drugList.push_back(drug("Amoxicillin", 25.0));
    drugList.push_back(drug("Insulin", 50.0));
}

// We have a function to get the pharmacy by index
Pharmacy* PharmacySystem::getPharmacy(int index) {
    if (index >= 0 && static_cast<size_t>(index) < pharmacies.size()) {
        return pharmacies[index];   // Return the pharmacy at the given index
    }
    return nullptr;
}

// We have a function to get all the pharmacies in the vector
vector<Pharmacy*> PharmacySystem::getAllPharmacies() {
    return pharmacies;
}

// We have a function to get the pharmacy status which will show the status of all pharmacies
string PharmacySystem::getPharmacyStatus() {
    stringstream status;
    status << "=== Pharmacy System Status ===\n\n"; 
    // We have a loop to iterate through all pharmacies and get their status
    for (auto pharmacy : pharmacies) {
        status << "Pharmacy: " << pharmacy->getPharmacyName() << " (ID: " << pharmacy->getPharmacyID() << ")\n";
        status << "Pending Payments: $" << pharmacy->getPendingPayments() << "\n\n";
    }
    return status.str();    // Return the status as a string
}

// Destructor to clean up the pharmacies and free memory
PharmacySystem::~PharmacySystem() {
    for (auto pharmacy : pharmacies) {
        delete pharmacy;
    }
    pharmacies.clear();     
}