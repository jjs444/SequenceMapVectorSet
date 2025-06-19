#pragma once
#include <utility>
#include <type_traits>

namespace detail {
    template <class T>
    using RemoveConstRef_t = std::remove_const_t<std::remove_reference_t<T>>;

    // assumes _Args have already been RemoveConstRef_t'd
    template <class Key, class... Args>
    struct InPlaceKeyExtract {
        // by default we can't extract the key in the emplace family and must construct a node we might not use
        static constexpr bool Extractable = false;
    };

    template <class Key, class Value>
    struct InPlaceKeyExtract<Key, Key, Value> {
        // if we would call the pair(key, value) constructor family, we can use the first parameter as the key
        static constexpr bool Extractable = true;
        static const Key& Extract(const Key& key, const Value&) noexcept {
            return key;
        }
    };

    template <class Key, class First, class Second>
    struct InPlaceKeyExtract<Key, std::pair<First, Second>> {
        // if we would call the pair(pair<other, other>) constructor family, we can use the pair.first member as the key
        static constexpr bool Extractable = std::is_same_v<Key, RemoveConstRef_t<First>>;
        static const Key& Extract(const std::pair<First, Second>& val) noexcept {
            return val.first;
        }
    };

    // Custom in-place element relocation used when shifting elements after erase.
    // Since the vector's value_type is std::pair<const K, V>, we cannot move the pair as a whole
    // (due to the const key), so std::vector::erase cannot be used directly.
    //
    // Instead, we manually destroy the destination element, copy the key (K), and move the value (V)
    // into the vacated slot using placement new.
    //
    // Note: Only mapped_type elements that are being erased will be destroyed. All other
    // mapped_type elements are moved before target.~value_type(); is called
    //
    // Need to consider exception safety here, Nothing implemented for that yet.
    template <typename VectorType, typename MapType>
    void reconstruct_element_in_place(
        VectorType& data,
        MapType& key_to_index,
        size_t target_idx,
        size_t source_idx)
    {
        using value_type = typename VectorType::value_type;

        auto key_copy = data[source_idx].first; // This can throw

        auto& target = data[target_idx];

        target.~value_type(); // This can throw, very rare though

        key_to_index[key_copy] = target_idx;

        // I don't think this can throw as target memory is already alloced
        new (&target) value_type(std::move(key_copy), std::move(data[source_idx].second));
    }

    template <class Key, class MapType, class VecIteratorType>
    auto find_impl(const Key& key, const MapType& map, VecIteratorType begin, VecIteratorType end) {
        auto it = map.find(key);
        if (it == map.end()) {
            return end;
        }
        return begin + it->second;
    }

    template <class ReferenceType, class VectorType, class Size>
    ReferenceType at_idx_impl(VectorType& data, Size index) {
        if (index >= data.size()) {
            throw std::out_of_range("SequenceMap::atIdx(index): index out of bounds");
        }
        return data[index];
    }

    template <class ReferenceType, class MapType, class VectorType, class Key >
    ReferenceType at_key_impl(const MapType& map, VectorType& data, const Key& key) {
        auto it = map.find(key);
        if (it == map.end()) {
            throw std::out_of_range("SequenceMap::atKey(key): key not found");
        }
        return data[it->second].second;
    }

}