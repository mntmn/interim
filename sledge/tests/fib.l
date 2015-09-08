(
(def fib (fn on (do
  (let i 1)
  (let j 1)
  (let r 1)
  (let n on)

  (while (gt n 2) (do
    (let r (+ i j))
    (let i j)
    (let j r)
    (let n (- n 1))
  ))

  r
)))
)
