#pragma once

#include <string>
#include <string_view>
#include <vector>
#include "Medicine.h"

class PharmacyWarehouse {
private:
    std::string warehouseName;
    std::vector<Medicine> medicines;
    static const int MAX_CAPACITY = 100;

public:
    // Sonar требует string_view
    explicit PharmacyWarehouse(std::string_view name);

    std::string getName() const;
    int getCount() const;

    void addMedicine(const Medicine& m);
    Medicine* findByName(std::string_view n);

    bool issueMedicine(std::string_view n, int count);
    void receiveMedicine(std::string_view n, int count);
    int removeExpired();

    void printStockReport() const;
    void printExpired() const;
    void printAvailable() const;
};