#ifndef KOJO_CC2_ASBR_MESSAGEINFO
#define KOJO_CC2_ASBR_MESSAGEINFO

#include <nucc/utils/crc32.hpp>

#include <cstdint>
#include <map>
#include <string>

namespace cc2::asbr {
        
class message_info {
public:
    struct entry { 
        nucc::crc32 crc32_id;
        std::string message{"<EMPTY>"};
        std::uint32_t ref_crc32_id{0};
        std::int16_t is_ref{-1};
        std::int64_t file_index{-1};
        std::int64_t cue_index{-1};

        std::uint32_t key() const {
            return crc32_id.id();
        }
        std::string order_key() {
            return std::format("{:03}{:03}{}", file_index, cue_index, crc32_id.string());
        }
    };
    std::map<std::uint32_t, entry> entries;
    std::map<std::string, std::uint32_t> entry_order;

    std::uint32_t version{1001};
    std::string language{"Unknown"};
};

}

#endif // KOJO_CC2_ASBR_MESSAGEINFO