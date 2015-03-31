(do
(mut c 0)
(while 1
  (do
    (mut c (+ 1 (% (+ c 1) 256)))
    (mut b 320)
    (while (mut b (- b 1))
      (do
        (mut a 320)
        (while (mut a (- a 1))
          (pixel a b (/ (+ a b) c)))))
    (flip))))























