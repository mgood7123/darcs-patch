#ifndef DARCS_PATCH_H
#define DARCS_PATCH_H

#include <string_adapter.h>
#include "darcs_types.h"
// #include "darcs_commute.h"

namespace DarcsPatch {
    template <typename char_t, typename adapter_t, typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type>
    struct Patcher {
        std::deque<Patch> hunks;
        
        void add(size_t line, const adapter_t & old_line, const adapter_t & new_line) {
            
            FileHunk p = {hunks.size(), line, old_line, new_line};
            
            hunks.push_back(p);
            
            printf("patch add hunk: %zu, ", p.line);
            
            auto h = p.old_line.lines();
            printf("[");
            if (h.size() != 0) {
                bool f = true;
                for (auto & s : h) {
                    if (!f) {
                        printf(", ");
                    }
                    printf("\"%s\"", s.c_str());
                    f = false;
                }
            }
            printf("], ");
            h = p.new_line.lines();
            printf("[");
            if (h.size() != 0) {
                bool f = true;
                for (auto & s : h) {
                    if (!f) {
                        printf(", ");
                    }
                    printf("\"%s\"", s.c_str());
                    f = false;
                }
            }
            printf("]");
            
            printf("\n\n\n\n");
        }
        
        /*
        ```
        allDeps j = uncurry S.union . fromJust . M.lookup j
        addDeps j = S.insert j . S.union (allDeps j m)
        addDeps j indirect

        // foldDeps qs (q :>: p_and_deps) non_deps (S.insert j direct, addDeps j indirect)
        ```

        my understanding of the above

        ```
        // assume j is an index/key

        // we look up j, union its values together and union the result with the indirect set and add j to the resulting set


        sets = M.lookup j m
        // assume fromJust simply returns what its given

        // tuple of set
        set1 = sets first
        set2 = sets second

        u1 = union set1 set2
        // u1 must be a set
        // indirect must be a set

        u = union u1 indirect
        // u must be a set

        insert j u
        ```

        `j` is inserted to sets `direct` and computed `u`
        */

        template <typename T>
        Maybe<Set<PatchId<T, char_t, adapter_t>>> allDeps(const PatchId<T, char_t, adapter_t> & j, const Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> & m) {
            auto sets = m.lookup(j);
            return !sets.has_value ? sets : std::set_union(sets->v1.set, sets->v2.set);
        }

        template <typename T>
        Maybe<Set<PatchId<T, char_t, adapter_t>>> addDeps(const PatchId<T, char_t, adapter_t> & j, const Set<PatchId<T, char_t, adapter_t>> & indirect, const Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> & m) {
            auto v = allDeps(j, m);
            return !v.has_value ? v : Set<PatchId<T, char_t, adapter_t>>(std::set_union(v.value_ref().set, indirect.set)).append(j);
        }

        // depsGraph
        // we take a list of types extending Patch
        // and return map<patch_id, deps>
        //
        // a patch_id can be simply an index into the patch array
        //
        // deps is a structure representing the dependencies of patch patch_id
        //
        // note:
        //   depsGraph :: forall p wX wY. (Commute p, Ident p) => RL p wX wY -> DepsGraph p 
        //
        // "forall p wX wY. (Commute p, Ident p) " are used for type checking to make sure we pass the correct types that extend the correct interfaces (each value in p implements Commute and implements Ident)
        //
        
        // FL and RL provide fast access to start and end respectively
        //
        // list order is not changed
        //

