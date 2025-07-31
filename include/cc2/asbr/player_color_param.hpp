#ifndef KOJO_CC2_ASBR_PLAYERCOLORPARAM
#define KOJO_CC2_ASBR_PLAYERCOLORPARAM

#include <nucc/utils/rgb.hpp>

#include <map>

namespace cc2::asbr {
        
class player_color_param {
public:
    std::map<std::string, nucc::rgb> entries;

    std::uint32_t version{1000};

    void merge(player_color_param&);
};

}

#endif // KOJO_CC2_ASBR_PLAYERCOLORPARAM