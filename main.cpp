#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

/**
 * @brief Структура единичное прилета самолета
 * 
 * Все поля хранятся в формате string, дата и время используют
 * фиксированный по ширине формат "ГГГГ-ММ-ДД" и "ЧЧ:ММ" соответственно,
 * чтобы при сравнении не было ошибок т.к. 3:20 < 11:20, но 3 > 1 (сравниваются первые символы)
 * 
 */

struct Flight{
    std::string flightNumber;
    std::string airline;
    std::string arrivalDate;
    std::string arrivalTime;
    int passengers = 0;

    int compare(const Flight& other) const {
        if (arrivalDate < other.arrivalDate) return -1;
        if (arrivalDate > other.arrivalDate) return +1;

        if (arrivalTime < other.arrivalTime) return -1;
        if (arrivalTime > other.arrivalTime) return +1;

        if (airline < other.airline) return -1;
        if (airline > other.airline) return +1;

        if (passengers > other.passengers) return -1;
        if (passengers < other.passengers) return +1;

        return 0;
    }

    bool operator<(const Flight& o) const { return compare(o) < 0; }
    bool operator>(const Flight& o)  const { return compare(o) >  0; }
    bool operator<=(const Flight& o) const { return compare(o) <= 0; }
    bool operator>=(const Flight& o) const { return compare(o) >= 0; }
};

std::vector<Flight> generateFlights(size_t n, unsigned seed){
    std::mt19937 rng(seed);
    std::vector<std::string> airlines = {
        "Aeroflot", "S7", "Pobeda", "UralAirlines", "Utair", "RedWings", "Azimuth"
    };
    std::uniform_int_distribution<int> airlineIdx(0, (int)airlines.size() - 1);
    std::uniform_int_distribution<int> day(1, 28);
    std::uniform_int_distribution<int> month(1, 12);
    std::uniform_int_distribution<int> hour(0, 23);
    std::uniform_int_distribution<int> minute(0, 59);
    std::uniform_int_distribution<int> passengers(50, 350);
    std::uniform_int_distribution<int> flightNo(100, 9999);

    std::vector<Flight> flights;
    flights.reserve(n);

    for (size_t i = 0; i < n; ++i){
        Flight f;
        f.airline = airlines[airlineIdx(rng)];
        f.flightNumber = f.airline.substr(0, 2) + std::to_string(flightNo(rng));

        std::ostringstream date;
        date << "2026-"
             << std::setw(2) << std::setfill('0') << month(rng) << "-"
             << std::setw(2) << std::setfill('0') << day(rng);
        f.arrivalDate = date.str();

        std::ostringstream time;
        time << std::setw(2) << std::setfill('0') << hour(rng) << ":"
             << std::setw(2) << std::setfill('0') << minute(rng);
        f.arrivalTime = time.str();

        f.passengers = passengers(rng);
        flights.push_back(f);
    }
    return flights;
}

double measure(const std::vector<Flight>& source,
               void (*sortFunc)(std::vector<Flight>&),
               int runs = 3) {
    double total = 0.0;
    for (int r = 0; r < runs; ++r) {
        std::vector<Flight> data = source;
        auto t1 = std::chrono::high_resolution_clock::now();
        sortFunc(data);
        auto t2 = std::chrono::high_resolution_clock::now();
        total += std::chrono::duration<double, std::milli>(t2 - t1).count();
    }
    return total / runs;
}

// std::sort немного изменненый чтобы measure мог принять его

void stdSort(std::vector<Flight>& a){
    std::sort(a.begin(), a.end());
}

// Пузырёк
/**
 * @brief Сортирует вектор полетов пузырьком с флагом на ранний выход
 *
 * Проходит по парам элементов в массиве и меняет соседние элементы,
 * если они не отсоритрованы. Каждый цикл прохода уменьшает неотсортированный массив на один элемент,
 * т.к. самый большой элемент оказывается в конце цикла. Если посде цилка ни один элемент не меняется,
 * то массив уже отсортирован и мы заканчиваем.
 *
 * Сложность: O(n^2) в худшем случае и в среднем, O(n) в лучшем случае (если уже отсортирован).
 *
 * @param a Вектор из полетов, который нужно отсортировать.
 */

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

// Шейкер
/**
 * @brief Сортирует вектор полетов шейкером с флагом на ранний выход
 *
 * Проходит по парам элементов в массиве с двух сторон и меняет соседние элементы,
 * если они не отсоритрованы. Каждый цикл прохода уменьшает неотсортированный массив на два элемента,
 * т.к. самый большой и самый маленький элементы оказываются в концах цикла. Если после цилка ни один элемент не меняется,
 * то массив уже отсортирован и мы заканчиваем.
 *
 * Сложность: O(n^2) в худшем случае и в среднем, O(n) в лучшем случае (если уже отсортирован).
 *
 * @param a Вектор из полетов, который нужно отсортировать.
 */

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

