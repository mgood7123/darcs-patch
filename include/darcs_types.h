#ifndef DARCS_PATCH_TYPES_H
#define DARCS_PATCH_TYPES_H

#include <deque>
#include <optional>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <map>

#include <string_adapter.h>
#include <list>
#include <forward_list>
#include <memory>

#include <sha1.h>

/*

-- enabling stuff seems to mess with the haskell debugger
:set -v3
:load darcs/MemHunk.hs
:set stop :list
import MemHunk
:b makeHunk
:step memhunk

*/

namespace DarcsPatch {
    template <typename T1, typename T2>
    struct Tuple2 :
        public StringAdapter::Comparable<Tuple2<T1, T2>>,
        public StringAdapter::Hashable<Tuple2<T1, T2>>
    {
        using THIS = Tuple2<T1, T2>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Tuple2() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::v1, &THIS::v2); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::v1, &THIS::v2); })
        {}

        T1 v1;
        T2 v2;

        Tuple2(const std::pair<T1, T2> & pair) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::v1, &THIS::v2); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::v1, &THIS::v2); }),
            v1(pair.first),
            v2(pair.second)
        {}

        Tuple2(const T1 & t1, const T2 & t2) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::v1, &THIS::v2); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::v1, &THIS::v2); }),
            v1(t1),
            v2(t2)
        {
        }

        void to_string() const {
            std::cout << *this;
        }

        void to_string() {
            std::cout << *this;
        }
    };

    template <typename T1, typename T2>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple2<T1, T2> & item) {
        os << "{ Tuple2, v1 = " << item.v1 << ", v2 = " << item.v2 << " }";
        return os;
    }

    template <typename T1, typename T2>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple2<T1*, T2> & item) {
        os << "{ Tuple2, v1 = " << *item.v1 << ", v2 = " << item.v2 << " }";
        return os;
    }

    template <typename T1, typename T2>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple2<T1, T2*> & item) {
        os << "{ Tuple2, v1 = " << item.v1 << ", v2 = " << *item.v2 << " }";
        return os;
    }

    template <typename T1, typename T2>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple2<T1*, T2*> & item) {
        os << "{ Tuple2, v1 = " << *item.v1 << ", v2 = " << *item.v2 << " }";
        return os;
    }
}

namespace DarcsPatch {
    template <typename T1, typename T2, typename T3>
    struct Tuple3 :
        public StringAdapter::Comparable<Tuple3<T1, T2, T3>>,
        public StringAdapter::Hashable<Tuple3<T1, T2, T3>>
    {
        using THIS = Tuple3<T1, T2, T3>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Tuple3() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::v1, &THIS::v2, &THIS::v3); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::v1, &THIS::v2, &THIS::v3); })
        {}

        T1 v1;
        T2 v2;
        T3 v3;

        Tuple3(const T1 & t1, const T2 & t2, const T3 & t3) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::v1, &THIS::v2, &THIS::v3); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::v1, &THIS::v2, &THIS::v3); }),
            v1(t1),
            v2(t2),
            v3(t3)
        {}

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }
    };

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1, T2, T3> & item) {
        os << "{ Tuple3, v1 = " << item.v1 << ", v2 = " << item.v2 << ", v3 = " << item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1*, T2, T3> & item) {
        os << "{ Tuple3, v1 = " << *item.v1 << ", v2 = " << item.v2 << ", v3 = " << item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1, T2*, T3> & item) {
        os << "{ Tuple3, v1 = " << item.v1 << ", v2 = " << *item.v2 << ", v3 = " << item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1, T2, T3*> & item) {
        os << "{ Tuple3, v1 = " << item.v1 << ", v2 = " << item.v2 << ", v3 = " << *item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1*, T2*, T3> & item) {
        os << "{ Tuple3, v1 = " << *item.v1 << ", v2 = " << *item.v2 << ", v3 = " << item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1*, T2, T3*> & item) {
        os << "{ Tuple3, v1 = " << *item.v1 << ", v2 = " << item.v2 << ", v3 = " << *item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1, T2*, T3*> & item) {
        os << "{ Tuple3, v1 = " << item.v1 << ", v2 = " << *item.v2 << ", v3 = " << *item.v3 << " }";
        return os;
    }

    template <typename T1, typename T2, typename T3>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Tuple3<T1*, T2*, T3*> & item) {
        os << "{ Tuple3, v1 = " << *item.v1 << ", v2 = " << *item.v2 << ", v3 = " << *item.v3 << " }";
        return os;
    }
}

namespace DarcsPatch {
    template <typename T> struct FL;
    template <typename T> struct RL;

    struct NilFL_T {
        template <typename T>
        const FL<T> push(const T & value) const;
    };

    struct NilRL_T {
        template <typename T>
        const RL<T> push(const T & value) const;
    };

    extern const NilFL_T NilFL;
    extern const NilRL_T NilRL;

