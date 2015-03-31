(def draw-rect (fn
                (while (def rw (- rw 1))
                  (do (def rrh rh)
                      (while (def rrh (- rrh 1))
                        (pixel (+ rx rw) (+ ry rrh) rcolor))))))

(def i 12)
(while (def i (- i 1)) (do 
                       (def rx (* 40 (+ i 2)))
                       (def ry rx)
                       (def rw 150)
                       (def rh 50)
                       (def rcolor (+ 255 (* 256 (* i 30))))
                       (draw-rect)))
