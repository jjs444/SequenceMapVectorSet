#pragma once

#include <iostream>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <initializer_list>
#include <utility> 

/**
 * @class VectorSet
 * @brief A set-like container implemented using a sorted std::vector.
 *
 * VectorSet provides a subset of the interface of std::set but with different
 * performance characteristics. It stores unique elements in a sorted contiguous
 * array.
 *
 * Performance:
 * - Search: O(log n) - Fast due to binary search and cache locality.
 * - Insertion: O(n) - Slower due to potential element shifting.
 * - Deletion: O(n) - Slower due to potential element shifting.
 * - Iteration: Fast due to contiguous memory.
 * - Memory Usage: Lower than std::set as it avoids per-node overhead.
 *
 * This container is ideal for scenarios where the data is written once or
 * modified infrequently, but read or searched many times.
 *
 * @tparam T The type of the elements.
 * @tparam Compare The comparison function object type, defaults to std::less<T>.
 */
template<typename T, typename Compare = std::less<T>>
class VectorSet {
public:
    using value_type = T;
    using size_type = typename std::vector<T>::size_type;
    using difference_type = typename std::vector<T>::difference_type;
    using value_compare = Compare;
    using reference = typename std::vector<T>::reference;
    using const_reference = typename std::vector<T>::const_reference;
    using pointer = typename std::vector<T>::pointer;
    using const_pointer = typename std::vector<T>::const_pointer;
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using reverse_iterator = typename std::vector<T>::reverse_iterator;
    using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

private:
    std::vector<value_type> data;
    Compare comp;

public:
    VectorSet() = default;
    ~VectorSet() = default;
    VectorSet(VectorSet&& other) noexcept = default;
    VectorSet& operator=(VectorSet&& other) noexcept = default;
    VectorSet(const VectorSet& other) noexcept = default;
    VectorSet& operator=(const VectorSet& other) noexcept = default;

    /**
     * @brief Constructs with a specific comparator.
     * Complexity: O(1)
     */
    explicit VectorSet(const Compare& comp) : comp(comp) {}

    /**
     * @brief Constructs from an initializer list.
     * Ensures elements are sorted and unique.
     * Complexity: O(N log N) where N is the number of elements in the list.
     */
    VectorSet(std::initializer_list<value_type> ilist, const Compare& comp = Compare())
        : data(ilist), comp(comp) {
        std::sort(data.begin(), data.end(), this->comp);
        data.erase(std::unique(data.begin(), data.end()), data.end());
    }

    /**
     * @brief Constructs from a range of iterators.
     * Ensures elements are sorted and unique.
     * Complexity: O(N log N) where N is the distance between first and last.
     */
    template<typename InputIt>
    VectorSet(InputIt first, InputIt last, const Compare& comp = Compare())
        : data(first, last), comp(comp) {
        std::sort(data.begin(), data.end(), this->comp);
        data.erase(std::unique(data.begin(), data.end()), data.end());
    }

    bool empty() const noexcept { return data.empty(); }
    size_type size() const noexcept { return data.size(); }
    size_type max_size() const noexcept { return data.max_size(); }
    void reserve(size_type new_cap) { data.reserve(new_cap); }

    /**
     * @brief Clears the contents of the set.
     * Complexity: O(N)
     */
    void clear() noexcept { data.clear(); }

    /**
     * @brief Constructs an element in-place.
     * Avoids extra copy or move operations when constructing elements directly.
     * Note: A temporary object is created to find the correct insertion position.
     * Complexity: O(log N) for search, O(N) for insertion. Total: O(N).
     * @param args Arguments to forward to the constructor of the element.
     * @return A pair, with its first element an iterator to the new element
     * (or the existing one), and the second a bool indicating whether
     * the insertion took place.
     */
    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        T value_to_insert(std::forward<Args>(args)...);

        iterator it = lower_bound(value_to_insert);

        if (it != end() && !comp(value_to_insert, *it)) {
            // Element that compares equivalent already exists.
            return { it, false };
        }