    template <typename T, typename O>
    class Slice :
        public StringAdapter::Comparable<Slice<T, O>>,
        public StringAdapter::Hashable<Slice<T, O>>
    {
        using THIS = Slice<T, O>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        O * origin = nullptr;
        std::size_t start_ = 0;
        std::size_t end_ = 0;

        public:

        Slice() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, T>(a); })
        {}

        Slice(O* origin, std::size_t start, std::size_t end) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, T>(a); }),
            origin(origin),
            start_(start),
            end_(start < end ? end : start)
        {
        }

        ~Slice() {
        }

        using iterator = IndexedIterator::iterator<O, T>;

        iterator begin() {
            return iterator(origin, start_);
        }

        iterator end() {
            return iterator(origin, start_ == end_ ? start_ : end_);
        }

        using const_iterator = IndexedIterator::iterator<const O, const T>;

        const const_iterator cbegin() const {
            return const_iterator(origin, start_);
        }

        const const_iterator cend() const {
            return const_iterator(origin, start_ == end_ ? start_ : end_);
        }

        const const_iterator begin() const {
            return cbegin();
        }

        const const_iterator end() const {
            return cend();
        }

        T & operator[] (const std::size_t index) {
            return origin->operator[](start_ + index);
        }

        const std::size_t get_start() {
            return start_;
        };

        const std::size_t get_end() {
            return end_;
        };

        const std::size_t size() const {
            return end_ - start_;
        }

        O* get_origin() {
            return origin;
        }

        protected:
        O* get_origin_() {
            return origin;
        };
    };
    
    template <typename T, typename O>
    class CSlice :
        public StringAdapter::Comparable<CSlice<T, O>>,
        public StringAdapter::Hashable<CSlice<T, O>>
    {
        using THIS = CSlice<T, O>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        const O * origin = nullptr;
        std::size_t start_ = 0;
        std::size_t end_ = 0;

        public:

        CSlice() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, T>(a); })
        {}

        CSlice(const O* origin, const std::size_t start, const std::size_t end) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, T>(a); }),
            origin(origin),
            start_(start),
            end_(start < end ? end : start)
        {
        }

        ~CSlice() {
        }

        using iterator = IndexedIterator::iterator<O, T>;

        iterator begin() {
            return iterator(origin, start_);
        }

        iterator end() {
            return iterator(origin, start_ == end_ ? start_ : end_);
        }

        using const_iterator = IndexedIterator::iterator<const O, const T>;

        const const_iterator cbegin() const {
            return const_iterator(origin, start_);
        }

        const const_iterator cend() const {
            return const_iterator(origin, start_ == end_ ? start_ : end_);
        }

        const const_iterator begin() const {
            return cbegin();
        }

        const const_iterator end() const {
            return cend();
        }

        const std::size_t get_start() {
            return start_;
        };

        const std::size_t get_end() {
            return end_;
        };

        const T & operator[] (const std::size_t index) const {
            return origin->operator[](start_ + index);
        }

        const std::size_t size() const {
            return end_ - start_;
        }

        const O* get_origin() {
            return origin;
        }

        protected:
        const O* get_origin_() {
            return origin;
        };
    };

    template <typename T>
    struct FL_BASE {
        
        typedef T TYPE;
        mutable std::forward_list<T> list;
        mutable std::size_t len;
        
        FL_BASE() {
            list = {};
            len = 0;
        }

        FL_BASE(const FL_BASE & other) {
            list = other.list;
            len = other.len;
        }
        
        const FL_BASE & operator =(const FL_BASE & other) {
            list = other.list;
            len = other.len;
            return *this;
        }
        
        FL_BASE(FL_BASE && other) {
            list = std::move(other.list);
            len = std::move(other.len);
        }
        
        const FL_BASE & operator =(FL_BASE && other) {
            list = std::move(other.list);
            len = std::move(other.len);
            return *this;
        }

        using iterator = IndexedIterator::iterator<FL_BASE<T>, T>;
        using const_iterator = IndexedIterator::iterator<const FL_BASE<T>, const T>;

        iterator begin() {
            return iterator(this, 0);
        }

        iterator end() {
            return iterator(this, length()+1);
        }

        const const_iterator cbegin() const {
            return const_iterator(this, 0);
        }

        const const_iterator cend() const {
            return const_iterator(this, length()+1);
        }

        const const_iterator begin() const {
            return cbegin();
        }

        const const_iterator end() const {
            return cend();
        }

        Slice<T, FL_BASE<T>>* slice(std::size_t start, std::size_t end) {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new Slice<T, FL_BASE<T>>(this, start, end);
        }
        const CSlice<T, FL_BASE<T>>* cslice(std::size_t start, std::size_t end) {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new CSlice<T, FL_BASE<T>>(this, start, end);
        }
        const CSlice<T, FL_BASE<T>>* slice(std::size_t start, std::size_t end) const {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new CSlice<T, FL_BASE<T>>(this, start, end);
        }
        
        T & get_item_at_index(const std::size_t index) {
            if (index >= len) {
                throw std::runtime_error("INDEX OUT OF RANGE");
            }
            return *std::next(list.begin(), index);
        }
        const T & get_item_at_index(const std::size_t index) const {
            if (index >= len) {
                throw std::runtime_error("INDEX OUT OF RANGE");
            }
            return *std::next(list.begin(), index);
        }
        
        T & operator[] (const std::size_t index) {
            return get_item_at_index(index);
        };

        const T & operator[] (const std::size_t index) const {
            return get_item_at_index(index);
        };
        
        const std::size_t length() const {
            return len;
        }
        
        const std::size_t size() const {
            return length();
        }
        
        const bool operator == (const FL_BASE<T> & other) const {
            const std::size_t max = length();
            if (max != other.length()) {
                return false;
            }
            for (std::size_t i = 0; i < max; i++) {
                if (operator[](i) != other.operator[](i)) {
                    return false;
                }
            }
            return true;
        }

        const bool operator != (const FL_BASE<T> & other) const {
            return !(*this == other);
        }

        void emplace(const T & item) {
            list.emplace_front(item);
            len++;
        }

        ~FL_BASE() {
        }
    };

    template <typename T>
    struct RL_BASE {
        typedef T TYPE;
        mutable std::list<T> list;
        mutable std::size_t len;
        
        RL_BASE() {
            list = {};
            len = 0;
        }

        RL_BASE(const RL_BASE & other) {
            list = other.list;
            len = other.len;
        }
        
        const RL_BASE & operator =(const RL_BASE & other) {
            list = other.list;
            len = other.len;
            return *this;
        }
        
        RL_BASE(RL_BASE && other) {
            list = std::move(other.list);
            len = std::move(other.len);
        }
        
        const RL_BASE & operator =(RL_BASE && other) {
            list = std::move(other.list);
            len = std::move(other.len);
            return *this;
        }

        using iterator = IndexedIterator::iterator<RL_BASE<T>, T>;
        using const_iterator = IndexedIterator::iterator<const RL_BASE<T>, const T>;

        iterator begin() {
            return iterator(this, 0);
        }

        iterator end() {
            return iterator(this, length()+1);
        }

        const const_iterator cbegin() const {
            return const_iterator(this, 0);
        }

        const const_iterator cend() const {
            return const_iterator(this, length()+1);
        }

        const const_iterator begin() const {
            return cbegin();
        }

        const const_iterator end() const {
            return cend();
        }

        Slice<T, RL_BASE<T>>* slice(std::size_t start, std::size_t end) {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new Slice<T, RL_BASE<T>>(this, start, end);
        }
        const CSlice<T, RL_BASE<T>>* cslice(std::size_t start, std::size_t end) {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new CSlice<T, RL_BASE<T>>(this, start, end);
        }
        const CSlice<T, RL_BASE<T>>* slice(std::size_t start, std::size_t end) const {
            if (start == 0 && end == 0 && len == 0) {
            } else {
                if (start >= len) {
                    return slice(0, 0);
                }
                if (end > len) {
                    throw std::runtime_error("ATTEMPTING TO SLICE OUT OF RANGE");
                }
            }
            return new CSlice<T, RL_BASE<T>>(this, start, end);
        }

        T & get_item_at_index(const std::size_t index) {
            if (index >= len) {
                throw std::runtime_error("INDEX OUT OF RANGE");
            }
            return *std::next(list.begin(), index);
        }
        const T & get_item_at_index(const std::size_t index) const {
            if (index >= len) {
                throw std::runtime_error("INDEX OUT OF RANGE");
            }
            return *std::next(list.begin(), index);
        }
        
        T & operator[] (const std::size_t index) {
            return get_item_at_index(index);
        };

        const T & operator[] (const std::size_t index) const {
            return get_item_at_index(index);
        };
        
        const std::size_t length() const {
            return len;
        }
        
        const std::size_t size() const {
            return length();
        }
        
        const bool operator == (const RL_BASE<T> & other) const {
            const std::size_t max = length();
            if (max != other.length()) {
                return false;
            }
            for (std::size_t i = 0; i < max; i++) {
                if (operator[](i) != other.operator[](i)) {
                    return false;
                }
            }
            return true;
        }

        const bool operator != (const RL_BASE<T> & other) const {
            return !(*this == other);
        }

        void emplace(const T & item) {
            list.emplace_back(item);
            len++;
        }

        ~RL_BASE() {
        }
    };

    #define USING_FL_RL_COMMON(BASE, BASE_CONSTRUCTOR) \
        COMPARABLE_USING_BASE(INDEXED_ITERATOR_EMBED_COMMAS(BASE)); \
        HASHABLE_USING_BASE(INDEXED_ITERATOR_EMBED_COMMAS(BASE)); \
        using BASE::BASE_CONSTRUCTOR; \
        using BASE::base; \
        using BASE::slice; \
        using BASE::to_string; \
        using BASE::operator []; \
        using BASE::begin; \
        using BASE::end; \
        using BASE::cbegin; \
        using BASE::cend; \
        using BASE::size; \
        using BASE::push; \
        using BASE::insert; \
        using BASE::erase; \
        using BASE::print; \

    template <typename CLASS, typename CLASS_BASE, typename NILL_CLASS, typename T>
    struct FL_RL_COMMON :
        public StringAdapter::Comparable<CLASS>,
        public StringAdapter::Hashable<CLASS>
    {
        std::shared_ptr<CLASS_BASE> base;
        Slice<T, CLASS_BASE> * slice = nullptr;

        COMPARABLE_USING_BASE(StringAdapter::Comparable<CLASS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<CLASS>);

        FL_RL_COMMON() :
            CLASS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &CLASS::cbegin, &CLASS::cend); }),
            CLASS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<CLASS, T>(a); })
        {
            base = std::make_shared<CLASS_BASE>();
            slice = base->slice(0, 0);
        }

        FL_RL_COMMON(const std::initializer_list<T> & list) :
            CLASS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &CLASS::cbegin, &CLASS::cend); }),
            CLASS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<CLASS, T>(a); })
        {
            base = std::make_shared<CLASS_BASE>();
            std::size_t size = 0;
            for(const T & item : list) {
                base->emplace(item);
                size++;
            }
            slice = base->slice(0, size);
        }

        FL_RL_COMMON(const NILL_CLASS & nil) : FL_RL_COMMON() {}

        ~FL_RL_COMMON() {
            delete slice;
            slice = nullptr;
        }

        const bool operator == (const NILL_CLASS & other) const {
            return slice->get_start() == 0 && slice->get_end() == 0;
        }

        const bool operator != (const NILL_CLASS & other) const {
            return !(*this == other);
        }

        FL_RL_COMMON(const FL_RL_COMMON & other) : StringAdapter::Comparable<CLASS>(other), StringAdapter::Hashable<CLASS>(other) {
            base = other.base;
            if (slice == nullptr) {
                slice = base->slice(0, 0);
            }
            *slice = *other.slice;
        }

        FL_RL_COMMON(FL_RL_COMMON && other) : StringAdapter::Comparable<CLASS>(std::move(other)), StringAdapter::Hashable<CLASS>(std::move(other)) {
            base = std::move(other.base);
            if (slice == nullptr) {
                slice = base->slice(0, 0);
            }
            *slice = *other.slice;
        }

        FL_RL_COMMON & operator =(const FL_RL_COMMON & other) {
            StringAdapter::Comparable<CLASS>::operator =(other);
            StringAdapter::Hashable<CLASS>::operator =(other);
            base = other.base;
            if (slice == nullptr) {
                slice = base->slice(0, 0);
            }
            *slice = *other.slice;
            return *this;
        }

        FL_RL_COMMON & operator =(FL_RL_COMMON && other) {
            StringAdapter::Comparable<CLASS>::operator =(std::move(other));
            StringAdapter::Hashable<CLASS>::operator =(std::move(other));
            base = std::move(other.base);
            if (slice == nullptr) {
                slice = base->slice(0, 0);
            }
            *slice = *other.slice;
            return *this;
        }

        void to_string() const {
            std::cout << *static_cast<const CLASS*>(this);
        }
        
        void to_string() {
            std::cout << *static_cast<const CLASS*>(this);
        }

        T & operator[] (const std::size_t index) {
            return slice->operator[](index);
        }

        const T & operator[] (const std::size_t index) const {
            return slice->operator[](index);
        }

        using iterator = typename Slice<T, CLASS_BASE>::iterator;
        using const_iterator = typename Slice<T, CLASS_BASE>::const_iterator;

        iterator begin() {
            return slice->begin();
        }

        iterator end() {
            return slice->end();
        }

        const const_iterator cbegin() const {
            return slice->cbegin();
        }

        const const_iterator cend() const {
            return slice->cend();
        }

        const const_iterator begin() const {
            return cbegin();
        }

        const const_iterator end() const {
            return cend();
        }

        // using reverse_iterator = typename Slice<T, FL_BASE<T>>::reverse_iterator;
        // using const_reverse_iterator = typename Slice<T, FL_BASE<T>>::const_reverse_iterator;

        // reverse_iterator rbegin() {
        //     return slice->rbegin();
        // }

        // reverse_iterator rend() {
        //     return slice->rend();
        // }

        // const const_reverse_iterator crbegin() const {
        //     return slice->crbegin();
        // }

        // const const_reverse_iterator crend() const {
        //     return slice->crend();
        // }

        // const const_reverse_iterator rbegin() const {
        //     return crbegin();
        // }

        // const const_reverse_iterator rend() const {
        //     return crend();
        // }

        const std::size_t size() const {
            return slice->size();
        }

        CLASS push(const T & item) const {
            CLASS copy;
            if (slice->get_end() != base->length()) {
                auto b = begin();
                auto e = end();
                for (; b != e; ++b) {
                    copy.base->emplace(*b);
                }
            } else {
                copy.base = base;
            }
            copy.base->emplace(item);
            auto s = copy.base->slice(0, slice->get_end()+1);
            *copy.slice = *s;
            delete s;
            return copy;
        }

        CLASS push(const CLASS & fl) const {
            if (fl.size() == 0) return *static_cast<const CLASS*>(this);
            CLASS copy;
            if (slice->get_end() != base->length()) {
                auto b = begin();
                auto e = end();
                for (; b != e; ++b) {
                    copy.base->emplace(*b);
                }
            } else {
                copy.base = base;
            }
            for(const T & item : fl) {
                copy.base->emplace(item);
            }
            auto s = copy.base->slice(0, slice->get_end()+fl.size());
            *copy.slice = *s;
            delete s;
            return copy;
        }

        template <typename ForwardIter>
        Tuple2<CLASS, const_iterator> insert(const ForwardIter & pos, const T & item) const {
            static_assert(std::is_base_of<IndexedIterator::forward_iterator_base_obj, ForwardIter>::value, "ForwardIter must extend from IndexedIterator::forward_iterator_base_obj");
            CLASS copy;
            auto b = begin();
            auto e = end();
            ForwardIter p = pos;
            if (p <= b) INDEXED_ITERATOR_ASSIGN(p, b, ForwardIter);
            auto l = p >= e;
            if (l) {
                INDEXED_ITERATOR_ASSIGN(p, e, ForwardIter);
            }
            const_iterator r;
            if (p < e) {
                std::cout << "b: " << b << std::endl;
                std::cout << "p: " << p << std::endl;
            } else {
                std::cout << "b: " << b << std::endl;
                std::cout << "p: " << "end()" << std::endl;
            }
            for (; b != e; ++b) {
                if (b == p) {
                    r = b;
                    if (r < e) {
                        std::cout << "emplacing item at r: " << r << std::endl;
                    } else {
                        std::cout << "emplacing item at r: " << "end()" << std::endl;
                    }
                    copy.base->emplace(item);
                }
                copy.base->emplace(*b);
            }
            if (e == p) {
                r = const_iterator(e.origin[0], e.index);
                std::cout << "emplacing item at r: " << "end()" << std::endl;
                copy.base->emplace(item);
            }
            auto s = copy.base->slice(0, slice->get_end()+1);
            *copy.slice = *s;
            delete s;
            return {copy, r};
        }

        template <typename ForwardIter>
        Tuple2<CLASS, const_iterator> insert(const ForwardIter & pos, const CLASS & fl) const {
            static_assert(std::is_base_of<IndexedIterator::forward_iterator_base_obj, ForwardIter>::value, "ForwardIter must extend from IndexedIterator::forward_iterator_base_obj");
            if (fl.size() == 0) return {*static_cast<const CLASS*>(this), cend()-1};
            CLASS copy;
            auto b = begin();
            auto e = end();
            auto p = pos;
            if (*p <= b) INDEXED_ITERATOR_ASSIGN(p, b, ForwardIter);
            if (*p >= e) INDEXED_ITERATOR_ASSIGN(p, e, ForwardIter);
            const_iterator r;
            for (; b != e; ++b) {
                if (b == p) {
                    r = b;
                    for(const T & item : fl) {
                        copy.base->emplace(item);
                        e++;
                    }
                }
                copy.base->emplace(*b);
            }
            auto s = copy.base->slice(0, slice->get_end()+fl.size());
            *copy.slice = *s;
            delete s;
            return {copy, r};
        }

        template <typename ForwardIter>
        Tuple2<CLASS, const_iterator> erase(const ForwardIter & pos) const {
            static_assert(std::is_base_of<IndexedIterator::forward_iterator_base_obj, ForwardIter>::value, "ForwardIter must extend from IndexedIterator::forward_iterator_base_obj");
            auto e = end();
            if (pos >= e) return {*static_cast<const CLASS*>(this), e};
            CLASS copy;
            auto b = begin();
            auto p = pos;
            if (p <= b) INDEXED_ITERATOR_ASSIGN(p, b, ForwardIter);
            const_iterator r;
            for (; b != e; ++b) {
                if (b == p) {
                    r = b+1;
                } else {
                    copy.base->emplace(*b);
                }
            }
            auto s = copy.base->slice(0, slice->get_end()-1);
            *copy.slice = *s;
            delete s;
            return {copy, r};
        }

        template <typename ForwardIter>
        Tuple2<CLASS, const_iterator> erase(const ForwardIter & iterator_first, const ForwardIter & iterator_last) const {
            static_assert(std::is_base_of<IndexedIterator::forward_iterator_base_obj, ForwardIter>::value, "ForwardIter must extend from IndexedIterator::forward_iterator_base_obj");
            if (iterator_first == iterator_last) {
                return {*static_cast<const CLASS*>(this), end()-1};
            }
            CLASS copy;
            auto b = begin();
            auto e = end();
            auto first = iterator_first;
            auto last = iterator_last;
            if (first < b) first = e;
            if (first > e) first = e;
            if (last < b) last = e;
            if (last > e) last = e;
            const_iterator r;
            for (; b != e; ++b) {
                if (b >= first && b < last) {
                    r = b+1;
                } else {
                    copy.base->emplace(*b);
                }
            }
            auto s = copy.base->slice(0, slice->get_end()-(iterator_last - iterator_first));
            *copy.slice = *s;
            delete s;
            return {copy, r};
        }

        void print(const std::string & id) {
            auto b = begin();
            auto e = end();
            std::cout << id << " = [";
            for (; b != e; ++b) {
                std::cout << *b;
                if (b+1 != e) {
                    std::cout << ", ";
                }
            }
            std::cout << "]\n";
        }
    };

    template <typename T>
    struct FL : public FL_RL_COMMON<FL<T>, FL_BASE<T>, NilFL_T, T>
    {
        USING_FL_RL_COMMON(INDEXED_ITERATOR_EMBED_COMMAS(FL_RL_COMMON<FL<T>, FL_BASE<T>, NilFL_T, T>), FL_RL_COMMON);

        FL(const FL<T> & other) : FL<T>::FL_RL_COMMON(other) {}

        FL(FL<T> && other) : FL<T>::FL_RL_COMMON(std::move(other)) {}

        FL<T> & operator =(const FL<T> & other) {
            FL<T>::FL_RL_COMMON::operator =(other);
            return *this;
        }

        FL<T> & operator =(FL<T> && other) {
            FL<T>::FL_RL_COMMON::operator =(std::move(other));
            return *this;
        }

        void extract(T & out_a, FL<T> & out_fl) const {
            if (base != out_fl.base) {
                auto e = slice->begin();
                auto & eo = *e;
                out_a = eo;
                out_fl.base = base;
                auto s = base->slice(slice->get_start()+1, slice->get_end());
                *out_fl.slice = *s;
                delete s;
            }
        }
    };

    template <typename T>
    struct RL : public FL_RL_COMMON<RL<T>, RL_BASE<T>, NilRL_T, T>
    {

        USING_FL_RL_COMMON(INDEXED_ITERATOR_EMBED_COMMAS(FL_RL_COMMON<RL<T>, RL_BASE<T>, NilRL_T, T>), FL_RL_COMMON);

        RL(const RL<T> & other) : RL<T>::FL_RL_COMMON(other) {}

        RL(RL<T> && other) : RL<T>::FL_RL_COMMON(std::move(other)) {}

        RL<T> & operator =(const RL<T> & other) {
            RL<T>::FL_RL_COMMON::operator =(other);
            return *this;
        }

        RL<T> & operator =(RL<T> && other) {
            RL<T>::FL_RL_COMMON::operator =(std::move(other));
            return *this;
        }

        void extract(T & out_a, RL<T> & out_rl) const {
            if (base != out_rl.base) {
                auto e = end()-1;
                auto & eo = *e;
                out_a = eo;
                out_rl.base = base;
                auto s = base->slice(slice->get_start(), slice->get_end()-1);
                *out_rl.slice = *s;
                delete s;
            }
        }
    };

    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::NilFL_T & item);

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::FL<T> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << *b;
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::FL<T*> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << **b;
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::NilRL_T & item);

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::RL<T> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << *b;
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "]";
        return os;
    }

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::RL<T*> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << **b;
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "]";
        return os;
    }
}

