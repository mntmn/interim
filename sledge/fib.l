(def fib (fn n (if (lt n 3)
                   1
                 (+ (fib (- n 1)) (fib (- n 2))) )))
(fib 5)
