#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iomanip>
#include "../utils/utils.h"
#include "FileScanner.h"


FileScanner::FileScanner(
    const std::vector<std::string>& include_dirs,
    const std::vector<std::string>& exclude_dirs,
    std::size_t scan_level,
    std::size_t min_file_size,
    const std::vector<std::string>& file_masks,
    std::size_t block_size,
    const std::string& hash_algorithm
    ) : include_dirs_(include_dirs),
        exclude_set_(exclude_dirs.begin(),
        exclude_dirs.end()),
        scan_level_(scan_level),
        min_file_size_(min_file_size),
        file_masks_(file_masks),
        block_size_(block_size),
        hash_algorithm_(hash_algorithm) {}

void FileScanner::scan() {
    for (const auto& dir : include_dirs_) {
        scan_directory(dir);
    }
}

void FileScanner::print_duplicates() const {
    for (const auto& [_, files] : file_groups_) {
        if (files.size() > 1) {
            for (const auto& file : files) std::cout << file << std::endl;
            std::cout << std::endl;
        }
    }
}

void FileScanner::scan_directory(const std::string& dir) {
    try {
        fs::recursive_directory_iterator it(dir, fs::directory_options::skip_permission_denied | fs::directory_options::follow_directory_symlink);
        fs::recursive_directory_iterator end;

        while (it != end) {
            const auto& entry = *it;
            fs::path file_path = entry.path();

            if (it.depth() > static_cast<int>(scan_level_)) {
                ++it;
                continue;
            }

            if (should_exclude(file_path)) {
                ++it;
                continue;
            }

            if (fs::is_regular_file(file_path) && fs::file_size(file_path) >= min_file_size_ && matches_mask(file_path.filename().string(), file_masks_)) {
                HashSeq file_hash = hash_file_blocks(file_path, block_size_, hash_algorithm_);
                file_groups_[file_hash].push_back(file_path);
            }

            ++it;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Ошибка файловой системы при сканировании директории " << dir << ": " << e.what() << std::endl;
    }
}


bool FileScanner::should_exclude(const fs::path& file_path) const {
    for (const auto& excl : exclude_set_) {
        if (file_path.string().find(excl.string()) == 0) {
            return true;
        }
    }
    return false;
}
