#include "PharmacyWarehouse.h"
#include <iostream>
#include <stdexcept>

using namespace std;

PharmacyWarehouse::PharmacyWarehouse(const string& name)
    : warehouseName(name) {}

string PharmacyWarehouse::getName() const { return warehouseName; }
int PharmacyWarehouse::getCount() const { return (int)medicines.size(); }

void PharmacyWarehouse::addMedicine(const Medicine& m) {
    if ((int)medicines.size() >= MAX_CAPACITY)
        throw runtime_error("Sklad perepolnen! Maksimum: " + to_string(MAX_CAPACITY));
    medicines.push_back(m);
}

Medicine* PharmacyWarehouse::findByName(const string& n) {
    for (auto& m : medicines)
        if (m.getName() == n) return &m;
    return nullptr;
}

bool PharmacyWarehouse::issueMedicine(const string& n, int count) {
    Medicine* m = findByName(n);
    if (!m) throw runtime_error("Preparat ne nayden: " + n);
    return m->issue(count);
}

void PharmacyWarehouse::receiveMedicine(const string& n, int count) {
    Medicine* m = findByName(n);
    if (!m) throw runtime_error("Preparat ne nayden: " + n);
    m->receive(count);
}

int PharmacyWarehouse::removeExpired() {
    int removed = 0;
    for (auto it = medicines.begin(); it != medicines.end(); ) {
        if (it->isExpired()) {
            cout << "  [UTILIZATSIYA] " << it->getName()
                 << " (seriya " << it->getSeries() << ")\n";
            it = medicines.erase(it);
            ++removed;
        } else ++it;
    }
    return removed;
}

void PharmacyWarehouse::printStockReport() const {
    cout << "\n  SKLAD: " << warehouseName << "\n";
    cout << "  Vsego pozitsiy: " << medicines.size()
         << " / " << MAX_CAPACITY << "\n\n";
    if (medicines.empty()) { cout << "  Sklad pust.\n"; return; }
    for (const auto& m : medicines) {
        m.printInfo();
        cout << "\n";
    }
}

void PharmacyWarehouse::printExpired() const {
    bool found = false;
    for (const auto& m : medicines)
        if (m.isExpired()) { m.printInfo(); cout << "\n"; found = true; }
    if (!found) cout << "  Prosrochennyh preparatov net.\n";
}

void PharmacyWarehouse::printAvailable() const {
    bool found = false;
    for (const auto& m : medicines)
        if (!m.isExpired()) { m.printInfo(); cout << "\n"; found = true; }
    if (!found) cout << "  Dostupnyh k vydache preparatov net.\n";
}