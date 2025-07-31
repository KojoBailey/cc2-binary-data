#include <cc2/asbr/player_color_param.hpp>

using namespace cc2::asbr;

void player_color_param::merge(player_color_param& param) {
    for (auto& [key, value] : param.entries) {
        entries[key] = value;
    }
}