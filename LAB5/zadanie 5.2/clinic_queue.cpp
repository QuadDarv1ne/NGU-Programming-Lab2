/**
 * Программа "Электронная очередь в поликлинике"
 * 
 * Обрабатывает команды:
 *   ENQUEUE <время> - добавить посетителя в очередь
 *   DISTRIBUTE      - распределить очередь по окнам
 * 
 * Алгоритм:
 *   1. Посетители сортируются по убыванию времени обслуживания
 *   2. Каждый посетитель направляется в наименее загруженное окно
 *   3. Внутри окна посетители сортируются по порядку добавления
 * 
 * Формат талона: TXXX (XXX - номер с ведущими нулями для номеров <1000)
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace std;

struct Visitor {
    string ticket;
    int time;
    int order;
};

bool compareByTimeDesc(const Visitor &a, const Visitor &b) {
    if (a.time == b.time) 
        return a.order < b.order;
    return a.time > b.time;
}

bool compareByOrder(const Visitor &a, const Visitor &b) {
    return a.order < b.order;
}

int main() {
    int windowsCount;
    cout << ">>> Введите кол-во окон" << endl;
    cin >> windowsCount;

    vector<Visitor> visitors;
    string command;
    int orderCounter = 0;

    while (true) {
        cin >> command;
        if (command == "ENQUEUE") {
            int time;
            cin >> time;
            stringstream ticketStream;
            if (orderCounter < 1000) {
                ticketStream << "T" << setw(3) << setfill('0') << orderCounter;
            } else {
                ticketStream << "T" << orderCounter;
            }
            visitors.push_back({ticketStream.str(), time, orderCounter});
            orderCounter++;
            cout << ">>> " << ticketStream.str() << endl;
        } else if (command == "DISTRIBUTE") {
            break;
        }
    }

    vector<vector<Visitor>> windows(windowsCount);
    vector<int> windowTime(windowsCount, 0);
    
    sort(visitors.begin(), visitors.end(), compareByTimeDesc);
    
    for (const auto& v : visitors) {
        int minIndex = 0;
        int minTime = windowTime[0];
        for (int i = 1; i < windowsCount; i++) {
            if (windowTime[i] < minTime) {
                minTime = windowTime[i];
                minIndex = i;
            }
        }
        windows[minIndex].push_back(v);
        windowTime[minIndex] += v.time;
    }
    
    for (int i = 0; i < windowsCount; i++) {
        sort(windows[i].begin(), windows[i].end(), compareByOrder);
    }
    
    cout << ">>> ";
    for (int i = 0; i < windowsCount; i++) {
        cout << "Окно " << i+1 << " (" << windowTime[i] << " минут): ";
        for (size_t j = 0; j < windows[i].size(); j++) {
            cout << windows[i][j].ticket;
            if (j < windows[i].size() - 1) 
                cout << ", ";
        }
        if (i < windowsCount - 1) 
            cout << " >>> ";
    }
    cout << endl;

    return 0;
}
