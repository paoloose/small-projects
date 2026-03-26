module Cardinality where

{-
Definition of Cardinality:

    Two sets, A and B, have the same cardinality if there exists a bijective
    function between them

    This bijective function ensures every element in A pairs with exactly one element in B,
    and vice versa.
-}

{-
Exercises
    Prove that these functions have the same cardinality:

    1. (b -> a, c -> a)   and   (Either b c -> a)
    2. c -> (a, b)        and   (c -> a, c -> b)
    3. c -> (b -> a)      and   (b, c) -> a

    Note that these functions are polimorphic
-}

-- ⚫ 1.

to1 :: (b -> a, c -> a) -> (Either b c -> a)
to1 (f, g) bc = case bc of
  Left  b -> f b
  Right c -> g c

from1 :: (Either b c -> a) -> (b -> a, c -> a)
from1 h = (h . Left, h . Right)

-- ⚫ 2.

to2 :: (c -> (a, b)) -> (c -> a, c -> b)
to2 f = (fst . f, snd . f)

from2 :: (c -> a, c -> b) -> (c -> (a, b))
from2 (f, g) c = (f c, g c)

-- ⚫ 3.

to3 :: (c -> b -> a) -> ((b, c) -> a)
to3 f (b, c) = f c b

from3 :: ((b, c) -> a) -> (c -> b -> a)
from3 f c b = f (b, c)

{-
Type-Kwon-Do

Exercise 4.
    verif :: Eq b => (a -> b) -> a -> b -> Bool
    verif = undefined

    We apply f to a, and because the result has type b, which supports equality,
    then we can compare the result with our third parameter
-}

verif :: Eq b => (a -> b) -> a -> b -> Bool
verif f a b = f a == b


{-
Type-Kwon-Do

Exercise 5.
    calc :: Num b
        => (a -> b)
        -> b
        -> a
        -> b
    calc = undefined

    We apply f to a, and because the result has type b (Num), we can use any of the
    operations defined for numbers with our second parameter ((+) in this case)
-}

calc :: Num b
    => (a -> b)
    -> b
    -> a
    -> b
calc f b a = f a + b
