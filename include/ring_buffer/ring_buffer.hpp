#pragma once

extern "C" {
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
}

#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

#include <ring_buffer/details/ceil2.hpp>
#include <ring_buffer/details/config.hpp>
#include <ring_buffer/details/max.hpp>
#include <ring_buffer/details/min.hpp>
#include <ring_buffer/details/split_helper.hpp>

namespace wiz {

    template <typename T>
    class ring_buffer;

    template <typename T>
    bool operator==(ring_buffer<T> const&, ring_buffer<T> const&);

    template <typename T>
    bool operator!=(ring_buffer<T> const&, ring_buffer<T> const&);

    //    template <typename T>
    //    bool operator<(ring_buffer<T> const&, ring_buffer<T> const&);
    //
    //    template <typename T>
    //    bool operator<=(ring_buffer<T> const&, ring_buffer<T> const&);
    //
    //    template <typename T>
    //    bool operator>(ring_buffer<T> const&, ring_buffer<T> const&);
    //
    //    template <typename T>
    //    bool operator>=(ring_buffer<T> const&, ring_buffer<T> const&);

    template <typename T>
    void swap(ring_buffer<T>&, ring_buffer<T>&) noexcept;

    //    template <typename T, typename U>
    //    void erase(ring_buffer<T>& c, U const& value);
    //    template <typename T, typename Pred>
    //    void erase_if(ring_buffer<T>& c, Pred&& pred);

    template <typename T>
    class ring_buffer {
    public:
        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using const_reference = value_type const&;
        using pointer = T*;
        using const_pointer = T const*;

    private:
        class _iterator {
            ring_buffer const* _container{nullptr};
            size_type _index{0};

            friend ring_buffer;

        public:
            constexpr _iterator() noexcept = default;
            constexpr explicit _iterator(ring_buffer const*, size_type) noexcept;
            _iterator(_iterator const&) = default;
            _iterator& operator=(_iterator const&) = default;
            _iterator(_iterator&&) noexcept = default;
            _iterator& operator=(_iterator&&) noexcept = default;

            constexpr reference operator*() const;
            constexpr pointer operator->() const;

            constexpr _iterator& operator++() noexcept;
            constexpr _iterator& operator--() noexcept;
            constexpr _iterator operator++(int) noexcept;
            constexpr _iterator operator--(int) noexcept;
            constexpr _iterator operator+(difference_type) const noexcept;
            constexpr _iterator operator-(difference_type) const noexcept;
            constexpr _iterator& operator+=(difference_type) noexcept;
            constexpr _iterator& operator-=(difference_type) noexcept;

            constexpr bool operator==(_iterator const&) const noexcept;
            constexpr bool operator!=(_iterator const&) const noexcept;

            constexpr bool operator<(_iterator const&) const noexcept;
            constexpr bool operator>(_iterator const&) const noexcept;
            constexpr bool operator<=(_iterator const&) const noexcept;
            constexpr bool operator>=(_iterator const&) const noexcept;
        };

        class _const_iterator {
            _iterator _inner;

            friend ring_buffer;

        public:
            constexpr _const_iterator() noexcept = default;
            constexpr _const_iterator(_iterator&&) noexcept;
            _const_iterator(_const_iterator const&) = default;
            _const_iterator& operator=(_const_iterator const&) = default;
            _const_iterator(_const_iterator&&) noexcept = default;
            _const_iterator& operator=(_const_iterator&&) noexcept = default;

            constexpr const_reference operator*() const;
            constexpr const_pointer operator->() const;

            constexpr _const_iterator& operator++() noexcept;
            constexpr _const_iterator& operator--() noexcept;
            constexpr _const_iterator operator++(int) noexcept;
            constexpr _const_iterator operator--(int) noexcept;
            constexpr _const_iterator operator+(difference_type) const noexcept;
            constexpr _const_iterator operator-(difference_type) const noexcept;
            constexpr _const_iterator& operator+=(difference_type) noexcept;
            constexpr _const_iterator& operator-=(difference_type) noexcept;

            constexpr bool operator==(_const_iterator const&) const noexcept;
            constexpr bool operator!=(_const_iterator const&) const noexcept;

