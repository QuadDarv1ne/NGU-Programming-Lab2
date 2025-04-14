#include <vector>
#include <algorithm>

/**
 * Находит количество валунов, которые не посетит ни одна птица.
 * 
 * @param N Количество птиц.
 * @param stones Общее количество валунов.
 * @param birds Массив с дистанциями перелёта каждой птицы.
 * @return Количество непосещённых валунов.
 */

int unvisitedStones(int N, int stones, std::vector<int>& birds) {
    std::vector<bool> visited(stones + 1, false); // +1, так как валуны нумеруются с 1
    for (int bird : birds) {
        for (int pos = bird; pos <= stones; pos += bird) {
            visited[pos] = true;
        }
    }
    return std::count(visited.begin() + 1, visited.end(), false);
}