namespace DarcsPatch {

    template <typename T>
    const FL<T> NilFL_T::push(const T & value) const {
        return FL<T>().push(value);
    }
    
    template <typename T>
    const RL<T> NilRL_T::push(const T & value) const {
        return RL<T>().push(value);
    }

    // in pattern matching, x :<: xs
    //  the compiler deconstructs the list
    //   and then reconstructs it for pattern matching
    //
    //  [1, 2]   ->   x :<: xs   ->   1 :<: [2]
    //  [1, 2]   ->   x :>: xs   ->   [1] :>: 2
    //

    template <typename T>
    static const FL<T> ToFL(const T & item) {
        return NilFL.push(item);
    }

    template <typename T>
    static const FL<T> ToFL(const std::vector<T> & vec) {
        FL<T> fl;
        // avoid recursion
        for (const T & item : vec) fl = fl.push(item);
        return fl;
    }

    template <typename T>
    static const FL<T> ToFL(const RL<T> & rl) {
        FL<T> fl;
        // avoid recursion
        for (const T & item : rl) fl = fl.push(item);
        return fl;
    }

    template <typename T>
    static const RL<T> ToRL(const T & item) {
        return NilRL.push(item);
    }

    template <typename T>
    static const RL<T> ToRL(const std::vector<T> & vec) {
        RL<T> rl;
        // avoid recursion
        for (const T & item : vec) rl = rl.push(item);
        return rl;
    }