            constexpr bool operator<(_const_iterator const&) const noexcept;
            constexpr bool operator>(_const_iterator const&) const noexcept;
            constexpr bool operator<=(_const_iterator const&) const noexcept;
            constexpr bool operator>=(_const_iterator const&) const noexcept;
        };

        friend bool operator==<T>(ring_buffer const&, ring_buffer const&);
        friend bool operator!=<T>(ring_buffer const&, ring_buffer const&);
        //        friend bool operator< <T>(ring_buffer const&, ring_buffer const&);
        //        friend bool operator<= <T>(ring_buffer const&, ring_buffer const&);
        //        friend bool operator> <T>(ring_buffer const&, ring_buffer const&);
        //        friend bool operator>= <T>(ring_buffer const&, ring_buffer const&);

    public:
        using iterator = _iterator;
        using const_iterator = _const_iterator;

    private:
        pointer _buf{nullptr};
        size_type _capacity{0};
        size_type _head{0};
        size_type _tail{0};

    public:
        constexpr ring_buffer() noexcept = default;
        explicit ring_buffer(size_type, T const&);
        explicit ring_buffer(size_type);
        template <typename InputIt>
        ring_buffer(InputIt*, InputIt*);
        ring_buffer(ring_buffer const&);
        ring_buffer& operator=(ring_buffer const&);
        ring_buffer(ring_buffer&& other) noexcept;
        ring_buffer& operator=(ring_buffer&&) noexcept;
        ring_buffer(std::initializer_list<T>);
        ring_buffer& operator=(std::initializer_list<T>);
        ~ring_buffer();

        void assign(size_type, T const&);
        template <typename InputIt>
        void assign(InputIt*, InputIt*);
        void assign(std::initializer_list<T>);

        reference at(size_type);
        const_reference at(size_type) const;
        reference operator[](size_type);
        const_reference operator[](size_type) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;
        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        [[nodiscard]] constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        void shrink_to_fit();
        constexpr size_type capacity() const noexcept;

        void clear() noexcept;
        iterator erase(const_iterator);
        iterator erase(const_iterator, const_iterator);
        void push_back(T const&);
        void push_back(T&&);
        template <typename... Args>
        void emplace_back(Args&&...);
        void pop_front();
        void reserve(size_type);
        void resize(size_type);
        void resize(size_type, value_type const&);
        void swap(ring_buffer&) noexcept;

    private:
        RING_BUFFER_HIDE_FROM_ABI constexpr size_type _mask(size_type) const noexcept;
        RING_BUFFER_HIDE_FROM_ABI inline void _destruct() noexcept;
        RING_BUFFER_HIDE_FROM_ABI inline void _assign(ring_buffer const&) noexcept;
        template <typename It>
        RING_BUFFER_HIDE_FROM_ABI inline void _assign(It*, It*) noexcept;
        RING_BUFFER_HIDE_FROM_ABI inline void _resize(size_type);
        RING_BUFFER_HIDE_FROM_ABI inline T* _malloc(size_type) noexcept;
    };

    // constructor

