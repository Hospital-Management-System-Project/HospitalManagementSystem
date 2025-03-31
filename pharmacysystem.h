#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include <vector>
#include <map>
#include <string>
#include "pharmacy.h"
#include "drugs.h"

using namespace std;

class PharmacySystem {
private:
    static PharmacySystem* instance;
    vector<Pharmacy*> pharmacies;
    vector<drug> drugList; 
    
    // Private constructor
    PharmacySystem() {}

public:
    static PharmacySystem* getInstance() {
        if (!instance) {
            instance = new PharmacySystem();
        }
        return instance;
    }
    
    void initializeDrugs();
    void initializePharmacies();
    Pharmacy* getPharmacy(int index);
    vector<Pharmacy*> getAllPharmacies();
    std::vector<drug> getAllDrugs() const { return drugList; }
        
    string getPharmacyStatus();
    ~PharmacySystem();
};

#endif // PHARMACYSYSTEM_H
