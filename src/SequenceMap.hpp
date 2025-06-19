#pragma once

#include <vector>
#include <unordered_map>
#include <utility>   
#include <stdexcept>  
#include <initializer_list> 
#include <iterator>
#include <optional>

#include "detail/SequenceMapIterator.hpp"
#include "detail/Utilities.hpp"  

template <typename K, typename V>
class SequenceMap {
public:
    using key_type = K;
    using mapped_type = V;
    using value_type = std::pair<const K, V>;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = detail::SequenceMapIterator<key_type, mapped_type, typename std::vector<value_type>::iterator>;
    using const_iterator = detail::SequenceMapIterator<key_type, mapped_type, typename std::vector<value_type>::const_iterator>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    SequenceMap() = default;
    ~SequenceMap() = default;
    SequenceMap(SequenceMap&& other) noexcept = default;
    SequenceMap& operator=(SequenceMap&& other) noexcept = default;
    SequenceMap(const SequenceMap& other) = default;
    SequenceMap& operator=(const SequenceMap& other) = default;

    /**
     * @brief Initializer list constructor. Constructs a SequenceMap with the contents
     * of the initializer list. Elements are inserted in the order they appear.
     * @param init An initializer_list of key-value pairs.
     */
    SequenceMap(std::initializer_list<value_type> init) {
        for (auto&& pair : init) {
            emplace_back(std::move(pair));
        }
    }

