/*
Основной модуль демонстрации работы AES-128 в режиме CFB

Демонстрирует:
1. Генерацию ключа и вектора инициализации
2. Шифрование пользовательского ввода
3. Дешифрование и проверку результата
*/
package main

import (
	"aes_cfb_demo/aes_cfb"
	"bufio"
	"fmt"
	"log"
	"os"
)

// addPadding добавляет PKCS7 padding к данным
//
// Параметры:
//   - data: исходные данные
//   - blockSize: размер блока для выравнивания
//
// Возвращает:
//   - []byte: данные с padding
func addPadding(data []byte, blockSize int) []byte {
	padding := blockSize - (len(data) % blockSize)
	if padding == 0 {
		padding = blockSize
	}
	padded := make([]byte, len(data)+padding)
	copy(padded, data)
	for i := len(data); i < len(padded); i++ {
		padded[i] = byte(padding)
	}
	return padded
}

// removePadding удаляет PKCS7 padding из данных
//
// Параметры:
//   - data: данные с padding
//
// Возвращает:
//   - []byte: данные без padding
func removePadding(data []byte) []byte {
	if len(data) == 0 {
		return data
	}
	padding := int(data[len(data)-1])
	if padding > len(data) {
		return data
	}
	return data[:len(data)-padding]
}

func main() {
	// Генерация ключа и IV
	key, err := aes_cfb.GenerateRandomBytes(aes.BlockSize)
	if err != nil {
		log.Fatalf("Failed to generate key: %v", err)
	}

	iv, err := aes_cfb.GenerateRandomBytes(aes.BlockSize)
	if err != nil {
		log.Fatalf("Failed to generate IV: %v", err)
	}

	// Вывод сгенерированных ключей
	aes_cfb.PrintState(key, "Ключ")
	aes_cfb.PrintState(iv, "Вектор инициализации (IV)")

	// Запись ключа в файл
	if err := os.WriteFile("aes_key.bin", key, 0644); err != nil {
		log.Fatalf("Failed to write key file: %v", err)
	}

	// Ввод текста для шифрования
	fmt.Print("Введите текст для шифрования: ")
	reader := bufio.NewReader(os.Stdin)
	plaintext, _ := reader.ReadString('\n')
	plaintext = plaintext[:len(plaintext)-1] // Удаляем \n

	// Добавление padding
	paddedText := addPadding([]byte(plaintext), aes.BlockSize)

	// Шифрование
	ciphertext, err := aes_cfb.Encrypt(paddedText, key, iv)
	if err != nil {
		log.Fatalf("Encryption failed: %v", err)
	}
	aes_cfb.PrintState(ciphertext, "Зашифрованный текст")

	// Расшифрование
	decrypted, err := aes_cfb.Decrypt(ciphertext, key, iv)
	if err != nil {
		log.Fatalf("Decryption failed: %v", err)
	}

	// Удаление padding
	decrypted = removePadding(decrypted)

	fmt.Printf("Расшифрованный текст: %s\n", string(decrypted))
}
