#ifndef DARCS_PATCH_COMMUTE_H
#define DARCS_PATCH_COMMUTE_H

#include "darcs_types.h"
#include <functional>

namespace DarcsPatch {
    /*

    -- | Commute represents things that can be (possibly) commuted.
    --
    -- Instances should obey the following laws:
    --
    -- * Symmetry
    --
    --   prop> commute (p:>q) == Just (q':>p') <=> commute (q':>p') == Just (p':>q)
    --
    -- * If an instance @'Invert' p@ exists, then
    --
    --   prop> commute (p:>q) == Just (q':>p') <=> commute (invert q:>invert p) == Just (invert p':>invert q')
    --
    -- * The more general Square-Commute law
    --
    --   prop> commute (p:>q) == Just (q':>p') => commute (invert p:>q') == Just (q:>invert p')
    --
    --   is required to hold only for primitive patches, i.e. if there is /no/
    --   instance @'Merge' p@, because together with 'merge' it implies that
    --   any two patches commute.

    */

    /*
    // generic
    class Commute p where
        commute :: (p :> p) wX wY -> Maybe ((p :> p) wX wY)
        // commute(T, T) where T = `=> Commute (*T* p)`
            // where *T* is replaced with desired type

    // define implementation for FL
    instance Commute p => Commute (FL p) where // ...
        // commute (FL, FL)

    // define implementation for RL
    instance Commute p => Commute (RL p) where // ...
        // commute (RL, RL)
    */

    /*
        // (p :> p) asserts that we have a tuple that is the same type as p
        //   p :: Int // (Int, Int)
        //
        class Commute p where
            commute :: (p :> p) wX wY -> Maybe ((p :> p) wX wY)
    */

    // this basically means Commute is a generic class that accepts
    // a (T, T) and returns a (T, T)
    // where T is specified in an instance of this class

    // OPTIMIZATION: we know that Commute.hs only defines implementations for FL and RL
    //
    // this means we can transform the generic/template into a static class and utilize function overload resolution

    // Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, RL<Core_FP<char_t, adapter_t>>>> CommuteRLFL(const Tuple2<RL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> & p);

    struct Commute {

    //     static Maybe<Core_FP<char_t, adapter_t>> isConsistent(Core_FP<char_t, adapter_t> prim) {
    //         switch(prim->v2_prim) {
    //             case NORMAL:
    //             case DUPLICATE:
    //             case ETACILPUD:
    //                 return Nothing();
    //             case INV_CONFLICTOR:
    //                 {
    //                     return isConsistent(invert(prim));
    //                 }
    //             default:
    //                 return {prim};
    //         };
    //     }

    //     static Core_FP<char_t, adapter_t> assertConsistent(Core_FP<char_t, adapter_t> x) {
    //         auto e = isConsistent(x);
    //         return nullptr;
    //     }

    //     static Maybe<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> commute (Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & pair) {
    //         /*
    //             x ::
    //             Darcs.Patch.V2.RepoPatch.RepoPatchV2
    //                 Darcs.Patch.Prim.V1.Core.Prim wX wZ
    //             y ::
    //             Darcs.Patch.V2.RepoPatch.RepoPatchV2
    //                 Darcs.Patch.Prim.V1.Core.Prim wZ wY
    //             643      commute pair@(x :> y) =
    //             644        commuteNoConflicts (assertConsistent x :> assertConsistent y)
    //             645        `mplus`
    //             646        commuteConflicting pair
    //             647  
    //             ... [src/Darcs/Patch/V2/RepoPatch.hs:(644,7)-(646,29)] darcs>
    //         */

    //         Patch * x = pair.v1;
    //         Patch * y = pair.v2;
    //         return Nothing();
    //     }

        enum PERHAPHS_E {
            UNKNOWN, FAILED, SUCCEEDED
        };

        template <typename T>
        using Perhaps = Tuple2<PERHAPHS_E, T>;

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Perhaps<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> speedyCommute(const Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & p) {
            std::cout << "speedyCommute called with arguments p = " << p << "\n";
            // (p1@(FP f1 _) :> p2@(FP f2 _))
            // saves the FP to p1 and p2 in addition to extracting their members
            auto p1 = p.v1;
            auto p2 = p.v2;
            auto f1 = p1.anchor_path;
            auto f2 = p2.anchor_path;
            if (f1 != f2) {
                return {SUCCEEDED, {p2, p1}};
            }
            return {UNKNOWN, {}};
        }

