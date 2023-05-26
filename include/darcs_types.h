#pragma once

#include <deque>
#include <optional>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include "adapter.h"

/*

-- enabling stuff seems to mess with the debugger
:load darcs/MemHunk.hs
:set -v3
:set stop :list
import MemHunk
:b makeHunk
:step memhunk

*/

namespace DarcsPatch {

    template <typename T1, typename T2>
    struct Tuple2 {
        T1 v1;
        T2 v2;
        Tuple2(const T1 & t1, const T2 & t2) {
            v1 = t1;
            v2 = t2;
        }
    };

    template <typename T1, typename T2, typename T3>
    struct Tuple3 {
        T1 v1;
        T2 v2;
        T3 v3;
        Tuple3(const T1 & t1, const T2 & t2, const T3 & t3) {
            v1 = t1;
            v2 = t2;
            v3 = t3;
        }
    };

    enum ListConst { CONS, SNOC, NIL };

    template <typename T, typename LIST>
    struct List {
        const ListConst tag;
        struct CONS {
            const T head;
            const List<T, LIST> * tail;
            CONS() : head(T()), tail(nullptr) {}
            CONS(const T head, const List<T, LIST> * tail) : head(head), tail(tail) {}
        } const dataCONS;
        struct SNOC {
            // this is the only variable in RL that MUST be mutable
            const mutable List<T, LIST> * head;
            const T tail;
            SNOC() : head(nullptr), tail(T()) {}
            SNOC(const List<T, LIST> * head, const T tail) : head(head), tail(tail) {}
        } const dataSNOC;
        
        const LIST * list;
        List() : tag(NIL), list(nullptr), dataCONS({}), dataSNOC({}) {}
        List(const LIST * list) : tag(NIL), list(list), dataCONS({}), dataSNOC({}) {}
        List(const ListConst tag, const LIST * list) : tag(tag), list(list), dataCONS({}), dataSNOC({}) {}
        List(const LIST * list, const CONS cons) : tag(ListConst::CONS), list(list), dataCONS(cons), dataSNOC({}) {}
        List(const LIST * list, const SNOC snoc) : tag(ListConst::SNOC), list(list), dataCONS({}), dataSNOC(snoc) {}
        List(const LIST * list, const List<T, LIST> * list_) : tag(list_->tag), list(list), dataCONS(list_->dataCONS), dataSNOC(list_->dataSNOC) {}
        virtual ~List() {}
    };

    template <typename T>
    struct FL;
    template <typename T>
    struct RL;

    struct NilFL_T {
        template <typename T>
        const FL<T> append(const T & value) const;
    };

    extern const NilFL_T NilFL;

    template <typename T>
    struct FL {
        // needs to be mutable for assignments to work
        mutable List<T, FL<T>> * h;

        FL() : h(new List<T, FL<T>>(NIL, this)) {};
        FL(const NilFL_T) : h(new List<T, FL<T>>(NIL, this)) {};

        FL(const List<T, FL<T>> * h) : h(new List<T, FL<T>>(this, h)) {};

        FL(const FL & other) : h(new List<T, FL<T>>(this, other.h)) {}

        const FL<T> operator=(const FL & other) const {
            h = new List<T, FL<T>>(this, other.h);
            return *this;
        }

        const bool operator == (const NilFL_T & other) const {
            return !iterator().has_next();
        }

        const FL<T> append(const T & value) const {
            // we are immutable
            // return a new instance
            // that links to this (h)
            //
            // this would prepend
            // [h] -> [a, h]
            //
            return FL<T>(new List<T, FL<T>>(this, {value, h}));
        }

        struct Iterator {
            const FL<T> * i;
            mutable const List<T, FL<T>> * ptr;

            Iterator(const FL<T> * i) : i(i), ptr(i->h) {}


            const bool has_next() const {
                return ptr->tag != NIL;
            }

            const T & next() const {
                if (!has_next()) {
                    throw std::runtime_error("attempting to iterate an empty list");
                }
                const T & value = ptr->dataCONS.head;
                ptr = ptr->dataCONS.tail;
                return value;
            }

            const FL<T> * nextFL() const {
                return ptr->list;
            }
        };

        const Iterator iterator() const {
            return Iterator(this);
        }

        void print(const char * id) const {
            std::cout << id << " = [";
            auto it = iterator();
            if (it.has_next()) {
                std::cout << it.next();
                while (it.has_next()) {
                    std::cout << ", " << it.next();
                }
            }
            std::cout << "]" << std::endl;
        }