    template <typename T>
    static const RL<T> ToRL(const FL<T> & fl) {
        // avoid recursion
        RL<T> rl;
        for (const T & item : fl) rl = rl.push(item);
        return rl;
    }

    struct Nothing {};

    template <typename T>
    struct Maybe :
        public StringAdapter::Comparable<Maybe<T>>,
        public StringAdapter::Hashable<Maybe<T>>
    {
        using THIS = Maybe<T>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Maybe() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::has_value, &THIS::value); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::has_value, &THIS::value); }),
            has_value(false)
        {}

        bool has_value;
        T value;

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }

        Maybe(const Nothing & nothing) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::has_value, &THIS::value); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::has_value, &THIS::value); }),
            has_value(false)
        {}

        Maybe(const T & value) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::has_value, &THIS::value); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::has_value, &THIS::value); }),
            has_value(true),
            value(value)
        {
        }

        const T & value_ref() const {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }

        T & value_ref() {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }

        const T value_copy() const {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }

        T value_copy() {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }

        const T* operator -> () const {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return &value;
        }

        T* operator -> () {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return &value;
        }
    };

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Maybe<T> & item) {
        os << "{ Maybe, has_value = " << (item.has_value ? "true" : "false");
        if (item.has_value) {
            os << ", value = " << item.value;
        }
        os << " }";
        return os;
    }

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Maybe<T*> & item) {
        os << "{ Maybe, has_value = " << (item.has_value ? "true" : "false");
        if (item.has_value) {
            os << ", value = " << *item.value;
        }
        os << " }";
        return os;
    }
}

namespace DarcsPatch {