        // a very small regex implementation, as per darcs
        //
        template <typename char_t>
        static std::function<bool(const char_t &)> normalRegChars(const FL<char_t> & cs) {
            std::cout << "normalRegChars called with arguments cs = " << cs << "\n";
            if (cs.size() == 0) {
                return [] (const char_t & f) { return false; };
            } else {
                char_t c1;
                FL<char_t> tmp1;
                cs.extract(c1, tmp1);
                if (c1 == '\\') {
                    char_t c2;
                    FL<char_t> tmp2;
                    tmp1.extract(c2, tmp2);
                    if (c2 == '.' || c2 == '-' || c2 == '\\') {
                        return [=] (const char_t & f) { return f == c2 || normalRegChars<char_t>(tmp2)(f); };
                    }
                    std::string m = "'\\";
                    m += ((char)c2);
                    m += "' not supported";
                    throw new std::runtime_error(m);
                } else {
                    char_t c2;
                    FL<char_t> tmp2;
                    tmp1.extract(c2, tmp2);
                    if (c2 == '-') {
                        char_t c3;
                        FL<char_t> tmp3;
                        tmp2.extract(c3, tmp3);
                        return [=] (const char_t & f) { return (f >= c1 && f <= c3) || normalRegChars<char_t>(tmp3)(f); };
                    }
                    return [=] (const char_t & f) { return f == c1 || normalRegChars<char_t>(tmp1)(f); };
                }
            }
        }

        template <typename char_t>
        static FL<char_t> unescapeChars(const FL<char_t> & cs) {
            std::cout << "unescapeChars called with arguments cs = " << cs << "\n";
            if (cs.size() == 0) {
                return cs;
            } else {
                char_t c1;
                FL<char_t> tmp1;
                cs.extract(c1, tmp1);
                if (c1 == '\\') {
                    char_t c2;
                    FL<char_t> tmp2;
                    tmp1.extract(c2, tmp2);
                    if (c2 == 'n') {
                        FL<char_t> tmp3;
                        tmp3 = tmp3.push('\n');
                        tmp3 = tmp3.push(unescapeChars<char_t>(tmp2));
                        return tmp3;
                    } else if (c2 == 't') {
                        FL<char_t> tmp3;
                        tmp3 = tmp3.push('\t');
                        tmp3 = tmp3.push(unescapeChars<char_t>(tmp2));
                        return tmp3;
                    } else if (c2 == '^') {
                        FL<char_t> tmp3;
                        tmp3 = tmp3.push('^');
                        tmp3 = tmp3.push(unescapeChars<char_t>(tmp2));
                        return tmp3;
                    }
                }
                FL<char_t> tmp2;
                tmp2 = tmp2.push(c1);
                tmp2 = tmp2.push(unescapeChars<char_t>(tmp1));
                return tmp2;
            }
        }

