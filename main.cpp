#include <iostream>


struct Flight{
    std::string flightNumber;
    std::string airline;
    std::string arrivalDate;
    std::string arrivalTime;
    int passangers = 0;

    int compare(const Flight& other) const {
        if (arrivalDate < other.arrivalDate) return -1;
        if (arrivalDate > other.arrivalDate) return +1;

        if (arrivalTime < other.arrivalTime) return -1;
        if (arrivalTime > other.arrivalTime) return +1;

        if (airline < other.airline) return -1;
        if (airline > other.airline) return +1;

        if (passangers > other.passangers) return -1;
        if (passangers < other.passangers) return +1;

        return 0;
    }

    bool operator<(const Flight& o) const { return compare(o) < 0; }
    bool operator>(const Flight& o)  const { return compare(o) >  0; }
    bool operator<=(const Flight& o) const { return compare(o) <= 0; }
    bool operator>=(const Flight& o) const { return compare(o) >= 0; }
};

int main() {
    Flight a{"SU100", "Aeroflot", "2024-03-15", "08:30", 180};
    Flight b{"SU200", "Aeroflot", "2024-03-15", "08:30", 200};
    Flight c{"S7300",  "S7",      "2024-03-16", "06:00", 150};

    // a и b: всё совпадает, кроме пассажиров. У b их больше -> b раньше.
    // Значит a > b
    std::cout << "a < b  : " << (a < b)  << "  (expected 0)\n";
    std::cout << "a > b  : " << (a > b)  << "  (expected 1)\n";

    // c прилетает 16-го, a и b -- 15-го. Дата важнее всего -> c позже.
    std::cout << "a < c  : " << (a < c)  << "  (expected 1)\n";
    std::cout << "c < a  : " << (c < a)  << "  (expected 0)\n";

    // равенство с самим собой
    std::cout << "a <= a : " << (a <= a) << "  (expected 1)\n";
    std::cout << "a <  a : " << (a < a)  << "  (expected 0)\n";
    return 0;
}