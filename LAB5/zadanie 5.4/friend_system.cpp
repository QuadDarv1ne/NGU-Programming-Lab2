/**
 * Программа "Система друзей"
 * 
 * Реализует автоматизированную систему учета дружеских связей:
 * - Добавление дружбы между пользователями
 * - Подсчет количества друзей
 * - Проверка наличия дружбы
 * 
 * Основные структуры данных:
 *   map<string, set<string>> friends - хранит для каждого пользователя множество его друзей
 * 
 * Поддерживаемые команды:
 *   FRIENDS i j   - Добавляет дружбу между i и j
 *   COUNT i       - Возвращает количество друзей пользователя i
 *   QUESTION i j  - Проверяет, являются ли i и j друзьями (YES/NO)
 * 
 * Особенности реализации:
 *   - Использует set для гарантии уникальности связей
 *   - Автоматически создает записи для новых пользователей
 *   - Обрабатывает дублирующиеся добавления дружбы
 *   - Эффективная проверка дружбы за O(log n)
 */

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;  // Количество запросов
    
    // Хранилище данных: для каждого пользователя - множество его друзей
    map<string, set<string>> friends;
    vector<string> output;  // Буфер для результатов
    
    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;
        
        if (command == "FRIENDS") {
            string person1, person2;
            cin >> person1 >> person2;
            
            // Добавление двусторонней дружбы
            friends[person1].insert(person2);
            friends[person2].insert(person1);
        }
        else if (command == "COUNT") {
            string person;
            cin >> person;
            
            // Возврат количества друзей (0 если пользователь не найден)
            output.push_back(friends.count(person) ? to_string(friends[person].size()) : "0");
        }
        else if (command == "QUESTION") {
            string person1, person2;
            cin >> person1 >> person2;
            
            // Проверка наличия дружбы
            if (friends.count(person1) && friends[person1].count(person2)) {
                output.push_back("YES");
            } else {
                output.push_back("NO");
            }
        }
    }
    
    // Вывод результатов
    for (const auto& res : output) {
        cout << res << endl;
    }
    
    return 0;
}
