#pragma once

#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

class Hasher {
public:
    virtual ~Hasher() = default;
    virtual std::string compute(const std::string& data) const = 0;
};

class CRC32Hasher : public Hasher {
public:
    std::string compute(const std::string& data) const override {
        boost::crc_32_type crc;
        crc.process_bytes(data.data(), data.size());

        return std::to_string(crc.checksum());
    }
};

class MD5Hasher : public Hasher {
public:
    std::string compute(const std::string& data) const override {
        boost::uuids::detail::md5 hash;
        hash.process_bytes(data.data(), data.size());

        boost::uuids::detail::md5::digest_type digest;
        hash.get_digest(digest);
        
        std::ostringstream result;
        for (int i : digest) {
            result << std::hex << std::setw(8) << std::setfill('0') << i;
        }

        return result.str();
    }
};

class HasherFactory {
public:
    enum class HashType { CRC32, MD5 };

    static std::unique_ptr<Hasher> createHasher(HashType type) {
        switch (type) {
            case HashType::CRC32:
                return std::make_unique<CRC32Hasher>();
            case HashType::MD5:
                return std::make_unique<MD5Hasher>();
            default:
                throw std::invalid_argument("Unknown hash");
        }
    }
};
