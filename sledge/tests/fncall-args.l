
(def quantum (fn a b c (do
  (print (list "args" a b c))
  )))

(def pixel (fn a b c (do
  (print (list "args" a b c))
  (quantum a b c)
  )))

(def line (fn a b c (do
  (print (list "args" a b c))
  (pixel a b c)
)))

(def box (fn a b c (do
  (print (list "args" a b c))
  (line a b c)
)))

(def main (fn (do
   (box 1 2 3)
)))

(main)
