/*
Package aes_cfb реализует AES-128 шифрование в режиме CFB (Cipher Feedback)

Содержит функции для:
- Генерации случайных ключей и векторов инициализации
- Шифрования/дешифрования данных
- Вспомогательные функции для работы с данными
*/
package aes_cfb

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
)

// GenerateRandomBytes генерирует случайную последовательность байтов заданной длины
//
// Параметры:
//   - n: длина последовательности в байтах
//
// Возвращает:
//   - []byte: сгенерированные случайные байты
//   - error: ошибка, если не удалось сгенерировать
func GenerateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	if _, err := io.ReadFull(rand.Reader, b); err != nil {
		return nil, fmt.Errorf("failed to generate random bytes: %v", err)
	}
	return b, nil
}

// PrintState выводит состояние (байты) в шестнадцатеричном формате
//
// Параметры:
//   - data: данные для вывода
//   - label: метка для идентификации вывода
func PrintState(data []byte, label string) {
	fmt.Printf("%s: %s\n", label, hex.EncodeToString(data))
}

// Encrypt шифрует данные в режиме CFB
//
// Параметры:
//   - plaintext: исходные данные
//   - key: ключ шифрования (16 байт)
//   - iv: вектор инициализации (16 байт)
//
// Возвращает:
//   - []byte: зашифрованные данные
//   - error: ошибка, если что-то пошло не так
func Encrypt(plaintext, key, iv []byte) ([]byte, error) {
	if len(key) != aes.BlockSize {
		return nil, errors.New("invalid key size")
	}
	if len(iv) != aes.BlockSize {
		return nil, errors.New("invalid IV size")
	}

	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, fmt.Errorf("failed to create cipher: %v", err)
	}

	ciphertext := make([]byte, len(plaintext))
	stream := cipher.NewCFBEncrypter(block, iv)
	stream.XORKeyStream(ciphertext, plaintext)

	return ciphertext, nil
}

// Decrypt дешифрует данные в режиме CFB
//
// Параметры:
//   - ciphertext: зашифрованные данные
//   - key: ключ шифрования (16 байт)
//   - iv: вектор инициализации (16 байт)
//
// Возвращает:
//   - []byte: расшифрованные данные
//   - error: ошибка, если что-то пошло не так
func Decrypt(ciphertext, key, iv []byte) ([]byte, error) {
	if len(key) != aes.BlockSize {
		return nil, errors.New("invalid key size")
	}
	if len(iv) != aes.BlockSize {
		return nil, errors.New("invalid IV size")
	}

	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, fmt.Errorf("failed to create cipher: %v", err)
	}

	plaintext := make([]byte, len(ciphertext))
	stream := cipher.NewCFBDecrypter(block, iv)
	stream.XORKeyStream(plaintext, ciphertext)

	return plaintext, nil
}
