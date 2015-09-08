(
(def fib (fn on (do
  (let i 1)
  (print i)
  (let j 1)
  (print j)
  (let r 1)
  (print r)
  (let n on)
  (print on)
  (print n)
  (print (gt n 2))

  (while (gt n 2) (do
    (let r (+ i j))
    (let i j)
    (let j r)
    (let n (- n 1))
  ))

  r
)))
)
