#ifndef DARCS_PATCH_H
#define DARCS_PATCH_H

#include "darcs_types.h"
#include "darcs_commute.h"

namespace DarcsPatch {
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

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Set<PatchInfo<char_t, adapter_t>> allDeps(const PatchInfo<char_t, adapter_t> & j, const LazyValue<DepsGraph<char_t, adapter_t>> & m) {
        std::cout << "called allDeps with arguments j = " << j << ", m = " << m() << "\n";
        Maybe<Deps<char_t, adapter_t>> sets = m().lookup(j);
        std::cout << "m.lookup(j) = " << sets << "\n";
        return sets->v1.Union(sets->v2);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    Set<PatchInfo<char_t, adapter_t>> addDeps(const PatchInfo<char_t, adapter_t> & j, const Set<PatchInfo<char_t, adapter_t>> & indirect, const LazyValue<DepsGraph<char_t, adapter_t>> & m) {
        std::cout << "called addDeps with arguments j = " << j << ", indirect = " << indirect << "\n";
        return allDeps(j, m).Union(indirect).insert(j);
    }

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    const LazyValue<Deps<char_t, adapter_t>> foldDeps(
        const RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & p,
        const FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & p_and_deps,
        const FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & non_deps,
        const LazyValue<Deps<char_t, adapter_t>> & acc,
        const LazyValue<DepsGraph<char_t, adapter_t>> & m
    ) {
        std::cout << "foldDeps called with arguments p = " << p << ", p_and_deps = " << p_and_deps << ", non_deps = " << non_deps << "\n";
        puts("FOLD_DEPS ENTER");
        if (p == NilRL) {
            puts("FOLD_DEPS NILRL");
            puts("FOLD_DEPS EXIT");
            return acc;
        }

        puts("FOLD_DEPS NON NILRL");

        Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> q;
        RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> qs;
        p.extract(q, qs);

        puts("FOLD_DEPS IDENT");
        PatchInfo<char_t, adapter_t> j = ident(q);
        std::cout << "foldDeps called with arguments q = " << q << ", qs = " << qs << ", j = " << j << "\n";

        std::cout << "\n\nq = " << q << "\n\np_and_deps = " << p_and_deps << "\n\n\n";

        if (acc().v2.contains(j)) {
            puts("FOLD_DEPS INDIRECT CONTAINS J");
            return foldDeps<char_t, adapter_t>(qs, p_and_deps.push(q), non_deps, acc, m);
        }

        std::cout << "calling commuteFL with arguments q = " << q << ", p_and_deps = " << p_and_deps << "\n";
        auto tmp = commuteFL<char_t, adapter_t>({q, p_and_deps});
        if (tmp.has_value) {
            auto r = foldDeps<char_t, adapter_t>(qs, tmp->v1, non_deps.push(tmp->v2), acc, m);
            puts("FOLD_DEPS EXIT");
            return r;
        } else {
            auto r = foldDeps<char_t, adapter_t>(qs, p_and_deps.push(q), non_deps, LazyValue<Deps<char_t, adapter_t>>([=]() {
                auto acc_ = acc();
                return Deps<char_t, adapter_t>(acc_.v1.insert(j), addDeps<char_t, adapter_t>(j, acc_.v2, m));
            }), m);
            puts("FOLD_DEPS EXIT");
            return r;
        }

        puts("FOLD_DEPS EXIT");
        return acc;
    };

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    LazyValue<DepsGraph<char_t, adapter_t>> depsGraph(const RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ps_) {
        std::cout << "depsGraph called with arguments ps_ = " << ps_ << "\n";
        puts("DEPS_GRAPH ENTER");

        if (ps_ == NilRL) {
            puts("DEPS_GRAPH NILRL");
            puts("DEPS_GRAPH EXIT");
            return LazyValue<DepsGraph<char_t, adapter_t>>([]() { return DepsGraph<char_t, adapter_t>(); });
        }

        puts("DEPS_GRAPH NON NILRL");

        Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> p;
        RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ps;
        ps_.extract(p, ps);

        puts("DEPS_GRAPH DEPS GRAPH");
        LazyValue<DepsGraph<char_t, adapter_t>> m = depsGraph<char_t, adapter_t>(ps);
        puts("DEPS_GRAPH IDENT");
        PatchInfo<char_t, adapter_t> j2 = ident(p);

        puts("DEPS_GRAPH FOLD DEPS");
        std::cout << "calling FoldDeps with arguments p = " << p << "\n";
        std::cout << "calling FoldDeps with arguments ps = " << ps << "\n";
        auto folded = foldDeps<char_t, adapter_t>(ps, NilFL.push(p), NilFL, LazyValue<Deps<char_t, adapter_t>>([](){ return Deps<char_t, adapter_t>(); }), m);

        puts("DEPS_GRAPH INSERT");

        return LazyValue<DepsGraph<char_t, adapter_t>>([=]() {
            std::cout << "map m = " << m() << "\n";
            std::cout << "inserting key j1 = " << j2 << "\n";
            std::cout << "inserting value folded = " << folded() << "\n";
            auto m1 = m().insert(j2, folded());
            std::cout << "returning from depsGraph with m1 = " << m1 << "\n";
            puts("DEPS_GRAPH EXIT");
            return m1;
        });
    }

    DepsGraph_T depsGraph_T(const RL<Named_T<Core_FP_T>> ps) {
        return depsGraph<char, StringAdapter::CharAdapter>(ps)();
    }
}

#endif