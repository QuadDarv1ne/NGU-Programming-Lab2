import random
import math

def is_prime(n, k=20):
    """Проверка числа на простоту с помощью теста Миллера-Рабина."""
    if n <= 1:
        return False
    if n <= 3:
        return True
    if n % 2 == 0:
        return False

    # Представим n-1 как (2^r) * d
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    # Проведем k тестов
    for _ in range(k):
        a = random.randint(2, n - 2)
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for __ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def generate_prime(bits):
    """Генерация случайного простого числа заданной длины."""
    while True:
        num = random.getrandbits(bits)
        num |= (1 << bits - 1) | 1  # Устанавливаем старший бит в 1 и младший в 1 (нечетное)
        if is_prime(num):
            return num

def extended_gcd(a, b):
    """Расширенный алгоритм Евклида для нахождения НОД и коэффициентов Безу."""
    if b == 0:
        return a, 1, 0
    gcd, x1, y1 = extended_gcd(b, a % b)
    x = y1
    y = x1 - (a // b) * y1
    return gcd, x, y

def mod_inverse(e, phi):
    """Нахождение обратного элемента по модулю."""
    g, x, _ = extended_gcd(e, phi)
    if g != 1:
        raise ValueError("Обратный элемент не существует")
    return x % phi

def generate_rsa_keys(bits=256):
    """Генерация ключей RSA."""
    # Генерация двух простых чисел
    p = generate_prime(bits // 2)
    q = generate_prime(bits // 2)
    while p == q:
        q = generate_prime(bits // 2)

    n = p * q
    phi = (p - 1) * (q - 1)

    # Выбор открытой экспоненты (обычно 65537)
    e = 65537
    while math.gcd(e, phi) != 1:
        e = random.randint(2, phi - 1)

    # Вычисление секретной экспоненты
    d = mod_inverse(e, phi)
    
    # Открытый ключ: (e, n), закрытый ключ: (d, n)
    return (e, n), (d, n)

def rsa_encrypt(public_key, plaintext):
    """Шифрование сообщения с использованием открытого ключа."""
    e, n = public_key
    # Преобразование текста в числовое представление
    m = int.from_bytes(plaintext.encode('utf-8'), 'big')
    if m >= n:
        raise ValueError("Сообщение слишком велико для данного ключа")
    # Шифрование: c = m^e mod n
    c = pow(m, e, n)
    return c

def rsa_decrypt(private_key, ciphertext):
    """Дешифрование сообщения с использованием закрытого ключа."""
    d, n = private_key
    # Дешифрование: m = c^d mod n
    m = pow(ciphertext, d, n)
    # Преобразование числа обратно в текст
    plaintext = m.to_bytes((m.bit_length() + 7) // 8, 'big').decode('utf-8')
    return plaintext

# Пример использования
if __name__ == "__main__":
    # Генерация ключей (256 бит для примера)
    public_key, private_key = generate_rsa_keys(256)
    print(f"Открытый ключ (e, n): {public_key}")
    print(f"Закрытый ключ (d, n): {private_key}")

    # Ввод сообщения
    message = input("Введите сообщение для шифрования: ")
    
    # Шифрование
    try:
        ciphertext = rsa_encrypt(public_key, message)
        print(f"Зашифрованное сообщение (число): {ciphertext}")
    except ValueError as e:
        print(f"Ошибка шифрования: {e}")
        exit()

    # Дешифрование
    try:
        decrypted_message = rsa_decrypt(private_key, ciphertext)
        print(f"Расшифрованное сообщение: {decrypted_message}")
    except Exception as e:
        print(f"Ошибка дешифрования: {e}")
