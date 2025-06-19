#pragma once
#include <vector>
#include <iterator>

template<typename K, typename V>
class SequenceMap;

namespace detail {
    template<typename K, typename V, typename VecIter>
    class SequenceMapIterator {
    public:
        // Iterator traits (required for standard library compatibility)
        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename std::iterator_traits<VecIter>::value_type;
        using difference_type = typename std::iterator_traits<VecIter>::difference_type;
        using pointer = typename std::iterator_traits<VecIter>::pointer;
        using reference = typename std::iterator_traits<VecIter>::reference;

        SequenceMapIterator() = default;

        // Conversion from non-const to const iterator
        operator SequenceMapIterator<K, V, typename std::vector<value_type>::const_iterator>() const {
            return SequenceMapIterator<K, V, typename std::vector<value_type>::const_iterator>(current_it_);
        }

        reference operator*() const {
            return *current_it_;
        }

        pointer operator->() const {
            return &(*current_it_);
        }

        SequenceMapIterator& operator++() {
            ++current_it_;
            return *this;
        }

        SequenceMapIterator operator++(int) {
            SequenceMapIterator temp = *this;
            ++(*this);
            return temp;
        }

        SequenceMapIterator& operator--() {
            --current_it_;
            return *this;
        }

        SequenceMapIterator operator--(int) {
            SequenceMapIterator temp = *this;
            --(*this);
            return temp;
        }

        SequenceMapIterator operator+(difference_type n) const {
            return SequenceMapIterator(current_it_ + n);
        }

        SequenceMapIterator& operator+=(difference_type n) {
            current_it_ += n;
            return *this;
        }

        SequenceMapIterator operator-(difference_type n) const {
            return SequenceMapIterator(current_it_ - n);
        }

        SequenceMapIterator& operator-=(difference_type n) {
            current_it_ -= n;
            return *this;
        }

        difference_type operator-(const SequenceMapIterator& other) const {
            return current_it_ - other.current_it_;
        }

        bool operator<(const SequenceMapIterator& other) const {
            return current_it_ < other.current_it_;
        }

        bool operator>(const SequenceMapIterator& other) const {
            return current_it_ > other.current_it_;
        }

        bool operator<=(const SequenceMapIterator& other) const {
            return current_it_ <= other.current_it_;
        }

        bool operator>=(const SequenceMapIterator& other) const {
            return current_it_ >= other.current_it_;
        }

        bool operator==(const SequenceMapIterator& other) const {
            return current_it_ == other.current_it_;
        }

        bool operator!=(const SequenceMapIterator& other) const {
            return current_it_ != other.current_it_;
        }
        
    private:
        VecIter current_it_;

        friend class SequenceMap<K,V>;

        // Needed for conversion operator
        template <typename, typename, typename>
        friend class SequenceMapIterator;

        // Private constructor for use by SequenceMap
        SequenceMapIterator(VecIter it) : current_it_(it) {}
    };
}