#include "..\head\CarSkin.h"

CarSkin::CarSkin(const std::string& name, const std::string& imagePath)
    : name(name), imagePath(imagePath) {}

std::string CarSkin::getName() const {
    return name;
}

std::string CarSkin::getImagePath() const {
    return imagePath;
}

const std::vector<CarSkin>& CarSkin::getAllSkins() {
    static std::vector<CarSkin> skins = {
        CarSkin("1", "img/car1.bmp"),
        CarSkin("2", "img/car2.bmp"),
        CarSkin("3", "img/car3.bmp")
    };
    return skins;
}