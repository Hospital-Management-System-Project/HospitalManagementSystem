#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include <vector>
#include <map>
#include <string>
#include "drug.h"
#include "pharmacy.h"

class PharmacySystem {
private:
    static PharmacySystem* instance;

    std::vector<Pharmacy*> pharmacies;

    std::vector<Drug> drugList;

    PharmacySystem() {}

public:
    static PharmacySystem* getInstance() {
        if (!instance) {
            instance = new PharmacySystem();
        }
        return instance;
    }

    void initializePharmacies();

    void initializeDrugs();

    Pharmacy* getPharmacy(int index);
    Pharmacy* findPharmacyByID(std::string pharmacyID);
    std::vector<Pharmacy*> getAllPharmacies();

    std::vector<Drug> getAllDrugs() const { return drugList; }

    bool addPrescription(std::string pharmacyID, std::string prescriptionID,
                         std::string patientID, std::string medication, double price);
    bool deliverMedication(std::string pharmacyID, std::string prescriptionID);
    std::string createBill(std::string pharmacyID, std::string hospitalID, std::string prescriptionID);
    bool payBill(std::string pharmacyID, std::string billID);

    std::string getPharmacyStatus();

    ~PharmacySystem();
};

#endif // PHARMACYSYSTEM_H
