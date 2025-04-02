#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include <vector>
#include <map>
#include <string>
#include "pharmacy.h"
#include "drugs.h"

using namespace std;    // We used the using directive to avoid typing std:: everywhere

// Singleton class to manage the pharmacy system
class PharmacySystem {
private:
    static PharmacySystem* instance;    // Singleton instance to manage the pharmacy system
    vector<Pharmacy*> pharmacies;       // This is the vector of pharmacies that will be used
    vector<drug> drugList;              // This is the vector of drugs that will be used

    PharmacySystem() {}                 // Private constructor for singleton

public:
    // Static method to get the singleton instance which will be used to access the pharmacy system
    static PharmacySystem* getInstance() {
        if (!instance) {    // If the instance is not created yet
            instance = new PharmacySystem();    // Then create it 
        }
        return instance;    // Return the instance of the pharmacy system
    }
    
    // Initialize the pharmacies and drugs
    void initializeDrugs();
    void initializePharmacies();

    // Getters for pharmacies and drugs
    Pharmacy* getPharmacy(int index);
    vector<Pharmacy*> getAllPharmacies();

    // Getters for drugs as a vector which will be used to get all the drugs
    std::vector<drug> getAllDrugs() const { 
        return drugList; 
    }
    
    // Getters for the pharmacy status
    string getPharmacyStatus();   // Get the status of the pharmacy system
    ~PharmacySystem();   // Destructor to clean up the pharmacies
};
#endif // PHARMACYSYSTEM_H