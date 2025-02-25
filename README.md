# Утилита для быстрого поиска дублирующихся по содержимому файлов

## Описание
Программа предназначена для сканирования файловых директорий с возможностью фильтрации по маскам, уровням вложенности и минимальному размеру файла. Поддерживаются алгоритмы хеширования `CRC32` и `MD5` для выявления дубликатов файлов.

## Компиляция и запуск

### Требования
- Компилятор с поддержкой C++17 и выше
- `Boost` (модуль `program_options`)

### Запуск
```bash
./file_scanner [OPTIONS]
```

## Опции командной строки
Программа принимает следующие аргументы:

| Опция | Аргумент | Описание |
|--------|------------|------------|
| `-i, --include` | `<path> ...` | Директории для сканирования |
| `-e, --exclude` | `<path> ...` | Директории, исключаемые из сканирования |
| `-l, --level` | `<int>` | Максимальный уровень вложенности (по умолчанию 0) |
| `-s, --min-size` | `<int>` | Минимальный размер файла (в байтах, по умолчанию 1) |
| `-m, --mask` | `<string> ...` | Маски имен файлов (например, `*.txt`, `file?.log`) |
| `-S, --block-size` | `<int>` | Размер блока чтения файлов (по умолчанию 5) |
| `-H, --hash` | `<string>` | Алгоритм хэширования (`crc32` или `md5`) |
| `-h, --help` | | Вывести справку |

## Файлы проекта

### `main.cpp`
Точка входа в программу. Обрабатывает аргументы командной строки, вызывает `FileScanner` для сканирования и вывода дубликатов файлов.

### `FileScanner.h`
Класс `FileScanner`, выполняющий рекурсивное сканирование директорий, проверку файлов на соответствие критериям и вычисление их хэша.

### `Hasher.h`
Абстрактный класс `Hasher` с реализациями `CRC32Hasher` и `MD5Hasher` для вычисления контрольных сумм файлов.

### `utils.h`
Функции:
- `hash_file_blocks()` — разбивает файлы на блоки и хеширует их
- `matches_mask()` — проверяет соответствие имени файла заданным маскам

## Пример использования

Сканирование каталога `/home/user/docs` на `.txt` файлы размером от 100 байт с использованием `MD5`:
```bash
./file_scanner -i /home/user/docs -m "*.txt" -s 100 -H md5
```

Исключение подкаталога `backup`:
```bash
./file_scanner -i /home/user/docs -e /home/user/docs/backup -m "*.log" -H crc32
```

