#ifndef KOJO_CC2_ASBR_MESSAGEINFO
#define KOJO_CC2_ASBR_MESSAGEINFO

#include <nucc/utils/crc32.hpp>

#include <cstdint>
#include <format>
#include <string>
#include <unordered_map>
#include <vector>

namespace cc2::asbr {
        
class message_info {    
public:
    struct entry { 
        nucc::crc32 crc32_id;
        std::string message{"<EMPTY>"};
        nucc::crc32 ref_crc32_id;
        // std::int16_t is_ref{-1};
        std::int64_t file_index{-1};
        std::int64_t cue_index{-1};

        bool has_crc32_id{false};
        bool has_message{false};
        bool has_ref_crc32_id{false};
        bool has_file_index{false};
        bool has_cue_index{false};

        std::uint32_t key() const {
            return crc32_id.id();
        }
        std::string order_key() {
            return std::format("{:03}{:03}{}", file_index, cue_index, crc32_id.string());
        }
    };
    std::unordered_map<std::uint32_t, entry> entries;
    std::vector<std::uint32_t> sorted_keys;

    std::uint32_t version{1001};
    std::string language{"Unknown"};

    void sort_keys();

    void merge(message_info& param);
};

}

#endif // KOJO_CC2_ASBR_MESSAGEINFO