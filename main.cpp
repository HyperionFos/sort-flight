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

int main() {
    std::vector<Flight> flights = readFlights("data/input.csv");
    std::cout << "read " << flights.size() << " flights\n";

    for (const Flight& f : flights) {
        std::cout << f.flightNumber << " | " << f.airline << " | "
                << f.arrivalDate << " | " << f.arrivalTime << " | "
                << f.passangers << "\n";
    }

    writeFlights("data/output.csv", flights);
    std::cout << "wrote data/output.csv\n";
    return 0;
}