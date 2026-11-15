#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

#include "include/Medicine.h"
#include "include/PharmacyWarehouse.h"

using namespace std;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) { clearInput(); return value; }
        cout << " Error: enter an integer number.\n";
        clearInput();
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) { clearInput(); return value; }
        cout << " Error: enter a number.\n";
        clearInput();
    }
}

string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void printMedicineMenu() {
    cout << "\nMENU: MEDICINE\n";
    cout << "  1. Create new medicine\n";
    cout << "  2. Show current medicine\n";
    cout << "  3. Change name\n";
    cout << "  4. Change manufacturer\n";
    cout << "  5. Change series\n";
    cout << "  6. Change category\n";
    cout << "  7. Change number of packs\n";
    cout << "  8. Change storage temperature\n";
    cout << "  9. Change expiry date\n";
    cout << " 10. Check if expired\n";
    cout << " 11. Send medicine to warehouse\n";
    cout << "  0. Back\n";
}

void medicineSubMenu(PharmacyWarehouse& warehouse) {
    Medicine med;
    int choice = -1;
    do {
        printMedicineMenu();
        choice = readInt("Your choice: ");

        switch (choice) {
        case 1: {
            cout << "\n--- Create medicine ---\n";
            string name  = readLine("Name: ");
            string manuf = readLine("Manufacturer: ");
            string serie = readLine("Series: ");
            string exp   = readLine("Expiry date (DD.MM.YYYY): ");
            int    packs = readInt("Number of packs: ");
            string cat   = readLine("Category: ");
            double temp  = readDouble("Temperature (C): ");
            try {
                med = Medicine(name, manuf, serie, exp, packs, cat, temp);
                cout << " Medicine created.\n";
            } catch (const MedicineError& e) { 
                cout << "  ERROR: " << e.what() << "\n";
            }
            break;
        }
        case 2:
            cout << "\nCurrent medicine\n";
            med.printInfo();
            break;
        case 3: med.setName(readLine("New name: "));    cout << "  [OK]\n"; break;
        case 4: med.setManufacturer(readLine("New manufacturer: ")); cout << "  [OK]\n"; break;
        case 5: med.setSeries(readLine("New series: "));      cout << "  [OK]\n"; break;
        case 6: med.setCategory(readLine("New category: "));cout << "  [OK]\n"; break;
        case 7: {
            int p = readInt("New number of packs: ");
            try { med.setPacks(p); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  ERROR: " << e.what() << "\n"; }
            break;
        }
        case 8: {
            double t = readDouble("New temperature (C): ");
            try { med.setTemperature(t); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  ERROR: " << e.what() << "\n"; }
            break;
        }
        case 9: {
            string exp = readLine("New expiry date (DD.MM.YYYY): ");
            try { med.setExpiryDate(exp); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  ERROR: " << e.what() << "\n"; }
            break;
        }
        case 10:
            cout << "  Medicine is " << (med.isExpired() ? "EXPIRED" : "VALID") << "\n";
            break;
        case 11: {
            try {
                warehouse.addMedicine(med);
                cout << "  Medicine sent to warehouse: " << warehouse.getName() << "\n";
            } catch (const MedicineError& e) {
                cout << "  ERROR: " << e.what() << "\n";
            }
            break;
        }
        case 0: cout << "  Returning to menu.\n"; break;
        default: cout << " Invalid choice.\n"; break;
        }
    } while (choice != 0);
}

void printWarehouseMenu() {
    cout << "\nMENU: WAREHOUSE\n";
    cout << "  1. Show all medicines\n";
    cout << "  2. Find medicine by name\n";
    cout << "  3. Receive medicine\n";
    cout << "  4. Issue medicine\n";
    cout << "  5. Dispose expired medicines\n";
    cout << "  6. Show only expired\n";
    cout << "  7. Show only available for issue\n";
    cout << "  8. Operator demonstration\n"; 
    cout << "  0. Back\n";
}

void demonstrateOperators(PharmacyWarehouse& warehouse) {
    cout << "\nOperator Demonstration\n";
    Medicine m1, m2;
    
    cout << "Enter first medicine:\n";
    cin >> m1;
    clearInput(); 

    cout << "\nEnter second medicine:\n";
    cin >> m2;
    clearInput();

    cout << "\nOutput via operator<<:\n";
    cout << m1 << "\n\n" << m2 << "\n\n";

    cout << "Comparison operator== (by series): ";
    if (m1 == m2) cout << "Medicines are IDENTICAL.\n";
    else cout << "Medicines are DIFFERENT.\n";

    cout << "Comparison operator< (by packs): ";
    if (m1 < m2) cout << "First medicine has FEWER packs.\n";
    else if (m1 > m2) cout << "First medicine has MORE packs.\n";
    else cout << "Same number of packs.\n";

    cout << "Friend function hasSpecialStorage: ";
    if (hasSpecialStorage(m1)) cout << "First medicine needs REFRIGERATION.\n";
    else cout << "First medicine stored at ROOM TEMP.\n";

    cout << "\nAdding via operator+= ...\n";
    warehouse += m1; 
    
    cout << "Removing via operator-= ...\n";
    warehouse -= m1; 
    cout << "Trying to remove second (not in warehouse):\n";
    warehouse -= m2; 
}

void warehouseSubMenu(PharmacyWarehouse& warehouse) {
    int choice = -1;
    do {
        printWarehouseMenu();
        choice = readInt("Your choice: ");

        switch (choice) {
        case 1: warehouse.printStockReport(); break;
        case 2: {
            cout << "\nSearch\n";
            string name = readLine("Name: ");
            if (const Medicine* m = warehouse.findByName(name)) { 
                m->printInfo();
            } else {
                cout << "  Medicine not found.\n";
            }
            break;
        }
        case 3: {
            cout << "\nReceive\n";
            string name = readLine("Name: ");
            int    cnt  = readInt("Count to receive: ");
            try {
                warehouse.receiveMedicine(name, cnt);
                if (const Medicine* m = warehouse.findByName(name)) {
                    cout << "Remaining: " << m->getPacks() << "\n";
                }
            } catch (const MedicineError& e) { cout << "  ERROR: " << e.what() << "\n"; }
            break;
        }
        case 4: {
            cout << "\nIssue\n";
            string name = readLine("Name: ");
            int    cnt  = readInt("Count to issue: ");
            try {
                warehouse.issueMedicine(name, cnt);
                if (const Medicine* m = warehouse.findByName(name)) {
                    cout << "Remaining: " << m->getPacks() << "\n";
                }
            } catch (const MedicineError& e) { cout << "  ERROR: " << e.what() << "\n"; }
            break;
        }
        case 5: {
            cout << "\nDispose expired\n";
            int removed = warehouse.removeExpired();
            cout << "  Items disposed: " << removed << "\n";
            break;
        }
        case 6: warehouse.printExpired();   break;
        case 7: warehouse.printAvailable(); break;
        case 8: demonstrateOperators(warehouse); break;
        case 0: cout << "  Returning to menu.\n"; break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);
}

void printMainMenu() {
    cout << "\nMAIN MENU\n";
    cout << "  1. Medicine\n";
    cout << "  2. Warehouse\n";
    cout << "  0. Exit\n";
}

int main() {
    PharmacyWarehouse warehouse("Main Pharmacy Warehouse N1");
    int choice = -1;
    do {
        printMainMenu();
        choice = readInt("Your choice: ");

        switch (choice) {
        case 1: medicineSubMenu(warehouse);   break;
        case 2: warehouseSubMenu(warehouse);  break;
        case 0: cout << "\nExit. Goodbye!\n"; break;
        default: cout << "Invalid choice.\n"; break;
        }
    } while (choice != 0);

    return 0;
}