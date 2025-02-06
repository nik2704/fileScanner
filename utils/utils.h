#pragma once

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <filesystem>
#include <fstream>
#include <regex>
#include "../hashier/Hashier.h"

namespace fs = std::filesystem;
using HashSeq = std::vector<std::string>;

HashSeq hash_file_blocks(const fs::path& path, size_t block_size, const std::string& algorithm);
bool matches_mask(const std::string& filename, const std::vector<std::string>& masks);
