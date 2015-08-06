(
(def fb (mmap "/framebuffer"))

(def goa-main (fn
(do
(let c 0)
(let tick 0)
(while 1
  (do
    (let c (+ 1 c))
    (let b 1080)
    (while b
      (do
        (let b (- b 1))
          (let a 1920)
        (while a (do
          (let a (- a 1))
          (put32 fb (+ a (+ a (* b 3840))) (* 0x1000 (* a (+ c b))))
        ))))
    (let tick (+ tick 1))
    (gc)
    )))
))

(goa-main)
)
