(def demo (fn
(do (mut c 0)
    (def a 1024)
  (while 1 (do
    (mut c (+ 1 (% (+ c 1) 256)))
    (mut b 768)
    (while (mut b (- b 1))
      (do
        (mut a 512)
        (mut c (+ 1 c))
        (while (mut a (- a 1))
          (pixel (+ a 512) b (* (+ c a) (/ (* a b) c))))))
    (flip))))
))
