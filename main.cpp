#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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

void bubbleSort(std::vector<Flight>& a) {
    if (a.size() < 2) return;

    size_t n = a.size();
    for (size_t i = 0; i < n-1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - 1 - i; ++j) {
            if (a[j] > a[j+1]) {
                std::swap(a[j], a[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    } 
}

void shakerSort(std::vector<Flight>& a) {
    if (a.size() < 2) return;

    size_t left = 0;
    size_t right = a.size() - 1;
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (size_t j = left; j < right; ++j) {
            if (a[j] > a[j+1]) {
                std::swap(a[j], a[j+1]);
                swapped = true;
            }
        }
        -- right;
        if (!swapped) break;
        swapped = false;
        for (size_t j = right; j > left; --j) {
            if (a[j - 1] > a[j]) {
                std::swap(a[j-1], a[j]);
                swapped = true;
            }
        }
        ++ left;
    }
}

std::vector<Flight> readFlights(const std::string& path){
    std::vector<Flight> flights;
    std::ifstream in(path);
    if (!in){
        std::cerr << "Error: cannont open file " << path << "\n";
        return flights;
    }
    std::string line;
    std::getline(in, line);
    while (std::getline(in, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        Flight f;
        std::string token;
        std::getline(ss, f.flightNumber, ',');
        std::getline(ss, f.airline, ',');
        std::getline(ss, f.arrivalDate, ',');
        std::getline(ss, f.arrivalTime, ',');
        std::getline(ss, token, ',');
        f.passangers = std::stoi(token);

        flights.push_back(f);
    }
    return flights;
}

void writeFlights(const std::string& path, const std::vector<Flight>& flights) {
    std::ofstream out(path);
    if (!out) {
        std::cerr << "Error: cannot write file " << path << "\n";
        return;
    }

    out << "flightNumber,airline,arrivalDate,arrivalTime,passangers\n";
    for (const Flight& f : flights) {
        out << f.flightNumber << ','
            << f.airline << ','
            << f.arrivalDate << ','
            << f.arrivalTime << ','
            << f.passangers << '\n';
    }
}

void printFlights(const std::string& title, const std::vector<Flight>& flights) {
    std::cout << "===== " << title << " =====\n";
    for (const Flight& f : flights) {
        std::cout << f.flightNumber << " | " << f.airline << " | "
                << f.arrivalDate << " | " << f.arrivalTime << " | "
                << f.passangers << "\n";
    }
}

int main() {
    std::vector<Flight> flights = readFlights("data/input.csv");
    std::cout << "read " << flights.size() << " flights\n";

    printFlights("before sort", flights);
    
    std::vector<Flight> a = flights;
    bubbleSort(a);
    printFlights("after bubble sort", a);

    std::vector<Flight> b = flights;
    shakerSort(b);
    printFlights("after shaker sort", b);

    writeFlights("data/output.csv", flights);
    std::cout << "wrote data/output.csv\n";
    return 0;
}