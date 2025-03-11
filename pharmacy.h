#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>

using namespace std;

class Pharmacy {
public:
  string pharmacyID;
  string name;

  Pharmacy(string id, string n);
};

#endif // PHARMACY_H
