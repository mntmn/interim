(def outer (fn (do
  (let a 1)

  (let inner (fn (do
    (let b 4)
    b
  )))
  
  (inner)
)))

(def x (outer))

