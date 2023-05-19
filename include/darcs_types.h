#pragma once

#include <deque>
#include <optional>
#include <utility>
#include <stdexcept>
#include <algorithm>

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
        mutable ListConst tag = NIL;
        struct CONS {
            mutable T head;
            mutable const List<T> * tail = nullptr;
        } dataCONS;
        struct SNOC {
            mutable const List<T> * head = nullptr;
            mutable T tail;
        } dataSNOC;
        mutable LIST * list = nullptr;
    };

    template <typename T>
    struct FL {
        const List<T, FL<T>> * h;

        FL() : h(new List<T, FL<T>>()) {
            h->list = this;
        };

        FL(const List<T, FL<T>> * h) : h(h) {
            h->list = this;
        };

        FL<T> append(const T & value) const {
            List<T> * a = new List<T, FL<T>>();
            a->list = this;
            a->tag = CONS;

            // this would prepend
            // [h] -> [a, h]

            a->dataCONS.head = value;
            a->dataCONS.tail = h; // this

            // we are immutable
            // return a new instance
            // that links to this
            return FL<T>(a);
        }

        struct Iterator {
            const FL<T> * i;
            mutable const List<T, FL<T>> * ptr;

            Iterator(const FL<T> * i) : i(i), ptr(i->h) {}


            bool has_next() const {
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

        bool IsNil() {
            return iterator().has_next();
        }

        bool contains(const T & item) {
            auto it = iterator();
            while (it.has_next()) {
                if (it.next() == item) return true;
            }
            return false;
        }

        const FL<T> & popFL() {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return *it.nextFL();
        }

        const T & pop() {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return it.next();
        }

        void extract(T & x, FL<T> & xs) {
            x = pop();
            xs = popRL();
        }
    };

    template <typename T>
    struct RL {
        const List<T, RL<T>> *h = nullptr;
        const List<T, RL<T>> *start = nullptr;
        const List<T, RL<T>> *end = nullptr;

        RL() : h(new List<T, RL<T>>()), start(h), end(start) {
            h->list = this;
        };
        
        RL(const List<T, RL<T>> * h, const List<T, RL<T>> * start, const List<T, RL<T>> * end) : h(h), start(start), end(end) {
            h->list = this;
        };

        const RL<T> append(const T & value) const {

            // this would append
            // [] -> [h] -> [h, a]

            List<T, RL<T>> *a = new List<T, RL<T>>();
            a->list = this;
            a->tag = SNOC;
            a->dataSNOC.tail = value;
            end->dataSNOC.head = a;

            // we are immutable
            // return a new instance
            // that links to this
            return RL<T>(h, start, a);
        }

        struct Iterator {
            const RL<T> * i;
            mutable const List<T, RL<T>> * ptr;

            Iterator(const RL<T> * i) : i(i), ptr(i->start) {}


            bool has_next() const {
                return ptr != i->end;
            }

            const T & next() const {
                if (!has_next()) {
                    throw std::runtime_error("attempting to iterate an empty list");
                }
                ptr = ptr->dataSNOC.head;
                return ptr->dataSNOC.tail;
            }

            const RL<T> & nextRL() const {
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

        bool IsNil() {
            return iterator().has_next();
        }

        bool contains(const T & item) {
            auto it = iterator();
            while (it.has_next()) {
                if (it.next() == item) return true;
            }
            return false;
        }

        const RL<T> & popRL() {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return *it.nextRL();
        }

        const T & pop() {
            auto it = iterator();
            if (!it.has_next()) {
                throw std::runtime_error("attempting to pop an empty list");
            }
            return it.next();
        }

        void extract(T & x, RL<T> & xs) {
            x = pop();
            xs = popRL();
        }
    };

    // in pattern matching, x :<: xs
    //  the compiler deconstructs the list
    //   and then reconstructs it for pattern matching
    //
    //  [1, 2]   ->   x :<: xs   ->   1 :<: [2]
    //  [1, 2]   ->   x :>: xs   ->   [1] :>: 2
    //

    template <typename T>
    static const FL<T> ToFL(const RL<T> & rl) {
        auto it = rl.iterator();
        // avoid recursion
        const FL<T> fl;
        while (it.has_value()) fl = fl.append(it.value());
        return fl;
    }

    template <typename T>
    static const RL<T> ToRL(const FL<T> & fl) {
        auto it = fl.iterator();
        // avoid recursion
        const RL<T> rl;
        while (it.has_value()) rl = rl.append(it.value());
        return rl;
    }


    struct Nothing {};

    template <typename T>
    struct Maybe {
        bool has_value;
        T value_;

        Maybe() {
            has_value = false;
        }
        Maybe(const Nothing & nothing) {
            has_value = false;
        }
        Maybe(const T & value) {
            value_ = value;
            has_value = true;
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
        Set(const std::set<T> & set) {
            this->set = set;
        }

        Set<T> & append_in_place(const T & item) {
            set.push(item);
            return *this;
        }

        Set<T> append(const T & item) {
            Set<T> copy = *this;
            copy.append_in_place(item);
            return copy;
        }

        bool contains(const T & item) {
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
        Map(const std::map<T> & map) {
            this->map = map;
        }

        Map<K, V> & insert_in_place(const K & key, const V & value) {
            map.insert(key, value);
            return *this;
        }

        Map<K, V> & insert(const K & key, const V & value) {
            Map<K, V> copy = *this;
            copy.insert(key, value);
            return copy;
        }

        bool contains(const K & key) {
            return map.find(key) != map.end();
        }

        Maybe<V> lookup(const K & key) {
            auto s = map.find(key);
            return s != map.end() ? s.second : Nothing();
        }
    };


    // invertFL :: Invert p => FL p wX wY -> RL p wY wX
    template <typename T>
    static const RL<T> invertFL(const FL<T> & fl) {
        // invertFL NilFL = NilRL
        if (fl.isNil()) {
            return RL<T>();
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
        if (rl.isNil()) {
            return FL<T>();
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
    static Tuple2<T, T> invert(Tuple2<T, T> p) {
        // invert (a :> b) = invert b :> invert a
        return Tuple2<T, T>(invert(p.v1), invert(p.v2));
    }

    typedef size_t PatchId;

    struct Patch {
        virtual PatchId ident() = 0;
        virtual void invert() = 0;
    }

    template <typename T>
    struct Named {
        T n;
        FL<T> d;
        FL<T> p;
        Named(const T & n, const FL<T> & d, const FL<T> & p) {
            this->n = n;
            this->d = d;
            this->p = p;
        }
    };

    struct PatchInfoAndG {
        Named<Patch*> n;
        PatchInfoAndG(Patch* p) {
            n = {p, {}, {}};
        }
        PatchInfoAndG(Named<Patch*> n) {
            this->n = n;
        }
    };

    // PIAP defines the following
    /*
        PIAP
            PatchInfo
                META-DATA
            Hashed
                WithSize
                PatchInfoAnd.Actually
                    NamedP
                        PatchInfo
                            META-DATA
                        []
                        (:>:
                            (
                                COMMIT_MESSAGE
                                Hunk
                            )
                            NilFL
                        )
    */

    /*
        // reads patch metadata and returns a FileHunk
        //
        // when might we need to skip new lines ?
        //

        readHunk :: FileNameFormat -> Parser (Prim wX wY)
        readHunk fmt = do
        string hunk'
        fi <- readFileName fmt
        l <- int
        have_nl <- skipNewline
        if have_nl
            then do
            _ <- linesStartingWith ' ' -- skipping context
            old <- linesStartingWith '-'
            new <- linesStartingWith '+'
            _ <- linesStartingWith ' ' -- skipping context
            return $ hunk fi l old new
            else return $ hunk fi l [] []

        skipNewline :: Parser Bool
        skipNewline = option False (char '\n' >> return True)
    */

    struct FileHunk : Patch {
        PatchId id;
        size_t line;
        adapter_t old_line;
        adapter_t new_line;

        FileHunk() {
            id = -1;
            line = 0;
        }
        FileHunk(PatchId id, size_t line, const adapter_t & old_line, const adapter_t & new_line) {
            this->id = id;
            this->line = line;
            this->old_line = old_line;
            this->new_line = new_line;
        }

        PatchId ident() override {
            return id;
        }

        void invert() override {
            return FileHunk(id, line, new_line, old_line);
        }
    };

    typedef Tuple2<Set<PatchId>, Set<PatchId>> Dep;
    typedef Tuple2<Set<PatchInfoAndG>, Set<PatchInfoAndG>> Deps;

};