        /*
            foldDeps ::
                RL (PatchInfoAndG a) wA1 wB
                -> FL (PatchInfoAndG a) wB wC1
                -> FL (PatchInfoAndG a) wC1 wD1
        */
        template <typename T>
        Deps<char_t, adapter_t> foldDeps(const RL<PatchInfoAndG<char_t, adapter_t>> & arg, const FL<PatchInfoAndG<char_t, adapter_t>> & p_and_deps, const FL<PatchInfoAndG<char_t, adapter_t>> & non_deps, Deps<char_t, adapter_t> & acc, const Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> & m) {
            PatchInfoAndG<char_t, adapter_t> q;
            RL<PatchInfoAndG<char_t, adapter_t>> qs;
            arg.extract(q, qs);
            if (qs.isNil()) {
                return acc;
            }
            PatchId<T, char_t, adapter_t> j = q.n.p->ident();
            
            Set<PatchId<T, char_t, adapter_t>> & direct = acc.v1;
            Set<PatchId<T, char_t, adapter_t>> & indirect = acc.v2;
            
            /*
            -- If we already know we indirectly depend on q, then there is
            -- nothing left to do. Note that (j `S.member` direct) is impossible.
            | j `S.member` indirect = foldDeps qs (q :>: p_and_deps) non_deps acc
            //
            or


            ```
            -- If we already know we indirectly depend on q, then there is
            -- nothing left to do. Note that (j `S.member` direct) is impossible.
            | j `S.member` indirect = foldDeps qs (q :>: p_and_deps) non_deps acc
            ```
            in psuedo code
            ```
            if `indirect` (which is a set) contains the value `j` then return `foldDeps qs (q :>: p_and_deps) non_deps acc`
            ```
            */
            if (indirect.contains(j)) {
                return foldDeps(qs, {q, p_and_deps.append(q)}, non_deps, acc, m);
            }


            /*
            -- If q commutes past p_and_deps then we don't depend on it
            | Just (p_and_deps' :> q') <- commuteFL (q :> p_and_deps) =
                foldDeps qs p_and_deps' (q' :>: non_deps) acc
            */
            /*
            `q :> p_and_deps` makes a tuple of q and p_and_deps
            */

            std::cout << "calling commuteFL with arguments q = " << q << ", p_and_deps = " << p_and_deps << "\n";
            auto tmp = commuteFL({q, p_and_deps});
            if (tmp.has_value) {
                return foldDeps(qs, tmp->v1, {non_deps.append(tmp->v2)}, acc, m);
            }

            /*
            -- We have a new dependency which must be a direct one, so add it to
            -- 'direct' and all its dependencies to 'indirect'. The invariant that
            -- direct and indirect are disjoint is maintained because neither the
            -- direct nor indirect deps of a patch contain its own 'PatchId<T, char_t, adapter_t>'.
            | otherwise =
                foldDeps qs (q :>: p_and_deps) non_deps (S.insert j direct, addDeps j indirect)
            */
            return foldDeps(qs, p_and_deps.append(q), non_deps, {direct.append(j), addDeps(j, indirect, m).value_copy()}, m);
        }
        
        template <typename T>
        Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> depsGraph(const Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> & m, const RL<PatchInfoAndG<char_t, adapter_t>> & arg) {
            PatchInfoAndG<char_t, adapter_t> p;
            RL<PatchInfoAndG<char_t, adapter_t>> ps;
            arg.extract(p, ps);
            if (ps.isNil()) {
                return m;
            }
            m = depsGraph(m, ps);

            /*
                we get called with a p of the folllowing
                "PIAP (
                    PatchInfo {
                        _piDate = \"20230508093807\",
                        _piName = \"d\",
                        _piAuthor = \"smallville7123 <smallville7123@gmail.com>\",
                        _piLog = [\"Ignore-this: bdcb1045459063facc23938f4872c9b3\"],
                        _piLegacyIsInverted = False
                        }
                )
                (
                    Hashed (
                        PatchHash (
                            WithSize 126 (
                                SHA256 \"\\191\\144V\\198@|\\158M\\194\\207\\192+\\f\\167aUwq\\b\\209\\DLE-\\244\\137\\202a\\226-ej\\135\\196\"
                            )
                        )
                    )
                    (
                        Actually (
                            NamedP (
                                PatchInfo {
                                    _piDate = \"20230508093807\",
                                    _piName = \"d\",
                                    _piAuthor = \"smallville7123 <smallville7123@gmail.com>\",
                                    _piLog = [\"Ignore-this: bdcb1045459063facc23938f4872c9b3\"],
                                    _piLegacyIsInverted = False
                                }
                            )
                            [] (
                                Normal (
                                    FP (
                                        AnchoredPath [
                                            Name {
                                                unName = \"p\"
                                            }
                                        ]
                                    )
                                    (
                                        Hunk 6 [] [\"duly\"]
                                    )
                                ) :>: NilFL
                            )
                        )
                    )
                )"
            */

            // allDeps j = uncurry S.union . fromJust . M.lookup j
            //
            // equivalent to lambda name allDeps with auto j
            //
            // auto allDeps = [](auto j) { ... }
            //
            // code here
            
            auto j2 = p.n.p->ident();
            auto folded = foldDeps(ps, {p}, {}, {}, m);
            
            m.insert(j2, folded);
            return m;
        }
        
        template <typename T>
        Map<PatchId<T, char_t, adapter_t>, Deps<char_t, adapter_t>> depsGraph(const std::deque<Patch> & ps) {
            // patches are already allocated in deque
            RL<PatchInfoAndG<char_t, adapter_t>> p;
            for(const Patch & patch : ps) p = p.append({&patch});
            return depsGraph({}, p);
        }
        
        // void calculateDeps() {
            
        //     hunksLC = {};
            
