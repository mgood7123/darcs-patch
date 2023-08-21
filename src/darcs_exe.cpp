#include <darcs_patch.h>

void darcs_t() {
    auto p1 = DarcsPatch::makeNamedWithType_T("p1", DarcsPatch::makeAddFile());
    auto p2 = DarcsPatch::makeNamedHunk_T("p2", 1, "", "all\nthe\nlines");
    auto p3 = DarcsPatch::makeNamedHunk_T("p3", 1, "", "foo");
    auto p4 = DarcsPatch::makeNamedHunk_T("p4", 4, "", "jar");
    auto tmp1 = DarcsPatch::makePatchInfo_T("p1");
    auto tmp2 = DarcsPatch::makePatchInfo_T("p2");
    #define compare_print(a, op, b) std::cout << " " << #a << " " << #op << " " << #b << " = " << ((a op b) ? "true" : "false") << std::endl
    #define compare_print_all(a, b) compare_print(a, <, b); compare_print(a, ==, b); compare_print(a, >, b)
    #define compare_print_all_member(a, b, member) compare_print_all(a.member, b.member)
    #define compare_print_all2(a, b, c) std::cout << " " << #a << " " << "compare_3" << " " << #b << " = " << StringAdapter::compare_3(a, b, &c::date, &c::name, &c::author, &c::log, &c::legacyIsInverted) << std::endl
    std::cout << " tmp1 = " << tmp1 << std::endl;
    std::cout << " tmp2 = " << tmp2 << std::endl;
    compare_print_all_member(tmp2, tmp1, date);
    compare_print_all_member(tmp2, tmp1, name);
    compare_print_all_member(tmp2, tmp1, author);
    compare_print_all_member(tmp2, tmp1, log);
    compare_print_all_member(tmp2, tmp1, legacyIsInverted);
    compare_print_all(tmp2, tmp1);
    compare_print_all2(tmp2, tmp1, DarcsPatch::PatchInfo_T);
    std::cout << "calling depsGraph with p1 = " << p1 << ", p2 = " << p2 << ", p3 = " << p3 << ", p4 = " << p4 << "\n";
    auto graph = DarcsPatch::depsGraph_T({p1, p2, p3, p4});
    DarcsPatch::renderDepsGraphAsDot(graph);
}

void darcs_t2() {
    auto graph = DarcsPatch::depsGraph_T({

        DarcsPatch::makeNamedWithType_T ("p1", DarcsPatch::makeAddFile()),

        DarcsPatch::makeNamedHunk_T("0", 1, "", "hello"),
        DarcsPatch::makeNamedHunk_T("1", 2, "", "world"),
        DarcsPatch::makeNamedHunk_T("2", 1, "", "foo"),
        DarcsPatch::makeNamedHunk_T("3", 3, "", "AFCD")

    });

    DarcsPatch::renderDepsGraphAsDot(graph, false);
}

void darcs_t3() {
    auto graph = DarcsPatch::depsGraph_T({

        DarcsPatch::makeNamedWithType_T ("p1", DarcsPatch::makeAddFile()),

        DarcsPatch::makeNamedHunk_T("0", 3, "", "\n\n\n\n\n"),
        DarcsPatch::makeNamedHunk_T("1", 5, "", "\n\n\n"),
        DarcsPatch::makeNamedHunk_T("2", 4, "", "\n\n\n\n\n"),
        DarcsPatch::makeNamedHunk_T("3", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("4", 3, "", ""),
        DarcsPatch::makeNamedHunk_T("5", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("6", 3, "", ""),
        DarcsPatch::makeNamedHunk_T("7", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("8", 6, "", ""),
        DarcsPatch::makeNamedHunk_T("9", 7, "", ""),
        DarcsPatch::makeNamedHunk_T("10", 8, "", ""),
        DarcsPatch::makeNamedHunk_T("11", 9, "", ""),
        DarcsPatch::makeNamedHunk_T("12", 10, "", ""),
        DarcsPatch::makeNamedHunk_T("13", 11, "", ""),

    });

    DarcsPatch::renderDepsGraphAsDot(graph, false);
}

void darcs_t4() {
    auto graph = DarcsPatch::depsGraph_T({

        DarcsPatch::makeNamedWithType_T ("p1", DarcsPatch::makeAddFile()),

        DarcsPatch::makeNamedHunk_T("edit 1", 1, "", "hello"),
        DarcsPatch::makeNamedHunk_T("edit 2", 1, "hello", "hello world"),
        DarcsPatch::makeNamedHunk_T("edit 3", 1, "hello world", "fire world"),
        DarcsPatch::makeNamedHunk_T("edit 4", 1, "fire world", "lamp world"),

    });

    DarcsPatch::renderDepsGraphAsDot(graph, false);
}

void darcs_t5() {
    auto graph = DarcsPatch::depsGraph_T({

        DarcsPatch::makeNamedWithType_T ("p1", DarcsPatch::makeAddFile()),

        DarcsPatch::makeNamedHunk_T("0", 1, "", "\n\n\n\n\n"),
        DarcsPatch::makeNamedHunk_T("1", 1, "\n\n\n", "\n\n"),
        DarcsPatch::makeNamedHunk_T("2", 4, "", "\n\n\n\n\n"),
        DarcsPatch::makeNamedHunk_T("3", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("4", 3, "", ""),
        DarcsPatch::makeNamedHunk_T("5", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("6", 3, "", ""),
        DarcsPatch::makeNamedHunk_T("7", 4, "", ""),
        DarcsPatch::makeNamedHunk_T("8", 6, "", ""),
        DarcsPatch::makeNamedHunk_T("9", 7, "", ""),
        DarcsPatch::makeNamedHunk_T("10", 8, "", ""),
        DarcsPatch::makeNamedHunk_T("11", 9, "", ""),
        DarcsPatch::makeNamedHunk_T("12", 10, "", ""),
        DarcsPatch::makeNamedHunk_T("13", 11, "", ""),

    });

    DarcsPatch::renderDepsGraphAsDot(graph, false);
}

int main() {
    darcs_t4();
    return 0;
}