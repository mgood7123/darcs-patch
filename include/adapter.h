#pragma once

#include <cstring>
#include <vector>
#include <string>

namespace StringAdapter {
    
    // reasoning:
    //
    // as long as we are able to represent as structure
    // array, as a minimal string-like wrapper with one
    // way conversion to string, then it is technically
    // possible to then attempt to match sequential
    // structures and eventually specific structure
    // sequences
    //
    // we CANNOT ust std::basic_string<T> because it
    // requires TWO-WAY conversion from T to int
    // and int to T
    
    template <typename T>
    struct BasicStringAdapter {
        
        typedef T TYPE;
        
        BasicStringAdapter() {} // default constructor
        
        BasicStringAdapter(const T*ptr) {
            puts("BasicStringAdapter( T * ptr ) has not been overridden");
            std::terminate();
        };
        
        BasicStringAdapter(const T*ptr, size_t length) {
            puts("BasicStringAdapter( T * ptr , size_t length ) has not been overridden");
            std::terminate();
        };
        
        virtual T & get_item_at_index(size_t index) const = 0;
        
        T & operator[](size_t index) const {
            return get_item_at_index(index);
        };
        
        T* begin() const {
            return data();
        }
        T* end() const {
            return data()+length();
        }
        
        // must never be null
        virtual T* data() const = 0;
        
        virtual std::vector<T>* data_as_vector() const = 0;
        
        virtual size_t length() const = 0;
        
        size_t line_count() const {
            size_t len = length();
            if (len == 0) {
                return 0;
            }
            size_t lines = 1;
            const T nl = get_new_line();
            for (const T & t : *this) {
                if (t == nl) {
                    lines++;
                }
            }
            return lines;
        }
        
        virtual void resize(size_t capacity) = 0;
        
        size_t size() const {
            return length();
        }
        
        virtual const char * c_str() const = 0;
        virtual bool c_str_is_allocated() const = 0;
        
        template <typename T2 = T, typename std::enable_if<std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        void mem_cpy(T2* dest, const T2* src, size_t len) const {
            std::memcpy(dest, src, sizeof(T2)*len);
        }
        
        template <typename T2 = T, typename std::enable_if<!std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        void mem_cpy(T2* dest, const T2* src, size_t len) const {
            for(size_t s = 0; s < len; s++) {
                dest[s] = src[s];
            }
        }
        
        void append(const BasicStringAdapter<T> & what) {
            insert(what, -1);
        }
        
        virtual void insert(const BasicStringAdapter<T> & what, size_t pos) = 0;
        
        virtual void replace(const BasicStringAdapter<T> & what, size_t pos, size_t length) {
            
            // be naive
            
            size_t len = size();
            
            size_t p = clamp_pos(pos);
            
            if (p >= len) {
                insert(what, p);
                return;
            }
            
            size_t l = clamp_length(p, length);
            
            if (l != 0) {
                erase(p, l);
            }
            
            insert(what, p);
        }
        
        virtual void erase(size_t pos, size_t length) = 0;
        
        virtual const T get_new_line() const = 0;
        
        virtual ~BasicStringAdapter() {};
        
        size_t clamp_pos(size_t pos) const {
            size_t length = size();
            return pos == -1 ? length : pos >= length ? length : pos;
        }
        
        size_t clamp_length(size_t clamped_pos, size_t len) const {
            size_t length = size();
            return clamped_pos == length ? 0 : len == -1 ? length : clamped_pos + len >= length ? length : clamped_pos + len;
        }
    };
    
    template<typename T, typename DERIVED>
    struct VectorAdapter : public BasicStringAdapter<T> {
        
        std::vector<T> vector;
        
        using BASE = BasicStringAdapter<T>;
        using BASE::append;
        using BASE::replace;
        using BASE::size;
        using BASE::line_count;
        using BASE::get_new_line;
        using BASE::operator[];
        using BASE::clamp_pos;
        using BASE::clamp_length;
        
        VectorAdapter() {
            init(nullptr);
        }
        
        VectorAdapter(const T * ptr) {
            init(ptr);
        }
        
        VectorAdapter(const T * ptr, size_t length) {
            init(ptr, length);
        }
        
        void init(const T * ptr) {
            
            T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            T empty[1] { eof };
            
            if (ptr == nullptr) {
                init(empty, 0);
                return;
            }
            size_t length = 0;
            
            while(true) {
                if (ptr[length] == eof) {
                    break;
                }
                length++;
            }
            
            init(ptr, length);
            
        }
        
        void init(const T * ptr, size_t length) {
            T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            T empty[1] { eof };
            
            if (ptr == nullptr) {
                init(empty, 0);
                return;
            }
            
            vector = { length };
            if (length != 0) {
                for(size_t s = 0; s < length; s++) {
                    vector[s] = ptr[s];
                }
            }
            vector[length] = eof;
            
        }
        