        //     for (Hunk & h1 : hunks) {
        //         printf("brute forcing hunk p%zu\n", h1.idx+1);
        //         if (h1.map == -1) {
        //             h1.map = hunksLC.size();
        //             hunksLC.push_back({{h1.line, h1.old_line.line_count(), h1.new_line.line_count()}, {}, h1.idx});
        //         }
        //         Dep & hlc1 = hunksLC[h1.map];
        //         for (Hunk & h2 : hunks) {
        //             if (h1.idx >= h2.idx) {
        //                 continue;
        //             }
        //             printf("  ");
        //             for (size_t i = 0, m = hunks.size(); i < m; i++) {
        //                 printf("p%zu%s%s", hunks[i].idx+1, hunks[i].i ? "'" : "", i != m ? ";" : "");
        //             }
        //             printf("\n");
        //             printf("    testing hunk p%zu\n", h2.idx+1);
        //             if (h2.map == -1) {
        //                 h2.map = hunksLC.size();
        //                 hunksLC.push_back({{h2.line, h2.old_line.line_count(), h2.new_line.line_count()}, {}, h2.idx});
        //             }
        //             Dep & hlc2 = hunksLC[h2.map];
        //             auto r = commuteHunkLines(hlc1.hunk, hlc2.hunk);
        //             printf("      commuteHunkLines(p%zu, p%zu)\n", h1.idx+1, h2.idx+1);
        //             if (!r) {
        //                 printf("        p%zu and p%zu are related\n", h1.idx+1, h2.idx+1);
        //                 hlc1.deps.push_back(h2.map);
        //             } else {
        //                 printf("        p%zu and p%zu are not related, swapping\n", h1.idx+1, h2.idx+1);
        //                 hlc1.hunk.line = r->old_line;
        //                 hlc2.hunk.line = r->new_line;
        //                 h1.i = true;
        //                 h2.i = true;
        //                 std::swap(h1, h2);
        //             }
        //         }
        //     }
            
        //     for (size_t i = 0, m = hunksLC.size(); i < m; i++) {
        //         for (size_t ii = 0, mm = hunksLC[i].deps.size(); ii < mm; ii++) {
        //             printf("  deps p%zu -> p%zu\n", hunksLC[hunksLC[i].deps[ii]].idx+1, hunksLC[i].idx+1);
        //         }
        //     }
        //     printf("\n");
        // }
        
        // TODO
        // void reorder(Hunk p, Hunk plc) {
        //     if (hunksLC.size() > 1) {
        //         for(size_t i = hunksLC.size()-2; i != -1; i--) {
        //             HunkLC o = hunksLC[i];
        //             HunkLC & n = plc;
        //             printf("calling commuteHunkLines( patch #%zu ( %zu, %zu, %zu ) , patch #%zu ( %zu, %zu, %zu ) )\n", i, o.line, o.old_line, o.new_line, hunksLC.size()-1, n.line, n.old_line, n.new_line);
        //             auto r = commuteHunkLines(o, n);
        //             printf("  patch #%zu ( %zu, %zu, %zu ) is related to patch #%zu ( %zu, %zu, %zu ) : %s\n", hunksLC.size()-1, n.line, n.old_line, n.new_line, i, o.line, o.old_line, o.new_line, r.has_value() ? "true" : "false");
        //             if (r.has_value()) {
        //                 printf("    offset: old line %zu, new line %zu\n", r->old_line, r->new_line);
        //             } else {
        //                 printf("    no offset\n");
        //             }
        //         }
        //     }
        // }
        
        // std::optional<HunkOffset> commuteHunkLines(const HunkLC & l1, const HunkLC & l2) {
        //     if (l1.line + l1.new_line < l2.line) {
        //         return {{ l2.line - l1.new_line + l1.old_line, l1.line }};
        //     }
        //     else if (l2.line + l2.old_line < l1.line) {
        //         return {{ l2.line, l1.line + l2.new_line - l2.old_line }};
        //     }
        //     else if (l2.old_line != 0 && l1.old_line != 0 && l2.new_line != 0 && l1.new_line != 0 && l1.line + l1.new_line == l2.line) {
        //         return {{ l2.line - l1.new_line + l1.old_line, l1.line }};
        //     }
        //     else if (l2.old_line != 0 && l1.old_line != 0 && l2.new_line != 0 && l1.new_line != 0 && l2.line + l2.old_line == l1.line) {
        //         return {{ l2.line, l1.line + l2.new_line - l2.old_line }};
        //     }
        //     else return std::nullopt;
        // }
    };
    
    using Patcher_T = Patcher<char, StringAdapter::CharAdapter>;
}

#endif