    template <typename T>
    struct Set :
        public StringAdapter::Comparable<Set<T>>,
        public StringAdapter::Hashable<Set<T>>
    {
        using THIS = Set<T>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Set() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, T>(a); })
        {}

        using SET_T = std::set<T>;

        SET_T set;

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }

        typename SET_T::iterator begin() {
            return set.begin();
        }

        typename SET_T::iterator end() {
            return set.end();
        }

        typename SET_T::const_iterator cbegin() const {
            return set.cbegin();
        }

        typename SET_T::const_iterator cend() const {
            return set.cend();
        }

        typename SET_T::const_iterator begin() const {
            return cbegin();
        }

        typename SET_T::const_iterator end() const {
            return cend();
        }

        typename SET_T::reverse_iterator rbegin() {
            return set.rbegin();
        }

        typename SET_T::reverse_iterator rend() {
            return set.rend();
        }

        typename SET_T::const_reverse_iterator crbegin() const {
            return set.crbegin();
        }

        typename SET_T::const_reverse_iterator crend() const {
            return set.crend();
        }

        typename SET_T::const_reverse_iterator rbegin() const {
            return crbegin();
        }

        typename SET_T::const_reverse_iterator rend() const {
            return crend();
        }

        const std::size_t size() const {
            return set.size();
        }

        Set<T> & insert_in_place(const T & item) {
            set.insert(item);
            return *this;
        }

        const Set<T> insert(const T & item) const {
            Set<T> copy = *this;
            copy.insert_in_place(item);
            return copy;
        }

        const bool contains(const T & item) const {
            for(const T & v : set) {
                if (v == item) return true;
            }
            return false;
        }

        const Set<T> Union(const Set<T> & other) const {
            Set<T> s;
            std::set_union(set.cbegin(), set.cend(), other.set.cbegin(), other.set.cend(), std::inserter(s.set, s.cend()));
            return s;
        }
    };

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Set<T> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << *b;
            b++;
            if (b != e) {
                os << ", ";
            } else {
                os << " ";
            }
            b--;
        }
        os << "]";
        return os;
    }

    template <typename T>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Set<T*> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "[]";
        }
        os << "[ ";
        for (; b != e; ++b) {
            os << **b;
            b++;
            if (b != e) {
                os << ", ";
            } else {
                os << " ";
            }
            b--;
        }
        os << "]";
        return os;
    }
}

namespace DarcsPatch {

    template <typename K, typename V>
    struct Map :
        public StringAdapter::Comparable<Map<K, V>>,
        public StringAdapter::Hashable<Map<K, V>>
    {
        using THIS = Map<K, V>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Map() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3_iterator(a, b, &THIS::cbegin, &THIS::cend); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3_iterator<THIS, Tuple2<const K, V>>(a); })
        {}

        // we cannot use std::map due to it requiring comparison operators
        // we cannot use std::unordered_map due to it requiring K to be hashable
        //
        // we must instead use std::vector
        //

        // using MAP_T = std::vector<std::pair<K, V>>;
        using MAP_T = std::map<K, V>;

        MAP_T map;

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }

        typename MAP_T::iterator begin() {
            return map.begin();
        }

        typename MAP_T::iterator end() {
            return map.end();
        }

        typename MAP_T::const_iterator cbegin() const {
            return map.cbegin();
        }

        typename MAP_T::const_iterator cend() const {
            return map.cend();
        }

        typename MAP_T::const_iterator begin() const {
            return cbegin();
        }

        typename MAP_T::const_iterator end() const {
            return cend();
        }

        typename MAP_T::reverse_iterator rbegin() {
            return map.rbegin();
        }

        typename MAP_T::reverse_iterator rend() {
            return map.rend();
        }

        typename MAP_T::const_reverse_iterator crbegin() const {
            return map.crbegin();
        }

        typename MAP_T::const_reverse_iterator crend() const {
            return map.crend();
        }

        typename MAP_T::const_reverse_iterator rbegin() const {
            return crbegin();
        }

        typename MAP_T::const_reverse_iterator rend() const {
            return crend();
        }

        const std::size_t size() const {
            return map.size();
        }

        Map<K, V> & insert_in_place(const K & key, const V & value) {
            map.insert({key, value});
            return *this;
        }

        const Map<K, V> insert(const K & key, const V & value) const {
            Map<K, V> copy = *this;
            copy.insert_in_place(key, value);
            return copy;
        }

        const bool contains(const K & key) const {
            return map.contains(key);
        }

        const Maybe<V> lookup(const K & key) const {
            if (auto search = map.find(key); search != map.end()) {
                return search->second;
            }
            return Nothing();
        }
    };

    template <typename K, typename V>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Map<K, V> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "{ Map, [] }";
        }
        os << "{ Map, [ ";
        for (; b != e; ++b) {
            os << "{ key = " << b->first << ", value = " << b->second << " }";
            b++;
            if (b != e) {
                os << ", ";
            } else {
                os << " ";
            }
            b--;
        }
        os << "] }";
        return os;
    }

    template <typename K, typename V>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Map<K*, V> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "{ Map, [] }";
        }
        os << "{ Map, [ ";
        for (; b != e; ++b) {
            os << "{ key = " << *b->first << ", value = " << b->second << " }";
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "] }";
        return os;
    }

    template <typename K, typename V>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Map<K, V*> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "{ Map, [] }";
        }
        os << "{ Map, [ ";
        for (; b != e; ++b) {
            os << "{ key = " << b->first << ", value = " << *b->second << " }";
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "] }";
        return os;
    }

    template <typename K, typename V>
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Map<K*, V*> & item) {
        auto b = item.begin();
        auto e = item.end();
        if (b == e) {
            return os << "{ Map, [] }";
        }
        os << "{ Map, [ ";
        for (; b != e; ++b) {
            os << "{ key = " << *b->first << ", value = " << *b->second << " }";
            if (b+1 != e) {
                os << ", ";
            } else {
                os << " ";
            }
        }
        os << "] }";
        return os;
    }
}

namespace DarcsPatch {

    enum V2_PRIM {
        NORMAL, DUPLICATE, ETACILPUD, CONFLICTOR, INV_CONFLICTOR
    };

    enum PATCH_TYPE {
        ADD_FILE, REMOVE_FILE, HUNK, TOK_REPLACE
    };

    struct Patch {
        // must always return an allocated patch
        virtual std::shared_ptr<Patch> invert() const = 0;

        virtual const PATCH_TYPE type() const = 0;
        const V2_PRIM v2_prim = NORMAL;
        virtual ::std::ostream & to_stream(::std::ostream & os) const;
        virtual ~Patch();
        
        void to_string() const;
        
        void to_string();

        virtual int cmp(const Patch & other) const;
        virtual std::size_t hashCode() const noexcept;
        bool operator == (const DarcsPatch::Patch & other) const;
        bool operator != (const DarcsPatch::Patch & other) const;
        bool operator < (const DarcsPatch::Patch & other) const;
        bool operator > (const DarcsPatch::Patch & other) const;
        bool operator <= (const DarcsPatch::Patch & other) const;
        bool operator >= (const DarcsPatch::Patch & other) const;
    };


    struct AddFile : Patch {
        const PATCH_TYPE type() const override;
        std::shared_ptr<Patch> invert() const override;
        ::std::ostream & to_stream(::std::ostream & os) const override;
        using Patch::cmp;
        using Patch::hashCode;
    };

