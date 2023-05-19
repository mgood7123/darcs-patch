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

    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> CommuteRLFL(const Tuple2<RL<Patch*>, FL<Patch*>> & p);

    struct Commute {

        static Maybe<Tuple2<Patch*, Patch*>> commute (Tuple2<Patch*, Patch*> & p) {
            /*
                x ::
                Darcs.Patch.V2.RepoPatch.RepoPatchV2
                    Darcs.Patch.Prim.V1.Core.Prim wX wZ
                y ::
                Darcs.Patch.V2.RepoPatch.RepoPatchV2
                    Darcs.Patch.Prim.V1.Core.Prim wZ wY
                643      commute pair@(x :> y) =
                644        commuteNoConflicts (assertConsistent x :> assertConsistent y)
                645        `mplus`
                646        commuteConflicting pair
                647  
                ... [src/Darcs/Patch/V2/RepoPatch.hs:(644,7)-(646,29)] darcs>
            */
        }
        static Maybe<Tuple2<FL<Patch*>, FL<Patch*>>> commute (Tuple2<FL<Patch*>, FL<Patch*>> & p) {
            if (p.v1.isNil()) {
                return {{p.v2, p.v1};
            }
            if (p.v2.isNil()) {
                return {{p.v2, p.v1};
            }
            auto tmp = CommuteRLFL(ToRL(p.v1), p.v2);
            if (!tmp.has_value) {
                return tmp;
            }
            auto ys1 = tmp.v1;
            auto rsx = tmp.v2;
            return {{ys1, ToFL(rsx)}};
        }
        
        /*
            // this is called with the following
            
            [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo x1

            PatchInfo {
                _piDate = "20230508084629",
                _piName = "TAG refs/branches/master",
                _piAuthor = "smallville7123 <smallville7123@gmail.com>",
                _piLog = ["Ignore-this: e6e67f33f9d47f7725b0f23a22a9f112"],
                _piLegacyIsInverted = False
            }



            [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo y1

            PatchInfo {
                _piDate = "20230508084629",
                _piName = "TAG refs/branches/master",
                _piAuthor = "smallville7123 <smallville7123@gmail.com>",
                _piLog = ["Ignore-this: 6915e596e1147deec72d6421d15982c9"],
                _piLegacyIsInverted = False
            }
        */
        static Maybe<Tuple2<Named<Patch*>, Named<Patch*>>> commute (Tuple2<Named<Patch*>, Named<Patch*>> & p) {
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
            auto tmp = commute(p.v1.p, p.v2.p);
            if (!tmp.has_value) {
                return tmp;
            }
            return {{{p.v2.n, p.v2.d, tmp->v1}, {p.v1.n, p.v1.d, tmp->v2}}};
        }

        static Maybe<Tuple2<PatchInfoAndG, PatchInfoAndG>> commute (Tuple2<PatchInfoAndG, PatchInfoAndG> & p) {
            PatchInfoAndG x = p.v1;
            PatchInfoAndG y = p.v2;

            // y' :> x' <- commute (hopefully x :> hopefully y)
            // 
            /*
                -- | @'hopefully' hp@ tries to get a patch from a 'PatchInfoAnd'
                -- value. If it fails, it outputs an error \"failed to read patch:
                -- \<description of the patch>\". We get the description of the patch
                -- from the info part of 'hp'
                hopefully :: PatchInfoAndG p wA wB -> p wA wB
                hopefully = conscientiously $ \e -> text "failed to read patch:" $$ e
            */
            // eventually calls       NamedP n d       n is patchinfo        d is NilFL
            /*
                // [hopefully] itself returns the following

                x1 :: Darcs.Patch.Named.Named a wX wZ = Darcs.Patch.Named.NamedP
                                                        (Darcs.Patch.Info.PatchInfo
                                                            (Data.ByteString.Internal.PS ...)
                                                            (Data.ByteString.Internal.PS ...) ....)
                                                        [] (Darcs.Patch.Witnesses.Ordered.:>: _ _)
                y1 :: Darcs.Patch.Named.Named a wZ wY = Darcs.Patch.Named.NamedP
                                                        (Darcs.Patch.Info.PatchInfo
                                                            (Data.ByteString.Internal.PS ...)
                                                            (Data.ByteString.Internal.PS ...) ....)
                                                        [] (Darcs.Patch.Witnesses.Ordered.:>: _ _)
                264                                !x1 = hopefully x
                265                                !y1 = hopefully y
                266                                !tuple = (x1 :> y1)
            */
            /*
                // this is called with the following

                [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo x1

                PatchInfo {
                    _piDate = "20230508084629",
                    _piName = "TAG refs/branches/master",
                    _piAuthor = "smallville7123 <smallville7123@gmail.com>",
                    _piLog = ["Ignore-this: e6e67f33f9d47f7725b0f23a22a9f112"],
                    _piLegacyIsInverted = False
                }



                [-2: src/Darcs/Patch/PatchInfoAnd.hs:266:40-49] darcs> patch2patchinfo y1

                PatchInfo {
                    _piDate = "20230508084629",
                    _piName = "TAG refs/branches/master",
                    _piAuthor = "smallville7123 <smallville7123@gmail.com>",
                    _piLog = ["Ignore-this: 6915e596e1147deec72d6421d15982c9"],
                    _piLegacyIsInverted = False
                }
            */
            auto tmp = commuter({x.n, y.n});
            if (!tmp.has_value) {
                return tmp;
            }
            return {tmp->v2, tmp->v1};
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

    using CommuteFn = std::function<Maybe<Tuple2<FL<PatchInfoAndG>, PatchInfoAndG>> (Tuple2<PatchInfoAndG, FL<PatchInfoAndG>>)>;
    using CommuteFn2 = std::function<Maybe<Tuple2<Patch*, RL<Patch*>>> (Tuple2<RL<Patch*>, Patch*>)>;
    using CommuteFn3 = std::function<Maybe<Tuple2<Patch*, Patch*>> (Tuple2<Patch*, Patch*>)>;

    Maybe<Tuple2<FL<PatchInfoAndG>, PatchInfoAndG>> CommuteIdFL(const CommuteFn & commuter, const Tuple2<PatchInfoAndG, FL<PatchInfoAndG>> & p) {
        if (p.v2.isNil()) {
            return {p.v2, p.v1};
        }
        PatchInfoAndG x = p.v1;
        PatchInfoAndG y;
        FL<PatchInfoAndG> ys;
        p.v2.extract(y, ys);

        auto tmp = commuter({x, y}); // at this point, we are just metadata + hash
        if (!tmp.has_value) {
            return tmp;
        }
        PatchInfoAndG y1 = tmp->v1;
        PatchInfoAndG x1 = tmp->v2;
        auto tmp1 = commuterIdFL(commuter, {x1, ys});
        if (!tmp1.has_value) {
            return tmp1;
        }
        FL<PatchInfoAndG> ys1 = tmp1->v1;
        PatchInfoAndG x2 = tmp1->v2;
        return {ys1.append(y1), x2};
    }

    Maybe<Tuple2<FL<PatchInfoAndG>, PatchInfoAndG>> CommuteFL(const Tuple2<PatchInfoAndG, FL<PatchInfoAndG>> & p) {
        return commuterIdFL(Commute::commute, p);
    }

    Maybe<Tuple2<Patch*, RL<Patch*>> commuterRLId(const CommuteFn3 & commuter, const Tuple2<RL<Patch*>, Patch*> & p) {
        if (p.v1.isNil()) {
            return {p.v2, p.v1};
        }
        Patch* y = p.v2;
        Patch* x;
        RL<Patch*> xs;
        p.v1.extract(x, xs);

        auto tmp = commuter({x, y}); // at this point, we compare patches, eg FileHunk
        if (!tmp.has_value) {
            return tmp;
        }
        Patch* y1 = tmp->v1;
        Patch* x1 = tmp->v2;
        auto tmp1 = commuterRLId(commuter, {xs, y1});
        if (!tmp1.has_value) {
            return tmp1;
        }
        Patch* y2 = tmp1->v1;
        RL<Patch*> xs1 = tmp1->v2;
        return {y2, xs1.append(x1)};
    }

    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> left(const CommuteFn3 & commuter, const RL<Patch*> & p1, const FL<Patch*> & p2);

    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> right(const CommuteFn3 & commuter, const RL<Patch*> & p1, const FL<Patch*> & p2) {
        RL<Patch*> & as = p1;
        if (p2.isNil()) {
            return {{p2, as}};
        }
        FL<Patch*> bs;
        Patch * b;
        p2.extract(b, bs);
        auto tmp = commuterRLId(commuter, {as, b});
        if (!tmp.has_value) {
            return tmp;
        }
        auto b1 = tmp.v1;
        auto as1 = tmp.v2;
        auto tmp1 = left(commuter, as1, bs);
        if (!tmp1.has_value) {
            return tmp1;
        }
        auto bs1 = tmp.v1;
        auto as2 = tmp.v2;
        return {{bs1.append(b1), as2}};
    }
    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> left(const CommuteFn3 & commuter, const RL<Patch*> & p1, const FL<Patch*> & p2) {
        FL<Patch*> & bs = p2;
        if (p1.isNil()) {
            return {{bs, p1}};
        }
        FL<Patch*> as;
        Patch * a;
        p1.extract(a, as);
        auto tmp = commuterIdFL(commuter, {a, bs});
        if (!tmp.has_value) {
            return tmp;
        }
        auto bs1 = tmp.v1;
        auto a1 = tmp.v2;
        auto tmp1 = right(commuter, as, bs1);
        if (!tmp1.has_value) {
            return tmp1;
        }
        auto bs2 = tmp.v1;
        auto as1 = tmp.v2;
        return {bs2, {as1.append(a1)}};
    }
    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> CommuterRLFL(const CommuteFn3 & commuter, const RL<Patch*> & xs, const FL<Patch*> & ys) {
        return right(commuter, xs, ys);
    }

    Maybe<Tuple2<FL<Patch*>, RL<Patch*>>> CommuteRLFL(const Tuple2<RL<Patch*>, FL<Patch*>> & p) {
        return commuterRLFL(Commute::commute, p);
    }
}