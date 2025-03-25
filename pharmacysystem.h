#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include <vector>
#include <map>
#include <string>
#include "pharmacy.h"

using namespace std;

class PharmacySystem {
private:
    static PharmacySystem* instance;
    vector<Pharmacy*> pharmacies;
    
    // Private constructor for singleton
    PharmacySystem() {}

public:
    static PharmacySystem* getInstance() {
        if (!instance) {
            instance = new PharmacySystem();
        }
        return instance;
    }
    
    void initializePharmacies();
    Pharmacy* getPharmacy(int index);
    Pharmacy* findPharmacyByID(string pharmacyID);
    vector<Pharmacy*> getAllPharmacies();
    
    bool addPrescription(string pharmacyID, string prescriptionID, string patientID, string medication, double price);
    bool deliverMedication(string pharmacyID, string prescriptionID);
    string createBill(string pharmacyID, string hospitalID, string prescriptionID);
    bool payBill(string pharmacyID, string billID);
    
    string getPharmacyStatus();
    ~PharmacySystem();
};

#endif // PHARMACYSYSTEM_H