    struct RemoveFile : Patch {
        const PATCH_TYPE type() const override;
        std::shared_ptr<Patch> invert() const override;
        ::std::ostream & to_stream(::std::ostream & os) const override;
        using Patch::cmp;
        using Patch::hashCode;
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct TokReplace : Patch {
        adapter_t t;
        adapter_t o;
        adapter_t n;

        TokReplace() = default;
        TokReplace(const adapter_t & t, const adapter_t & o, const adapter_t & n) : t(t), o(o), n(n) {}

        const PATCH_TYPE type() const override {
            return TOK_REPLACE;
        }

        std::shared_ptr<Patch> invert() const override {
            return std::static_pointer_cast<Patch>(std::make_shared<TokReplace>(t, n, o));
        }

        ::std::ostream & to_stream(::std::ostream & os) const override {
            return os << "{ TokReplace, t = " << t << ", o = " << o << ", n = " << n << " }";
        }

        int cmp(const Patch & other) const override {
            int r = StringAdapter::compare_2(type(), other.type());
            if (r == 0) {
                return StringAdapter::compare_3(*this, reinterpret_cast<const TokReplace<char_t, adapter_t> &>(other), &TokReplace<char_t, adapter_t>::t, &TokReplace<char_t, adapter_t>::o, &TokReplace<char_t, adapter_t>::n);
            } 
            return r;
        }

        std::size_t hashCode() const noexcept override {
            // https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
            std::size_t hashCode_ = 1;
            hashCode_ = 31 * hashCode_ + std::hash<adapter_t>()(t);
            hashCode_ = 31 * hashCode_ + std::hash<adapter_t>()(o);
            hashCode_ = 31 * hashCode_ + std::hash<adapter_t>()(n);
            return hashCode_;
        }
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct FileHunk : Patch {
        std::size_t line;
        RL<adapter_t> old_lines;
        RL<adapter_t> new_lines;
        FileHunk() : line(0), old_lines(NilFL), new_lines(NilFL) {}
        FileHunk(const std::size_t & line,const RL<adapter_t>& old_lines, const RL<adapter_t>& new_lines) : line(line), old_lines(old_lines), new_lines(new_lines) {}
        FileHunk(const std::size_t & line,const adapter_t& old_line, const adapter_t& new_line) : line(line) {

            // need to cast from
            //       std::shared_ptr<BasicStringAdapter<T>>
            //   to
            //       std::shared_ptr<adapter_t>
            //
            // we know lines allocates an instance of adapter_t or higher derived object
            //
            auto ol = old_line.lines();
            auto nl = new_line.lines();
            for (auto & o : ol) {
                old_lines = old_lines.push(*static_cast<adapter_t*>(o.get()));
            }
            for (auto & n : nl) {
                new_lines = new_lines.push(*static_cast<adapter_t*>(n.get()));
            }
        }

        const PATCH_TYPE type() const override {
            return HUNK;
        }

        std::shared_ptr<Patch> invert() const override {
            return std::static_pointer_cast<Patch>(std::make_shared<FileHunk<char_t, adapter_t>>(line, new_lines, old_lines));
        }

        ::std::ostream & to_stream(::std::ostream & os) const override {
            return os << "{ FileHunk, line = " << line << ", old_lines = " << old_lines << ", new_lines = " << new_lines << " }";
        }

        int cmp(const Patch & other) const override {
            int r = StringAdapter::compare_2(type(), other.type());
            if (r == 0) {
                return StringAdapter::compare_3(*this, reinterpret_cast<const FileHunk<char_t, adapter_t> &>(other),
                    &FileHunk<char_t, adapter_t>::line,
                    &FileHunk<char_t, adapter_t>::old_lines,
                    &FileHunk<char_t, adapter_t>::new_lines
                );
            } 
            return r;
        }

        std::size_t hashCode() const noexcept override {
            // https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
            std::size_t hashCode_ = 1;
            hashCode_ = 31 * hashCode_ + std::hash<std::size_t>()(line);
            hashCode_ = 31 * hashCode_ + std::hash<RL<adapter_t>>()(old_lines);
            hashCode_ = 31 * hashCode_ + std::hash<RL<adapter_t>>()(new_lines);
            return hashCode_;
        }
    };

    using FileHunk_T = FileHunk<char, StringAdapter::CharAdapter>;

    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Patch & item);
}

namespace DarcsPatch {

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct PatchInfo :
        public StringAdapter::Comparable<PatchInfo<char_t, adapter_t>>,
        public StringAdapter::Hashable<PatchInfo<char_t, adapter_t>>
    {
        using THIS = PatchInfo<char_t, adapter_t>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        PatchInfo() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); })
        {}

        adapter_t date;
        adapter_t name;
        adapter_t author;
        RL<adapter_t> log;
        bool legacyIsInverted = false;

        PatchInfo(const adapter_t & name) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); }),
            name(name)
        {
        }

        PatchInfo(const adapter_t & date, const adapter_t & name, const adapter_t & author, const RL<adapter_t> & log) :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::date, &THIS::name, &THIS::author, &THIS::log, &THIS::legacyIsInverted); }),
            date(date),
            name(name),
            author(author),
            log(log)
        {
        }

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }
    };

    using PatchInfo_T = PatchInfo<char, StringAdapter::CharAdapter>;

    PatchInfo_T makePatchInfo_T(const StringAdapter::CharAdapter & patch_id_unique_label);
    
    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using PatchInfoAndG = PatchInfo<char_t, adapter_t>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::PatchInfo<char_t, adapter_t> & item) {
        os << "{ PatchInfo, date = " << item.date << ", name = " << item.name << ", author = " << item.author << ", log = " << item.log << ", legacyIsInverted = " << (item.legacyIsInverted ? "true" : "false") << " }";
        return os;
    }
}

namespace DarcsPatch {

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct AnchorPath {
        Set<adapter_t> names;
        AnchorPath() : names({}) {}
        AnchorPath(const Set<adapter_t> & names) : names(names) {}

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }

        bool operator == (const AnchorPath<char_t, adapter_t> & other) const {
            return names == other.names;
        }

        bool operator != (const AnchorPath<char_t, adapter_t> & other) const {
            return names != other.names;
        }

        bool operator < (const AnchorPath<char_t, adapter_t> & other) const {
            return names < other.names;
        }

        bool operator > (const AnchorPath<char_t, adapter_t> & other) const {
            return names > other.names;
        }

        bool operator <= (const AnchorPath<char_t, adapter_t> & other) const {
            return names <= other.names;
        }

        bool operator >= (const AnchorPath<char_t, adapter_t> & other) const {
            return names >= other.names;
        }

        std::size_t hashCode() const noexcept {
            // https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
            std::size_t hashCode_ = 1;
            hashCode_ = 31 * hashCode_ + std::hash<Set<adapter_t>>()(names);
            return hashCode_;
        }
    };

    using AnchorPath_T = AnchorPath<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::AnchorPath<char_t, adapter_t> & item) {
        os << "{ AnchorPath, names = " << item.names << " }";
        return os;
    }
}

namespace DarcsPatch {

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct Core_FP :
        public StringAdapter::Comparable<Core_FP<char_t, adapter_t>>,
        public StringAdapter::Hashable<Core_FP<char_t, adapter_t>>
    {
        using THIS = Core_FP<char_t, adapter_t>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Core_FP() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::anchor_path, &THIS::patch); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::anchor_path, &THIS::patch); })
        {}

        AnchorPath<char_t, adapter_t> anchor_path;
        std::shared_ptr<Patch> patch;

        Core_FP(std::shared_ptr<Patch> p) : Core_FP() {
            patch = p;
        }
        Core_FP(const AnchorPath<char_t, adapter_t> & anchor_path, std::shared_ptr<Patch> p) : Core_FP() {
            this->anchor_path = anchor_path;
            patch = p;
        }

        void to_string() const {
            std::cout << *this;
        }
        
        void to_string() {
            std::cout << *this;
        }
    };

    using Core_FP_T = Core_FP<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Core_FP<char_t, adapter_t> & item) {
        os << "{ Core_FP, anchor_path = " << item.anchor_path << ", patch = " << *item.patch << " }";
        return os;
    }
}