        const bool operator == (const FL<T> & other) const {
            if (h->tag == other.h->tag) {
                auto it1 = iterator();
                auto it2 = other.iterator();
                while (it1.has_next()) {
                    if (it2.has_next()) {
                        if (it1.next() != it2.next()) return false;
                    } else {
                        return false;
                    }
                }
            }
            return false;
        }

        const bool operator != (const FL<T> & other) const {
            return !(*this == other);
        }

        const bool contains(const T & item) const {
            auto it = iterator();
            while (it.has_next()) {
                if (it.next() == item) return true;
            }
            return false;
        }

        const FL<T> & popFL() const {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return *it.nextFL();
        }

        const T & pop() const {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return it.next();
        }

        void extract(T & x, FL<T> & xs) const {
            x = pop();
            xs = popFL();
        }
    };

    template <typename T>
    const FL<T> NilFL_T::append(const T & value) const {
        return FL<T>().append(value);
    }
        
    struct NilRL_T {
        template <typename T>
        const RL<T> append(const T & value) const;
    };

    extern const NilRL_T NilRL;

    template <typename T>
    struct RL {
        // needs to be mutable for assignments to work
        mutable List<T, RL<T>> *h = nullptr;
        const mutable List<T, RL<T>> *start = nullptr;
        const mutable List<T, RL<T>> *end = nullptr;

        RL() : h(new List<T, RL<T>>(NIL, this)), start(h), end(start) {};
        RL(const NilRL_T) : h(new List<T, RL<T>>(NIL, this)), start(h), end(start) {};
        
        RL(const List<T, RL<T>> * h, const List<T, RL<T>> * start, const List<T, RL<T>> * end) : h(new List<T, RL<T>>(this, h)), start(start), end(end) {};

        RL(const RL & other) : h(new List<T, RL<T>>(this, other.h)), start(other.start), end(other.end) {}

        const RL<T> operator=(const RL & other) const {
            h = new List<T, RL<T>>(this, other.h);
            start = other.start;
            end = other.end;
            return *this;
        }

        const bool operator == (const NilRL_T & other) const {
            return !iterator().has_next();
        }

        const RL<T> append(const T & value) const {

            // this would append
            // [] -> [h] -> [h, a]

            end->dataSNOC.head = new List<T, RL<T>>(this, {nullptr, value});

            // we are immutable
            // return a new instance
            // that links to this
            return RL<T>(h, start, end->dataSNOC.head);
        }

        struct Iterator {
            const RL<T> * i;
            mutable const List<T, RL<T>> * ptr;

            Iterator(const RL<T> * i) : i(i), ptr(i->start) {}


            const bool has_next() const {
                return ptr != i->end;
            }

            const T & next() const {
                if (!has_next()) {
                    throw std::runtime_error("attempting to iterate an empty list");
                }
                ptr = ptr->dataSNOC.head;
                return ptr->dataSNOC.tail;
            }

            const RL<T> * nextRL() const {
                ptr = ptr->dataSNOC.head;
                return ptr->list;
            }
        };

        const Iterator iterator() const {
            return Iterator(this);
        }

        void print(const char * id) const {
            std::cout << id << " = [";
            auto it = iterator();
            if (it.has_next()) {
                std::cout << it.next();
                while (it.has_next()) {
                    std::cout << ", " << it.next();
                }
            }
            std::cout << "]" << std::endl;
        }

        const bool operator == (const RL<T> & other) const {
            if (h->tag == other.h->tag) {
                auto it1 = iterator();
                auto it2 = other.iterator();
                while (it1.has_next()) {
                    if (it2.has_next()) {
                        if (it1.next() != it2.next()) return false;
                    } else {
                        return false;
                    }
                }
            }
            return false;
        }

        const bool operator != (const RL<T> & other) const {
            return !(*this == other);
        }

        const bool contains(const T & item) const {
            auto it = iterator();
            while (it.has_next()) {
                if (it.next() == item) return true;
            }
            return false;
        }

        const RL<T> & popRL() const {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return *it.nextRL();
        }

        const T & pop() const {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return it.next();
        }

        void extract(T & x, RL<T> & xs) const {
            x = pop();
            xs = popRL();
        }
    };

    template <typename T>
    const RL<T> NilRL_T::append(const T & value) const {
        return RL<T>().append(value);
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
        return NilFL.append(item);
    }

