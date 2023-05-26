#include <darcs_types.h>

using namespace DarcsPatch;

template <
    typename char_t,
    typename adapter_t,
    typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
>
Deps<char_t, adapter_t> foldDeps(const RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> & p, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> q_and_deps, FL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> non_deps, Deps<char_t, adapter_t> acc) {
    puts("FOLD_DEPS ENTER");
    if (p == NilRL) {
        puts("FOLD_DEPS NILRL");
        puts("FOLD_DEPS EXIT");
        return acc;
    }

    puts("FOLD_DEPS NON NILRL");
    Set<PatchInfo<char_t, adapter_t>> direct = acc.v1;
    Set<PatchInfo<char_t, adapter_t>> indirect = acc.v2;
    

    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> q;
    RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> qs;
    p.extract(q, qs);

    puts("FOLD_DEPS IDENT");
    PatchInfo<char_t, adapter_t> j = ident(q);

    if (indirect.contains(j)) {
        //return foldDeps();
    }

    puts("FOLD_DEPS EXIT");
    return acc;
};

template <
    typename char_t,
    typename adapter_t,
    typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
>
DepsGraph<char_t, adapter_t> depsGraph(const RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ps_) {
    puts("DEPS_GRAPH ENTER");

    if (ps_ == NilRL) {
        puts("DEPS_GRAPH NILRL");
        puts("DEPS_GRAPH EXIT");
        return {};
    }

    puts("DEPS_GRAPH NON NILRL");

    Named<Core_FP<char_t, adapter_t>, char_t, adapter_t> p;
    RL<Named<Core_FP<char_t, adapter_t>, char_t, adapter_t>> ps;
    ps_.extract(p, ps);

    puts("DEPS_GRAPH DEPS GRAPH");
    DepsGraph<char_t, adapter_t> m = depsGraph(ps);
    puts("DEPS_GRAPH IDENT");
    PatchInfo<char_t, adapter_t> j2 = ident(p);

    puts("DEPS_GRAPH FOLD DEPS");
    Deps<char_t, adapter_t> folded = foldDeps<char_t, adapter_t>(ps, NilFL.append(p), NilFL, {{}, {}});

    puts("DEPS_GRAPH INSERT");
    auto mr = m.insert(j2, folded);

    puts("DEPS_GRAPH EXIT");
    return mr;
}

DepsGraph_T depsGraph_T(const RL<Named_T<Core_FP_T>> ps) {
    return depsGraph<char, StringAdapter::CharAdapter>(ps);
}

int main() {
    auto p1 = makeNamedWithType_T("p1", new AddFile());
    auto p2 = makeNamedHunk_T("p2", 1, "", "all\nthe\nlines");
    auto p3 = makeNamedHunk_T("p3", 1, "", "foo");
    auto p4 = makeNamedHunk_T("p4", 4, "", "jar");
    auto rl = ToRL(std::vector {p1, p2, p3, p4});
    DepsGraph_T graph = depsGraph_T(rl);
    return 0;
}