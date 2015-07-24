(def fib (fn n (if (lt n 3) 1 (+ (fib (- n 1)) (fib (- n 2))) )))


(fib 5)

(def fib (fn n (do
  (let i 1)
  (let j 1)
  (let r 1)
  (let n n)

  (while (gt n 2) (do
    (let r (+ i j))
    (let i j)
    (let j r)
    (let n (- n 1))
  ))

  r
)))


