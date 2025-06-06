/*
Пакет aescfb реализует AES-128 шифрование в режиме CFB (Cipher Feedback)

Особенности:
- Автоматическая генерация ключей
- Безопасное управление памятью
- Поддержка PKCS7 заполнения
- Полная обработка ошибок
*/
package aescfb

import (
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"os"
)

const BlockSize = aes.BlockSize // Размер блока AES в байтах

// AESCFB представляет контекст шифрования AES-128 CFB
type AESCFB struct {
	key []byte // Ключ шифрования
	iv  []byte // Вектор инициализации
}

// New создает новый экземпляр AESCFB со случайными ключом и вектором инициализации
func New() (*AESCFB, error) {
	key, err := generateRandomBytes(BlockSize)
	if err != nil {
		return nil, fmt.Errorf("ошибка генерации ключа: %w", err)
	}

	iv, err := generateRandomBytes(BlockSize)
	if err != nil {
		return nil, fmt.Errorf("ошибка генерации вектора инициализации: %w", err)
	}

	return &AESCFB{key: key, iv: iv}, nil
}

// Encrypt шифрует данные с использованием AES-128 CFB
func (a *AESCFB) Encrypt(plaintext []byte) ([]byte, error) {
	if err := a.validate(); err != nil {
		return nil, err
	}

	block, err := aes.NewCipher(a.key)
	if err != nil {
		return nil, fmt.Errorf("ошибка создания шифра: %w", err)
	}

	padded := addPadding(plaintext, BlockSize)
	ciphertext := make([]byte, len(padded))
	stream := cipher.NewCFBEncrypter(block, a.iv)
	stream.XORKeyStream(ciphertext, padded)

	return ciphertext, nil
}

// Decrypt дешифрует данные с использованием AES-128 CFB
func (a *AESCFB) Decrypt(ciphertext []byte) ([]byte, error) {
	if err := a.validate(); err != nil {
		return nil, err
	}

	block, err := aes.NewCipher(a.key)
	if err != nil {
		return nil, fmt.Errorf("ошибка создания шифра: %w", err)
	}

	plaintext := make([]byte, len(ciphertext))
	stream := cipher.NewCFBDecrypter(block, a.iv)
	stream.XORKeyStream(plaintext, ciphertext)

	return removePadding(plaintext), nil
}

// Key возвращает текущий ключ шифрования
func (a *AESCFB) Key() []byte {
	return a.key
}

// IV возвращает текущий вектор инициализации
func (a *AESCFB) IV() []byte {
	return a.iv
}

// SaveKey сохраняет ключ в файл
func (a *AESCFB) SaveKey(filename string) error {
	return os.WriteFile(filename, a.key, 0600)
}

// LoadKey загружает ключ из файла
func (a *AESCFB) LoadKey(filename string) error {
	key, err := os.ReadFile(filename)
	if err != nil {
		return fmt.Errorf("ошибка чтения файла ключа: %w", err)
	}
	
	if len(key) != BlockSize {
		return errors.New("неверный размер ключа")
	}
	
	a.key = key
	return nil
}

// BytesToHex преобразует байты в шестнадцатеричную строку
func BytesToHex(data []byte) string {
	return hex.EncodeToString(data)
}

// validate проверяет корректность ключа и вектора инициализации
func (a *AESCFB) validate() error {
	if len(a.key) != BlockSize {
		return errors.New("неверный размер ключа")
	}
	if len(a.iv) != BlockSize {
		return errors.New("неверный размер вектора инициализации")
	}
	return nil
}

// generateRandomBytes генерирует криптографически безопасные случайные байты
func generateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	if _, err := io.ReadFull(rand.Reader, b); err != nil {
		return nil, err
	}
	return b, nil
}

// addPadding добавляет PKCS7 заполнение к данным
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

// removePadding удаляет PKCS7 заполнение из данных
func removePadding(data []byte) []byte {
	if len(data) == 0 {
		return data
	}
	padding := int(data[len(data)-1])
	if padding > len(data) || padding == 0 {
		return data
	}
	return data[:len(data)-padding]
}
