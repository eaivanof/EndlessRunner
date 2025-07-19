#pragma once
#include <string>
#include <vector>

class CarSkin {
public:
    CarSkin(const std::string& name, const std::string& imagePath);

    std::string getName() const;
    std::string getImagePath() const;

    static const std::vector<CarSkin>& getAllSkins();

private:
    std::string name;
    std::string imagePath;
};