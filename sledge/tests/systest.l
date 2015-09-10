(def keyboard (open "/keyboard"))

(def i 0)
(while (lt i 10) (do
  (print (list "i:" i))
  (print (recv keyboard))
  (def i (+ i 1))
))

(def fb (mmap "/framebuffer"))

(print (list "framebuffer: " fb))

