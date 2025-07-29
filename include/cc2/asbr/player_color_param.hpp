#ifndef KOJO_CC2_ASBR_PLAYERCOLORPARAM
#define KOJO_CC2_ASBR_PLAYERCOLORPARAM

#include <cc2/binary_data.hpp>

#include <nucc/utils/rgb.hpp>

#include <map>
#include <unordered_map>

namespace cc2 {
    namespace asbr {
        
class player_color_param : public binary_data {
public:
    struct entry {
        std::string character_id; // Length always 6.
        std::uint32_t tint_index; // Not written.
        std::uint32_t costume_index;
        nucc::rgb color;

        std::string key();
    };
    std::map<std::string, entry> entries;

    std::unordered_map<std::string, size_t> tint_tracker;

    int version() const { return VERSION; }

    size_t size() const override;

    void read(const std::byte* src, const size_t start = 0) override;
    const std::shared_ptr<std::vector<std::byte>> write() const override;

private:
    static const std::uint32_t VERSION = 1000;

    static const size_t HEADER_SIZE = 16;
    static const size_t ENTRY_SIZE = 24;
    static const size_t CHARACTER_ID_LENGTH = 8;
};

    } // namespace ASBR
} // namespace cc2

#endif // KOJO_CC2_ASBR_PLAYERCOLORPARAM