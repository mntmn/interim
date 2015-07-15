
(def s "abcdefghijklmnopqrstuvwxyz0123456789ABCD")
(def i 0)
(while (lt i 40) (do (print (cons (get s (+ i 1)) (get s i))) (def i (+ i 1))))


(def i 0)
(def j 0)
(def s "........................................")

(def loopit (fn
(while 1 (do
(def i 0)
(def j 0)
(while (lt j 40) (do
(def s "........................................")
(def i 0)
(while (lt i 40) (do (put s i (get "*" 0)) (put s (- i j) (get "." 0)) (def i (+ i 1)) (print s)))
(def j (+ j 1))
))
(gc)
))))