        template <typename char_t>
        static std::function<bool(const char_t &)> RegChars(const FL<char_t> & cs) {
            std::cout << "RegChars called with arguments cs = " << cs << "\n";
            if (cs[0] == '^') {
                char_t c;
                FL<char_t> tmp;
                cs.extract(c, tmp);
                return [=] (const char_t & f) { return ! normalRegChars<char_t>(unescapeChars<char_t>(tmp))(f); };
            }
            if (cs[0] == '\\' && cs[1] == '^') {
                char_t c;
                FL<char_t> tmp;
                cs.extract(c, tmp);
                return [=] (const char_t & f) { return normalRegChars<char_t>(unescapeChars<char_t>(tmp))(f); };
            }
            return [=] (const char_t & f) { return normalRegChars<char_t>(unescapeChars<char_t>(cs))(f); };
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static std::function<bool(const char_t &)> RegChars(const adapter_t & cs) {
            std::cout << "RegChars called with arguments cs = " << cs << "\n";
            FL<char_t> cs_;
            for (const char_t & c : cs) {
                cs_ = cs_.push(c);
            }
            return RegChars<char_t>(cs_);
        }

        //
        // a very small regex implementation

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static bool Any(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "Any called with arguments src = " << src << ", std::function\n";
            for (const char_t & c : src) {
                if (f(c)) {
                    return true;
                }
            }
            return false;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t Drop(const adapter_t & src, const std::size_t & count) {
            std::cout << "Drop called with arguments src = " << src << ", count = " << count << "\n";
            adapter_t copy = src;
            copy.erase(0, count);
            return copy;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<std::size_t> FindIndex(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "FindIndex called with arguments src = " << src << ", std::function\n";
            std::size_t x = 0;
            for (const char_t & c : src) {
                if (f(c)) {
                    return {x};
                }
                x++;
            }
            return Nothing();
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t TakeWhile(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "TakeWhile called with arguments src = " << src << ", std::function\n";
            adapter_t t;
            auto b = src.begin();
            auto e = src.end();
            for (; b != e; ++b) {
                if (f(*b)) {
                    t.append(*b);
                } else {
                    break;
                }
            }
            return t;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t TakeWhileEnd(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "TakeWhileEnd called with arguments src = " << src << ", std::function\n";
            adapter_t t;
            auto b = src.rbegin();
            auto e = src.rend();
            for (; b != e; ++e) {
                if (f(*e)) {
                    t.append(*e);
                } else {
                    break;
                }
            }
            return t;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t DropWhile(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "DropWhile called with arguments src = " << src << ", std::function\n";
            adapter_t t;
            auto b = src.begin();
            auto e = src.end();
            for (; b != e; ++b) {
                if (!f(*b)) {
                    break;
                }
            }
            for (; b != e; ++b) {
                t.append(*b);
            }
            return t;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t DropWhileEnd(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "DropWhileEnd called with arguments src = " << src << ", std::function\n";
            adapter_t t;
            auto b = src.rbegin();
            auto e = src.rend();
            for (; b != e; ++e) {
                if (!f(*e)) {
                    break;
                }
            }
            for (; b != e; ++b) {
                t.append(*b);
            }
            return t;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static adapter_t Take(const adapter_t & src, int n) {
            std::cout << "Take called with arguments src = " << src << ", n = " << n << "\n";
            adapter_t copy = src;
            copy.erase(n, -1);
            return copy;
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Tuple2<adapter_t, adapter_t> Span(const adapter_t & src, std::function<bool(const char_t &)> f) {
            std::cout << "Span called with arguments src = " << src << ", std::function\n";
            return {TakeWhile(src, f), DropWhile(src, f)};
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<RL<std::shared_ptr<adapter_t>>> loop(const std::size_t & from, const adapter_t & tokChars, const adapter_t & o, const adapter_t & n, const std::shared_ptr<adapter_t> & input) {
            std::cout << "loop called with arguments from = " << from << ", tokChars = " << tokChars << ", o = " << o << ", n = " << n << ", input = " << input << "\n";
            auto dropped = Drop<char_t, adapter_t>(*input.get(), from);
            auto start = FindIndex<char_t, adapter_t>(dropped, RegChars<char_t, adapter_t>(tokChars));
            if (!start.has_value) {
                return {{input}};
            } else {
                auto dropped1 = Drop<char_t, adapter_t>(*input.get(), from + start.value_ref());
                auto tmp = Span<char_t, adapter_t>(dropped1, RegChars<char_t, adapter_t>(tokChars));
                auto tok = tmp.v1;
                auto rest = tmp.v2;
                if (tok == o) {
                    // (B.take (from + start) input :).(new :) <$> loop 0 rest
                    //
                    // for each item in loop 0 rest
                    //   accumulate the concation of (B.take (from + start) input) + (new) + (item)
                    // and return the accumulated list as long as Nothing is never returned
                    //
                    auto items = loop<char_t, adapter_t>(0, tokChars, o, n, std::make_shared<adapter_t>(rest));
                    if (!items.has_value) {
                        return Nothing();
                    }
                    RL<std::shared_ptr<adapter_t>> r;
                    for (const std::shared_ptr<adapter_t> & item : items.value_ref()) {
                        adapter_t acc = Take<char_t, adapter_t>(*input.get(), from + start.value_ref());
                        acc.append_(n);
                        acc.append_(*item.get());
                        r = r.push(std::make_shared<adapter_t>(acc));
                    }
                    return {r};
                }
                if (tok == n) {
                    return Nothing();
                }
                return loop<char_t, adapter_t>(from + start.value_ref() + tok.length(), tokChars, o, n, input);
            }
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<adapter_t> tryTokReplace(const adapter_t & tokChars, const adapter_t & o, const adapter_t & n, const adapter_t & input) {
            std::cout << "tryTokReplace called with arguments tokChars = " << tokChars << ", o = " << o << ", n = " << n << ", input = " << input << "\n";
            if (o.size() == 0) {
                throw new std::runtime_error("tryTokReplace called with empty old token");
            }
            if (o.size() == 0) {
                throw new std::runtime_error("tryTokReplace called with empty old token");
            }
            if (Any<char_t, adapter_t>(o, [=] (const char_t & f) { return ! RegChars<char_t, adapter_t>(tokChars)(f); })) {
                throw new std::runtime_error("tryTokReplace called with old non-token");
            }
            if (Any<char_t, adapter_t>(n, [=] (const char_t & f) { return ! RegChars<char_t, adapter_t>(tokChars)(f); })) {
                throw new std::runtime_error("tryTokReplace called with new non-token");
            }

            // fmap B.concat . loop 0
            //
            // for each item in `loop 0 input`
            //   concat the result to a single item and then return that item
            //

            auto items1 = loop<char_t, adapter_t>(0, tokChars, o, n, std::make_shared<adapter_t>(input));
            if (!items1.has_value) {
                return Nothing();
            }
            adapter_t acc;
            for (const std::shared_ptr<adapter_t> & item : items1.value_ref()) {
                acc.append_(*item.get());
            }
            return {acc};
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<RL<adapter_t>> tryTokReplaces(const adapter_t & t, const adapter_t & o, const adapter_t & n, const RL<adapter_t>& items) {
            std::cout << "tryTokReplaces called with arguments t = " << t << ", o = " << o << ", n = " << n << ", items = " << items << "\n";
            // tryTokReplaces :: String -> B.ByteString -> B.ByteString
            //                -> [B.ByteString] -> Maybe [B.ByteString]
            // tryTokReplaces t o n = mapM (tryTokReplace t o n)
            //
            // translation:
            //
            // for    tryTokReplaces t po pn new1
            //
            // for each item in the list new1, invoke tryTokReplace t o n item
            // if the invokation returns Nothing then return Nothing
            // otherwise add the result to a list and then return that list as long as Nothing is never returned
            RL<adapter_t> result;
            for (const adapter_t & item : items) {
                Maybe<adapter_t> m = tryTokReplace<char_t, adapter_t>(t, o, n, item);
                if (!m.has_value) {
                    return Nothing();
                }
                result = result.push(m.value_ref());
            }
            return {result};
        }

        static Maybe<Tuple2<std::size_t, std::size_t>> commuteHunkLines(
            const std::size_t & line1, const std::size_t & len_old1, const std::size_t & len_new1,
            const std::size_t & line2, const std::size_t & len_old2, const std::size_t & len_new2
        ) {
            std::cout << "commuteHunkLines called with arguments line1 = " << line1 << ", len_old1 = " << len_old1 << ", len_new1 = " << len_new1 << ", line2 = " << line2 << ", len_old2 = " << len_old2 << ", len_new2 = " << len_new2 << "\n";
            if (line1 + len_new1 < line2) {
                return {{ line2 - len_new1 + len_old1, line1 }};
            }
            else if (line2 + len_old2 < line1) {
                return {{ line2, line1 + len_new2 - len_old2 }};
            }
            else if (len_old2 != 0 && len_old1 != 0 && len_new2 != 0 && len_new1 != 0 && line1 + len_new1 == line2) {
                return {{ line2 - len_new1 + len_old1, line1 }};
            }
            else if (len_old2 != 0 && len_old1 != 0 && len_new2 != 0 && len_new1 != 0 && line2 + len_old2 == line1) {
                return {{ line2, line1 + len_new2 - len_old2 }};
            }
            else return Nothing();
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Perhaps<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> commuteFP(const AnchorPath<char_t, adapter_t> & f, const Tuple2<std::shared_ptr<Patch>, std::shared_ptr<Patch>> & p) {
            std::cout << "commuteFP called with arguments f = " << f << ", p = " << p << "\n";
            if (p.v2->type() == HUNK) {
                FileHunk<char_t, adapter_t> * f2 = static_cast<FileHunk<char_t, adapter_t>*>(p.v2.get());
                if (f2->old_lines == NilRL && f2->new_lines == NilRL) {
                    return {SUCCEEDED, { Core_FP<char_t, adapter_t>(f, p.v2), Core_FP<char_t, adapter_t>(f, p.v1) }};
                }
            }
            if (p.v1->type() == HUNK) {
                FileHunk<char_t, adapter_t> * f1 = static_cast<FileHunk<char_t, adapter_t>*>(p.v1.get());
                if (f1->old_lines == NilRL && f1->new_lines == NilRL) {
                    return {SUCCEEDED, { Core_FP<char_t, adapter_t>(f, p.v2), Core_FP<char_t, adapter_t>(f, p.v1) }};
                }
            }
            if (p.v1->type() == HUNK && p.v2->type() == HUNK) {
                FileHunk<char_t, adapter_t> * f1 = static_cast<FileHunk<char_t, adapter_t>*>(p.v1.get());
                FileHunk<char_t, adapter_t> * f2 = static_cast<FileHunk<char_t, adapter_t>*>(p.v2.get());
                std::cout << "calling commuteHunkLines with arguments f1 = " << *f1 << ", f2 = " << *f2 << "\n";
                auto m = commuteHunkLines(f1->line, f1->old_lines.size(), f1->new_lines.size(), f2->line, f2->old_lines.size(), f2->new_lines.size());
                if (!m.has_value) {
                    return {FAILED, {}};
                } else {
                    auto line2 = m->v1;
                    auto line1 = m->v2;
                    Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> t;
                    t.v1 = Core_FP<char_t, adapter_t>(f, makeHunk<char_t, adapter_t>(line2, f2->old_lines, f2->new_lines));
                    t.v2 = Core_FP<char_t, adapter_t>(f, makeHunk<char_t, adapter_t>(line1, f1->old_lines, f1->new_lines));
                    return {SUCCEEDED, t};
                }
            }
            if (p.v1->type() == HUNK && p.v2->type() == TOK_REPLACE) {
                FileHunk<char_t, adapter_t> * f1 = static_cast<FileHunk<char_t, adapter_t>*>(p.v1.get());
                TokReplace<char_t, adapter_t> * t1 = static_cast<TokReplace<char_t, adapter_t>*>(p.v2.get());
                auto & po = t1->o;
                auto & pn = t1->n;
                auto tmp = tryTokReplaces<char_t, adapter_t>(t1->t, po, pn, f1->old_lines);
                if (!tmp.has_value) {
                    return {FAILED, {}};
                }
                auto & old1 = tmp.value_ref();
                auto tmp1 = tryTokReplaces<char_t, adapter_t>(t1->t, po, pn, f1->new_lines);
                if (!tmp1.has_value) {
                    return {FAILED, {}};
                }
                auto & new1 = tmp1.value_ref();
                Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> t;
                t.v1 = Core_FP<char_t, adapter_t>(f, makeTokReplace<char_t, adapter_t>(t1->t, t1->o, t1->n));
                t.v2 = Core_FP<char_t, adapter_t>(f, makeHunk<char_t, adapter_t>(f1->line, old1, new1));
                return {SUCCEEDED, t};
            }
            if (p.v1->type() == TOK_REPLACE && p.v2->type() == TOK_REPLACE) {
                TokReplace<char_t, adapter_t> * t1 = static_cast<TokReplace<char_t, adapter_t>*>(p.v1.get());
                TokReplace<char_t, adapter_t> * t2 = static_cast<TokReplace<char_t, adapter_t>*>(p.v2.get());
                if (t1->t != t2->t) return {FAILED, {}};
                if (t1->o == t2->o) return {FAILED, {}};
                if (t1->n == t2->o) return {FAILED, {}};
                if (t1->o == t2->n) return {FAILED, {}};
                if (t1->n == t2->n) return {FAILED, {}};
                Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> t;
                t.v1 = Core_FP<char_t, adapter_t>(f, makeTokReplace<char_t, adapter_t>(t2->t, t2->o, t2->n));
                t.v2 = Core_FP<char_t, adapter_t>(f, makeTokReplace<char_t, adapter_t>(t1->t, t1->o, t1->n));
                return {SUCCEEDED, t};
            }
            return {UNKNOWN, {}};
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Perhaps<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> commuteFileDir(const Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & p) {
            std::cout << "commuteFileDir called with arguments p = " << p << "\n";
            auto fp1 = p.v1;
            auto fp2 = p.v2;
            auto f1 = fp1.anchor_path;
            auto f2 = fp2.anchor_path;
            auto p1 = fp1.patch;
            auto p2 = fp2.patch;
            if (f1 != f2) {
                return {SUCCEEDED, {Core_FP<char_t, adapter_t>(f2, p2), Core_FP<char_t, adapter_t>(f1, p1)}};
            } else {
                return commuteFP<char_t, adapter_t>(f1, {p1, p2});
            }
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Perhaps<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> cleverCommute(const Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & p) {
            std::cout << "cleverCommute called with arguments std::function, p = " << p << "\n";
            auto p1 = p.v1;
            auto p2 = p.v2;
            auto tmp = commuteFileDir<char_t, adapter_t>({p1, p2});
            if (tmp.v1 == SUCCEEDED) {
                return tmp;
            }
            if (tmp.v1 == FAILED) {
                return {FAILED, {}};
            }
            auto tmp2 = commuteFileDir<char_t, adapter_t>({invert(p2), invert(p1)});
            if (tmp2.v1 == SUCCEEDED) {
                return {SUCCEEDED, {invert(tmp2.v2.v2), invert(tmp2.v2.v1)}};
            }
            return {tmp2.v1, {}};
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Perhaps<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> everythingElseCommute(const Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & p) {
            std::cout << "everythingElseCommute called with arguments p = " << p << "\n";
            // auto p1 = v.v1;
            // auto p2 = v.v2;

            // eec (p1 :> ChangePref p f t) = Succeeded (ChangePref p f t :> unsafeCoerceP p1)
            // eec (ChangePref p f t :> p2) = Succeeded (unsafeCoerceP p2 :> ChangePref p f t)

            // Core_FP is not an instance of ChangePref
            return cleverCommute(p);
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> commute2 (const Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> & p) {
            std::cout << "commute2 called with arguments p = " << p << "\n";
            // instance Commute Prim where
            //     commute x = toMaybe $ msum [speedyCommute x,
            //                                 everythingElseCommute x
            //                                ]
            //
            // this is equivilant to invoking Any with a predicate of == SUCCEEDED, and returning the result
            //

            auto m = speedyCommute<char_t, adapter_t>(p);
            if (m.v1 == SUCCEEDED) {
                return m.v2;
            }
            m = everythingElseCommute<char_t, adapter_t>(p);
            if (m.v1 == SUCCEEDED) {
                return m.v2;
            }
            return Nothing();
        }

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>>> commute1_ (const Tuple2<FL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> & p) {
            std::cout << "commute1_ called with arguments p = " << p << "\n";
            if (p.v1 == NilFL) {
                return {{p.v2, p.v1}};
            }
            if (p.v2 == NilFL) {
                return {{p.v2, p.v1}};
            }
            Tuple2<RL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> t;
            t.v1 = ToRL(p.v1);
            t.v2 = p.v2;
            auto tmp = commuteRLFL(t);
            if (!tmp.has_value) {
                return Nothing();
            }
            FL<Core_FP<char_t, adapter_t>> ys1 = tmp->v1;
            RL<Core_FP<char_t, adapter_t>> rsx = tmp->v2;
            FL<Core_FP<char_t, adapter_t>> rsx1 = ToFL(rsx);
            Tuple2<FL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> t2;
            t2.v1 = ys1;
            t2.v2 = rsx1;
            return {t2};
        }
        
    //     /*
    //         // this is called with the following
            
    //         [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo x1

    //         PatchInfo {
    //             _piDate = "20230508084629",
    //             _piName = "TAG refs/branches/master",
    //             _piAuthor = "smallville7123 <smallville7123@gmail.com>",
    //             _piLog = ["Ignore-this: e6e67f33f9d47f7725b0f23a22a9f112"],
    //             _piLegacyIsInverted = False
    //         }



    //         [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo y1

    //         PatchInfo {
    //             _piDate = "20230508084629",
    //             _piName = "TAG refs/branches/master",
    //             _piAuthor = "smallville7123 <smallville7123@gmail.com>",
    //             _piLog = ["Ignore-this: 6915e596e1147deec72d6421d15982c9"],
    //             _piLegacyIsInverted = False
    //         }
    //     */

        template <
            typename char_t,
            typename adapter_t,
            typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        >
        static Maybe<Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> commute1 (const Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & p) {
            std::cout << "commute1 called with arguments p = " << p << "\n";
            /*
                src/Darcs/Patch/Named.hs line 168

                d1 :: [Darcs.Patch.Info.PatchInfo]
                d2 :: [Darcs.Patch.Info.PatchInfo]
                n1 :: Darcs.Patch.Info.PatchInfo
                n2 :: Darcs.Patch.Info.PatchInfo
                p1 :: Darcs.Patch.Witnesses.Ordered.FL p wX wZ
                p2 :: Darcs.Patch.Witnesses.Ordered.FL p wZ wY
                197      commute (NamedP n1 d1 p1 :> NamedP n2 d2 p2) =
                198          if n2 `elem` d1 || n1 `elem` d2
                199          then Nothing
                200          else do (p2' :> p1') <- commute (p1 :> p2)
                201                  return (NamedP n2 d2 p2' :> NamedP n1 d1 p1')
                202  
            */

            if ((p.v1.d.size() != 0 && p.v1.d.contains(p.v2.n)) || (p.v2.d.size() != 0 && p.v2.d.contains(p.v1.n))) {
                return Nothing();
            }
            Tuple2<FL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> t;
            t.v1 = p.v1.p;
            t.v2 = p.v2.p;
            auto tmp = commute1_(t);
            if (!tmp.has_value) {
                return Nothing();
            }
            return {{{p.v2.n, p.v2.d, tmp->v1}, {p.v1.n, p.v1.d, tmp->v2}}};
        }

        // template <
        //     typename char_t,
        //     typename adapter_t,
        //     typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
        // >
        // static Maybe<Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> commute (const Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & p) {
    //         Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> x = p.v1;
    //         Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> y = p.v2;

    //         // y' :> x' <- commute (hopefully x :> hopefully y)
    //         // 
    //         /*
    //             -- | @'hopefully' hp@ tries to get a patch from a 'PatchInfoAnd'
    //             -- value. If it fails, it outputs an error \"failed to read patch:
    //             -- \<description of the patch>\". We get the description of the patch
    //             -- from the info part of 'hp'
    //             hopefully :: Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> p wA wB -> p wA wB
    //             hopefully = conscientiously $ \e -> text "failed to read patch:" $$ e
    //         */
    //         // eventually calls       NamedP n d       n is patchinfo        d is NilFL
    //         /*
    //             // [hopefully] itself returns the following

    //             x1 :: Darcs.Patch.Named.Named a wX wZ = Darcs.Patch.Named.NamedP
    //                                                     (Darcs.Patch.Info.PatchInfo
    //                                                         (Data.ByteString.Internal.PS ...)
    //                                                         (Data.ByteString.Internal.PS ...) ....)
    //                                                     [] (Darcs.Patch.Witnesses.Ordered.:>: _ _)
    //             y1 :: Darcs.Patch.Named.Named a wZ wY = Darcs.Patch.Named.NamedP
    //                                                     (Darcs.Patch.Info.PatchInfo
    //                                                         (Data.ByteString.Internal.PS ...)
    //                                                         (Data.ByteString.Internal.PS ...) ....)
    //                                                     [] (Darcs.Patch.Witnesses.Ordered.:>: _ _)
    //             264                                !x1 = hopefully x
    //             265                                !y1 = hopefully y
    //             266                                !tuple = (x1 :> y1)
    //         */
    //         /*
    //             // this is called with the following

    //             [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo x1

    //             PatchInfo {
    //                 _piDate = "20230508084629",
    //                 _piName = "TAG refs/branches/master",
    //                 _piAuthor = "smallville7123 <smallville7123@gmail.com>",
    //                 _piLog = ["Ignore-this: e6e67f33f9d47f7725b0f23a22a9f112"],
    //                 _piLegacyIsInverted = False
    //             }



    //             [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo y1

    //             PatchInfo {
    //                 _piDate = "20230508084629",
    //                 _piName = "TAG refs/branches/master",
    //                 _piAuthor = "smallville7123 <smallville7123@gmail.com>",
    //                 _piLog = ["Ignore-this: 6915e596e1147deec72d6421d15982c9"],
    //                 _piLegacyIsInverted = False
    //             }
    //         */
    //         auto tmp = commuter({x.n, y.n});
    //         if (!tmp.has_value) {
    //             return tmp;
    //         }
    //         return {tmp->v2, tmp->v1};
        // }

        // error for any other type
    };




    /*

    // (commuteFL (q :> p_and_deps))
    // constructs a tuple of {q, p_and_deps}

    // commuteFL calls the following

    -- |'commuteFL' commutes a single element past a FL.
    {-# INLINE commuteFL #-}
    commuteFL :: Commute p => (p :> FL p) wX wY -> Maybe ((FL p :> p) wX wY)
    commuteFL = commuterIdFL commute


    this constructs the Commute FL type

    geekosaur:
    commuteFL uses methods from Commute, which at that point the compiler has converted
    to a record of methods representing a specific instance

    geekosaur:
    Commute p => means that at each call site the compiler will solve the type signature
    at that point to determine p and pass the appropriate instance … Commute as a hidden parameter;
    the function (here, commuteFL) can then call methods from that instance


    it then calls commuterIdFL commute

    type CommuteFn p1 p2 = forall wX wY . (p1 :> p2) wX wY -> Maybe ((p2 :> p1) wX wY)

    //  a tuple in darcs includes additional context, so it can't be split into two separate parameters
    //  CommuteFn p1 p2 == (CommuteFn p1) (FL p2)
    commuterIdFL :: CommuteFn p1 p2 -> CommuteFn p1 (FL p2)
    commuterIdFL _ (x :> NilFL) = return (NilFL :> x)
    commuterIdFL commuter (x :> (y :>: ys))
    = do y' :> x' <- commuter (x :> y)
        ys' :> x'' <- commuterIdFL commuter (x' :> ys)
        return ((y' :>: ys') :> x'')
    */

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using CommuteFn = std::function<Maybe<Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> (Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>)>;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    using CommuteFn2 = std::function<Maybe<Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>> (Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>>)>;

    // template <
    //     typename char_t,
    //     typename adapter_t,
    //     typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::typeaz
    // >
    // using CommuteFn3 = std::function<Maybe<Tuple2<Core_FP<char_t, adapter_t>, RL<Core_FP<char_t, adapter_t>>>> (Tuple2<RL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>>)>;

    // template <
    //     typename char_t,
    //     typename adapter_t,
    //     typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    // >
    // Maybe<Tuple2<FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> CommuteIdFL(const CommuteFn<char_t, adapter_t> & commuter, const Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> & p) {
    //     if (p.v2 == NilFL) {
    //         return {p.v2, p.v1};
    //     }
    //     Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> x = p.v1;
    //     Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> y;
    //     FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ys;
    //     p.v2.extract(y, ys);

    //     auto tmp = commuter({x, y}); // at this point, we are just metadata + hash
    //     if (!tmp.has_value) {
    //         return tmp;
    //     }
    //     Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> y1 = tmp->v1;
    //     Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> x1 = tmp->v2;
    //     auto tmp1 = commuterIdFL(commuter, {x1, ys});
    //     if (!tmp1.has_value) {
    //         return tmp1;
    //     }
    //     FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ys1 = tmp1->v1;
    //     Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> x2 = tmp1->v2;
    //     return {ys1.append(y1), x2};
    // }

    // commuterIdFL :: CommuteFn p1 p2 -> CommuteFn p1 (FL p2)
    // commuterIdFL _ (x :> NilFL) = return (NilFL :> x)
    // commuterIdFL commuter (x :> (y :>: ys))
    // = do y' :> x' <- commuter (x :> y)
    //     ys' :> x'' <- commuterIdFL commuter (x' :> ys)
    //     return ((y' :>: ys') :> x'')

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> commuterIdFL(const Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> & p) {
        std::cout << "commuterIdFL called with arguments p = " << p << "\n";
        if (p.v2 == NilFL) {
            return Tuple2<FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>(p.v2, p.v1);
        }
        auto x = p.v1;
        Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> y;
        FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ys;
        p.v2.extract(y, ys);
        Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> t;
        t.v1 = x;
        t.v2 = y;
        auto tmp = Commute::commute1<char_t, adapter_t>(t);
        if (!tmp.has_value) {
            return Nothing();
        }
        auto y1 = tmp->v1;
        auto x1 = tmp->v2;
        Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> t1;
        t1.v1 = x1;
        t1.v2 = ys;
        auto tmp1 = commuterIdFL(t1);
        if (!tmp1.has_value) {
            return Nothing();
        }
        auto ys1 = tmp1->v1;
        auto x2 = tmp1->v2;
        return Tuple2<FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>(ys1.push(y1), x2);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>>> commuterIdFL(const Tuple2<Core_FP<char_t, adapter_t>, FL<Core_FP<char_t, adapter_t>>> & p) {
        std::cout << "commuterIdFL called with arguments p = " << p << "\n";
        if (p.v2 == NilFL) {
            return Tuple2<FL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>>(p.v2, p.v1);
        }
        auto x = p.v1;
        Core_FP<char_t, adapter_t> y;
        FL<Core_FP<char_t, adapter_t>> ys;
        p.v2.extract(y, ys);
        Tuple2<Core_FP<char_t, adapter_t>, Core_FP<char_t, adapter_t>> t;
        t.v1 = x;
        t.v2 = y;
        auto tmp = Commute::commute2<char_t, adapter_t>(t);
        if (!tmp.has_value) {
            return Nothing();
        }
        auto y1 = tmp->v1;
        auto x1 = tmp->v2;
        Tuple2<Core_FP<char_t, adapter_t>, FL<Core_FP<char_t, adapter_t>>> t1;
        t1.v1 = x1;
        t1.v2 = ys;
        auto tmp1 = commuterIdFL(t1);
        if (!tmp1.has_value) {
            return Nothing();
        }
        auto ys1 = tmp1->v1;
        auto x2 = tmp1->v2;
        return Tuple2<FL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>>(ys1.push(y1), x2);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>, Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> commuteFL(const Tuple2<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>>> & p) {
        std::cout << "commuterFL called with arguments p = " << p << "\n";
        return commuterIdFL<char_t, adapter_t>(p);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<Core_FP<char_t, adapter_t>, RL<Core_FP<char_t, adapter_t>>>> commuterRLId(const Tuple2<RL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>> & p) {
        std::cout << "commuterRLId called with arguments p = " << p << "\n";
        if (p.v1 == NilRL) {
            return {{p.v2, p.v1}};
        }
        Core_FP<char_t, adapter_t> y = p.v2;
        Core_FP<char_t, adapter_t> x;
        RL<Core_FP<char_t, adapter_t>> xs;
        p.v1.extract(x, xs);

        auto tmp = Commute::commute2<char_t, adapter_t>({x, y});
        if (!tmp.has_value) {
            return Nothing();
        }
        Core_FP<char_t, adapter_t> y1 = tmp->v1;
        Core_FP<char_t, adapter_t> x1 = tmp->v2;
        auto tmp1 = commuterRLId<char_t, adapter_t>({xs, y1});
        if (!tmp1.has_value) {
            return tmp1;
        }
        Core_FP<char_t, adapter_t> y2 = tmp1->v1;
        RL<Core_FP<char_t, adapter_t>> xs1 = tmp1->v2;
        return {{y2, xs1.push(x1)}};
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, RL<Core_FP<char_t, adapter_t>>>> right_or_left(const RL<Core_FP<char_t, adapter_t>> & p1, const FL<Core_FP<char_t, adapter_t>> & p2, bool is_left) {
        if (is_left) {
            std::cout << "left called with arguments p1 = " << p1 << ", p2 = " << p2 << "\n";
            auto & bs = p2;
            if (p1 == NilRL) {
                return {{bs, p1}};
            }
            RL<Core_FP<char_t, adapter_t>> as;
            Core_FP<char_t, adapter_t> a;
            p1.extract(a, as);
            Tuple2<Core_FP<char_t, adapter_t>, FL<Core_FP<char_t, adapter_t>>> t;
            t.v1 = a;
            t.v2 = bs;
            auto tmp = commuterIdFL(t);
            if (!tmp.has_value) {
                return Nothing();
            }
            auto bs1 = tmp->v1;
            auto a1 = tmp->v2;
            auto tmp1 = right_or_left(as, bs1, false);
            if (!tmp1.has_value) {
                return Nothing();
            }
            auto bs2 = tmp1->v1;
            auto as1 = tmp1->v2;
            Tuple2<FL<Core_FP<char_t, adapter_t>>, RL<Core_FP<char_t, adapter_t>>> t1;
            t1.v1 = bs2;
            t1.v2 = as1.push(a1);
            return {t1};
        } else {
            std::cout << "right called with arguments p1 = " << p1 << ", p2 = " << p2 << "\n";
            RL<Core_FP<char_t, adapter_t>> as = p1;
            if (p2 == NilFL) {
                return {{p2, as}};
            }
            FL<Core_FP<char_t, adapter_t>> bs;
            Core_FP<char_t, adapter_t> b;
            p2.extract(b, bs);
            Tuple2<RL<Core_FP<char_t, adapter_t>>, Core_FP<char_t, adapter_t>> t;
            t.v1 = as;
            t.v2 = b;
            auto tmp = commuterRLId(t);
            if (!tmp.has_value) {
                return Nothing();
            }
            auto b1 = tmp->v1;
            auto as1 = tmp->v2;
            auto tmp1 = right_or_left(as1, bs, true);
            if (!tmp1.has_value) {
                return tmp1;
            }
            auto bs1 = tmp1->v1;
            auto as2 = tmp1->v2;
            return {{bs1.push(b1), as2}};
        }
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, RL<Core_FP<char_t, adapter_t>>>> commuterRLFL(const Tuple2<RL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> & p) {
        std::cout << "commuterRLFL called with arguments p = " << p << "\n";
        return right_or_left(p.v1, p.v2, false);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Maybe<Tuple2<FL<Core_FP<char_t, adapter_t>>, RL<Core_FP<char_t, adapter_t>>>> commuteRLFL(const Tuple2<RL<Core_FP<char_t, adapter_t>>, FL<Core_FP<char_t, adapter_t>>> & p) {
        std::cout << "commuteRLFL called with arguments p = " << p << "\n";
        return commuterRLFL(p);
    }
}

#endif