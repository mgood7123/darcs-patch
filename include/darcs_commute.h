#pragma once

#include "darcs_types.h"

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

    struct Commute {

        static Maybe<Tuple2<FL<Patch>, PatchId>> commute (Tuple2<PatchId, FL<Patch>> & p) {
            if (p.first.isNil()) {
                return Nothing();
            }
        }

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

    geekosaur
    commuteFL uses methods from Commute, which at that point the compiler has converted
    to a record of methods representing a specific instance
    geekosaur
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

    using CommuteFn = std::function<Maybe<Tuple2<FL<Patch>, PatchId>> (Tuple2<PatchId, FL<Patch>>)>;

    Maybe<Tuple2<FL<Patch>, PatchId>> CommuteIdFL(const CommuteFn & commuter, const Tuple2<PatchId, FL<Patch>> & p) {
        if (p.v2.isNil()) {
            return {p.v2, p.v1};
        }
        PatchId x = p.v1;
        Patch y;
        FL<Patch> ys;
        p.v2.extract(y, ys);

        auto tmp = commuter({x, y});
        if (!tmp.has_value) {
            return tmp;
        }
        FL<Patch> y1 = tmp->v1;
        PatchId x1 = tmp->v2;
        auto tmp1 = commuterIdFL(commuter, {x1, ys});
        if (!tmp1.has_value) {
            return tmp1;
        }
        PatchId ys2 = tmp1->first;
        Patch y2;
        FL<Patch> x2;
        tmp1->second.extract(y2, x2);
        return {{x2, y2}, ys2};
    }

    Maybe<Tuple2<FL<Patch>, PatchId>> CommuteFL(const Tuple2<PatchId, FL<Patch>> & p) {
        return commuterIdFL(Commute::commute, p);
    }
}