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
            ring_buffer* _container = nullptr;
            size_type _index = 0;

        public:
            constexpr _iterator() noexcept = default;
            constexpr explicit _iterator(ring_buffer*, size_type) noexcept;
            _iterator(_iterator const&) = default;
            _iterator& operator=(_iterator const&) = default;
            _iterator(_iterator&&) noexcept = default;
            _iterator& operator=(_iterator&&) noexcept = default;

            constexpr reference operator*() const;

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

    public:
        using iterator = _iterator;
        using const_iterator = T const*;

    private:
        pointer _buf{nullptr};
        size_type _capacity{0};
        size_type _head{0};
        size_type _tail{0};

    public:
        constexpr ring_buffer() noexcept = default;
        explicit ring_buffer(size_type count, T const& value);
        explicit ring_buffer(size_type count);
        //        template<typename InputIt>
        //        ring_buffer(InputIt first, InputIt last);
        ring_buffer(ring_buffer const& other);
        ring_buffer& operator=(ring_buffer const& other);
        ring_buffer(ring_buffer&& other) noexcept;
        ring_buffer& operator=(ring_buffer&& other) noexcept;
        //        ring_buffer(std::initializer_list<T> init);
        //        ring_buffer& operator=(std::initializer_list<T> ilist);
        ~ring_buffer();
        //
        void assign(size_type count, T const& value);
        //        template <typename InputIt>
        //        void assign(InputIt first, InputIt last);
        //        void assign(std::initializer_list<T> ilist);

        reference at(size_type pos);
        const_reference at(size_type pos) const;
        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

        iterator begin() noexcept;
        //        const_iterator begin() const noexcept;
        //        const_iterator cbegin() const noexcept;
        iterator end() noexcept;
        //        const_iterator end() const noexcept;
        //        const_iterator cend() const noexcept;
        //
        [[nodiscard]] constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        void shrink_to_fit();
        constexpr size_type capacity() const noexcept;

        void clear() noexcept;
        //        iterator insert(const_iterator pos, T const& value);
        //        iterator insert(const_iterator pos, T&& value);
        //        iterator insert(const_iterator pos, size_type count, T const& value);
        //        template <typename InputIt>
        //        iterator insert(const_iterator pos, InputIt first, InputIt last);
        //        iterator insert(const_iterator pos, std::initializer_list<T> ilist);
        //        template <typename... Args >
        //        iterator emplace(const_iterator pos, Args&&... args);
        //        iterator erase(const_iterator pos);
        //        iterator erase(const_iterator first, const_iterator last);
        void push_back(T const& value);
        void push_back(T&& value);
        template <typename... Args>
        void emplace_back(Args&&... args);
        void pop_front();
        void reserve(size_type new_cap);
        void resize(size_type count);
        //        void resize(size_type count, value_type const& value);
        //        void swap(ring_buffer& other) noexcept(/* see below */);

    private:
        constexpr size_type _mask(size_type val) const noexcept;
        void _destruct() noexcept;
        void _copy(ring_buffer const& src) noexcept;
        void _assign(ring_buffer const& src) noexcept;
        void _resize(size_type count);
    };

    //    template <typename T>
    //    bool operator==(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    bool operator!=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    bool operator<(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    bool operator<=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    bool operator>(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    bool operator>=(ring_buffer<T> const& lhs, ring_buffer<T> const& rhs);
    //
    //    template <typename T>
    //    void swap(ring_buffer<T>& lhs, ring_buffer<T>& rhs) noexcept(/* see below */);
    //
    //    template <typename T, typename U>
    //    void erase(ring_buffer<T>& c, U const& value);
    //    template <typename T, typename Pred>
    //    void erase_if(ring_buffer<T>& c, Pred&& pred);

    // constructor

    template <typename T>
    ring_buffer<T>::ring_buffer(typename ring_buffer<T>::size_type count, T const& value)
    : _capacity{details::bit::ceil2(count)}
    , _head{count}
    , _tail{0} {
        ::posix_memalign(reinterpret_cast<void**>(&_buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * _capacity);
        details::algorithm::copy_value(_buf, _buf + count, value);
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(size_type count)
    : _capacity{details::bit::ceil2(count)}
    , _head{count}
    , _tail{0} {
        ::posix_memalign(reinterpret_cast<void**>(&_buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * _capacity);
        details::algorithm::construct_placement(_buf, _buf + count);
    }

    template <typename T>
    ring_buffer<T>::ring_buffer(ring_buffer<T> const& other)
    : _capacity{details::bit::ceil2(other.size())}
    , _head{other.size()}
    , _tail{0} {
        ::posix_memalign(reinterpret_cast<void**>(&_buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * _capacity);
        _copy(other);
    }

    template <typename T>
    ring_buffer<T>& ring_buffer<T>::operator=(ring_buffer<T> const& other) {
        _assign(other);
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
    void ring_buffer<T>::assign(typename ring_buffer<T>::size_type count, T const& value) {
        if (count <= _capacity) {
            typename ring_buffer<T>::size_type const tail{_mask(_tail)}, sz{size()}, head{tail + sz}, pivot{tail + count};
            if (sz >= count) {
                details::algorithm::split_copy_assign_value(_buf + tail, _buf + pivot, _buf, _capacity, value);
                details::algorithm::split_destruct(_buf + pivot, _buf + head, _buf, _capacity);
            } else {
                details::algorithm::split_copy_assign_value(_buf + tail, _buf + head, _buf, _capacity, value);
                details::algorithm::split_copy_value(_buf + head, _buf + pivot, _buf, _capacity, value);
            }
            _head = _tail + count;
        } else {
            _destruct();
            ::free(_buf);
            _capacity = details::bit::ceil2(count);
            _head = count;
            _tail = 0;
            ::posix_memalign(reinterpret_cast<void**>(&_buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * _capacity);
            details::algorithm::copy_value(_buf, _buf + count, value);
        }
    }

    // access

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::at(typename ring_buffer<T>::size_type pos) {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::at(typename ring_buffer<T>::size_type pos) const {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::reference ring_buffer<T>::operator[](typename ring_buffer<T>::size_type pos) {
        assert(pos < size()); // out of bounds
        return _buf[_mask(_tail + pos)];
    }

    template <typename T>
    typename ring_buffer<T>::const_reference ring_buffer<T>::operator[](typename ring_buffer<T>::size_type pos) const {
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
    typename ring_buffer<T>::iterator ring_buffer<T>::end() noexcept {
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
        if (typename ring_buffer<T>::size_type const sz{size()}; DEQUE_UNLIKELY(sz == 0)) {
            _destruct();
            ::free(_buf);
            _buf = nullptr;
            _capacity = 0;
            _tail = 0;
            _head = 0;
        } else if (typename ring_buffer<T>::size_type const new_cap {details::bit::ceil2(sz)}; new_cap != _capacity) {
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
    void ring_buffer<T>::reserve(typename ring_buffer<T>::size_type new_cap) {
        if (new_cap > _capacity) {
            new_cap = details::bit::ceil2(new_cap);
            _resize(new_cap);
        }
    }

    template <typename T>
    void ring_buffer<T>::resize(size_type count) {
        typename ring_buffer<T>::size_type const sz{size()};
        if (count < sz) {
            typename ring_buffer<T>::size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_destruct(_buf + pivot, _buf + head, _buf, _capacity);
            _head -= (sz - count);
        } else if (count > sz) {
            reserve(count);
            typename ring_buffer<T>::size_type const tail{_mask(_tail)}, head{tail + sz}, pivot{tail + count};
            details::algorithm::split_construct(_buf + head, _buf + pivot, _buf, _capacity);
            _head += (count - sz);
        }
    }

    // helper

    template <typename T>
    constexpr typename ring_buffer<T>::size_type ring_buffer<T>::_mask(typename ring_buffer<T>::size_type val) const noexcept {
        return val & (_capacity - static_cast<typename ring_buffer<T>::size_type>(1));
    }

    template <typename T>
    void ring_buffer<T>::_destruct() noexcept {
        typename ring_buffer<T>::size_type const tail{_mask(_tail)}, head{tail + size()};
        details::algorithm::split_destruct(_buf + tail, _buf + head, _buf, _capacity);
    }

    template <typename T>
    void ring_buffer<T>::_copy(ring_buffer<T> const& other) noexcept {
        typename ring_buffer<T>::size_type const tail{other._mask(other._tail)}, head{tail + other.size()};
        details::algorithm::split_copy(other._buf + tail, other._buf + head, other._buf, other._capacity, _buf, _buf, _capacity);
    }

    template <typename T>
    void ring_buffer<T>::_assign(ring_buffer<T> const& other) noexcept {
        if (typename ring_buffer<T>::size_type const src_sz{other.size()}; src_sz <= _capacity) {
            typename ring_buffer<T>::size_type const src_tail{other._mask(other._tail)}, src_head{src_tail + src_sz};
            typename ring_buffer<T>::size_type const dst_tail{_mask(_tail)}, dst_sz{size()}, dst_head{dst_tail + dst_sz};

            if (dst_sz >= src_sz) {
                details::algorithm::split_copy_assign(other._buf + src_tail, other._buf + src_tail + src_sz, other._buf, other._capacity, _buf + dst_tail, _buf,
                                                      _capacity);
                details::algorithm::split_destruct(_buf + dst_tail + src_sz, _buf + dst_head, _buf, _capacity);
            } else {
                details::algorithm::split_copy_assign(other._buf + src_tail, other._buf + src_tail + dst_sz, other._buf, other._capacity, _buf + dst_tail, _buf,
                                                      _capacity);
                details::algorithm::split_copy(other._buf + src_tail + dst_sz, other._buf + src_head, other._buf, other._capacity, _buf + dst_tail + dst_sz,
                                               _buf, _capacity);
            }

            _head = _tail + src_sz;
        } else {
            _destruct();
            ::free(_buf);
            _capacity = details::bit::ceil2(src_sz);
            _head = src_sz;
            _tail = 0;
            ::posix_memalign(reinterpret_cast<void**>(&_buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * _capacity);
            _copy(other);
        }
    }

    template <typename T>
    void ring_buffer<T>::_resize(typename ring_buffer<T>::size_type new_cap) {
        assert(new_cap > 0);
        typename ring_buffer<T>::size_type const tail{_mask(_tail)}, sz{details::algorithm::min(new_cap, size())}, head{tail + sz};
        ring_buffer<T>::pointer buf;
        ::posix_memalign(reinterpret_cast<void**>(&buf), details::algorithm::max(alignof(T), sizeof(void*)), sizeof(T) * new_cap);
        details::algorithm::split_move(_buf + tail, _buf + head, _buf, _capacity, buf, buf, new_cap);
        _destruct();
        free(_buf);
        _capacity = new_cap;
        _head = sz;
        _tail = 0;
        _buf = buf;
    }

    template <typename T>
    constexpr ring_buffer<T>::_iterator::_iterator(ring_buffer<T>* container, typename ring_buffer<T>::size_type index) noexcept
    : _container{container}
    , _index{index} {}

    template <typename T>
    constexpr typename ring_buffer<T>::reference ring_buffer<T>::_iterator::_iterator::operator*() const {
        return _container->_buf[_container->_mask(_index)];
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator++() noexcept {
        assert(_index != _container->_head);
        ++_index;
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator--() noexcept {
        assert(_index != _container->_tail);
        --_index;
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator++(int n) noexcept {
        assert(_index + n != _container->_head);
        return {_container, _index + n};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator--(int n) noexcept {
        assert(_index - n != _container->_tail);
        return {_container, _index - n};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator+(typename ring_buffer<T>::difference_type n) const noexcept {
        assert(_index + n != _container->_head);
        return {_container, _index + n};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator ring_buffer<T>::_iterator::operator-(typename ring_buffer<T>::difference_type n) const noexcept {
        assert(_index - n != _container->_tail);
        return {_container, _index - n};
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator+=(typename ring_buffer<T>::difference_type n) noexcept {
        assert(_index + n != _container->_head);
        _index += n;
        return *this;
    }

    template <typename T>
    constexpr typename ring_buffer<T>::_iterator& ring_buffer<T>::_iterator::operator-=(typename ring_buffer<T>::difference_type n) noexcept {
        assert(_index - n != _container->_tail);
        _index -= n;
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

} // namespace wiz
