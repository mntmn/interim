(
(def frame 0)
(def tridemo (fn (do
  (mut tx1 (+ 500 (/ (sin (* frame 100)) 100)))
  (mut ty1 (+ 300 (/ (cos (* frame 100)) 100)))
  (mut tx2 (+ 800 (/ (cos (* frame 200)) 100)))
  (mut ty2 (+ 600 (/ (sin (* frame 200)) 100)))
  (mut frame (+ frame 1))
  (flip)
  (gc)
)))
)