namespace DarcsPatch {

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct Named :
        public StringAdapter::Comparable<Named<T, char_t, adapter_t>>,
        public StringAdapter::Hashable<Named<T, char_t, adapter_t>>
    {
        using THIS = Named<T, char_t, adapter_t>;
        COMPARABLE_USING_BASE(StringAdapter::Comparable<THIS>);
        HASHABLE_USING_BASE(StringAdapter::Hashable<THIS>);

        Named() :
            THIS::Comparable([](auto & a, auto & b) { return StringAdapter::compare_3(a, b, &THIS::n, &THIS::d, &THIS::p); }),
            THIS::Hashable([](auto & a) { return StringAdapter::hash_3(a, &THIS::n, &THIS::d, &THIS::p); })
        {}

        PatchInfo<char_t, adapter_t> n;
        Set<PatchInfo<char_t, adapter_t>> d;
        FL<T> p;
        
        Named(const PatchInfo<char_t, adapter_t> & n, const Set<PatchInfo<char_t, adapter_t>> & d, const FL<T> & p) : Named() {
            this->n = n;
            this->d = d;
            this->p = p;
        }

        PatchInfo<char_t, adapter_t> ident() {
            return n;
        }

        void to_string() const {
            std::cout << *this;
        }

        void to_string() {
            std::cout << *this;
        }

        bool operator == (const Named<T, char_t, adapter_t> & other) const {
            return n == other.n && d == other.d && p == other.p;
        }

        bool operator != (const Named<T, char_t, adapter_t> & other) const {
            return !(*this == other);
        }

        bool operator < (const Named<T, char_t, adapter_t> & other) const {
            return n < other.n && d < other.d && p < other.p;
        }

        bool operator > (const Named<T, char_t, adapter_t> & other) const {
            return n > other.n && d > other.d && p > other.p;
        }

        bool operator <= (const Named<T, char_t, adapter_t> & other) const {
            return n <= other.n && d <= other.d && p <= other.p;
        }

        bool operator >= (const Named<T, char_t, adapter_t> & other) const {
            return n >= other.n && d >= other.d && p >= other.p;
        }

        std::size_t hashCode() const noexcept {
            // https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
            std::size_t hashCode_ = 1;
            hashCode_ = 31 * hashCode_ + std::hash<PatchInfo<char_t, adapter_t>>()(n);
            hashCode_ = 31 * hashCode_ + std::hash<Set<PatchInfo<char_t, adapter_t>>>()(d);
            hashCode_ = 31 * hashCode_ + std::hash<FL<T>>()(p);
            return hashCode_;
        }
    };

    template <typename T>
    using Named_T = Named<T, char, StringAdapter::CharAdapter>;

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Named<T, char_t, adapter_t> & item) {
        os << "{ Named, n = " << item.n << ", d = " << item.d << ", p = " << item.p << " }";
        return os;
    }

    template <
        typename T,
        typename char_t,
        typename adapter_t
    >
    ::std::ostream& operator <<(::std::ostream& os, const DarcsPatch::Named<T*, char_t, adapter_t, typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type> & item) {
        os << "{ Named, n = " << item.n << ", d = " << item.d << ", p = " << item.p << " }";
        return os;
    }
}

namespace DarcsPatch {

