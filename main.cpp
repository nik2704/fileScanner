#include <boost/program_options.hpp>
#include <iostream>
#include "./fileScanner/FileScanner.h"


namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        std::vector<std::string> include_dirs, exclude_dirs, file_masks;
        std::string hash_algorithm;
        std::size_t scan_level = 0, min_file_size = 1, block_size = 5;

        po::options_description desc("Допустимые опции", 120);
        desc.add_options()
            ("include,i", po::value<std::vector<std::string>>(&include_dirs)->multitoken(), "Директории для сканирования")
            ("exclude,e", po::value<std::vector<std::string>>(&exclude_dirs)->multitoken(), "Директории для исключения")
            ("level,l", po::value<std::size_t>(&scan_level)->default_value(scan_level), "Уровень сканирования")
            ("min-size,s", po::value<std::size_t>(&min_file_size)->default_value(1), "Минимальный размер файла (в байтах)")
            ("mask,m", po::value<std::vector<std::string>>(&file_masks)->multitoken(), "Маски имен файлов")
            ("block-size,S", po::value<std::size_t>(&block_size)->default_value(block_size), "Размер блока чтения файлов")
            ("hash,H", po::value<std::string>(&hash_algorithm)->required(), "Алгоритм хэширования (crc32 or md5)")
            ("help,h", "Помощь");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        po::notify(vm);


        FileScanner scanner(include_dirs, exclude_dirs, scan_level, min_file_size, file_masks, block_size, hash_algorithm);

        scanner.scan();// #include <numeric>
// #include <string>
// #include <fstream>
// #include <filesystem>
// #include <vector>
// #include <unordered_set>
// #include <unordered_map>
// #include <iomanip>

        scanner.print_duplicates();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