        T & get_item_at_index(size_t index) const override {
            return vector[index];
        }
        
        T * data() const override {
            return vector.data();
        }
        
        std::vector<T>* data_as_vector() const override {
            return &vector;
        }
        
        size_t length() const override {
            return vector.size();
        }
        
        std::vector<DERIVED> lines() const {
            size_t len = length();
            if (len == 0) {
                return {};
            }
            
            std::vector<DERIVED> vec;
            
            T eof = static_cast<const DERIVED*>(this)->get_end_of_file();
            
            const T nl = get_new_line();
            DERIVED c;
            bool l = true;
            for (const T & t : *this) {
                l = false;
                if (t == nl) {
                    l = true;
                    vec.push_back(c);
                    c = {};
                } else {
                    T s[2] = { t, eof };
                    c.append(DERIVED(s, 1));
                }
            }
            if (!l) {
                vec.push_back(c);
            }
            return vec;
        }
        
        void resize(size_t capacity) override {
            if (capacity == 0) {
                erase(0, length());
            } else {
                vector.resize(capacity);
            }
        }
        
        void insert(const BasicStringAdapter<T> & what, size_t pos) override {
            auto * vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                vector.insert(vector.begin() + clamp_pos(pos), vec->begin(), vec->end());
            } else {
                auto * p = what.data();
                auto * l = what.length();
                auto v = std::vector<T>(p, l);
                vector.insert(vector.begin() + clamp_pos(pos), v.begin(), v.end());
            }
        }
        
        virtual void erase(size_t pos, size_t length) {
            
            // be naive
            
            size_t len = size();
            
            size_t p = clamp_pos(pos);
            
            if (p >= len) {
                // nothing to erase
                return;
            }
            
            size_t l = clamp_length(p, length);
            
            if (pos == 0 && l == len) {
                vector = {};
            } else if (l != 0) {
                vector.erase(vector.begin() + p, l);
            }
        }
        
