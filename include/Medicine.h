#pragma once

#include <string>
#include <string_view>
#include <stdexcept>
#include <iostream> // [ЛАБА 2] Для перегрузки ввода/вывода

class MedicineError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error; 
};

class Medicine {
private:
    std::string name;
    std::string manufacturer;
    std::string series;
    std::string expiryDate;
    int         packs;
    std::string category;
    double      temperature;

public:
    Medicine();
    Medicine(std::string_view n, std::string_view m, std::string_view s,
             std::string_view exp, int p, std::string_view cat, double temp);

    std::string getName() const;
    std::string getManufacturer() const;
    std::string getSeries() const;
    std::string getExpiryDate() const;
    int getPacks() const;
    std::string getCategory() const;
    double getTemperature() const;

    void setName(std::string_view n);
    void setManufacturer(std::string_view m);
    void setSeries(std::string_view s);
    void setCategory(std::string_view c);
    void setPacks(int p);
    void setExpiryDate(std::string_view exp);
    void setTemperature(double t);

    bool isExpired() const;
    bool issue(int count);
    void receive(int count);
    void printInfo() const;

    bool operator==(const Medicine& other) const; 
    bool operator<(const Medicine& other) const;  
    bool operator>(const Medicine& other) const;  

    friend std::ostream& operator<<(std::ostream& os, const Medicine& med);
    friend std::istream& operator>>(std::istream& is, Medicine& med);
    friend bool hasSpecialStorage(const Medicine& med);
};