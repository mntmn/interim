(def xp (fn x y (print (list "x,y" x y))))
(def foo (fn a b c d e f (xp e f)))
(foo 1 2 3 4 5 6)
