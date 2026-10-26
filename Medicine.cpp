#include "Medicine.h"
#include <iostream>
#include <stdexcept>
#include <ctime>
#include <cstdio>

using namespace std;

// Вспомогательные функции для работы с датами
static string currentDate() {
    time_t t = time(nullptr);
    struct tm now;
    
    // Используем localtime_s напрямую (для Windows)
    localtime_s(&now, &t);
    
    char buf[11];
    snprintf(buf, sizeof(buf), "%02d.%02d.%04d",
             now.tm_mday, now.tm_mon + 1, now.tm_year + 1900);
    return string(buf);
}

static long dateToKey(const string& d) {
    if (d.size() != 10 || d[2] != '.' || d[5] != '.')
        throw invalid_argument("Nevernyy format daty: " + d);
    int day   = stoi(d.substr(0, 2));
    int month = stoi(d.substr(3, 2));
    int year  = stoi(d.substr(6, 4));
    return year * 10000L + month * 100L + day;
}

// Конструкторы
Medicine::Medicine()
    : name("Neizvestno"), manufacturer("Neizvestno"),
      series("0000"), expiryDate("01.01.2000"),
      packs(0), category("obychnyy"), temperature(20.0) {}

Medicine::Medicine(const string& n, const string& m, const string& s,
                   const string& exp, int p, const string& cat, double temp)
    : name(n), manufacturer(m), series(s), expiryDate(exp),
      packs(p), category(cat), temperature(temp)
{
    if (p < 0)
        throw invalid_argument("Kolichestvo upakovok ne mozhet byt otritsatelnym");
    if (temp < -30.0 || temp > 30.0)
        throw invalid_argument("Nedopustimaya temperatura hraneniya");
    dateToKey(exp); 
}

// Геттеры
string Medicine::getName() const { return name; }
string Medicine::getManufacturer() const { return manufacturer; }
string Medicine::getSeries() const { return series; }
string Medicine::getExpiryDate() const { return expiryDate; }
int Medicine::getPacks() const { return packs; }
string Medicine::getCategory() const { return category; }
double Medicine::getTemperature() const { return temperature; }

// Сеттеры
void Medicine::setName(const string& n) { name = n; }
void Medicine::setManufacturer(const string& m) { manufacturer = m; }
void Medicine::setSeries(const string& s) { series = s; }
void Medicine::setCategory(const string& c) { category = c; }

void Medicine::setPacks(int p) {
    if (p < 0)
        throw invalid_argument("Kolichestvo upakovok ne mozhet byt otritsatelnym");
    packs = p;
}

void Medicine::setExpiryDate(const string& exp) {
    dateToKey(exp);
    expiryDate = exp;
}

void Medicine::setTemperature(double t) {
    if (t < -30.0 || t > 30.0)
        throw invalid_argument("Temperatura dolzhna byt v diapazone [-30; 30]");
    temperature = t;
}

// Операции
bool Medicine::isExpired() const {
    return dateToKey(expiryDate) < dateToKey(currentDate());
}

bool Medicine::issue(int count) {
    if (isExpired())
        throw runtime_error("Nelzya vydat prosrochennyy preparat: " + name);
    if (count <= 0)
        throw invalid_argument("Kolichestvo vydachi dolzhno byt > 0");
    if (count > packs)
        throw runtime_error("Nedostatochno upakovok na sklade: " + name);
    packs -= count;
    return true;
}

void Medicine::receive(int count) {
    if (count <= 0)
        throw invalid_argument("Kolichestvo prihoda dolzhno byt > 0");
    packs += count;
}

// Вывод
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