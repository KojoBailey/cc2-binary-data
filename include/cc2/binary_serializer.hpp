#ifndef KOJO_CC2_BINARY_SERIALIZER
#define KOJO_CC2_BINARY_SERIALIZER

#include <kojo/binary.hpp>

namespace cc2 {

template<typename T>
class binary_serializer {
public:
    static T read(const std::byte* src, const size_t start = 0);
    static kojo::binary write(const T&);
};

}

#endif