        ~VectorAdapter() override {
        }
    };
    
    template <typename T, typename DERIVED>
    struct PointerAdapter : public BasicStringAdapter<T> {
        
        T* ptr = nullptr;
        size_t len = 0;
        
        using BASE = BasicStringAdapter<T>;
        using BASE::append;
        using BASE::replace;
        using BASE::size;
        using BASE::line_count;
        using BASE::get_new_line;
        using BASE::operator[];
        using BASE::mem_cpy;
        using BASE::clamp_pos;
        using BASE::clamp_length;
        
        PointerAdapter() {
            init(nullptr);
        }
        
        PointerAdapter(const T * ptr) {
            init(ptr);
        }
        
        PointerAdapter(const T * ptr, size_t length) {
            init(ptr, length);
        }
        
        PointerAdapter(const PointerAdapter & other) {
            len = other.len;
            ptr = new T[len+1];
            mem_cpy(ptr, other.ptr, len+1);
        }
        
        const PointerAdapter & operator=(const PointerAdapter & other) {
            len = other.len;
            ptr = new T[len+1];
            mem_cpy(ptr, other.ptr, len+1);
            return *this;
        }
        
        PointerAdapter(PointerAdapter && other) {
            len = other.len;
            ptr = new T[len+1];
            mem_cpy(ptr, other.ptr, len+1);
        }
        
        PointerAdapter & operator=(PointerAdapter && other) {
            len = other.len;
            ptr = new T[len+1];
            mem_cpy(ptr, other.ptr, len+1);
            return *this;
        }
        
        void init(const T * ptr) {
            
            T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            T empty[1] { eof };
            
            if (ptr == nullptr) {
                init(empty, 0);
                return;
            }
            size_t length = 0;
            
            while(true) {
                if (ptr[length] == eof) {
                    break;
                }
                length++;
            }
            
            init(ptr, length);
            
        }
        
        void init(const T * ptr, size_t length) {
            T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            T empty[1] { eof };
            
            if (ptr == nullptr) {
                init(empty, 0);
                return;
            }
            
            len = length;
            this->ptr = new T[len+1];
            if (len != 0) {
                mem_cpy(this->ptr, ptr, len);
            }
            this->ptr[len] = eof;
            
        }
        
        T & get_item_at_index(size_t index) const override {
            return ptr[index];
        }
        
        T * data() const override {
            return ptr;
        }
        
        std::vector<T>* data_as_vector() const override {
            return nullptr;
        }
        
        size_t length() const override {
            return len;
        }
        
        std::vector<DERIVED> lines() const {
            size_t len = length();
            if (len == 0) {
                return {};
            }
            
            std::vector<DERIVED> vec;
            
            T eof = static_cast<const DERIVED*>(this)->get_end_of_file();
            
            const T nl = get_new_line();
            DERIVED c;
            bool l = true;
            for (const T & t : *this) {
                l = false;
                if (t == nl) {
                    l = true;
                    vec.push_back(c);
                    c = {};
                } else {
                    T s[2] = { t, eof };
                    c.append(DERIVED(s, 1));
                }
            }
            if (!l) {
                vec.push_back(c);
            }
            return vec;
        }
        
        void resize(size_t capacity) override {
            auto len = length();
            if (capacity != len) {
                if (capacity == 0) {
                    for (size_t i = 0; i < len; i++) {
                        ptr[i].~T();
                    }
                    delete[] ptr;
                    ptr = nullptr;
                    len = 0;
                } else {
                    T * tmp = new T[capacity+1];
                    mem_cpy(tmp, ptr, capacity > len ? len : capacity);
                    for (size_t i = capacity; i < len; i++) {
                        ptr[i].~T();
                    }
                    delete[] ptr;
                    ptr = tmp;
                    len = capacity;
                    ptr[len] = static_cast<DERIVED*>(this)->get_end_of_file();
                }
            }
        }
        
        void insert(const BasicStringAdapter<T> & what, size_t pos) override {
            size_t what_len = what.size();
            size_t tmp_len = len + what_len;
            auto p = clamp_pos(pos);
            T * tmp = new T[tmp_len+1];
            // what.data is null terminated
            if (p == len) {
                // insert at end
                mem_cpy(&tmp[0], ptr, len);
                mem_cpy(&tmp[len], what.data(), what_len+1);
            } else if (p == 0) {
                // insert at beginning
                mem_cpy(&tmp[0], what.data(), what_len);
                mem_cpy(&tmp[what_len], ptr, len+1);
            } else {
                // insert at middle
                mem_cpy(&tmp[0], ptr, p);
                mem_cpy(&tmp[p], what.data(), what_len);
                mem_cpy(&tmp[p+what_len], &ptr[p], (len-p)+1);
            }
            delete[] ptr;
            ptr = tmp;
            len = tmp_len;
        }
        
        virtual void erase(size_t pos, size_t length) override {
            
            // be naive
            
            size_t len = size();
            
            size_t p = clamp_pos(pos);
            
            if (p >= len) {
                // nothing to erase
                return;
            }
            
            size_t l = clamp_length(p, length);
            
            if (pos == 0 && l == len) {
                for (size_t i = 0; i < len; i++) {
                    ptr[i].~T();
                }
                delete[] ptr;
                ptr = nullptr;
                len = 0;
            } else if (l != 0) {
                size_t tmp_len = len - l;
                T * tmp = new T[tmp_len+1];
                mem_cpy(&tmp[0], ptr, tmp_len);
                tmp[tmp_len] = static_cast<DERIVED*>(this)->get_end_of_file();
                for (size_t i = tmp_len; i < len; i++) {
                    ptr[i].~T();
                }
                delete[] ptr;
                ptr = tmp;
                len = tmp_len;
            }
        }
        
        ~PointerAdapter() override {
            for (size_t i = 0; i < len; i++) {
                ptr[i].~T();
            }
            delete[] ptr;
            ptr = nullptr;
            len = 0;
        }
    };
    
    template <typename UNUSED = void>
    struct CharAdapter_impl : PointerAdapter<char, CharAdapter_impl<UNUSED>> {
        using BASE = PointerAdapter<char, CharAdapter_impl<UNUSED>>;
        using BASE::BASE;
        using BASE::init;
        using BASE::data;
        using BASE::size;
        using BASE::line_count;
        using BASE::resize;
        using BASE::operator[];
        using BASE::append;
        using BASE::replace;
        using BASE::erase;
        
        CharAdapter_impl(const std::string & str) {
            init(str.data(), str.length());
        }
        
        void append(const char * s, size_t s_len) {
            CharAdapter_impl<void> tmp(s, s_len);
            append(tmp);
        }
        
        void insert(size_t pos, size_t len, const char * s) {
            CharAdapter_impl<void> tmp(s, strlen(s));
            insert(tmp, pos, len);
        }
        
        void replace(size_t pos, size_t len, const char * s) {
            CharAdapter_impl<void> tmp(s, strlen(s));
            replace(tmp, pos, len);
        }
        
        const char * c_str() const override {
            return data(); // no conversion needed
        }
        
        bool c_str_is_allocated() const override {
            return false;
        }
        
        const char get_new_line() const override {
            return '\n';
        }
        
        // called by base class
        const char get_end_of_file() const {
            return '\0';
        }
    };
    
    using CharAdapter = CharAdapter_impl<void>;
}