    template <typename T>
    ring_buffer<T>::ring_buffer(size_type count, T const& value) {
        if (RING_BUFFER_LIKELY(count > 0)) {
            _capacity = details::bit::ceil2(count);
            _head = count;
            _buf = _malloc(_capacity);
            details::algorithm::copy_placement_value(_buf, _buf + count, value);
        }
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(size_type count) {
        if (RING_BUFFER_LIKELY(count > 0)) {
            _capacity = details::bit::ceil2(count);
            _head = count;
            _buf = _malloc(_capacity);
            details::algorithm::construct_placement(_buf, _buf + count);
        }
    }

    template <typename T>
    template <typename InputIt>
    ring_buffer<T>::ring_buffer(InputIt* first, InputIt* last) {
        if (size_t const sz{static_cast<size_type>(last - first)}; RING_BUFFER_LIKELY(sz > 0)) {
            _capacity = details::bit::ceil2(sz);
            _head = sz;
            _buf = _malloc(_capacity);
            details::algorithm::copy_placement(first, last, _buf);
        }
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(ring_buffer<T> const& other) {
        if (size_t const sz{other.size()}; RING_BUFFER_LIKELY(sz > 0)) {
            _capacity = details::bit::ceil2(sz);
            _head = sz;
            _buf = _malloc(_capacity);
            details::algorithm::split_copy_placement(other._buf + other._tail, other._buf + other._tail + sz, other._buf, other._capacity, _buf);
        }
    }

    template <typename T>
    ring_buffer<T>& ring_buffer<T>::operator=(ring_buffer<T> const& other) {
        _assign(other);
        return *this;
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(std::initializer_list<T> init) {
        if (size_t const sz{init.size()}; RING_BUFFER_LIKELY(sz > 0)) {
            _capacity = details::bit::ceil2(sz);
            _head = sz;
            _buf = _malloc(_capacity);
            details::algorithm::copy_placement(init.begin(), init.end(), _buf);
        }
    }

    template <typename T>
    ring_buffer<T>& ring_buffer<T>::operator=(std::initializer_list<T> ilist) {
        _assign(ilist.begin(), ilist.end());
        return *this;
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(ring_buffer<T>&& other) noexcept
    : _buf{other._buf}
    , _capacity{other._capacity}
    , _head{other._head}
    , _tail{other._tail} {
        other._capacity = 0;
        other._head = 0;
        other._tail = 0;
        other._buf = nullptr;
    }

    template <typename T>
    ring_buffer<T>& ring_buffer<T>::operator=(ring_buffer<T>&& other) noexcept {
        _destruct();
        ::free(_buf);
        _buf = other._buf;
        _capacity = other._capacity;
        _head = other._head;
        _tail = other._tail;
        other._buf = nullptr;
        other._capacity = 0;
        other._head = 0;
        other._tail = 0;
        return *this;
    }

    // destructor

    template <typename T>
    ring_buffer<T>::~ring_buffer() {
        _destruct();
        ::free(_buf);
    }

    template <typename T>
    void ring_buffer<T>::assign(size_type count, T const& value) {
        if (count <= _capacity) {
            size_type const tail{_mask(_tail)}, sz{size()}, head{tail + sz}, pivot{tail + count};
            if (sz >= count) {
                details::algorithm::split_copy_assign_value(_buf + tail, _buf + pivot, _buf, _capacity, value);
                details::algorithm::split_destruct_placement(_buf + pivot, _buf + head, _buf, _capacity);
            } else {
                details::algorithm::split_copy_assign_value(_buf + tail, _buf + head, _buf, _capacity, value);
                details::algorithm::split_copy_placement_value(_buf + head, _buf + pivot, _buf, _capacity, value);
            }
            _head = _tail + count;
        } else {
            _destruct();
            ::free(_buf);
            _capacity = details::bit::ceil2(count);
            _head = count;
            _tail = 0;
            _buf = _malloc(_capacity);
            details::algorithm::copy_placement_value(_buf, _buf + count, value);
        }
    }

    template <typename T>
    void ring_buffer<T>::assign(std::initializer_list<T> ilist) {
        _assign(ilist.begin(), ilist.end());
    }

    template <typename T>
    template <typename InputIt>
    void ring_buffer<T>::assign(InputIt* first, InputIt* last) {
        _assign(first, last);
    }

    // access

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::at(size_type pos) {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::at(size_type pos) const {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::operator[](size_type pos) {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::operator[](size_type pos) const {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::front() {
        assert(size() != 0); // out of bounds
        return _buf[_mask(_tail)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::front() const {
        assert(size() != 0); // out of bounds
        return _buf[_mask(_tail)];
    }

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::back() {
        assert(size() != 0); // out of bounds
        return _buf[_mask(_head - 1)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::back() const {
        assert(size() != 0); // out of bounds
        return _buf[_mask(_head - 1)];
    }

    // iterators

    template <typename T>
    typename ring_buffer<T>::iterator ring_buffer<T>::begin() noexcept {
        return ring_buffer<T>::iterator{this, _tail};
    }

    template <typename T>
    typename ring_buffer<T>::const_iterator ring_buffer<T>::begin() const noexcept {
        return ring_buffer<T>::iterator{this, _tail};
    }

    template <typename T>
    typename ring_buffer<T>::const_iterator ring_buffer<T>::cbegin() const noexcept {
        return ring_buffer<T>::iterator{this, _tail};
    }

    template <typename T>
    typename ring_buffer<T>::iterator ring_buffer<T>::end() noexcept {
        return ring_buffer<T>::iterator{this, _head};
    }

    template <typename T>
    typename ring_buffer<T>::const_iterator ring_buffer<T>::end() const noexcept {
        return ring_buffer<T>::iterator{this, _head};
    }

    template <typename T>
    typename ring_buffer<T>::const_iterator ring_buffer<T>::cend() const noexcept {
        return ring_buffer<T>::iterator{this, _head};
    }

    // capacity

    template <typename T>
    [[nodiscard]] constexpr bool ring_buffer<T>::empty() const noexcept {
        return _tail == _head;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::size_type ring_buffer<T>::size() const noexcept {
        return _head - _tail;
    }

    template <typename T>
    void ring_buffer<T>::shrink_to_fit() {
        if (size_type const sz{size()}; RING_BUFFER_UNLIKELY(sz == 0)) {
            _destruct();
            ::free(_buf);
            _buf = nullptr;
            _capacity = 0;
            _tail = 0;
            _head = 0;
        } else if (size_type const new_cap{details::bit::ceil2(sz)}; new_cap != _capacity) {
            _resize(new_cap);
        }
    }

    template <typename T>
    constexpr typename ring_buffer<T>::size_type ring_buffer<T>::capacity() const noexcept {
        return _capacity;
    }

    template <typename T>
    void ring_buffer<T>::clear() noexcept {
        _destruct();
        _head = 0;
        _tail = 0;
    }

    template <typename T>
    typename ring_buffer<T>::iterator ring_buffer<T>::erase(const_iterator first) {
        size_type const pos{first._inner._index - _tail};
        if (size_type const sz{size()}, tail{_mask(_tail)}, remove{tail + pos}; pos <= (sz - 1) / 2) { // erase from front
            details::algorithm::split_move_backward(_buf + tail, _buf + remove, _buf, _capacity, _buf + remove + 1);
            (_buf + tail)->~T();
            ++_tail;
        } else { // erase from back
            size_type const head{tail + sz};
            pointer i = details::algorithm::split_move(_buf + remove + 1, _buf + head, _buf, _capacity, _buf + remove);
            i->~T();
            --_head;
        }
        return ring_buffer<T>::iterator{this, _tail + pos};
    }

    template <typename T>
    typename ring_buffer<T>::iterator ring_buffer<T>::erase(const_iterator first, const_iterator last) {
        size_type const pos{first._inner._index - _tail};
        if (size_type const sz{size()}, len{last._inner._index - first._inner._index}, tail{_mask(_tail)}, remove{tail + pos}; pos <= (sz - len) / 2) { // erase
                                                                                                                                                        // from
                                                                                                                                                        // front
            details::algorithm::split_move_backward(_buf + tail, _buf + remove, _buf, _capacity, _buf + remove + len);
            details::algorithm::split_destruct_placement(_buf + tail, _buf + tail + len, _buf, _capacity);
            _tail += len;
        } else { // erase from back
            size_type const head{tail + sz};
            details::algorithm::split_move(_buf + remove + len, _buf + head, _buf, _capacity, _buf + remove);
            details::algorithm::split_destruct_placement(_buf + tail + sz - len, _buf + head, _buf, _capacity);
            _head -= len;
        }
        return ring_buffer<T>::iterator{this, _tail + pos};
    }

    template <typename T>
    void ring_buffer<T>::push_back(T const& value) {
        reserve(size() + 1);
        new (_buf + _mask(_head++)) T{value};
    }

    template <typename T>
    void ring_buffer<T>::push_back(T&& value) {
        reserve(size() + 1);
        new (_buf + _mask(_head++)) T{std::move(value)};
    }

    template <typename T>
    template <typename... Args>
    void ring_buffer<T>::emplace_back(Args&&... args) {
        reserve(size() + 1);
        new (_buf + _mask(_head++)) T{std::forward<Args>(args)...};
    }

    template <typename T>
    void ring_buffer<T>::pop_front() {
        assert(!empty());
        (_buf + _mask(_tail++))->~T();
    }

    template <typename T>
    void ring_buffer<T>::reserve(size_type new_cap) {
        if (new_cap > _capacity) {
            _resize(details::bit::ceil2(new_cap));
        }
    }

    template <typename T>
    void ring_buffer<T>::resize(size_type count) {
        if (size_type const sz{size()}; count < sz) {
            size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_destruct_placement(_buf + pivot, _buf + head, _buf, _capacity);
            _head = _tail + count;
        } else if (count > sz) {
            reserve(count);
            size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_construct_placement(_buf + head, _buf + pivot, _buf, _capacity);
            _head = _tail + count;
        }
    }

    template <typename T>
    void ring_buffer<T>::resize(size_type count, value_type const& value) {
        if (size_type const sz{size()}; count < sz) {
            size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_destruct_placement(_buf + pivot, _buf + head, _buf, _capacity);
            _head = _tail + count;
        } else if (count > sz) {
            reserve(count);
            size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_copy_placement_value(_buf + head, _buf + pivot, _buf, _capacity, value);
            _head = _tail + count;
        }
    }

    template <typename T>
    void ring_buffer<T>::swap(ring_buffer& other) noexcept {
        pointer tmp_ptr{other._buf};
        other._buf = _buf;
        _buf = tmp_ptr;
        size_type tmp{other._capacity};
        other._capacity = _capacity;
        _capacity = tmp;
        tmp = other._head;
        other._head = _head;
        _head = tmp;
        tmp = other._tail;
        other._tail = _tail;
        _tail = tmp;
    }

    // helper

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI constexpr typename ring_buffer<T>::size_type ring_buffer<T>::_mask(size_type val) const noexcept {
        return val & (_capacity - static_cast<size_type>(1));
    }

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI inline void ring_buffer<T>::_destruct() noexcept {
        size_type const tail{_mask(_tail)}, head{tail + size()};
        details::algorithm::split_destruct_placement(_buf + tail, _buf + head, _buf, _capacity);
    }

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI inline void ring_buffer<T>::_assign(ring_buffer<T> const& other) noexcept {
        if (size_type const src_sz{other.size()}; src_sz <= _capacity) {
            size_type const src_tail{other._mask(other._tail)}, src_head{src_tail + src_sz};
            size_type const dst_tail{_mask(_tail)}, dst_sz{size()}, dst_head{dst_tail + dst_sz};

            if (dst_sz >= src_sz) {
                details::algorithm::split_copy_assign(other._buf + src_tail, other._buf + src_tail + src_sz, other._buf, other._capacity, _buf + dst_tail, _buf,
                                                      _capacity);
                details::algorithm::split_destruct_placement(_buf + dst_tail + src_sz, _buf + dst_head, _buf, _capacity);
            } else {
                details::algorithm::split_copy_assign(other._buf + src_tail, other._buf + src_tail + dst_sz, other._buf, other._capacity, _buf + dst_tail, _buf,
                                                      _capacity);
                details::algorithm::split_copy_placement(other._buf + src_tail + dst_sz, other._buf + src_head, other._buf, other._capacity,
                                                         _buf + dst_tail + dst_sz, _buf, _capacity);
            }

            _head = _tail + src_sz;
        } else {
            _destruct();
            ::free(_buf);
            _capacity = details::bit::ceil2(src_sz);
            _head = src_sz;
            _tail = 0;
            _buf = _malloc(_capacity);
            details::algorithm::split_copy_placement(other._buf + other._tail, other._buf + other._tail + src_sz, other._buf, other._capacity, _buf);
        }
    }

    template <typename T>
    template <typename It>
    RING_BUFFER_HIDE_FROM_ABI inline void ring_buffer<T>::_assign(It* first, It* last) noexcept {
        if (size_type const src_sz{static_cast<size_type>(last - first)}; src_sz <= _capacity) {
            size_type const dst_tail{_mask(_tail)}, dst_sz{size()}, dst_head{dst_tail + dst_sz};

            if (dst_sz >= src_sz) {
                details::algorithm::split_copy_assign(first, last, _buf + dst_tail, _buf, _capacity);
                details::algorithm::split_destruct_placement(_buf + dst_tail + src_sz, _buf + dst_head, _buf, _capacity);
            } else {
                details::algorithm::split_copy_assign(first, first + dst_sz, _buf + dst_tail, _buf, _capacity);
                details::algorithm::split_copy_placement(first + dst_sz, last, _buf + dst_tail + dst_sz, _buf, _capacity);
            }
            _head = _tail + src_sz;
        } else {
            _destruct();
            ::free(_buf);
            _capacity = details::bit::ceil2(src_sz);
            _head = src_sz;
            _tail = 0;
            _buf = _malloc(_capacity);
            details::algorithm::copy_placement(first, last, _buf);
        }
    }

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI inline void ring_buffer<T>::_resize(size_type new_cap) {
        assert(new_cap > 0);
        size_type const tail{_mask(_tail)}, sz{details::algorithm::min(new_cap, size())}, head{tail + sz};
        ring_buffer<T>::pointer buf = _malloc(new_cap);
        details::algorithm::split_move_placement(_buf + tail, _buf + head, _buf, _capacity, buf);
        _destruct();
        free(_buf);
        _capacity = new_cap;
        _head = sz;
        _tail = 0;
        _buf = buf;
    }

    template <typename T>
    RING_BUFFER_HIDE_FROM_ABI inline T* ring_buffer<T>::_malloc(size_type count) noexcept {
        ring_buffer<T>::pointer buf;
        int const errc = ::posix_memalign(reinterpret_cast<void**>(&buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * count);
        assert(errc == 0);
        if (RING_BUFFER_UNLIKELY(errc != 0)) {
            return nullptr;
        }
        return buf;
    }

    // free function

    template <typename T>
    bool operator==(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
        if (typename ring_buffer<T>::size_type const lhs_sz{lhs.size()}; lhs_sz == rhs.size()) {
            typename ring_buffer<T>::size_type const lhs_tail{lhs._mask(lhs._tail)}, lhs_head{lhs_tail + lhs_sz};
            typename ring_buffer<T>::size_type const rhs_tail{rhs._mask(rhs._tail)};
            return details::algorithm::split_equal(lhs._buf + lhs_tail, lhs._buf + lhs_head, lhs._buf, lhs._capacity, rhs._buf + rhs_tail, rhs._buf, rhs._capacity);
        }
        return false;
    }

    template <typename T>
    bool operator!=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
        return !operator==(lhs, rhs);
    }

    //    template <typename T>
    //    bool operator<(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
    //    }
    //
    //    template <typename T>
    //    bool operator<=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
    //
    //    }
    //
    //    template <typename T>
    //    bool operator>(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
    //
    //    }
    //
    //    template <typename T>
    //    bool operator>=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs) {
    //
    //    }

    template <typename T>
    void swap(ring_buffer<T>& lhs, ring_buffer<T>& rhs) noexcept {
        lhs.swap(rhs);
    }

    // _iterator

    template <typename T>
    constexpr ring_buffer<T>::_iterator::_iterator(ring_buffer<T> const* container, size_type index) noexcept
    : _container{container}
    , _index{index} {}

    template <typename T>
    constexpr typename ring_buffer<T>::reference ring_buffer<T>::_iterator::_iterator::operator*() const {
        return _container->_buf[_container->_mask(_index)];
    }

    template <typename T>
    constexpr typename ring_buffer<T>::pointer ring_buffer<T>::_iterator::_iterator::operator->() const {
        return &operator*();
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator++() noexcept {
        assert(_index + 1 <= _container->_head);
        ++_index;
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator--() noexcept {
        assert(_index >= _container->_tail + 1);
        --_index;
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator++(int) noexcept {
        assert(_index + 1 <= _container->_head);
        typename ring_buffer<T>::_iterator const tmp{*this};
        ++_index;
        return tmp;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator--(int) noexcept {
        assert(_index >= _container->_tail + 1);
        _iterator const tmp{*this};
        --_index;
        return tmp;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator+(difference_type n) const noexcept {
        assert(static_cast<size_type>(static_cast<difference_type>(_index) + n) >= _container->_tail ||
               static_cast<size_type>(static_cast<difference_type>(_index) + n) <= _container->_head);
        return _iterator{_container, static_cast<size_type>(static_cast<difference_type>(_index) + n)};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator-(difference_type n) const noexcept {
        assert(_index >= static_cast<size_type>(static_cast<difference_type>(_container->_tail) + n) ||
               _index <= static_cast<size_type>(static_cast<difference_type>(_container->_head) + n));
        return _iterator{_container, static_cast<size_type>(static_cast<difference_type>(_index) - n)};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator+=(difference_type n) noexcept {
        assert(static_cast<size_type>(static_cast<difference_type>(_index) + n) >= _container->_tail ||
               static_cast<size_type>(static_cast<difference_type>(_index) + n) <= _container->_head);
        _index = static_cast<size_type>(static_cast<difference_type>(_index) + n);
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator-=(difference_type n) noexcept {
        assert(_index >= static_cast<size_type>(static_cast<difference_type>(_container->_tail) + n) ||
               _index <= static_cast<size_type>(static_cast<difference_type>(_container->_head) + n));
        _index = static_cast<size_type>(static_cast<difference_type>(_index) - n);
        return *this;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator==(ring_buffer<T>::_iterator const& other) const noexcept {
        return _index == other._index;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator!=(ring_buffer<T>::_iterator const& other) const noexcept {
        return !operator==(other);
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator<(ring_buffer<T>::_iterator const& other) const noexcept {
        return _index < other._index;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator>(ring_buffer<T>::_iterator const& other) const noexcept {
        return _index > other._index;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator<=(ring_buffer<T>::_iterator const& other) const noexcept {
        return _index <= other._index;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_iterator::operator>=(ring_buffer<T>::_iterator const& other) const noexcept {
        return _index >= other._index;
    }

    // _const_iterator

    template <typename T>
    constexpr ring_buffer<T>::_const_iterator::_const_iterator(ring_buffer<T>::_iterator&& i) noexcept
    : _inner{std::move(i)} {}

    template <typename T>
    constexpr typename ring_buffer<T>::const_reference ring_buffer<T>::_const_iterator::operator*() const {
        return *_inner;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::const_pointer ring_buffer<T>::_const_iterator::operator->() const {
        return _inner.operator->();
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator& ring_buffer<T>::_const_iterator::operator++() noexcept {
        return ++_inner;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator& ring_buffer<T>::_const_iterator::operator--() noexcept {
        return --_inner;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator ring_buffer<T>::_const_iterator::operator++(int) noexcept {
        return _inner++;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator ring_buffer<T>::_const_iterator::operator--(int) noexcept {
        return _inner--;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator ring_buffer<T>::_const_iterator::operator+(difference_type n) const noexcept {
        return _inner + n;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator ring_buffer<T>::_const_iterator::operator-(difference_type n) const noexcept {
        return _inner - n;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator& ring_buffer<T>::_const_iterator::operator+=(difference_type n) noexcept {
        _inner += n;
        return *this;
    }
    template <typename T>
    constexpr typename ring_buffer<T>::_const_iterator& ring_buffer<T>::_const_iterator::operator-=(difference_type n) noexcept {
        _inner -= n;
        return *this;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator==(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner == other._inner;
    }
    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator!=(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner != other._inner;
    }

    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator<(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner < other._inner;
    }
    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator>(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner > other._inner;
    }
    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator<=(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner <= other._inner;
    }
    template <typename T>
    constexpr bool ring_buffer<T>::_const_iterator::operator>=(ring_buffer<T>::_const_iterator const& other) const noexcept {
        return _inner >= other._inner;
    }

} // namespace wiz
