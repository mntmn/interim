(def fb (mmap "/framebuffer"))

(def f (open "/sd/unifont.565"))
(def unifont (recv f))

(def unifont-pitch 8256)

(def screen-pitch 3840)

(def blit-char (fn rune x y (do
  (let sx (* (+ 2 (% rune 256)) 16))
  (let sy (* (+ 4 (/ rune 256)) 16))
  (print (cons sx sy))
  (let so (+ (* sx 2) (* sy unifont-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))
  (print (list "sx" sx "sx*2" (* sx 2) "sy*up" (* sy unifont-pitch)))
  (print (list "so" so "do" do))
  (let iy 0)
  
  (while (lt iy 16) (do
    (let ix 0)
    (while (lt ix 32) (do
      (print (list "line" iy "so" so "ix" ix))
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
