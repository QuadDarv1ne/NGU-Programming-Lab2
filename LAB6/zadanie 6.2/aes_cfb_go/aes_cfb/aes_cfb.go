/*
Package aes_cfb реализует AES-128 шифрование в режиме CFB (Cipher Feedback)

Содержит функции для:
- Генерации случайных ключей и векторов инициализации
- Шифрования/дешифрования данных
- Вспомогательные функции для работы с данными
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
)

const BlockSize = aes.BlockSize

type AESCFB struct {
	key []byte
	iv  []byte
}

func New() (*AESCFB, error) {
	key, err := generateRandomBytes(BlockSize)
	if err != nil {
		return nil, fmt.Errorf("failed to generate key: %w", err)
	}

	iv, err := generateRandomBytes(BlockSize)
	if err != nil {
		return nil, fmt.Errorf("failed to generate IV: %w", err)
	}

	return &AESCFB{key: key, iv: iv}, nil
}

func (a *AESCFB) Encrypt(plaintext []byte) ([]byte, error) {
	if err := a.validate(); err != nil {
		return nil, err
	}

	block, err := aes.NewCipher(a.key)
	if err != nil {
		return nil, fmt.Errorf("failed to create cipher: %w", err)
	}

	padded := addPadding(plaintext, BlockSize)
	ciphertext := make([]byte, len(padded))
	stream := cipher.NewCFBEncrypter(block, a.iv)
	stream.XORKeyStream(ciphertext, padded)

	return ciphertext, nil
}

func (a *AESCFB) Decrypt(ciphertext []byte) ([]byte, error) {
	if err := a.validate(); err != nil {
		return nil, err
	}

	block, err := aes.NewCipher(a.key)
	if err != nil {
		return nil, fmt.Errorf("failed to create cipher: %w", err)
	}

	plaintext := make([]byte, len(ciphertext))
	stream := cipher.NewCFBDecrypter(block, a.iv)
	stream.XORKeyStream(plaintext, ciphertext)

	return removePadding(plaintext), nil
}

func (a *AESCFB) Key() []byte {
	return a.key
}

func (a *AESCFB) IV() []byte {
	return a.iv
}

func (a *AESCFB) SetKey(key []byte) error {
	if len(key) != BlockSize {
		return errors.New("invalid key size")
	}
	a.key = key
	return nil
}

func (a *AESCFB) SetIV(iv []byte) error {
	if len(iv) != BlockSize {
		return errors.New("invalid IV size")
	}
	a.iv = iv
	return nil
}

func (a *AESCFB) validate() error {
	if len(a.key) != BlockSize {
		return errors.New("key not set or invalid size")
	}
	if len(a.iv) != BlockSize {
		return errors.New("IV not set or invalid size")
	}
	return nil
}

func generateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	if _, err := io.ReadFull(rand.Reader, b); err != nil {
		return nil, err
	}
	return b, nil
}

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

func BytesToHex(data []byte) string {
	return hex.EncodeToString(data)
}
