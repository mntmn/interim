(def fb (mmap "/framebuffer"))
(def f (open "/framebuffer"))

(def draw-bg (fn c (do
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
)))

(def sprite [0000 0000 0000 00f0 00f0 00f0 00f0 00f0 0000 0000 0000 0000
             0000 0000 00f0 00f0 00f0 00f0 00f0 00f0 00f0 00f0 00f0 0000
             0000 0000 0088 0088 0088 00ff 00ff 0088 00ff 0000 0000 0000
             0000 0088 00ff 0088 00ff 00ff 00ff 0088 00ff 00ff 00ff 0000
             0000 0088 00ff 0088 0088 00ff 00ff 00ff 0088 00ff 00ff 00ff
             0000 0088 0088 00ff 00ff 00ff 00ff 0088 0088 0088 0088 0000
             0000 0000 0000 00ff 00ff 00ff 00ff 00ff 00ff 00ff 0000 0000
             0000 0000 00f0 00f0 f000 00f0 00f0 00f0 00f0 0000 0000 0000
             0000 00f0 00f0 00f0 f000 00f0 00f0 f000 00f0 00f0 00f0 0000
             00f0 00f0 00f0 00f0 f000 f000 f000 f000 00f0 00f0 00f0 00f0
             ffff ffff 00f0 f000 00ff f000 f000 00ff f000 00f0 ffff ffff
             ffff ffff ffff f000 f000 f000 f000 f000 f000 ffff ffff ffff
             ffff ffff f000 f000 f000 f000 f000 f000 f000 f000 ffff ffff
             0000 0000 f000 f000 f000 0000 0000 f000 f000 f000 0000 0000
             0000 0088 0088 0088 0000 0000 0000 0000 0088 0088 0088 0000
             0088 0088 0088 0088 0000 0000 0000 0000 0088 0088 0088 0088
])
             
(def sprite-w 12)
(def sprite-h 16)

(def line (fn x y sy (do
                        (let x1 (* 2 x))
                        (let x2 (- (* 2 (+ x sprite-w)) 2))
                        (let ofs (* y 3840))
                        (let x3 (* sy 24))
                        (while (lt x1 x2) (do
                          (let color (get32 sprite x3))
                          (put32 fb (+ ofs x1) color)
                          (let x1 (+ x1 2))
                          (let x3 (+ x3 2))
                        )) )))

(def blit (fn x y (do
                     (let y1 y)
                     (let y2 (+ y sprite-h))
                     (let sy 0)
                     (while (lt y1 y2) (do
                       (line x y1 sy)
                       (let y1 (+ y1 1))
                       (let sy (+ sy 1)) ))
                     )))

(def main (fn (while 1 (do
  (draw-bg 0)
  (let offset-x 500)
  (let offset-y 500)
  (let dx 1)
  (let dy 1)
  (let tick 0)
  (while 1 (do
    (let offset-x (+ offset-x dx))
    (let offset-y (+ offset-y dy))

    (if (gt offset-x 1800) (let dx -1) 0)
    (if (gt offset-y 900)  (let dy -1) 0)
    (if (lt offset-x 100)  (let dx 1) 0)
    (if (lt offset-y 100)  (let dy 1) 0)

    ; draw marios
    (let x 0)
    (let y 0)
    (while (lt y 10) (do
      (let x 0)
      (while (lt x 10) (do
        (blit (+ offset-x (* x 8)) (+ offset-y (* y 8)))
        (let x (+ x 3))
        ))
      (let y (+ y 3))))

    ; collect garbage
    (let tick (+ tick 1))
    (if (gt tick 5) (do
      (gc)
      (let tick 0)
      (print "gc!")) 0)
  ))
))))

(main)
