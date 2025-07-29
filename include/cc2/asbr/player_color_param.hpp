#ifndef KOJO_CC2_ASBR_PLAYERCOLORPARAM
#define KOJO_CC2_ASBR_PLAYERCOLORPARAM

#include <nucc/utils/rgb.hpp>

#include <map>

namespace cc2::asbr {
        
class player_color_param {
public:
    std::map<std::string, nucc::rgb> entries;

    static int version() { return VERSION; }

private:
    static const std::uint32_t VERSION = 1000;
};

}

#endif // KOJO_CC2_ASBR_PLAYERCOLORPARAM