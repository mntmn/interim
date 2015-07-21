(def ofs 0)
(def sofs 0)
(def x 0)
(def y 0)
(def x1 0)
(def x2 0)
(def x3 0)
(def y1 0)
(def y2 0)
(def sy 0)
(def color 0)
(def fb (mmap "/framebuffer"))
(def f (open "/framebuffer"))

(def sprite [00000ff00ff000000ff00ff000000000
             0ff0000000000ff0000000000ff00000
             0ff0000000000000000000000ff00000
             00000ff00000000000000ff000000000
             00000ff00000000000000ff000000000
             000000000ff000000ff0000000000000
             0000000000000ff00000000000000000
             00000000000000000000000000000000])

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
                        (def x1 (* 2 x))
                        (def x2 (- (* 2 (+ x sprite-w)) 2))
                        (def ofs (* y 3840))
                        (def x3 (* sy 24))
                        (while (lt x1 x2) (do
                          (def color (get32 sprite x3))
                          (put32 fb (+ ofs x1) color)
                          (def x1 (+ x1 2))
                          (def x3 (+ x3 2))
                        )) )))
(def blit (fn x y (do
                     (def y1 y)
                     (def y2 (+ y sprite-h))
                     (def sy 0)
                     (while (lt y1 y2) (do
                       (line x y1 sy)
                       (def y1 (+ y1 1)) (def sy (+ sy 1)) ))
                     (gc)
                     )))
(def main (fn (do
  (def x 0)
  (def y 0)
  (while (lt y 100) (do
    (def x 0)
    (while (lt x 240) (do
      (blit (* x 8) (* y 8))
      (def x (+ x 3))
      ))
    (print y)
    (send f 0)
    (def y (+ y 3)))) )))
