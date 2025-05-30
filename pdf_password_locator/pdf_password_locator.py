import pikepdf
import itertools
import string
import time
from tqdm import tqdm

def main():
    # Настройки перебора
    charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
    max_length = 4
    pdf_path = "217.pdf"
    log_file = "password_progress.log"
    
    print(f"Начало перебора паролей для файла {pdf_path}")
    print(f"Используемые символы: {charset}")
    print(f"Максимальная длина пароля: {max_length}")
    print(f"Прогресс будет сохраняться в файл: {log_file}")
    
    start_time = time.time()
    total_attempts = 0
    
    # Создаем прогресс-бар
    total_combinations = sum(len(charset)**i for i in range(1, max_length + 1))
    progress_bar = tqdm(total=total_combinations, unit="pass", desc="Перебор паролей")
    
    # Открываем файл для логов
    with open(log_file, "w") as log:
        log.write(f"Начало перебора: {time.ctime()}\n")
        log.write(f"Всего комбинаций: {total_combinations}\n\n")
    
    # Перебор по длине пароля
    for length in range(1, max_length + 1):
        # Генерируем все возможные комбинации
        for pwd_tuple in itertools.product(charset, repeat=length):
            password = ''.join(pwd_tuple)
            total_attempts += 1
            
            try:
                # Пробуем открыть PDF с текущим паролем
                with pikepdf.open(pdf_path, password=password) as pdf:
                    # Если успешно - пароль найден!
                    end_time = time.time()
                    duration = end_time - start_time
                    
                    print(f"\n\nПАРОЛЬ НАЙДЕН: {password}")
                    print(f"Время поиска: {duration:.2f} секунд")
                    print(f"Проверено комбинаций: {total_attempts}")
                    
                    with open(log_file, "a") as log:
                        log.write(f"Пароль найден: {password}\n")
                        log.write(f"Время: {duration:.2f} сек, попыток: {total_attempts}\n")
                    
                    # Сохраняем разблокированный PDF
                    pdf.save("unlocked.pdf")
                    print("Разблокированный файл сохранен как 'unlocked.pdf'")
                    return
                    
            except pikepdf.PasswordError:
                # Неверный пароль - продолжаем
                if total_attempts % 1000 == 0:
                    # Обновляем прогресс каждые 1000 попыток
                    progress_bar.update(1000)
                    
                    with open(log_file, "a") as log:
                        log.write(f"Проверено: {total_attempts}, текущий: {password}\n")
                        
            except Exception as e:
                # Обработка других ошибок
                print(f"\nОшибка при обработке пароля '{password}': {str(e)}")
                
                with open(log_file, "a") as log:
                    log.write(f"Ошибка при пароле '{password}': {str(e)}\n")
    
    # Если дошли до сюда - пароль не найден
    end_time = time.time()
    duration = end_time - start_time
    
    print("\n\nПароль не найден")
    print(f"Проверено комбинаций: {total_attempts}")
    print(f"Затраченное время: {duration:.2f} секунд")
    
    with open(log_file, "a") as log:
        log.write(f"\nПароль не найден. Проверено: {total_attempts}, время: {duration:.2f} сек\n")

if __name__ == "__main__":
    main()