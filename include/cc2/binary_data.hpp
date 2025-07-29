#ifndef KOJO_CC2_BINARY_DATA
#define KOJO_CC2_BINARY_DATA

#include <kojo/binary.hpp>

namespace cc2 {

class binary_data {
public:
    virtual ~binary_data() = default;

    virtual size_t size() const = 0;

    virtual void read(const std::byte* src, const size_t start = 0) = 0;
    virtual const std::shared_ptr<std::vector<std::byte>> write() const = 0;
};

}

#endif // KOJO_CC2_BINARY_DATA