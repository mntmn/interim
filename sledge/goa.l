(def c 0)
(while 1
  (do
    (def c (% (+ c 1) 4096))
    (def b 480)
    (while (def b (- b 1))
      (do
        (def a 640)
        (while (def a (- a 1))
          (pixel a b (+ a (+ (* a b) (* c 2048)))))))
    (flip)))