        iterator new_it = data.insert(it, std::move(value_to_insert));
        return { new_it, true };
    }

    /**
     * @brief Inserts a value into the set (copy version).
     * Delegates to emplace for implementation.
     * Complexity: O(N).
     * @return A pair (iterator, bool) indicating success and position.
     */
    std::pair<iterator, bool> insert(const value_type& value) {
        return emplace(value);
    }

    /**
     * @brief Inserts a value into the set (move version).
     * Delegates to emplace for implementation.
     * Complexity: O(N).
     * @return A pair (iterator, bool) indicating success and position.
     */
    std::pair<iterator, bool> insert(value_type&& value) {
        return emplace(std::move(value));
    }

    /**
     * @brief Erases an element at a given position.
     * Complexity: O(N)
     * @return Iterator following the last removed element.
     */
    iterator erase(const_iterator pos) {
        return data.erase(pos);
    }

    /**
     * @brief Erases elements in a range.
     * Complexity: O(N)
     * @return Iterator following the last removed element.
     */
    iterator erase(const_iterator first, const_iterator last) {
        return data.erase(first, last);
    }

    /**
     * @brief Erases an element with a specific value.
     * Complexity: O(log N) for the search, O(N) for the erase. Total: O(N).
     * @return Number of elements removed (0 or 1).
     */
    size_type erase(const value_type& value) {
        iterator it = find(value);
        if (it != end()) {
            data.erase(it);
            return 1;
        }
        return 0;
    }

    /**
     * @brief Swaps the contents with another VectorSet.
     * Complexity: O(1)
     */
    void swap(VectorSet& other) noexcept {
        data.swap(other.data);
        std::swap(comp, other.comp);
    }

    // --- Lookup ---

    /**
     * @brief Finds an element with a specific value.
     * Complexity: O(log N)
     * @return An iterator to the element, or end() if not found.
     */
    iterator find(const value_type& value) {
        iterator it = lower_bound(value);
        if (it != end() && !comp(value, *it)) {
            return it;
        }
        return end();
    }

    const_iterator find(const value_type& value) const {
        const_iterator it = lower_bound(value);
        if (it != end() && !comp(value, *it)) {
            return it;
        }
        return end();
    }

    /**
     * @brief Checks if the container contains a specific value.
     * Complexity: O(log N)
     */
    bool contains(const value_type& value) const {
        return find(value) != end();
    }

    /**
     * @brief Counts elements with a specific value.
     * Since this is a set, the result is either 0 or 1.
     * Complexity: O(log N)
     */
    size_type count(const value_type& value) const {
        return contains(value) ? 1 : 0;
    }

    /**
     * @brief Returns an iterator to the first element not less than the given value.
     * Complexity: O(log N)
     */
    iterator lower_bound(const value_type& value) {
        return std::lower_bound(data.begin(), data.end(), value, comp);
    }
    const_iterator lower_bound(const value_type& value) const {
        return std::lower_bound(data.begin(), data.end(), value, comp);
    }

    /**
     * @brief Returns an iterator to the first element greater than the given value.
     * Complexity: O(log N)
     */
    iterator upper_bound(const value_type& value) {
        return std::upper_bound(data.begin(), data.end(), value, comp);
    }
    const_iterator upper_bound(const value_type& value) const {
        return std::upper_bound(data.begin(), data.end(), value, comp);
    }

    /**
     * @brief Returns a range of elements matching a specific value.
     * Since this is a set, the range will contain at most one element.
     * Complexity: O(log N)
     */
    std::pair<iterator, iterator> equal_range(const value_type& value) {
        return std::equal_range(data.begin(), data.end(), value, comp);
    }
    std::pair<const_iterator, const_iterator> equal_range(const value_type& value) const {
        return std::equal_range(data.begin(), data.end(), value, comp);
    }

    // --- Iterators ---
    iterator begin() noexcept { return data.begin(); }
    const_iterator begin() const noexcept { return data.begin(); }
    const_iterator cbegin() const noexcept { return data.cbegin(); }

    iterator end() noexcept { return data.end(); }
    const_iterator end() const noexcept { return data.end(); }
    const_iterator cend() const noexcept { return data.cend(); }

    reverse_iterator rbegin() noexcept { return data.rbegin(); }
    const_reverse_iterator rbegin() const noexcept { return data.rbegin(); }
    const_reverse_iterator crbegin() const noexcept { return data.crbegin(); }

    reverse_iterator rend() noexcept { return data.rend(); }
    const_reverse_iterator rend() const noexcept { return data.rend(); }
    const_reverse_iterator crend() const noexcept { return data.crend(); }
};
