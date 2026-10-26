#include "Medicine.h"
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
        throw MedicineError("Nevernyy format daty: " + string(d));
    int day   = stoi(string(d.substr(0, 2)));
    int month = stoi(string(d.substr(3, 2)));
    int year  = stoi(string(d.substr(6, 4)));
    return year * 10000L + month * 100L + day;
}

Medicine::Medicine()
    : name("Neizvestno"), manufacturer("Neizvestno"),
      series("0000"), expiryDate("01.01.2000"),
      packs(0), category("obychnyy"), temperature(20.0) {}

Medicine::Medicine(string_view n, string_view m, string_view s,
                   string_view exp, int p, string_view cat, double temp)
    : name(n), manufacturer(m), series(s), expiryDate(exp),
      packs(p), category(cat), temperature(temp)
{
    if (p < 0)
        throw MedicineError("Kolichestvo upakovok ne mozhet byt otritsatelnym");
    if (temp < -30.0 || temp > 30.0)
        throw MedicineError("Nedopustimaya temperatura hraneniya");
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
        throw MedicineError("Kolichestvo upakovok ne mozhet byt otritsatelnym");
    packs = p;
}

void Medicine::setExpiryDate(string_view exp) {
    dateToKey(exp);
    expiryDate = exp;
}

void Medicine::setTemperature(double t) {
    if (t < -30.0 || t > 30.0)
        throw MedicineError("Temperatura dolzhna byt v diapazone [-30; 30]");
    temperature = t;
}

bool Medicine::isExpired() const {
    return dateToKey(expiryDate) < dateToKey(currentDate());
}

bool Medicine::issue(int count) {
    if (isExpired())
        throw MedicineError("Nelzya vydat prosrochennyy preparat: " + name);
    if (count <= 0)
        throw MedicineError("Kolichestvo vydachi dolzhno byt > 0");
    if (count > packs)
        throw MedicineError("Nedostatochno upakovok na sklade: " + name);
    packs -= count;
    return true;
}

void Medicine::receive(int count) {
    if (count <= 0)
        throw MedicineError("Kolichestvo prihoda dolzhno byt > 0");
    packs += count;
}

void Medicine::printInfo() const {
    cout << "  Nazvanie:       " << name << "\n";
    cout << "  Proizvoditel:   " << manufacturer << "\n";
    cout << "  Seriya:         " << series << "\n";
    cout << "  Srok godnosti:  " << expiryDate
         << (isExpired() ? "  [PROSROCHEN]" : "  [gotov]") << "\n";
    cout << "  Kategoriya:     " << category << "\n";
    cout << "  Temp. hraneniya:" << temperature << " C\n";
    cout << "  Upakovok:       " << packs << "\n";
}