    /**
     * @brief Range constructor. Constructs a SequenceMap with elements from the given range.
     * Elements are inserted in the order they appear in the input range.
     * @tparam InputIt A valid input iterator type for a range of key-value pairs.
     * @param first Iterator to the beginning of the range.
     * @param last Iterator to the end of the range.
     */
    template <typename InputIt>
    SequenceMap(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            emplace_back(*it);
        }
    }

    // --- Element Access ---

    /**
     * @brief Accesses the element (value_type) at a specified index.
     * @param index The zero-based index of the element.
     * @return A reference to the value_type (pair<const K, V>) at the given index.
     * @throws std::out_of_range if index is out of bounds.
     * @complexity O(1).
     */
    reference atIdx(size_type index) {
        return detail::at_idx_impl<reference>(data_, index);
    }

    /**
     * @brief Const version of at(size_type index).
     * @param index The zero-based index of the element.
     * @return A const reference to the value_type at the given index.
     * @throws std::out_of_range if index is out of bounds.
     * @complexity O(1).
     */
    const_reference atIdx(size_type index) const {
        return detail::at_idx_impl<const_reference>(data_, index);
    }

    /**
     * @brief Accesses the mapped value associated with a specified key.
     * @param key The key to look up.
     * @return A reference to the mapped value associated with the key.
     * @throws std::out_of_range if key is not found.
     * @complexity Average O(1), Worst O(N) due to hash collisions.
     */
    mapped_type& atKey(const key_type& key) {
        return detail::at_key_impl<mapped_type&>(key_to_index_, data_, key);
    }

    /**
     * @brief Const version of at(const key_type& key).
     * @param key The key to look up.
     * @return A const reference to the mapped value associated with the key.
     * @throws std::out_of_range if key is not found.
     * @complexity Average O(1), Worst O(N) due to hash collisions.
     */
    const mapped_type& atKey(const key_type& key) const {
        return detail::at_key_impl<const mapped_type&>(key_to_index_, data_, key);
    }

    /**
     * @brief Accesses the mapped value at a specified index.
     * Does not perform bounds checking.
     * @param index The zero-based index of the element.
     * @return A reference to the pair at the given index.
     * @complexity O(1).
     */
    reference operator[](size_type index) {
        return data_[index];
    }

    /**
     * @brief Const version of operator[](size_type index).
     * Does not perform bounds checking.
     * @param index The zero-based index of the element.
     * @return A const reference to the mapped value at the given index.
     * @complexity O(1).
     */
    const_reference operator[](size_type index) const {
        return data_[index];
    }

    /**
     * @brief Accesses the mapped value associated with a specified key.
     * If the key does not exist, it inserts a new element with a
     * default-constructed mapped_type and returns a reference to it.
     * @param key The key to look up or insert.
     * @return A reference to the mapped value associated with the key.
     * @complexity Average O(1) (lookup/insertion), Worst O(N) if vector reallocates.
     */
    mapped_type& operator()(const key_type& key) {
        auto keyIdxEmp = key_to_index_.emplace(key, data_.size());
        if (!keyIdxEmp.second) {
            return data_[keyIdxEmp.first->second].second;
        }
        else {
            data_.emplace_back(key, mapped_type{});
            return data_.back().second;
        }
    }

    /**
     * @brief Gets the last element from the container. Undefinded behavior if
     * container is empty.
     * @return A reference to the last pair in the container.
     * @complexity O(1)
     */
    reference back() {
        return data_.back();
    }

    /**
     * @brief const version of back. Undefinded behavior if
     * container is empty.
     * @return A reference to the last pair in the container.
     * @complexity O(1)
     */
    const_reference back() const {
        return data_.back();
    }

    /**
     * @brief Gets the first element from the container. Undefinded behavior if
     * container is empty.
     * @return A reference to the first pair in the container.
     * @complexity O(1)
     */
    reference front() {
        return data_.front();
    }

    /**
     * @brief const version of front. Undefinded behavior if
     * container is empty.
     * @return A reference to the first pair in the container.
     * @complexity O(1)
     */
    const_reference front() const {
        return data_.front();
    }

    // --- Modifiers ---

    /**
     * @brief Inserts a new element (key-value pair) into the container.
     * If the key already exists, the function returns a pair consisting of an
     * iterator to the existing element and false. If the key is new, it inserts
     * the element, and returns a pair consisting of an iterator to the new element and true.
     * @param value The key-value pair to insert.
     * @return A pair, with its member pair::first set to an iterator pointing to either the newly
     * inserted element or to the element with an equivalent key already in the map.
     * The pair::second element in the pair is set to true if a new element was inserted
     * or false if an equivalent key already existed.
     * @complexity Average O(1), Worst O(N) if vector reallocates.
     */
    std::pair<iterator, bool> push_back(const value_type& value) {
        return emplace_back(value);
    }

    /**
     * @brief Inserts a new element (key-value pair) into the container (move version).
     * @param value The key-value pair to insert (moved).
     * @return A pair, with its member pair::first set to an iterator pointing to either the newly
     * inserted element or to the element with an equivalent key already in the map.
     * The pair::second element in the pair is set to true if a new element was inserted
     * or false if an equivalent key already existed.
     * @complexity Average O(1), Worst O(N) if vector reallocates.
     */
    std::pair<iterator, bool> push_back(value_type&& value) {
        return emplace_back(std::forward<value_type>(value));
    }

    /**
     * @brief Constructs an element in place inside the container.
     * @tparam Args Types of the arguments to forward to the constructor of value_type.
     * @param args Arguments to forward to the constructor of the element.
     * @return A pair, with its member pair::first set to an iterator pointing to either the newly
     * inserted element or to the element with an equivalent key already in the map.
     * The pair::second element in the pair is set to true if a new element was inserted
     * or false if an equivalent key already existed.
     * @complexity Average O(1), Worst O(N) if vector reallocates.
     */
    template <class... Args>
    std::pair<iterator, bool> emplace_back(Args&&... args) {

        using KeyExtract = detail::InPlaceKeyExtract<key_type, detail::RemoveConstRef_t<Args>...>;

        if constexpr (KeyExtract::Extractable) {
            auto const& key = KeyExtract::Extract(args...);
            auto keyIdxEmp = key_to_index_.emplace(key, data_.size());
            if (!keyIdxEmp.second) {
                return { iterator(data_.begin() + keyIdxEmp.first->second), false };
            }
            data_.emplace_back(std::forward<Args>(args)...);
            return { iterator(data_.end() - 1), true };
        }
        else {
            value_type temp_val(std::forward<Args>(args)...);
            auto keyIdxEmp = key_to_index_.emplace(temp_val.first, data_.size());
            if (!keyIdxEmp.second) {
                return { iterator(data_.begin() + keyIdxEmp.first->second), false };
            }
            data_.emplace_back(value_type(temp_val.first, std::move(temp_val.second)));
            return { iterator(data_.end() - 1), true };
        }
    }

    /**
     * @brief Inserts an element at the given position if the key does not already exist.
     * Shifts elements to the right. Invalidates iterators at or after pos.
     * @param pos Iterator position to insert before.
     * @param value The value_type (pair<const K, V>) to insert.
     * @return A pair of (iterator to element, true if inserted).
    */
    std::pair<iterator, bool> insert(const_iterator pos, const value_type& value) {
        return insert_impl(pos, value);
    }

    /**
     * @brief Rvalue overload for inserting at a specific position.
     */
    std::pair<iterator, bool> insert(const_iterator pos, value_type&& value) {
        return insert_impl(pos, std::move(value));
    }


    /**
     * @brief Inserts a range of elements at the given position, skipping duplicates.
     * @param pos Iterator position to insert before.
     * @param first Beginning of input range.
     * @param last End of input range.
     * @return A pair (iterator to first inserted element, count of elements inserted).
     */
    template <typename InputIt>
    std::pair<iterator, size_t> insert(const_iterator pos, InputIt first, InputIt last) {
        size_t inserted_count = 0;
        std::optional<size_t> result_idx;

        for (; first != last; ++first) {
            auto [it, inserted] = insert(pos, *first);
            if (inserted) {
                ++inserted_count;
                if (!result_idx) {
                    result_idx = std::distance(data_.begin(), it.current_it_); // capture first successful insert
                }
                // Advance position for next insert
                pos = it;
                ++pos;
            }
        }

        return { result_idx ? iterator(data_.begin() + *result_idx) : end(), inserted_count };
    }


    /**
     * @brief Erases the element at the specified position.
     * @param pos An iterator to the element to erase.
     * @return An iterator pointing to the element immediately following the erased element.
     * @complexity O(N) where N is the number of elements in the map.
     * This is because erasing from the vector shifts subsequent
     * elements, requiring all their indices in key_to_index_ to be updated.
     */
    iterator erase(iterator pos) {
        if (pos == end()) {
            return end(); // Cannot erase end iterator
        }
        auto const index_to_erase = std::distance(data_.begin(), pos.current_it_);

        // Remove key from map
        key_to_index_.erase(data_[index_to_erase].first);

        // Shift elements left to fill the gap. Can't call vector::erase because the 
        // std::pair<const K, V> value type cannot be moved.
        for (size_t i = index_to_erase + 1; i < data_.size(); ++i) {
            detail::reconstruct_element_in_place(data_, key_to_index_, i - 1, i);
        }

        // Remove the last element (now duplicated)
        data_.pop_back();

        if (index_to_erase >= data_.size()) {
            return end();
        }

        return iterator(data_.begin() + index_to_erase);
    }

    /**
     * @brief Erases the elements in the range [first, last).
     * @param first An iterator to the first element to erase.
     * @param last An iterator to the element after the last one to erase.
     * @return An iterator pointing to the element immediately following the last erased element.
     * @complexity O(N) where N is the number of elements in the map.
     * Similar to single element erase, requires re-indexing.
     */
    iterator erase(iterator first, iterator last) {
        if (first == last) {
            return first;
        }

        auto const start_idx = std::distance(data_.begin(), first.current_it_);
        auto const end_idx = std::distance(data_.begin(), last.current_it_);

        // Erase keys in the range from the index
        for (size_t i = start_idx; i < end_idx; ++i) {
            key_to_index_.erase(data_[i].first);
        }

        // Move elements down to fill the gap
        auto const shift_count = end_idx - start_idx;
        auto const new_size = data_.size() - shift_count;

        for (size_t i = end_idx; i < data_.size(); ++i) {
            detail::reconstruct_element_in_place(data_, key_to_index_, i - shift_count, i);
        }

        // Remove the now-duplicate trailing elements
        data_.resize(new_size);

        // If erase went to the end, return end()
        if (start_idx >= data_.size()) {
            return end();
        }

        return iterator(data_.begin() + start_idx);
    }

    /**
     * @brief Erases the element associated with the specified key.
     * @param key The key of the element to erase.
     * @return The number of elements erased (0 or 1).
     * @complexity Average O(N) where N is the number of elements in the map.
     * The lookup is O(1) on average, but the underlying vector erase
     * and subsequent index updates are O(N).
     */
    size_type erase(const key_type& key) {
        auto it = key_to_index_.find(key);
        if (it == key_to_index_.end()) {
            return 0; // Key not found
        }
        erase(iterator(data_.begin() + it->second));
        return 1;
    }

    /**
     * @brief Removes the last element from the container. Undefinded behavior if
     * container is empty.
     * @complexity O(1).
     */
    void pop_back() {
        key_to_index_.erase(data_.back().first);
        data_.pop_back();
    }


    /**
     * @brief Removes all elements from the container.
     * @complexity O(N).
     */
    void clear() noexcept {
        data_.clear();
        key_to_index_.clear();
    }

    // --- Capacity ---

    /**
     * @brief Returns the number of elements in the container.
     * @complexity O(1).
     */
    size_type size() const noexcept {
        return data_.size();
    }

    /**
     * @brief Returns the maximum number of elements the container can hold.
     * @complexity O(1).
     */
    size_type max_size() const noexcept {
        return data_.max_size();
    }

    /**
     * @brief Checks if the container has no elements.
     * @complexity O(1).
     */
    bool empty() const noexcept {
        return data_.empty();
    }

    /**
     * @brief Requests that the vector capacity be at least enough to contain n elements.
     * @param new_cap New capacity.
     * @complexity Average O(N) if reallocation occurs.
     */
    void reserve(size_type new_cap) {
        data_.reserve(new_cap);
    }

    /**
     * @brief Returns the total number of elements that the vector can currently hold.
     * @complexity O(1).
     */
    size_type capacity() const noexcept {
        return data_.capacity();
    }

    // --- Lookup ---

    /**
     * @brief Returns the number of elements with the specified key.
     * Since keys are unique, this will return 1 if the key exists, 0 otherwise.
     * @param key The key to search for.
     * @return 1 if the key exists, 0 otherwise.
     * @complexity Average O(1), Worst O(N) due to hash collisions.
     */
    size_type count(const key_type& key) const {
        return key_to_index_.count(key);
    }

    /**
     * @brief Finds an element with the specified key.
     * @param key The key to search for.
     * @return An iterator to the element with the specified key, or end() if not found.
     * @complexity Average O(1), Worst O(N) due to hash collisions.
     */
    iterator find(const key_type& key) {
        return iterator(detail::find_impl(key, key_to_index_, data_.begin(), data_.end()));
    }

    /**
     * @brief Const version of find(const key_type& key).
     * @param key The key to search for.
     * @return A const iterator to the element with the specified key, or cend() if not found.
     * @complexity Average O(1), Worst O(N) due to hash collisions.
     */
    const_iterator find(const key_type& key) const {
        return const_iterator(detail::find_impl(key, key_to_index_, data_.cbegin(), data_.cend()));
    }

    iterator begin() noexcept {
        return iterator(data_.begin());
    }

    const_iterator begin() const noexcept {
        return const_iterator(data_.begin());
    }

    const_iterator cbegin() const noexcept {
        return const_iterator(data_.cbegin());
    }

    iterator end() noexcept {
        return iterator(data_.end());
    }

    const_iterator end() const noexcept {
        return const_iterator(data_.end());
    }

    const_iterator cend() const noexcept {
        return const_iterator(data_.cend());
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    private:

    std::vector<value_type> data_;
    std::unordered_map<key_type, size_type> key_to_index_;


    template <typename T>
    std::pair<iterator, bool> insert_impl(const_iterator pos, T&& value) {
        const key_type& key = value.first;

        auto existing = key_to_index_.find(key);
        if (existing != key_to_index_.end()) {
            return { iterator(data_.begin() + existing->second), false };
        }

        const size_t insert_pos = std::distance(data_.cbegin(), pos.current_it_);

        // Grow the container by one
        data_.emplace_back(); // Placeholder at the end

        // Shift elements right one slot from insert_pos to end()-2
        for (size_t i = data_.size() - 1; i > insert_pos; --i) {
            detail::reconstruct_element_in_place(data_, key_to_index_, i, i-1);
        }

        // Now insert the new value in-place at insert_pos
        data_[insert_pos].~value_type();
        new (&data_[insert_pos]) value_type(std::forward<T>(value));
        key_to_index_[key] = insert_pos;

        return { iterator(data_.begin() + insert_pos), true };
    }

};
