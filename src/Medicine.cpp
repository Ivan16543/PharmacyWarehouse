#include "../include/Medicine.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

static string currentDate() {
    auto now = chrono::system_clock::now();
    time_t t = chrono::system_clock::to_time_t(now);
    struct tm tm_buf;
    localtime_s(&tm_buf, &t);

    ostringstream oss;
    oss << put_time(&tm_buf, "%d.%m.%Y");
    return oss.str();
}

static long dateToKey(string_view d) {
    if (d.size() != 10 || d[2] != '.' || d[5] != '.')
        throw MedicineError("Invalid date format: " + string(d));
    int day   = stoi(string(d.substr(0, 2)));
    int month = stoi(string(d.substr(3, 2)));
    int year  = stoi(string(d.substr(6, 4)));
    return year * 10000L + month * 100L + day;
}

Medicine::Medicine()
    : name("Unknown"), manufacturer("Unknown"),
      series("0000"), expiryDate("01.01.2000"),
      packs(0), category("regular"), temperature(20.0) {}

Medicine::Medicine(string_view n, string_view m, string_view s,
                   string_view exp, int p, string_view cat, double temp)
    : name(n), manufacturer(m), series(s), expiryDate(exp),
      packs(p), category(cat), temperature(temp)
{
    if (p < 0)
        throw MedicineError("Number of packs cannot be negative");
    if (temp < -30.0 || temp > 30.0)
        throw MedicineError("Invalid storage temperature");
    dateToKey(exp); 
}

string Medicine::getName() const { return name; }
string Medicine::getManufacturer() const { return manufacturer; }
string Medicine::getSeries() const { return series; }
string Medicine::getExpiryDate() const { return expiryDate; }
int Medicine::getPacks() const { return packs; }
string Medicine::getCategory() const { return category; }
double Medicine::getTemperature() const { return temperature; }

void Medicine::setName(string_view n) { name = n; }
void Medicine::setManufacturer(string_view m) { manufacturer = m; }
void Medicine::setSeries(string_view s) { series = s; }
void Medicine::setCategory(string_view c) { category = c; }

void Medicine::setPacks(int p) {
    if (p < 0)
        throw MedicineError("Number of packs cannot be negative");
    packs = p;
}

void Medicine::setExpiryDate(string_view exp) {
    dateToKey(exp);
    expiryDate = exp;
}

void Medicine::setTemperature(double t) {
    if (t < -30.0 || t > 30.0)
        throw MedicineError("Temperature must be in range [-30; 30]");
    temperature = t;
}

bool Medicine::isExpired() const {
    return dateToKey(expiryDate) < dateToKey(currentDate());
}

bool Medicine::issue(int count) {
    if (isExpired())
        throw MedicineError("Cannot issue expired medicine: " + name);
    if (count <= 0)
        throw MedicineError("Issue count must be > 0");
    if (count > packs)
        throw MedicineError("Not enough packs in stock: " + name);
    packs -= count;
    return true;
}

void Medicine::receive(int count) {
    if (count <= 0)
        throw MedicineError("Receive count must be > 0");
    packs += count;
}

void Medicine::printInfo() const {
    cout << "  Name:           " << name << "\n";
    cout << "  Manufacturer:   " << manufacturer << "\n";
    cout << "  Series:         " << series << "\n";
    cout << "  Expiry Date:    " << expiryDate
         << (isExpired() ? "  [EXPIRED]" : "  [VALID]") << "\n";
    cout << "  Category:       " << category << "\n";
    cout << "  Storage Temp:   " << temperature << " C\n";
    cout << "  Packs:          " << packs << "\n";
}

bool Medicine::operator==(const Medicine& other) const {
    return series == other.series;
}

bool Medicine::operator<(const Medicine& other) const {
    return packs < other.packs;
}

bool Medicine::operator>(const Medicine& other) const {
    return packs > other.packs;
}

ostream& operator<<(ostream& os, const Medicine& med) {
    os << "Medicine: " << med.name << " (" << med.category << ")\n"
       << " Series: " << med.series << "\n"
       << " Expiry: " << med.expiryDate << "\n"
       << " Packs: " << med.packs;
    return os;
}

istream& operator>>(istream& is, Medicine& med) {
    cout << "Name: "; getline(is >> ws, med.name);
    cout << "Manufacturer: "; getline(is >> ws, med.manufacturer);
    cout << "Series: "; getline(is >> ws, med.series);
    cout << "Expiry Date (DD.MM.YYYY): "; getline(is >> ws, med.expiryDate);
    cout << "Number of packs: "; is >> med.packs;
    cout << "Category: "; getline(is >> ws, med.category);
    cout << "Storage temperature (C): "; is >> med.temperature;
    return is;
}

bool hasSpecialStorage(const Medicine& med) {
    return med.temperature < 10.0;
}