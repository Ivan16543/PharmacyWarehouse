#include "../include/PharmacyWarehouse.h"
#include <iostream>
#include <string>

using namespace std;

PharmacyWarehouse::PharmacyWarehouse(string_view name)
    : warehouseName(name) {}

string PharmacyWarehouse::getName() const { return warehouseName; }
int PharmacyWarehouse::getCount() const { return (int)medicines.size(); }

void PharmacyWarehouse::addMedicine(const Medicine& m) {
    if ((int)medicines.size() >= MAX_CAPACITY) {
        string msg = "Warehouse is full! Maximum capacity: ";
        msg += to_string(MAX_CAPACITY);
        throw MedicineError(msg);
    }
    medicines.push_back(m);
}

Medicine* PharmacyWarehouse::findByName(string_view n) {
    for (auto& m : medicines) {
        if (m.getName() == n) return &m;
    }
    return nullptr;
}

bool PharmacyWarehouse::issueMedicine(string_view n, int count) {
    Medicine* m = findByName(n);
    if (!m) {
        string msg = "Medicine not found: ";
        msg += n;
        throw MedicineError(msg);
    }
    return m->issue(count);
}

void PharmacyWarehouse::receiveMedicine(string_view n, int count) {
    Medicine* m = findByName(n);
    if (!m) {
        string msg = "Medicine not found: ";
        msg += n;
        throw MedicineError(msg);
    }
    m->receive(count);
}

int PharmacyWarehouse::removeExpired() {
    int removed = 0;
    vector<Medicine> validMedicines; 

    for (const auto& med : medicines) {
        if (med.isExpired()) {
            cout << "  [DISPOSAL] " << med.getName()
                 << " (series " << med.getSeries() << ")\n";
            ++removed;
        } else {
            validMedicines.push_back(med); 
        }
    }
    medicines = validMedicines; 
    return removed;
}

void PharmacyWarehouse::printStockReport() const {
    cout << "\n  WAREHOUSE: " << warehouseName << "\n";
    cout << "  Total items: " << medicines.size()
         << " / " << MAX_CAPACITY << "\n\n";
    if (medicines.empty()) { cout << "  Warehouse is empty.\n"; return; }
    for (const auto& m : medicines) {
        m.printInfo();
        cout << "\n";
    }
}

void PharmacyWarehouse::printExpired() const {
    bool found = false;
    for (const auto& m : medicines) {
        if (m.isExpired()) { m.printInfo(); cout << "\n"; found = true; }
    }
    if (!found) cout << "  No expired medicines.\n";
}

void PharmacyWarehouse::printAvailable() const {
    bool found = false;
    for (const auto& m : medicines) {
        if (!m.isExpired()) { m.printInfo(); cout << "\n"; found = true; }
    }
    if (!found) cout << "  No available medicines for issue.\n";
}

PharmacyWarehouse& PharmacyWarehouse::operator+=(const Medicine& med) {
    addMedicine(med);
    return *this;
}

PharmacyWarehouse& PharmacyWarehouse::operator-=(const Medicine& med) {
    for (auto it = medicines.begin(); it != medicines.end(); ++it) {
        if (*it == med) { 
            medicines.erase(it);
            cout << " Medicine removed from warehouse.\n";
            return *this;
        }
    }
    cout << "  ERROR: Medicine with this series not found in warehouse.\n";
    return *this;
}