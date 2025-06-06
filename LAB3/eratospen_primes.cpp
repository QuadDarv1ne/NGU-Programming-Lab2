/**
 * Оптимизированная генерация и проверка простых чисел методами Эратосфена, Миллера, Поклингтона и ГОСТ
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <chrono>
#include <iomanip>
#include <numeric>

using namespace std;

// Глобальные счетчики для отброшенных вариантов (потокобезопасные атомарные)
atomic<int> miller_rejected{0};
atomic<int> poklington_rejected{0};
atomic<int> gost_rejected{0};

// 1. Оптимизированное решето Эратосфена с битовой упаковкой
vector<int> sieve_of_eratosthenes(int limit = 500) {
    if (limit < 2) return {};
    
    const int size = (limit + 1) / 2;
    vector<bool> sieve(size, true);
    sieve[0] = false; // 1 не является простым
    
    for (int i = 1; 2*i+1 <= sqrt(limit); ++i) {
        if (sieve[i]) {
            int prime = 2*i + 1;
            for (int j = prime*prime; j <= limit; j += 2*prime) {
                sieve[(j-1)/2] = false;
            }
        }
    }
    
    vector<int> primes = {2};
    for (int i = 1; i < size; ++i) {
        if (sieve[i]) {
            primes.push_back(2*i + 1);
        }
    }
    return primes;
}

// 2a. Оптимизированная модульная экспонентация с предварительным вычислением
uint64_t modular_pow(uint64_t base, uint64_t exponent, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) 
            result = (result * base) % mod;
        exponent >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

// Тест Миллера с предварительной проверкой малых делителей
bool miller_test(uint64_t n, const vector<int>& factors, int iterations = 5) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    
    static const int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int p : small_primes) {
        if (n % p == 0) return n == p;
    }
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(2, n-2);
    
    uint64_t d = n - 1;
    while (d % 2 == 0) d /= 2;
    
    for (int i = 0; i < iterations; ++i) {
        uint64_t a = dis(gen);
        uint64_t x = modular_pow(a, d, n);
        
        if (x == 1 || x == n-1) continue;
        
        bool composite = true;
        uint64_t temp = d;
        while (temp != n-1) {
            x = (x * x) % n;
            temp *= 2;
            if (x == 1) {
                miller_rejected++;
                return false;
            }
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) {
            miller_rejected++;
            return false;
        }
    }
    return true;
}

uint64_t generate_prime_miller(int target_bits, const vector<int>& primes) {
    if (target_bits < 2) return 0;
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<size_t> prime_dis(0, primes.size()-1);
    uniform_int_distribution<int> alpha_dis(1, 2);
    
    const uint64_t min_val = 1ULL << (target_bits - 1);
    const uint64_t max_val = (1ULL << target_bits) - 1;
    
    while (true) {
        uint64_t m = 1;
        vector<int> factors;
        
        while (m < (1ULL << (target_bits - 2))) {
            int q = primes[prime_dis(gen)];
            int alpha = alpha_dis(gen);
            uint64_t power = static_cast<uint64_t>(pow(q, alpha));
            
            if (m > max_val / power) break;
            m *= power;
            if (find(factors.begin(), factors.end(), q) == factors.end()) {
                factors.push_back(q);
            }
        }
        
        uint64_t n = 2 * m + 1;
        if (n < min_val || n > max_val) continue;
        
        if (miller_test(n, factors)) {
            return n;
        }
    }
}

// 2б. Оптимизированный тест Поклингтона
uint64_t generate_prime_poklington(int target_bits, const vector<int>& primes) {
    if (target_bits < 2) return 0;
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<size_t> prime_dis(0, primes.size()-1);
    
    const uint64_t min_val = 1ULL << (target_bits - 1);
    const uint64_t max_val = (1ULL << target_bits) - 1;
    
    while (true) {
        uint64_t F = 1;
        vector<int> F_factors;
        
        // Генерация F (размер ~ target_bits/2)
        while (F < (1ULL << (target_bits/2 - 1))) {
            int q = primes[prime_dis(gen)];
            if (find(F_factors.begin(), F_factors.end(), q) == F_factors.end()) {
                if (F > max_val / q) break;
                F *= q;
                F_factors.push_back(q);
            }
        }
        
        // Генерация R
        uint64_t R = 1;
        while (true) {
            int q = primes[prime_dis(gen)];
            if (find(F_factors.begin(), F_factors.end(), q) == F_factors.end()) {
                if (R > max_val / q) break;
                R *= q;
                uint64_t n = F * R + 1;
                if (n >= min_val) break;
            }
        }
        
        uint64_t n = F * R + 1;
        if (n < min_val || n > max_val) continue;
        
        // Тест Поклингтона
        uniform_int_distribution<uint64_t> a_dis(2, n-2);
        uint64_t a = a_dis(gen);
        
        if (modular_pow(a, n-1, n) != 1) {
            poklington_rejected++;
            continue;
        }
        
        bool valid = true;
        for (int q : F_factors) {
            if (gcd(modular_pow(a, (n-1)/q, n) - 1, n) != 1) {
                valid = false;
                break;
            }
        }
        
        if (valid) {
            return n;
        } else {
            poklington_rejected++;
        }
    }
}

// 2в. Оптимизированный тест ГОСТ Р 34.10-94
bool gost_test(uint64_t n, int iterations = 5) {
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0) return false;
    
    // Проверка малых делителей
    static const int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (int p : small_primes) {
        if (n % p == 0) return n == p;
    }
    
    uint64_t d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(2, n-2);
    
    for (int i = 0; i < iterations; ++i) {
        uint64_t a = dis(gen);
        uint64_t x = modular_pow(a, d, n);
        
        if (x == 1 || x == n-1) continue;
        
        bool composite = true;
        for (int j = 0; j < s-1; ++j) {
            x = modular_pow(x, 2, n);
            if (x == n-1) {
                composite = false;
                break;
            }
        }
        if (composite) {
            gost_rejected++;
            return false;
        }
    }
    return true;
}

uint64_t gost_generate_prime(int target_bits) {
    if (target_bits < 2) return 0;
    
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dis(
        1ULL << (target_bits - 1),
        (1ULL << target_bits) - 1
    );
    
    while (true) {
        uint64_t candidate = dis(gen);
        candidate |= 1; // Гарантируем нечетность
        
        if (gost_test(candidate)) {
            return candidate;
        }
    }
}

// Функция для вывода таблицы
void print_table(const string& title, const vector<uint64_t>& numbers) {
    const int col_width = 14;
    const int result_width = 17;
    
    cout << "┌" << string(col_width + result_width + 3, '
