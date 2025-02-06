#pragma once

#include <filesystem>
#include <numeric>
#include <unordered_set>
#include "../utils/utils.h"

namespace fs = std::filesystem;

/**
 * @brief Хеш для вектора строк, используемого как ключ в unordered_set.
 * 
 */
struct VectorStringHash {
    /**
     * @brief Хеш-функция для вектора строк.
     *  
     * @param vec Вектор строк для вычисления хеша.
     * @return Вычисленное хеш-значение.
     */
    std::size_t operator()(const std::vector<std::string>& vec) const {
        std::hash<std::string> hasher;
        
        return std::accumulate(vec.begin(), vec.end(), std::size_t(0),
            [&](std::size_t seed, const std::string& str) {
                return seed ^ (hasher(str) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
            });
    }
};

/**
 * @brief Класс для сканирования директорий на наличие файлов и определения дубликатов.
 */
class FileScanner {
public:
    /**
     * @brief Конструктор
     * 
     * @param include_dirs Список директорий для сканирования.
     * @param exclude_dirs Список директорий, которые нужно исключить из сканирования.
     * @param scan_level Уровень глубины сканирования директорий.
     * @param min_file_size Минимальный размер файла (в байтах), чтобы он был учтен в сканировании.
     * @param file_masks Список масок файлов (например, "*.txt") для фильтрации файлов.
     * @param block_size Размер блока для чтения файлов (для вычисления хешей).
     * @param hash_algorithm Алгоритм хеширования, используемый для определения дубликатов.
     */
    FileScanner(const std::vector<std::string>& include_dirs, const std::vector<std::string>& exclude_dirs,
                std::size_t scan_level, std::size_t min_file_size, const std::vector<std::string>& file_masks,
                std::size_t block_size, const std::string& hash_algorithm);

    /**
     * @brief Выполняет сканирование директорий для поиска файлов, вычисление хешей и группировку дубликатов.
     */
    void scan();

    /**
     * @brief Выводит список дубликатов файлов, найденных в процессе сканирования.
     */
    void print_duplicates() const;

private:
    std::vector<std::string> include_dirs_; ///< Список директорий для сканирования.
    std::unordered_set<fs::path> exclude_set_; ///< Множество директорий, которые исключаются из сканирования.
    std::size_t scan_level_; ///< Уровень глубины сканирования.
    std::size_t min_file_size_; ///< Минимальный размер файла для сканирования.
    std::vector<std::string> file_masks_; ///< Список масок для фильтрации файлов.
    std::size_t block_size_; ///< Размер блока для чтения файлов.
    std::string hash_algorithm_; ///< Алгоритм хеширования для вычисления хешей файлов.
    std::unordered_map<HashSeq, std::vector<fs::path>, VectorStringHash> file_groups_; ///< Группы файлов по их хешам.

    /**
     * @brief Рекурсивно сканирует директорию для поиска файлов.
     * 
     * @param dir Путь к директории, которую необходимо сканировать.
     */
    void scan_directory(const std::string& dir);

    /**
     * @brief Проверяет, следует ли исключить файл из сканирования.
     * 
     * @param file_path Путь к файлу для проверки.
     * @return true, если файл следует исключить, иначе false.
     */
    bool should_exclude(const fs::path& file_path) const;
};
