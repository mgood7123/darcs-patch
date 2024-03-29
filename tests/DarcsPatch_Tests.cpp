#include <gtest/gtest.h>

#include <darcs_patch.h>

#define ERASE_TEST(T, it_begin, it_end, index) \
{ \
    { \
        DarcsPatch::T<int> p = {1, 2, 3, 4}; \
        auto erase_tmp = p.insert(p.it_begin + index, 88); \
        std::ostringstream erase_tmp_o; \
        erase_tmp_o << "p.insert(" << #it_begin << " + " << index << ", 88)"; \
        auto erase_tmp_s = erase_tmp_o.str(); \
        erase_tmp.v1.print(erase_tmp_s); \
        std::cout << erase_tmp_s << " returned iterator: "; \
        if (erase_tmp.v2 < p.it_end) { \
            std::cout << erase_tmp.v2 << "\n"; \
        } else { \
            std::cout << "end()\n"; \
        } \
    } \
\
    { \
        DarcsPatch::T<int> p = {1, 2, 3, 4}; \
        auto erase_tmp = p.erase(p.it_begin + index); \
        std::ostringstream erase_tmp_o; \
        erase_tmp_o << "p.erase(" << #it_begin << " + " << index << ")"; \
        auto erase_tmp_s = erase_tmp_o.str(); \
        erase_tmp.v1.print(erase_tmp_s); \
        std::cout << erase_tmp_s << " returned iterator: "; \
        if (erase_tmp.v2 < p.it_end) { \
            std::cout << erase_tmp.v2 << "\n"; \
        } else { \
            std::cout << "end()\n"; \
        } \
    } \
}

TEST(DarcsPatch_, FL_) {
    DarcsPatch::FL<int> int_fl;
    auto fl_a = int_fl.push(1);
    auto fl_b = fl_a.push(2);
    auto fl_c = fl_b.push(3);
    int_fl.print("int_fl");
    fl_a.print("fl_a");
    fl_b.print("fl_b");
    fl_c.print("fl_c");
    auto fl_d = fl_a.push(8);
    int_fl.print("int_fl");
    fl_a.print("fl_a");
    fl_b.print("fl_b");
    fl_c.print("fl_c");
    fl_d.print("fl_d");
    int a;
    DarcsPatch::FL<int> b;
    fl_c.extract(a, b);
    fl_c.print("fl_c");
    DarcsPatch::FL<int> a_;
    DarcsPatch::FL<int> a_t;
    a_t = a_t.push(3);
    a_ = a_.push(a);
    a_.print("int a");
    b.print("int b");
    EXPECT_EQ(int_fl, int_fl);
    EXPECT_EQ(fl_a, fl_a);
    EXPECT_EQ(fl_b, fl_b);
    EXPECT_EQ(fl_c, fl_c);
    EXPECT_EQ(a_, a_t);
    EXPECT_EQ(a, 3);
    DarcsPatch::FL<int> ps2 = {1, 2, 3, 4};
    int p;
    DarcsPatch::FL<int> ps;
    ps2.extract(p, ps);
    ps2.print("ps2");
    ps.print("ps");
    std::cout << "p = " << p << "\n";
    EXPECT_EQ(p, 4);
    ps2 = {1, 2, 3, 4};
    ERASE_TEST(FL, begin(), end(), 0);
    ERASE_TEST(FL, begin(), end(), 1);
    ERASE_TEST(FL, begin(), end(), 2);
    ERASE_TEST(FL, begin(), end(), 3);
    ERASE_TEST(FL, begin(), end(), 4);
    ERASE_TEST(FL, begin(), end(), 5);
}

TEST(DarcsPatch_, RL_) {
    DarcsPatch::RL<int> int_rl;
    auto rl_a = int_rl.push(1);
    auto rl_b = rl_a.push(2);
    auto rl_c = rl_b.push(3);
    int_rl.print("int_rl");
    rl_a.print("rl_a");
    rl_b.print("rl_b");
    rl_c.print("rl_c");
    auto rl_d = rl_a.push(8);
    int_rl.print("int_rl");
    rl_a.print("rl_a");
    rl_b.print("rl_b");
    rl_c.print("rl_c");
    rl_d.print("rl_d");
    int a;
    DarcsPatch::RL<int> b;
    rl_c.extract(a, b);
    rl_c.print("rl_c");
    DarcsPatch::RL<int> a_;
    DarcsPatch::RL<int> a_t;
    a_t = a_t.push(3);
    a_ = a_.push(a);
    a_.print("int a");
    b.print("int b");
    EXPECT_EQ(int_rl, int_rl);
    EXPECT_EQ(rl_a, rl_a);
    EXPECT_EQ(rl_b, rl_b);
    EXPECT_EQ(rl_c, rl_c);
    EXPECT_EQ(a_, a_t);
    EXPECT_EQ(a, 3);
    DarcsPatch::RL<int> ps2 = {1, 2, 3, 4};
    int p;
    DarcsPatch::RL<int> ps;
    ps2.extract(p, ps);
    ps2.print("ps2");
    ps.print("ps");
    std::cout << "p = " << p << "\n";
    EXPECT_EQ(p, 4);
    ERASE_TEST(RL, begin(), end(), 0);
    ERASE_TEST(RL, begin(), end(), 1);
    ERASE_TEST(RL, begin(), end(), 2);
    ERASE_TEST(RL, begin(), end(), 3);
    ERASE_TEST(RL, begin(), end(), 4);
    ERASE_TEST(RL, begin(), end(), 5);
}
