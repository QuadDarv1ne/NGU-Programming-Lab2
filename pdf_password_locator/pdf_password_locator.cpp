/**
 * # Установка зависимостей (Ubuntu)
 * sudo apt-get install libpoppler-cpp-dev pkg-config
 * 
 * # Компиляция
 * 
 * g++ -std=c++11 -O3 -o pdf_password_finder pdf_password_locator.cpp $(pkg-config --cflags --libs poppler-cpp)
 * 
 * g++ -std=c++11 -O3 -o pdf_password_finder pdf_finder.cpp $(pkg-config --cflags --libs poppler-cpp)
 * 
 * # Запуск
 * ./pdf_password_finder
 */

#include <iostream>
#include <vector>
#include <poppler/cpp/poppler-document.h>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <memory>

using namespace std;
using namespace std::chrono;

int main() {
    const vector<string> charset = {
        "a", "A", 
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
    };
    
    const int MAX_LENGTH = 4;
    const string pdf_path = "217.pdf";

    // Чтение файла в память для ускорения работы
    ifstream file(pdf_path, ios::binary);
    vector<char> buffer(istreambuf_iterator<char>(file), {});
    file.close();

    auto start_time = high_resolution_clock::now();
    size_t total_attempts = 0;
    auto last_update = start_time;

    // Основной цикл перебора паролей
    for (int len = 1; len <= MAX_LENGTH; ++len) {
        vector<size_t> indices(len, 0);
        
        do {
            // Сборка пароля
            string password;
            for (int i = 0; i < len; ++i) {
                password += charset[indices[i]];
            }
            
            // Загрузка документа для каждой попытки
            auto doc = poppler::document::load_from_raw_data(
                buffer.data(), buffer.size()
            );
            
            if (!doc) {
                cerr << "Ошибка загрузки PDF" << endl;
                continue;
            }
            
            // Проверка статуса блокировки
            if (!doc->is_locked()) {
                cout << "\n\nДокумент не защищен паролем!" << endl;
                return 0;
            }

            // Попытка разблокировки
            bool unlocked = false;
            try {
                unlocked = doc->unlock("", password) || doc->unlock(password, "");
            } catch (...) {
                unlocked = false;
            }
            
            if (unlocked && doc->is_locked()) {
                unlocked = false;
            }
            
            if (unlocked) {
                auto end_time = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end_time - start_time);
                
                cout << "\n\nПароль найден: " << password << endl;
                cout << "Время поиска: " << duration.count() / 1000.0 << " сек" << endl;
                cout << "Всего проверок: " << total_attempts << endl;
                return 0;
            }
            
            total_attempts++;
            
            // Вывод прогресса
            if (total_attempts % 100 == 0) {
                auto now = high_resolution_clock::now();
                auto elapsed = duration_cast<milliseconds>(now - last_update);
                
                if (elapsed.count() > 1000) {
                    last_update = now;
                    auto total_time = duration_cast<milliseconds>(now - start_time);
                    double rate = total_attempts / max(0.001, total_time.count() / 1000.0);
                    
                    cout << fixed << setprecision(1);
                    cout << "\rПроверено: " << total_attempts 
                         << " | Скорость: " << rate << " пар/сек"
                         << " | Текущий: " << password
                         << string(20, ' ') << flush;
                }
            }
            
            // Переход к следующей комбинации
            int pos = len - 1;
            while (pos >= 0) {
                indices[pos]++;
                
                if (indices[pos] >= charset.size()) {
                    indices[pos] = 0;
                    pos--;
                } else {
                    break;
                }
            }
            
            if (pos < 0) break;
            
        } while (true);
    }

    cout << "\nПароль не найден. Проверено: " << total_attempts << endl;
    return 0;
}
