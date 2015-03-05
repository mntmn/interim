(do (def c 0)
(def demo (fn (do
    (def c (+ 1 (% (+ c 64) 64)))
    (def b 256)
    (while (def b (- b 1))
      (do
        (def a 320)
        (while (def a (- a 1))
          (pixel a b (* (+ c a) (/ (* a b) c))))))))))

(while 1 (do (demo) (flip)))
