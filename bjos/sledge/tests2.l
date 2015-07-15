
(def s "abcdefghijklmnopqrstuvwxyz0123456789ABCD")
(def i 0)
(while (lt i 40) (do (print (cons (get s (+ i 1)) (get s i))) (def i (+ i 1))))


(def i 0)
(def j 0)
(def s "........................................")

(def loopit (fn
(while 1 (do
(def i 0)
(def j 0)
(while (lt j 40) (do
(def s "........................................")
(def i 0)
(while (lt i 40) (do (put s i (get "*" 0)) (put s (- i j) (get "." 0)) (def i (+ i 1)) (print s)))
(def j (+ j 1))
))
(gc)
))))


(def fb (open "/framebuffer"))
(def pixel (fn x y color (do (send fb x) (send fb y) (send fb color) )))

(def y 50)
(def x 50)
(def c 0)
(def paint (fn (do
(def y 50)
(def x 50)
(while (lt y 400) (do
  (def x 0)
  (while (lt x 400) (do
    (pixel x y (+ c (* y x)))
    (def x (+ x 1))
  ))
  (def y (+ y 1))
))
(def c (+ c 0x010101))
(gc)
)))

(while 1 (paint))