// Быстрая сортировка и все вспомогательные функции для нее
// Среднее из трех
/**
 * @brief Находит pivot сортирует 3 элемента (на первой позиции меньший элемент из трех,
 * на pivot средний из трех, на последней позиции самый большой из трех)
 * 
 * @param a Вектор из полетов, где ищем средний индекс.
 * @param lo нижняя граница которую мы выбираем
 * @param hi верхняя граница которую мы выбираем
 * @return возвращает индекс pivot
 */
size_t medianOfThree(std::vector<Flight>& a, size_t lo, size_t hi){
    size_t mid = lo + (hi - lo) / 2;
    if (a[mid] < a[lo]) std::swap(a[lo], a[mid]);
    if (a[hi] < a[lo]) std::swap(a[lo], a[hi]);
    if (a[hi] < a[mid]) std::swap(a[mid], a[hi]);
    return mid;
}
// Разбиение на меньшие массивы
/**
 * @brief Разбиение Хоара для быстрой сортировки
 * 
 * Использует функцию medianOfThree чтобы выбрать значение pivot,
 * затем двиагется с двух сторон к элементу pivot, меняя элементы если они не на месте.
 * После исполнения возвращает индекс разделения: все элементы в [lo...значения которое он вернет] <= значение pivot,
 * все элементы в [граничное значение+1...hi] >= значения pivot.
 * Граничное значение не обязательно будет на месте pivot.
 * 
 * @param a Вектор из полетов, который будем разбивать на два меньших массива.
 * @param lo нижняя граница которую мы выбираем
 * @param hi верхняя граница которую мы выбираем
 * @return Граничний индекс разделения
 */

size_t partition(std::vector<Flight>& a, size_t lo, size_t hi) {
    size_t pivotIndex = medianOfThree(a, lo, hi);
    Flight pivot = a[pivotIndex];

    size_t i = lo;
    size_t j = hi;

    while (true) {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;

        if (i >= j) return j;

        std::swap(a[i], a[j]);
        ++i;
        --j;
    }
}
// Исполнение быстрой сортировки с задаваемыми границами
/**
 * @brief Выполняет быструю сортировку с принимаемыми границами,
 * основная рекурсиваная функция которая и сортирует массив при помощи
 * partition и medianOfThreee
 * 
 * @param a Вектор из полетов
 * @param lo нижняя граница которую мы выбираем
 * @param hi верхняя граница которую мы выбираем
 */

void quickSortImpl(std::vector<Flight>& a, size_t lo, size_t hi) {
    if (lo >= hi) return;
    size_t p = partition(a, lo, hi);
    quickSortImpl(a, lo, p);
    quickSortImpl(a, p + 1, hi);
}
//Вызов функции quickSort чтобы всегда не выбирать начальные значения
void quickSort(std::vector<Flight>& a) {
    if (a.size() < 2) return;
    quickSortImpl(a, 0, a.size() - 1);
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
        f.passengers = std::stoi(token);

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

    out << "flightNumber,airline,arrivalDate,arrivalTime,passengers\n";
    for (const Flight& f : flights) {
        out << f.flightNumber << ','
            << f.airline << ','
            << f.arrivalDate << ','
            << f.arrivalTime << ','
            << f.passengers << '\n';
    }
}

void printFlights(const std::string& title, const std::vector<Flight>& flights) {
    std::cout << "===== " << title << " =====\n";
    for (const Flight& f : flights) {
        std::cout << f.flightNumber << " | " << f.airline << " | "
                << f.arrivalDate << " | " << f.arrivalTime << " | "
                << f.passengers << "\n";
    }
}

int main() {
    std::vector<size_t> sizes = {
        100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000, 100000
    };

    std::ofstream out("data/timings.csv");
    out << "size,algorithm,time_ms\n";

    for (size_t n : sizes) {
        std::cout << "size = " << n << "\n";
        std::vector<Flight> source = generateFlights(n, 52);

        double t_bubble = (n <= 50000) ? measure(source, bubbleSort) : -1.0;
        double t_shaker = (n <= 50000) ? measure(source, shakerSort) : -1.0;
        double t_quick = measure(source, quickSort);
        double t_std = measure(source, stdSort);

        if (t_bubble >= 0) std::cout << "  bubble: " << t_bubble << " ms\n";
        if (t_shaker >= 0) std::cout << "  shaker: " << t_shaker << " ms\n";
        std::cout << "  quick : " << t_quick << " ms\n";
        std::cout << "  std   : " << t_std << " ms\n";

        if (t_bubble >= 0) out << n << ",bubble," << t_bubble << "\n";
        if (t_shaker >= 0) out << n << ",shaker," << t_shaker << "\n";
        out << n << ",quick," << t_quick << "\n";
        out << n << ",std,"   << t_std   << "\n";
    }

    std::cout << "\nwrote data/timings.csv\n";
    return 0;
}