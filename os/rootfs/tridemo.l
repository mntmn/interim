(
(def frame 0)
(def tridemo (fn (do
  (def tx1 (+ 500 (/ (sin (* frame 100)) 100)))
  (def ty1 (+ 300 (/ (cos (* frame 100)) 100)))
  (def tx2 (+ 800 (/ (cos (* frame 200)) 100)))
  (def ty2 (+ 600 (/ (sin (* frame 200)) 100)))
  (def frame (+ frame 1))
  (flip)
  (gc)
)))
)
