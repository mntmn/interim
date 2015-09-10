(
(def gfx-w 16)
(def gfx-h 16)
(def block-size 16)
(def gfxbuf (alloc (shl (* gfx-w gfx-h) 1)))
(def palette (list 0x0000 0xffff 0x8888 0xf000 0x0f00 0x00f0 0x0f70 0xf00f))
(def gfx-color 0x0000)

(def get-gfx (fn x y (do
  (let ofs 0)
  (let ofs (shl (+ x (* y gfx-w)) 1))
  (let a (get gfxbuf ofs))
  (let b (get gfxbuf (+ 1 ofs)))
  (bitor (shl a 8) b)
)))

(def put-gfx (fn x y c (do
  (put gfxbuf (shl (+ x (* y gfx-w)) 1) (shr c 8))
  (put gfxbuf (+ (shl (+ x (* y gfx-w)) 1) 1) (bitand c 0xff))
  c
)))

(def render-gfx (fn (do
  (let x 0)
  (let y 0)
  (while (lt y gfx-h) (do
    (let x 0)
    (while (lt x gfx-w) (do
      (let c (get-gfx x y))
      (boxfill (* x block-size) (* y block-size) block-size block-size c)
      (let x (+ x 1))
    ))
    (let y (+ y 1))
  ))
  0
)))

(def render-palette (fn (do
  (let x (* block-size gfx-w))
  (let y 0)
  (let pal palette)
  (let pal-bs 32)
  (while (cdr pal) (do
    (let c (car pal))
    (let x1 x)
    (let y1 (* y pal-bs))
    (boxfill x1 y1 pal-bs pal-bs c)
    (if (and mouse-btn (and (gt mouse-x x1) (and (lt mouse-y (+ y1 pal-bs)) (and (gt mouse-y y1) (lt mouse-x (+ x1 pal-bs))))))
      (def gfx-color c)
    0)
    (let y (+ y 1))
    (let pal (cdr pal))
  ))
  0
)))

(def gfx-task (fn (do
  (if mouse-btn (do
    (let mx (/ mouse-x block-size))
    (let my (/ mouse-y block-size))
    (if (and (lt mx gfx-w) (lt my gfx-h)) (do
      (put-gfx mx my gfx-color)
    ) 0)
  ) 0)

  (render-gfx)
  (render-palette)
)))

(def gfx (fn do
  (def tasks (list)) ; FIXME hack
  (mt-test)
  (add-task gfx-task)
))
)
