#ifndef KOJO_CC2_JSON_SERIALIZER
#define KOJO_CC2_JSON_SERIALIZER

#include <nlohmann/json.hpp>

namespace cc2 {

template<typename T>
class json_serializer {
public:
    static T read(const nlohmann::ordered_json&);
    static nlohmann::ordered_json write(const T&);
};

}

#endif // KOJO_CC2_JSON_SERIALIZER