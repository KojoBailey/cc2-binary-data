#include <cc2/asbr/message_info.hpp>

#include <algorithm>

#include <kojo/binary.hpp>

using namespace cc2::asbr;

void message_info::load_hashlist(std::filesystem::path hashlist_path) {
    kojo::binary hashlist_store{hashlist_path};
    kojo::binary_view hashlist_data{hashlist_store};
    while (hashlist_data.get_pos() < hashlist_store.size()) {
        std::string buffer = hashlist_data.read<std::string_view>().data();
        hashlist[buffer] = hashlist_data.read<std::string_view>();
    }
}

void message_info::sort_keys() {
    sorted_keys.clear();
    sorted_keys.reserve(entries.size());
    for (auto& [key, _] : entries) {
        sorted_keys.push_back(key);
    }

    std::sort(sorted_keys.begin(), sorted_keys.end(),
        [&](std::uint32_t a, std::uint32_t b) {
            return entries.at(a).order_key() < entries.at(b).order_key();
    });
}

void message_info::merge(message_info& param) {
    log.show_debug = true;

    if (language != param.language) {
        log.warn(
            kojo::logger::status::value_mismatch,
            std::format("Language of both message_info objects do not match ({} vs {}).", language, param.language),
            std::format("Assuming this was intentional, the new language, {}, will be used.", param.language)
        );
        language = param.language;
    }

    for (auto& [key, value] : param.entries) {
        if (!entries.contains(key)) {
            entries[key] = value;
            continue;
        }
        auto& entry = entries[key];

        if (value.has_crc32_id)
            entry.crc32_id = value.crc32_id;
        if (value.has_message)
            entry.message = value.message;
        if (value.has_ref_crc32_id)
            entry.ref_crc32_id = value.ref_crc32_id;
        if (value.has_file_index)
            entry.file_index = value.file_index;
        if (value.has_cue_index)
            entry.cue_index = value.cue_index;
    }

    sort_keys();
}