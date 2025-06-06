/*
Основной модуль демонстрации работы AES-128 в режиме CFB

Демонстрирует:
1. Генерацию ключа и вектора инициализации
2. Шифрование пользовательского ввода
3. Дешифрование и проверку результата
*/
package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"

	"aes_cfb_demo/aescfb"
)

func main() {
	fmt.Println("▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄")
	fmt.Println("█ Демонстрация AES-128 CFB (Cipher Feedback) █")
	fmt.Println("▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀")
	fmt.Println()

	// Инициализация AES
	aes, err := aescfb.New()
	if err != nil {
		log.Fatalf("Ошибка инициализации AES: %v", err)
	}

	// Вывод сгенерированных ключей
	fmt.Printf("Сгенерированный ключ: %s\n", aescfb.BytesToHex(aes.Key()))
	fmt.Printf("Вектор инициализации: %s\n\n", aescfb.BytesToHex(aes.IV()))

	// Сохранение ключа
	if err := aes.SaveKey("aes_key.bin"); err != nil {
		log.Fatalf("Ошибка сохранения ключа: %v", err)
	}
	fmt.Println("Ключ сохранен в файл: aes_key.bin\n")

	// Ввод текста для шифрования
	fmt.Print("Введите текст для шифрования: ")
	reader := bufio.NewReader(os.Stdin)
	plaintext, err := reader.ReadString('\n')
	if err != nil {
		log.Fatalf("Ошибка чтения ввода: %v", err)
	}
	plaintext = strings.TrimSpace(plaintext)

	// Шифрование
	ciphertext, err := aes.Encrypt([]byte(plaintext))
	if err != nil {
		log.Fatalf("Ошибка шифрования: %v", err)
	}
	fmt.Println("\nШифрование успешно завершено!")
	fmt.Printf("Зашифрованный текст (HEX): %s\n\n", aescfb.BytesToHex(ciphertext))

	// Дешифрование
	decrypted, err := aes.Decrypt(ciphertext)
	if err != nil {
		log.Fatalf("Ошибка дешифрования: %v", err)
	}
	fmt.Println("Дешифрование успешно завершено!")
	fmt.Printf("Дешифрованный текст: %s\n", string(decrypted))

	fmt.Println("\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄")
	fmt.Println("█          Программа успешно завершена          █")
	fmt.Println("▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀")
}
