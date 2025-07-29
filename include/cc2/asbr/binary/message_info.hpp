#ifndef KOJO_CC2_ASBR_BINARY_MESSAGEINFO
#define KOJO_CC2_ASBR_BINARY_MESSAGEINFO

#include <cc2/binary_serializer.hpp>
#include <cc2/asbr/message_info.hpp>

#include <kojo/logger.hpp>

#include <unordered_map>

namespace cc2 {

template<>
class binary_serializer<asbr::message_info> {
public:
    static asbr::message_info read(const std::byte* src, const size_t start = 0) {
        asbr::message_info result;
        kojo::logger log{"CC2 Binary Data"};
        
        if (src == nullptr) {
                log.fatal(
                kojo::logger::status::null_pointer,
                "Attempted to load data, but received null memory.",
                "Ensure the data provided is valid and not null."
            );
            return result;
        }
        kojo::binary_view data{src, start};

        result.version = data.read<std::uint32_t>(std::endian::little);
        if (result.version != 1001)
            log.warn(
                kojo::logger::status::version,
                std::format("Expected version `1001` for messageInfo data, but instead got `{}`.", 1001, result.version),
                "Ensure the data matches the same format as version `1001`."
            );
        auto entry_count = data.read<std::uint32_t>(std::endian::little);
        auto first_pointer = data.read<std::uint64_t>(std::endian::little);
        data.change_pos(first_pointer - 8);

        asbr::message_info::entry entry_buffer;
        for (int i = 0; i < entry_count; i++) {
            entry_buffer.crc32_id.load(data.read<std::uint32_t>(std::endian::big));
            data.change_pos(12); // Skip unknown constants.
            auto message_ptr            = data.read<std::uint64_t>(std::endian::little);
            entry_buffer.message        = data.read<std::string_view>(message_ptr - sizeof(message_ptr));
            entry_buffer.ref_crc32_id   = data.read<std::uint32_t>(std::endian::big);
            entry_buffer.is_ref         = data.read<std::int16_t>(std::endian::little);
            entry_buffer.file_index     = data.read<std::int16_t>(std::endian::little);
            entry_buffer.cue_index      = data.read<std::int16_t>(std::endian::little);
            data.change_pos(6); // Skip unknown constants.

            result.entries[entry_buffer.key()] = entry_buffer;
            result.entry_order[entry_buffer.order_key()] = entry_buffer.key();
        }

        return result;
    }
    
    static kojo::binary write(const asbr::message_info& param) {
        kojo::binary output_data;

        

        return output_data;
    }
};

}

#endif // KOJO_CC2_ASBR_BINARY_MESSAGEINFO