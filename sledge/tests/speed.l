(
(def scr (open "/framebuffer"))
(def fb (mmap "/framebuffer"))
(def f (open "/sd/unifont.565"))

(def unifont (recv f))
(def unifont-pitch 8256)
(def screen-pitch  3840)

(def blit-char (fn rune x y (do
  (let sx (* (+ 2 (% rune 256)) 16))
  (let sy (* (+ 2 (/ rune 256)) 16))
  (let so (+ (* sx 2) (* sy unifont-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))
  (let iy 0)
  
  (while (lt iy 16) (do
    (let ix 0)
    (while (lt ix 32) (do
      (let c (get unifont (+ so ix)))
      (put fb (+ do ix) c)
      (put fb (+ (+ do ix) 1) c)
      (let ix (+ ix 2))
    ))
    (let so (+ so unifont-pitch))
    (let do (+ do screen-pitch))
    (let iy (+ iy 1))
  ))
)))

(def main (fn (do
  (let i 1)
  (while 1 (do
    (let y 0)
    (while (lt y 48) (do
      (let x 0)
      (while (lt x 128) (do
        (blit-char i (+ 100 (* x 8)) (+ 100 (* y 16)))
        (let x (+ x 1))
      ))
      (let y (+ y 1))
    ))
    (send scr 0)
    (gc)
    (let i (+ i 1))
    (if (gt i 1000) (do (let i 0)) 0)
  ))
)))

(main)
)
