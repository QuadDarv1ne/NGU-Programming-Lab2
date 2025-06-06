/*
Основной модуль демонстрации работы AES-128 в режиме CFB

Демонстрирует:
1. Генерацию ключа и вектора инициализации
2. Шифрование пользовательского ввода
3. Дешифрование и проверку результата
*/
package main

import (
	"fmt"
	"log"
	"os"

	"yourmodule/aescfb"
)

func main() {
	aes, err := aescfb.New()
	if err != nil {
		log.Fatalf("Failed to create AES instance: %v", err)
	}

	fmt.Printf("Key: %s\n", aescfb.BytesToHex(aes.Key()))
	fmt.Printf("IV: %s\n", aescfb.BytesToHex(aes.IV()))

	fmt.Print("Enter text to encrypt: ")
	var plaintext string
	fmt.Scanln(&plaintext)

	ciphertext, err := aes.Encrypt([]byte(plaintext))
	if err != nil {
		log.Fatalf("Encryption failed: %v", err)
	}
	fmt.Printf("Ciphertext: %s\n", aescfb.BytesToHex(ciphertext))

	decrypted, err := aes.Decrypt(ciphertext)
	if err != nil {
		log.Fatalf("Decryption failed: %v", err)
	}
	fmt.Printf("Decrypted: %s\n", string(decrypted))
}
