#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

#include "Medicine.h"
#include "PharmacyWarehouse.h"

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
        cout << "  [!] Oshibka: vvedite tseloe chislo.\n";
        clearInput();
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) { clearInput(); return value; }
        cout << "  [!] Oshibka: vvedite chislo.\n";
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
    cout << "\nMENU: PREPARAT\n";
    cout << "  1. Sozdat novyy preparat\n";
    cout << "  2. Pokazat tekushchiy preparat\n";
    cout << "  3. Izmenit nazvanie\n";
    cout << "  4. Izmenit proizvoditelya\n";
    cout << "  5. Izmenit seriyu\n";
    cout << "  6. Izmenit kategoriyu\n";
    cout << "  7. Izmenit kol-vo upakovok\n";
    cout << "  8. Izmenit temperaturu hraneniya\n";
    cout << "  9. Izmenit srok godnosti\n";
    cout << " 10. Proverit, prosrochen li\n";
    cout << " 11. Otpravit preparat na sklad\n";
    cout << "  0. Nazad\n";
}

void medicineSubMenu(PharmacyWarehouse& warehouse) {
    Medicine med;
    int choice = -1;
    do {
        printMedicineMenu();
        choice = readInt("Vash vybor: ");

        switch (choice) {
        case 1: {
            cout << "\n--- Sozdanie preparata ---\n";
            string name  = readLine("Nazvanie: ");
            string manuf = readLine("Proizvoditel: ");
            string serie = readLine("Seriya: ");
            string exp   = readLine("Srok godnosti (DD.MM.GGGG): ");
            int    packs = readInt("Kolichestvo upakovok: ");
            string cat   = readLine("Kategoriya: ");
            double temp  = readDouble("Temperatura (C): ");
            try {
                med = Medicine(name, manuf, serie, exp, packs, cat, temp);
                cout << "  [OK] Preparat sozdan.\n";
            } catch (const MedicineError& e) { 
                cout << "  OSHIBKA: " << e.what() << "\n";
            }
            break;
        }
        case 2:
            cout << "\n--- Tekushchiy preparat ---\n";
            med.printInfo();
            break;
        case 3: med.setName(readLine("Novoe nazvanie: "));    cout << "  [OK]\n"; break;
        case 4: med.setManufacturer(readLine("Novyy proizvoditel: ")); cout << "  [OK]\n"; break;
        case 5: med.setSeries(readLine("Novaya seriya: "));      cout << "  [OK]\n"; break;
        case 6: med.setCategory(readLine("Novaya kategoriya: "));cout << "  [OK]\n"; break;
        case 7: {
            int p = readInt("Novoe kolichestvo upakovok: ");
            try { med.setPacks(p); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  OSHIBKA: " << e.what() << "\n"; }
            break;
        }
        case 8: {
            double t = readDouble("Novaya temperatura (C): ");
            try { med.setTemperature(t); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  OSHIBKA: " << e.what() << "\n"; }
            break;
        }
        case 9: {
            string exp = readLine("Novyy srok godnosti (DD.MM.GGGG): ");
            try { med.setExpiryDate(exp); cout << "  [OK]\n"; }
            catch (const MedicineError& e) { cout << "  OSHIBKA: " << e.what() << "\n"; }
            break;
        }
        case 10:
            cout << "  Preparat " << (med.isExpired() ? "PROSROCHEN" : "gotov") << "\n";
            break;
        case 11: {
            try {
                warehouse.addMedicine(med);
                cout << "  Preparat otpravlen na sklad: " << warehouse.getName() << "\n";
            } catch (const MedicineError& e) {
                cout << "  OSHIBKA: " << e.what() << "\n";
            }
            break;
        }
        case 0: cout << "  Vozvrat v menu.\n"; break;
        default: cout << "  [!] Nevernyy punkt.\n"; break;
        }
    } while (choice != 0);
}

void printWarehouseMenu() {
    cout << "\nMENU: SKLAD\n";
    cout << "  1. Pokazat vse preparaty\n";
    cout << "  2. Nayti preparat po nazvaniyu\n";
    cout << "  3. Postavka preparata\n";
    cout << "  4. Vydacha preparata\n";
    cout << "  5. Utilizirovat prosrochennye partii\n";
    cout << "  6. Pokazat tolko prosrochennye\n";
    cout << "  7. Pokazat tolko dostupnye k vydache\n";
    cout << "  0. Nazad\n";
}

void warehouseSubMenu(PharmacyWarehouse& warehouse) {
    int choice = -1;
    do {
        printWarehouseMenu();
        choice = readInt("Vash vybor: ");

        switch (choice) {
        case 1:
            warehouse.printStockReport();
            break;
        case 2: {
            cout << "\n--- Poisk ---\n";
            string name = readLine("Nazvanie: ");
            
            if (const Medicine* m = warehouse.findByName(name)) { 
                m->printInfo();
            } else {
                cout << "  Preparat ne nayden.\n";
            }
            break;
        }
        case 3: {
            cout << "\n--- Prihod ---\n";
            string name = readLine("Nazvanie: ");
            int    cnt  = readInt("Kolichestvo dlya prihoda: ");
            try {
                warehouse.receiveMedicine(name, cnt);
                if (const Medicine* m = warehouse.findByName(name)) {
                    cout << "  [OK] Ostatok: " << m->getPacks() << "\n";
                }
            } catch (const MedicineError& e) { cout << "  OSHIBKA: " << e.what() << "\n"; }
            break;
        }
        case 4: {
            cout << "\n--- Vydacha ---\n";
            string name = readLine("Nazvanie: ");
            int    cnt  = readInt("Kolichestvo k vydache: ");
            try {
                warehouse.issueMedicine(name, cnt);
                if (const Medicine* m = warehouse.findByName(name)) {
                    cout << "  [OK] Ostatok: " << m->getPacks() << "\n";
                }
            } catch (const MedicineError& e) { cout << "  OSHIBKA: " << e.what() << "\n"; }
            break;
        }
        case 5: {
            cout << "\n--- Utilizatsiya prosrochennyh ---\n";
            int removed = warehouse.removeExpired();
            cout << "  Utilizirovano pozitsiy: " << removed << "\n";
            break;
        }
        case 6: warehouse.printExpired();   break;
        case 7: warehouse.printAvailable(); break;
        case 0: cout << "  Vozvrat v menu.\n"; break;
        default: cout << "  [!] Nevernyy punkt.\n"; break;
        }
    } while (choice != 0);
}

void printMainMenu() {
    cout << "\nMENU\n";
    cout << "  1. Preparat\n";
    cout << "  2. Sklad\n";
    cout << "  0. Vyhod\n";
}

int main() {
    PharmacyWarehouse warehouse("Glavnyy aptechnyy sklad N1");
    int choice = -1;
    do {
        printMainMenu();
        choice = readInt("Vash vybor: ");

        switch (choice) {
        case 1: medicineSubMenu(warehouse);   break;
        case 2: warehouseSubMenu(warehouse);  break;
        case 0: cout << "\nVyhod. Do svidaniya!\n"; break;
        default: cout << "  [!] Nevernyy punkt.\n"; break;
        }
    } while (choice != 0);

    return 0;
}