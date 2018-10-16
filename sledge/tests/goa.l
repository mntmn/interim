(def screen (open "/framebuffer"))
(def fb (mmap "/framebuffer"))
(def load (fn path (recv (open path))))

(def width  (load "/framebuffer/width"))
(def height (load "/framebuffer/height"))
(def shift  (- (load "/framebuffer/depth") 1))
(def pitch (shl width shift))

(print (list width "x" height ":" pitch))

(def width 256)
(def height 256)

(def goa-main (fn
(do
(let c 0)
(let tick 0)
(while 1
  (do
    (let c (+ 1 c))
    (let b height)
    (while b
      (do
        (let b (- b 1))
        (let a width)
        (while a (do
          (let a (- a 1))
          (put16 fb (+ (shl a 1) (* b pitch)) (* c (* a (+ c b))))
        ))))
    (let tick (+ tick 1))
    (gc)
    (send screen 0)
    )))
))

(goa-main)
