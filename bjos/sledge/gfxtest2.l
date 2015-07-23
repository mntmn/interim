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
                        (print (list "(line)" x y sy))
                        (let x1 (* 2 x))
                        (let x2 (- (* 2 (+ x sprite-w)) 2))
                        (let ofs (* y 3840))
                        (let x3 (* sy 24))
                        (print (list "x3" x3))
                        (while (lt x1 x2) (do
                          (let color (get32 sprite x3))
                          (put32 fb (+ ofs x1) color)
                          (let x1 (+ x1 2))
                          (let x3 (+ x3 2))
                        )) )))
(def blit (fn x yy (do
                     (let y1 yy) ; FIXME bogus without +
                     (let y2 (+ yy sprite-h))
                     (let sy 0)
                     (print (list "x" x "y1" y1 "y2" y2))
                     (while (lt y1 y2) (do
                       (print (list "line" x y1 sy))
                       (line x y1 sy)
                       (let y1 (+ y1 1))
                       (let sy (+ sy 1)) ))
                     )))
(def main (fn (do
  (let x 0)
  (let y 0)
  (while (lt y 100) (do
    (let x 0)
    (while (lt x 240) (do
      (blit (* x 8) (* y 8))
      (let x (+ x 3))
      ))
    (print y)
    (let y (+ y 3)))) )))