    template <typename T>
    static const FL<T> ToFL(const std::vector<T> & vec) {
        const FL<T> fl;
        // avoid recursion
        for (const T & item : vec) fl = fl.append(item);
        return fl;
    }

    template <typename T>
    static const FL<T> ToFL(const RL<T> & rl) {
        auto it = rl.iterator();
        const FL<T> fl;
        // avoid recursion
        while (it.has_next()) fl = fl.append(it.next());
        return fl;
    }

    template <typename T>
    static const RL<T> ToRL(const T & item) {
        return NilRL.append(item);
    }

    template <typename T>
    static const RL<T> ToRL(const std::vector<T> & vec) {
        const RL<T> rl;
        // avoid recursion
        for (const T & item : vec) rl = rl.append(item);
        return rl;
    }

    template <typename T>
    static const RL<T> ToRL(const FL<T> & fl) {
        auto it = fl.iterator();
        // avoid recursion
        const RL<T> rl;
        while (it.has_next()) rl = rl.append(it.next());
        return rl;
    }


    struct Nothing {};

    template <typename T>
    struct Maybe {
        bool has_value;
        T value;

        Maybe() : has_value(false) {}
        Maybe(const Nothing & nothing) : has_value(false) {}
        Maybe(const T & value) : value(value), has_value(true) {}

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

        const T & operator -> () const {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }

        T & operator -> () {
            if (!has_value) {
                throw std::runtime_error("attempting to obtain the value of nothing");
            }
            return value;
        }
    };

    template <typename T>
    struct Set {
        std::set<T> set;

        Set() {}
        Set(const std::set<T> & set) : set(set) {}

        Set<T> & append_in_place(const T & item) {
            set.push(item);
            return *this;
        }

        Set<T> append(const T & item) {
            Set<T> copy = *this;
            copy.append_in_place(item);
            return copy;
        }

        const bool contains(const T & item) const {
            for(const T & v : set) {
                if (v == item) return true;
            }
            return false;
        }
    };

    template <typename K, typename V>
    struct Map {
        std::map<K, V> map;

        Map() {}
        Map(const std::map<K, V> & map) {
            this->map = map;
        }

        Map<K, V> & insert_in_place(const K & key, const V & value) {
            map.insert(key, value);
            return *this;
        }

        Map<K, V> insert(const K & key, const V & value) {
            Map<K, V> copy = *this;
            copy.insert(key, value);
            return copy;
        }

        const bool contains(const K & key) const {
            return map.find(key) != map.end();
        }

        const Maybe<V> lookup(const K & key) const {
            auto s = map.find(key);
            return s != map.end() ? s.second : Nothing();
        }
    };


    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct H {
        const adapter_t h;
        H(const adapter_t & h) : h(h) {};
    };

    enum V2_PRIM {
        NORMAL, DUPLICATE, ETACILPUD, CONFLICTOR, INV_CONFLICTOR
    };

    enum PATCH_TYPE {
        ADD_FILE, REMOVE_FILE, HUNK
    };

    // this MUST NOT be a template
    struct Patch {
        // must always return an allocated patch
        virtual const Patch* invert() const = 0;

        virtual const PATCH_TYPE type() const = 0;
        const V2_PRIM v2_prim = NORMAL;
        virtual ~Patch() {}
    };


    struct AddFile : Patch {
        AddFile();
        const PATCH_TYPE type() const override;
        const Patch* invert() const override;
    };

    struct RemoveFile : Patch {
        RemoveFile();
        const PATCH_TYPE type() const override;
        const Patch* invert() const override;
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct FileHunk : Patch {
        const size_t line;
        const FL<adapter_t> old_lines;
        const FL<adapter_t> new_lines;
        FileHunk() : line(0), old_lines(NilFL), new_lines(NilFL) {}
        FileHunk(const size_t & line, const FL<adapter_t>& old_lines, const FL<adapter_t>& new_lines) : line(line), old_lines(old_lines), new_lines(new_lines) {}
        FileHunk(const size_t & line, const adapter_t& old_line, const adapter_t& new_line) : line(line), old_lines(ToFL(old_line.lines())), new_lines(ToFL(new_line.lines())) {}

        const PATCH_TYPE type() const override {
            return HUNK;
        }

        const Patch* invert() const override {
            return new FileHunk(line, new_lines, old_lines);
        }
    };

