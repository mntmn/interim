(def fb (mmap "/framebuffer"))

(def f (open "/sd/unifont.565"))

(do (def unifont (recv f)) 0)

(def unifont-pitch 8256)

(def screen-pitch 3840)

(def blit-char (fn rune x y (do
  (let sx (* (+ 2 (% rune 256)) 16))
  (let sy (* (+ 4 (/ rune 256)) 16))
  (let so (+ (* sx 2) (* sy unifont-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))
  (let iy 0)
  
  (while (lt iy 16) (do
    (let ix 0)
    (while (lt ix 32) (do
      (let c (get32 unifont (+ so ix)))
      (put32 fb (+ do ix) c)
      (let ix (+ ix 2))
    ))
    (let so (- (+ so unifont-pitch) 16))
    (let do (- (+ do screen-pitch) 16))
    (let iy (+ iy 1))
  ))
)))

