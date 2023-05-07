#pragma once

#include "adapter.h"
#include <deque>
#include <optional>

namespace DarcsPatch {
    template <typename char_t, typename adapter_t, typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type* = nullptr>
    struct Patcher {
        struct Hunk {
            size_t idx = 0;
            size_t line = 0;
            adapter_t old_line;
            adapter_t new_line;
            size_t map = -1;
            bool i = false;
        };
        struct HunkLC {
            size_t line = 0;
            size_t old_line = 0;
            size_t new_line = 0;
        };
        struct HunkOffset {
            size_t old_line = 0;
            size_t new_line = 0;
        };
        
        std::deque<Hunk> hunks;
        
        void add(size_t line, const adapter_t & old_line, const adapter_t & new_line) {
            
            Hunk p = {hunks.size(), line, old_line, new_line};
            //HunkLC plc = {p.line, p.old_line.line_count(), p.new_line.line_count()};
            
            //printf("\n\npatch add: %zu, %s, %s\n", p.line, p.old_line.c_str(), p.new_line.c_str());
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
            
            //printf("patchLC: %zu, %zu, %zu\n", plc.line, plc.old_line, plc.new_line);
            
            hunks.push_back(p);
            //hunksLC.push_back(plc);
            
            printf("\n\n\n\n");
        }
        
        struct Dep {
            HunkLC hunkLC;
            size_t dependsOnLC;
        };
        
        std::deque<Dep> deps;
        std::deque<HunkLC> hunksLC;
        
        void calculateDeps() {
            
            hunksLC = {};
            
            for (Hunk & h : hunks) {
                h.map = -1;
                h.i = false;
            }
            
            size_t m = hunks.size()-1;
            size_t ni = m;
            printf("\n");
            for(;ni != -1; ni--) {
                Hunk & p2 = hunks[ni];
                if (p2.map == -1) {
                    p2.map = hunksLC.size();
                    hunksLC.push_back({p2.line, p2.old_line.line_count(), p2.new_line.line_count()});
                }
                HunkLC & n = hunksLC[p2.map];
                for(size_t oi = ni-1; oi != -1; oi--) {
                    Hunk & p1 = hunks[oi];
                    if (p1.map == -1) {
                        p1.map = hunksLC.size();
                        hunksLC.push_back({p1.line, p1.old_line.line_count(), p1.new_line.line_count()});
                    }
                    HunkLC & o = hunksLC[p1.map];
                    
                    for (size_t i = 0; i <= m; i++) {
                        printf("p%zu%s%s", hunks[i].idx+1, hunks[i].i ? "'" : "", i != m ? ";" : "");
                    }
                    printf("\n");
                    printf("commuteHunkLines(p%zu, p%zu)\n", p1.idx+1, p2.idx+1);
                    
                    
                    auto r = commuteHunkLines(o, n);
                    
                    if (!r) {
                        printf("  hunk #%zu ( %zu, %zu, %zu ) is related to hunk #%zu ( %zu, %zu, %zu )\n", p2.idx+1, n.line, n.old_line, n.new_line, p1.idx+1, o.line, o.old_line, o.new_line);
                        printf("\n");
                        break;
                    } else {
                        printf("  hunk #%zu ( %zu, %zu, %zu ) is not related to hunk #%zu ( %zu, %zu, %zu )\n", p2.idx+1, n.line, n.old_line, n.new_line, p1.idx+1, o.line, o.old_line, o.new_line);
                        printf("    offset: old line %zu, new line %zu\n", r->old_line, r->new_line);
                        o.line = r->old_line;
                        n.line = r->new_line;
                        p1.i = true;
                        p2.i = true;
                        std::swap(p1, p2);
                        printf("\n");
                    }
                }
            }
        }
        
        // TODO
        void reorder(Hunk p, Hunk plc) {
            if (hunksLC.size() > 1) {
                for(size_t i = hunksLC.size()-2; i != -1; i--) {
                    HunkLC o = hunksLC[i];
                    HunkLC & n = plc;
                    printf("calling commuteHunkLines( patch #%zu ( %zu, %zu, %zu ) , patch #%zu ( %zu, %zu, %zu ) )\n", i, o.line, o.old_line, o.new_line, hunksLC.size()-1, n.line, n.old_line, n.new_line);
                    auto r = commuteHunkLines(o, n);
                    printf("  patch #%zu ( %zu, %zu, %zu ) is related to patch #%zu ( %zu, %zu, %zu ) : %s\n", hunksLC.size()-1, n.line, n.old_line, n.new_line, i, o.line, o.old_line, o.new_line, r.has_value() ? "true" : "false");
                    if (r.has_value()) {
                        printf("    offset: old line %zu, new line %zu\n", r->old_line, r->new_line);
                    } else {
                        printf("    no offset\n");
                    }
                }
            }
        }
        
        std::optional<HunkOffset> commuteHunkLines(const HunkLC & l1, const HunkLC & l2) {
            if (l1.line + l1.new_line < l2.line) {
                return {{ l2.line - l1.new_line + l1.old_line, l1.line }};
            }
            else if (l2.line + l2.old_line < l1.line) {
                return {{ l2.line, l1.line + l2.new_line - l2.old_line }};
            }
            else if (l2.old_line != 0 && l1.old_line != 0 && l2.new_line != 0 && l1.new_line != 0 && l1.line + l1.new_line == l2.line) {
                return {{ l2.line - l1.new_line + l1.old_line, l1.line }};
            }
            else if (l2.old_line != 0 && l1.old_line != 0 && l2.new_line != 0 && l1.new_line != 0 && l2.line + l2.old_line == l1.line) {
                return {{ l2.line, l1.line + l2.new_line - l2.old_line }};
            }
            else return std::nullopt;
        }
    };
    
    using Patcher_T = Patcher<char, StringAdapter::CharAdapter>;
}