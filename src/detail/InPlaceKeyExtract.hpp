#pragma once
#include <utility>
#include <type_traits>

namespace detail {
    template <class _Ty>
    using RemoveConstRef_t = std::remove_const_t<std::remove_reference_t<_Ty>>;

    // assumes _Args have already been RemoveConstRef_t'd
    template <class _Key, class... _Args>
    struct InPlaceKeyExtract {
        // by default we can't extract the key in the emplace family and must construct a node we might not use
        static constexpr bool Extractable = false;
    };

    template <class _Key, class _Second>
    struct InPlaceKeyExtract<_Key, _Key, _Second> {
        // if we would call the pair(key, value) constructor family, we can use the first parameter as the key
        static constexpr bool Extractable = true;
        static const _Key& Extract(const _Key& _Val, const _Second&) noexcept {
            return _Val;
        }
    };

    template <class _Key, class _First, class _Second>
    struct InPlaceKeyExtract<_Key, std::pair<_First, _Second>> {
        // if we would call the pair(pair<other, other>) constructor family, we can use the pair.first member as the key
        static constexpr bool Extractable = std::is_same_v<_Key, RemoveConstRef_t<_First>>;
        static const _Key& Extract(const std::pair<_First, _Second>& _Val) noexcept {
            return _Val.first;
        }
    };
}