    using FileHunk_T = FileHunk<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct PatchInfo {
        const adapter_t date;
        const adapter_t name;
        const adapter_t author;
        const FL<adapter_t> log;
        bool legacyIsInverted = false;
        PatchInfo() : date(adapter_t()), name(adapter_t()), author(adapter_t()), log(NilFL) {}
        PatchInfo(const adapter_t & name) : date(adapter_t()), name(name), author(adapter_t()), log(NilFL) {}
        PatchInfo(const adapter_t & date, const adapter_t & name, const adapter_t & author, const FL<adapter_t> & log) : date(date), name(name), author(author), log(log) {}

        const bool operator == (const PatchInfo<char_t, adapter_t> & other) const {
            return date == other.date && name == other.name && author == other.author && log == other.log;
        }
    };

    using PatchInfo_T = PatchInfo<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct AnchorPath {
        const std::vector<adapter_t> names;
        AnchorPath() : names({}) {}
        AnchorPath(const std::vector<adapter_t> & names) : names(names) {}
    };

    using AnchorPath_T = AnchorPath<char, StringAdapter::CharAdapter>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct Core_FP {
        const AnchorPath<char_t, adapter_t> anchor_path;
        const Patch* patch;
        Core_FP() : anchor_path(AnchorPath<char_t, adapter_t>()), patch(nullptr) {}
        Core_FP(const Patch* p) : anchor_path(AnchorPath<char_t, adapter_t>()), patch(p) {}
    };

    using Core_FP_T = Core_FP<char, StringAdapter::CharAdapter>;

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct Named {
        PatchInfo<char_t, adapter_t> n;
        const FL<PatchInfo<char_t, adapter_t>> d;
        const FL<T> p;
        Named() : n(PatchInfo<char_t, adapter_t>()), d(NilFL), p(NilFL) {}
        Named(const PatchInfo<char_t, adapter_t> & n, const FL<PatchInfo<char_t, adapter_t>> & d, const FL<T> & p) : n(n), d(d), p(p) {}

        PatchInfo<char_t, adapter_t> ident() {
            return n;
        }
    };

    template <typename T>
    using Named_T = Named<T, char, StringAdapter::CharAdapter>;

    template <
        typename T,
        typename char_t,
        typename adapter_t
    >
    struct Named<T*, char_t, adapter_t, typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type> {
        PatchInfo<char_t, adapter_t>* n;
        const FL<PatchInfo<char_t, adapter_t>> d;
        const FL<T*> p;
        Named() : n(nullptr), d(NilFL), p(NilFL) {}
        Named(const PatchInfo<char_t, adapter_t> * n, const FL<PatchInfo<char_t, adapter_t>> & d, const FL<T*> & p) : n(n), d(d), p(p) {}
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    FileHunk<char_t, adapter_t>* makeHunk(const size_t & line, const adapter_t& old_line, const adapter_t& new_line) {
        return new FileHunk<char_t, adapter_t>(line, old_line, new_line);
    }

    FileHunk_T* makeHunk_T(const size_t & line, const StringAdapter::CharAdapter& old_line, const StringAdapter::CharAdapter& new_line);
    
    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> makeNamedWithType(Patch* patch_type) {
        return Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>(PatchInfo<char_t, adapter_t>(), NilFL, ToFL(Core_FP<char_t, adapter_t>(patch_type)));
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> makeNamedHunk(const adapter_t & patch_id_unique_label, const size_t & line, const adapter_t& old_line, const adapter_t& new_line) {
        return makeNamedWithType<Core_FP<char_t, adapter_t>, char_t, adapter_t>(patch_id_unique_label, makeHunk(line, old_line, new_line));
    }

    Named_T<Core_FP_T> makeNamedWithType_T(const StringAdapter::CharAdapter & patch_id_unique_label, Patch* patch_type);

    Named_T<Core_FP_T> makeNamedHunk_T(const StringAdapter::CharAdapter & patch_id_unique_label, const size_t & line, const StringAdapter::CharAdapter & old_line, const StringAdapter::CharAdapter & new_line);

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using Deps__ = Tuple2<Set<Named<Patch*, char_t, adapter_t>>, Set<Named<Patch*, char_t, adapter_t>>>;

    using Deps___T = Deps__<char, StringAdapter::CharAdapter>;

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

    template <typename T>
    static const T invert(const T & ptr);

    template <typename T>
    static const T* invert(const T* ptr);
    
    template <>
    const Patch* invert<Patch>(const Patch* p) {
        return p->invert();
    }

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
        return invert<T>(xs).append(invert<T>(x));
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
        return invert<T>(xs).append(invert<T>(x));
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
    
}
