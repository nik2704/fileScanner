#include "utils.h"

HashSeq hash_file_blocks(const fs::path& path, size_t block_size, const std::string& algorithm) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return {};

    HashSeq hashes;
    std::string buffer(block_size, '\0');

    while (file.read(buffer.data(), block_size) || file.gcount() > 0) {
        if (static_cast<size_t>(file.gcount()) < block_size) {
            buffer.resize(static_cast<size_t>(file.gcount()), '\0');
        }

        std::unique_ptr<Hasher> hashier = algorithm == "md5" ?
            HasherFactory::createHasher(HasherFactory::HashType::MD5) : HasherFactory::createHasher(HasherFactory::HashType::CRC32);

        hashes.push_back(hashier->compute(buffer));
    }
    return hashes;
}

bool matches_mask(const std::string& filename, const std::vector<std::string>& masks) {
    if (masks.empty()) return true;

    for (const auto& mask : masks) {
        std::string regex_pattern = std::regex_replace(mask, std::regex(R"(\*)"), ".*");
        regex_pattern = std::regex_replace(regex_pattern, std::regex(R"(\?)"), ".");

        std::regex file_regex("^" + regex_pattern + "$", std::regex::icase);

        if (std::regex_match(filename, file_regex)) {
            return true;
        }
    }
    return false;
}