    std::shared_ptr<Patch> makeAddFile();
    std::shared_ptr<Patch> makeRemoveFile();

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    std::shared_ptr<Patch> makeTokReplace(const adapter_t & t, const adapter_t & o, const adapter_t & n) {
        return std::static_pointer_cast<Patch>(std::make_shared<TokReplace<char_t, adapter_t>>(t, o, n));
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    std::shared_ptr<Patch> makeHunk(const std::size_t & line, const adapter_t& old_line, const adapter_t& new_line) {
        return std::static_pointer_cast<Patch>(std::make_shared<FileHunk<char_t, adapter_t>>(line, old_line, new_line));
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    std::shared_ptr<Patch> makeHunk(const std::size_t & line,const RL<adapter_t>& old_lines, const RL<adapter_t>& new_lines) {
        return std::static_pointer_cast<Patch>(std::make_shared<FileHunk<char_t, adapter_t>>(line, old_lines, new_lines));
    }

    std::shared_ptr<Patch> makeHunk_T(const std::size_t & line,const RL<StringAdapter::CharAdapter>& old_lines, const RL<StringAdapter::CharAdapter>& new_lines);
    std::shared_ptr<Patch> makeHunk_T(const std::size_t & line, const StringAdapter::CharAdapter& old_line, const StringAdapter::CharAdapter& new_line);
    
    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> makeNamedWithType(std::shared_ptr<Patch> patch_type) {
        return Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>(PatchInfo<char_t, adapter_t>(), {}, ToFL(Core_FP<char_t, adapter_t>(patch_type)));
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> makeNamedHunk(const adapter_t & patch_id_unique_label, const std::size_t & line, const adapter_t& old_line, const adapter_t& new_line) {
        return makeNamedWithType<char_t, adapter_t>(patch_id_unique_label, makeHunk(line, old_line, new_line));
    }

    Named_T<Core_FP_T> makeNamedWithType_T(const StringAdapter::CharAdapter & patch_id_unique_label, std::shared_ptr<Patch> patch_type);

    Named_T<Core_FP_T> makeNamedHunk_T(const StringAdapter::CharAdapter & patch_id_unique_label, const std::size_t & line, const StringAdapter::CharAdapter & old_line, const StringAdapter::CharAdapter & new_line);

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using Deps = Tuple2<Set<PatchInfo<char_t, adapter_t>>, Set<PatchInfo<char_t, adapter_t>>>;

    using Deps_T = Deps<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using DepsGraph = Map<PatchInfo<char_t, adapter_t>, Deps<char_t, adapter_t>>;

    using DepsGraph_T = DepsGraph<char, StringAdapter::CharAdapter>;

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    PatchInfo<char_t, adapter_t> patch2patchinfo(const Named<T, char_t, adapter_t> & p) {
        return p.n;
    }

    template <typename T>
    PatchInfo_T patch2patchinfo_T(const Named_T<T> & p) {
        return p.n;
    }

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using PatchId = PatchInfo<T, char_t, adapter_t>;

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    static const PatchInfo<char_t, adapter_t> ident(const Named<T, char_t, adapter_t> & p) {
        return patch2patchinfo(p);
    }

    template <typename T>
    static const PatchInfo_T ident_T(const Named_T<T> & p) {
        return patch2patchinfo_T(p);
    }

    template <typename T>
    static const FL<T> invert(const FL<T> & fl);

    template <typename T>
    static const RL<T> invert(const RL<T> & rl);

    template <typename T>
    static const Tuple2<T, T> invert(const Tuple2<T, T> & p);

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    static const Core_FP<char_t, adapter_t> invert(const Core_FP<char_t, adapter_t> & p) {
        return Core_FP<char_t, adapter_t>(p.anchor_path, invert(p.patch));
    }

    std::shared_ptr<Patch> invert(std::shared_ptr<Patch> p);

    // invertFL :: Invert p => FL p wX wY -> RL p wY wX
    template <typename T>
    static const RL<T> invertFL(const FL<T> & fl) {
        // invertFL NilFL = NilRL
        if (fl == NilFL) {
            return NilRL;
        }
        // invertFL (x:>:xs) = invertFL xs :<: invert x
        T x;
        FL<T> xs;
        fl.extract(x, xs);
        return invert<T>(xs).push(invert<T>(x));
    }

    // invertRL :: Invert p => RL p wX wY -> FL p wY wX
    template <typename T>
    static const FL<T> invertRL(const RL<T> & rl) {
        // invertRL NilRL = NilFL
        if (rl == NilRL) {
            return NilFL;
        }
        // invertRL (xs:<:x) = invert x :>: invertRL xs
        T x;
        RL<T> xs;
        rl.extract(x, xs);
        return invert<T>(xs).push(invert<T>(x));
    }

    // instance Invert p => Invert (FL p) where
    template <typename T>
    static const FL<T> invert(const FL<T> & fl) {
        // invert = reverseRL . invertFL
        return ToFL(invertFL<T>(fl));
    }

    // instance Invert p => Invert (RL p) where
    template <typename T>
    static const RL<T> invert(const RL<T> & rl) {
        // invert = reverseFL . invertRL
        return ToRL(invertRL<T>(rl));
    }

    // instance Invert p => Invert (p :> p) where
    template <typename T>
    static const Tuple2<T, T> invert(const Tuple2<T, T> & p) {
        // invert (a :> b) = invert b :> invert a
        return Tuple2<T, T>(invert(p.v1), invert(p.v2));
    }









    // STD IMPL - LLDB by default will not step into std code, this is good EXCEPT if we want to step into std::function

    /**
      *  @brief  Forward an lvalue.
      *  @return The parameter cast to the specified type.
      *
      *  This function is used to implement "perfect forwarding".
      */
    template<typename _Tp>
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type& __t) noexcept
    { return static_cast<_Tp&&>(__t); }

    /**
      *  @brief  Forward an rvalue.
      *  @return The parameter cast to the specified type.
      *
      *  This function is used to implement "perfect forwarding".
      */
    template<typename _Tp>
    constexpr _Tp&&
    forward(typename std::remove_reference<_Tp>::type&& __t) noexcept
    {
        static_assert(!std::is_lvalue_reference<_Tp>::value, "template argument"
            " substituting _Tp is an lvalue reference type");
        return static_cast<_Tp&&>(__t);
    }

    namespace detail
    {
        template<class>
        constexpr bool is_reference_wrapper_v = false;
        template<class U>
        constexpr bool is_reference_wrapper_v<std::reference_wrapper<U>> = true;
    
        template<class C, class Pointed, class T1, class... Args>
        constexpr decltype(auto) invoke_memptr(Pointed C::* f, T1&& t1, Args&&... args)
        {
            if constexpr (std::is_function_v<Pointed>)
            {
                if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
                    return (DarcsPatch::forward<T1>(t1).*f)(DarcsPatch::forward<Args>(args)...);
                else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                    return (t1.get().*f)(DarcsPatch::forward<Args>(args)...);
                else
                    return ((*DarcsPatch::forward<T1>(t1)).*f)(DarcsPatch::forward<Args>(args)...);
            }
            else
            {
                static_assert(std::is_object_v<Pointed> && sizeof...(args) == 0);
                if constexpr (std::is_base_of_v<C, std::decay_t<T1>>)
                    return DarcsPatch::forward<T1>(t1).*f;
                else if constexpr (is_reference_wrapper_v<std::decay_t<T1>>)
                    return t1.get().*f;
                else
                    return (*DarcsPatch::forward<T1>(t1)).*f;
            }
        }
    } // namespace detail
    
    template<class F, class... Args>
    constexpr std::invoke_result_t<F, Args...> invoke(F&& f, Args&&... args)
        noexcept(std::is_nothrow_invocable_v<F, Args...>)
    {
        if constexpr (std::is_member_pointer_v<std::decay_t<F>>)
            return DarcsPatch::detail::invoke_memptr(f, DarcsPatch::forward<Args>(args)...);
        else
            return DarcsPatch::forward<F>(f)(DarcsPatch::forward<Args>(args)...);
    }

    template<typename Result,typename ...Args>
    struct abstract_function
    {
        virtual Result operator()(Args... args)=0;
        virtual abstract_function *clone() const =0;
        virtual ~abstract_function() = default;
    };

    template<typename Func,typename Result,typename ...Args>
    class concrete_function: public abstract_function<Result,Args...>
    {
        Func f;
    public:
        concrete_function(const Func &x)
            : f(x)
        {}
        Result operator()(Args... args) override
        {
            return DarcsPatch::invoke(f, args...);
        }
        concrete_function *clone() const override
        {
            return new concrete_function{f};
        }
    };

    template<typename Func>
    struct func_filter
    {
        typedef Func type;
    };
    template<typename Result,typename ...Args>
    struct func_filter<Result(Args...)>
    {
        typedef Result (*type)(Args...);
    };

    template<typename signature>
    class function;

    template<typename Result,typename ...Args>
    class function<Result(Args...)>
    {
        abstract_function<Result,Args...> *f;
    public:
        function()
            : f(nullptr)
        {}
        template<typename Func> function(const Func &x)
            : f(new concrete_function<typename func_filter<Func>::type,Result,Args...>(x))
        {}
        function(const function &rhs)
            : f(rhs.f ? rhs.f->clone() : nullptr)
        {}
        function &operator=(const function &rhs)
        {
            if( (&rhs != this ) && (rhs.f) )
            {
                auto *temp = rhs.f->clone();
                delete f;
                f = temp;
            }
            return *this;
        }
        template<typename Func> function &operator=(const Func &x)
        {
            auto *temp = new concrete_function<typename func_filter<Func>::type,Result,Args...>(x);
            delete f;
            f = temp;
            return *this;
        }
        Result operator()(Args... args) const
        {
            if(f)
                return DarcsPatch::invoke(*f, args...);
            else
                throw std::bad_function_call();
        }
        ~function()
        {
            delete f;
        }
    };

    template <typename R>
    struct LazyValue {
        std::shared_ptr<function<R()>> func;
        mutable std::shared_ptr<R> value;
        mutable std::shared_ptr<bool> called;

        LazyValue(const function<R()> & f) : func(std::make_shared<function<R()>>(f)), called(std::make_shared<bool>(false)) {}

        LazyValue(const LazyValue<R> & f) {
            func = f.func;
            value = f.value;
            called = f.called;
        }
        LazyValue(const LazyValue<R> && f) {
            func = f.func;
            value = f.value;
            called = f.called;
        }
        LazyValue<R>&operator=(const LazyValue<R> & f) {
            func = f.func;
            value = f.value;
            called = f.called;
            return *this;
        }
        LazyValue<R>&operator=(const LazyValue<R> && f) {
            func = f.func;
            value = f.value;
            called = f.called;
            return *this;
        }
        R & operator ()() const {
            if (*called.get()) {
                return *value.get();
            } else {
                value = std::make_shared<R>((*func.get())());
                called = std::make_shared<bool>(true);
                return *value.get();
            }
        }
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    void renderDepsGraphAsDot(const DepsGraph<char_t, adapter_t> & g) {
        std::cout << "digraph {";
        auto indent = "   ";
        std::cout << "\n" << indent << "graph [rankdir=LR];";
        std::cout << "\n" << indent << "node [imagescale=true];";

        auto showID = [] (auto & key) {
            SHA1 sha1;
            auto & n = key.name.c_str();
            return sha1(n.ptr(), n.lengthInBytes());
        };

        auto showNode = [&](auto & key) {
            std::cout << "\n" << indent << "\"" << showID(key) << "\" [label=" << key.name << "]";
        };

        auto showEdges = [&](auto & key, auto & value) {
            auto begin = value.begin();
            if (begin != value.end()) {
                std::cout << "\n" << indent << "\"" << showID(key) << "\" -> " << "{\"" << showID(*begin) << "\"}";
            }
        };

        for(auto & pair : g) {
            showNode(pair.first);
        }

        for(auto & pair : g) {
            showEdges(pair.first, pair.second.v1);
        }

        std::cout << "\n}\n";
    }
}

STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Tuple2<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2, typename T3), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Tuple3<T1, T2, T3>));
STRING_ADAPTER_HASHCODE_SPEC(DarcsPatch::Patch);
STRING_ADAPTER_HASHCODE_SPEC(DarcsPatch::AddFile);
STRING_ADAPTER_HASHCODE_SPEC(DarcsPatch::RemoveFile);
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::FileHunk<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::FL<T>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::RL<T>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Maybe<T>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Set<T>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Map<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::PatchInfo<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::AnchorPath<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Core_FP<T1, T2>));
STRING_ADAPTER_HASHCODE_SPEC_T(INDEXED_ITERATOR_EMBED_COMMAS(typename T1, typename T2, typename T3), INDEXED_ITERATOR_EMBED_COMMAS(DarcsPatch::Named<T1, T2, T3>));

#endif