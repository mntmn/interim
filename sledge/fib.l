(def fib (fn n (if (lt n 3) 1 (+ (fib (- n 1)) (fib (- n 2))) )))


(fib 5)

(def fib (fn n (do
  (def i (+ 0 1))
  (def j (+ 0 1))
  (def r (+ 0 1))

  (while (gt n 2) (do
    (mut r (+ i j))
    (mut i j)
    (mut j r)
    (mut n (- n 1))
  ))

  r
)))


