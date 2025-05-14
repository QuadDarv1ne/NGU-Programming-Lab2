/**
 * Election.cpp
 * VotingAlgorithms.cpp
 * BordaCondorcet.cpp
 */

/**
 * @file ElectionSystem.cpp
 * @brief Реализация методов Борда и Кондорсе для определения победителей выборов.
 * 
 * Класс Election предоставляет функционал для:
 * - Подсчета голосов по методу Борда (начисление баллов за позиции).
 * - Определения победителя по методу Кондорсе (попарные сравнения).
 * - Обработки крайних случаев: ничья, отсутствие победителя Кондорсе.
 * 
 * Пример использования:
 * @code
 * vector<string> candidates = {"A", "B", "C"};
 * vector<vector<string>> voters = {{"A", "B", "C"}, {"B", "C", "A"}};
 * Election election(candidates, voters);
 * cout << "Борда: " << election.BordaCount() << endl;
 * @endcode
 */

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Election {
private:
    vector<string> candidates;
    vector<vector<string>> votersPreferences;
    vector<unordered_map<string, int>> voterRankMaps; ///< Кэш позиций кандидатов для быстрого доступа

    /**
     * @brief Инициализирует кэш позиций кандидатов для каждого избирателя.
     */
    void initRankCache() {
        for (const auto& voter : votersPreferences) {
            unordered_map<string, int> rankMap;
            for (int i = 0; i < voter.size(); ++i) {
                rankMap[voter[i]] = i;
            }
            voterRankMaps.push_back(rankMap);
        }
    }

public:
    /**
     * @brief Конструктор класса Election.
     * @param cand Список кандидатов (должен быть непустым).
     * @param prefs Предпочтения избирателей (каждый подсписок должен содержать всех кандидатов).
     * @throws invalid_argument Если входные данные некорректны.
     */
    Election(vector<string> cand, vector<vector<string>> prefs) 
        : candidates(cand), votersPreferences(prefs) {
        if (candidates.empty() || prefs.empty()) {
            throw invalid_argument("Список кандидатов или избирателей пуст");
        }
        initRankCache();
    }

    /**
     * @brief Вычисляет победителя по методу Борда.
     * @return Имя победителя или "Ничья", если несколько кандидатов имеют максимальный балл.
     */
    string BordaCount() {
        unordered_map<string, int> scores;
        int n = candidates.size();

        for (const auto& voter : votersPreferences) {
            for (size_t i = 0; i < voter.size(); ++i) {
                scores[voter[i]] += (n - 1 - i);
            }
        }

        // Поиск максимального балла
        auto maxIt = max_element(scores.begin(), scores.end(), 
            [](const auto& a, const auto& b) { return a.second < b.second; });

        // Проверка ничьей
        int count = count_if(scores.begin(), scores.end(), 
            [&](const auto& pair) { return pair.second == maxIt->second; });

        return (count == 1) ? maxIt->first : "Ничья";
    }

    /**
     * @brief Вычисляет победителя по методу Кондорсе.
     * @return Имя победителя или "Нет победителя", если такого нет.
     */
    string CondorcetWinner() {
        for (const auto& candidateA : candidates) {
            bool dominatesAll = true;
            for (const auto& candidateB : candidates) {
                if (candidateA == candidateB) continue;

                int aWins = 0;
                for (const auto& rankMap : voterRankMaps) {
                    if (rankMap.at(candidateA) < rankMap.at(candidateB)) {
                        aWins++;
                    }
                }

                if (aWins <= votersPreferences.size() / 2) {
                    dominatesAll = false;
                    break;
                }
            }
            if (dominatesAll) return candidateA;
        }
        return "Нет победителя";
    }
};
