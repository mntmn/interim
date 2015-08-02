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

(def blit-str (fn str x y (do
  (let i 0)
  (let xx x)
  (let sz (size str))
  (while (lt i sz) (do
    (blit-char (get str i) xx y)
    (let xx (+ xx 8))
    (let i (+ i 1))
    (if (get str i) 0 (let i sz)) ; stop at 0
  ))
)))

(blit-str "Welcome to Interim OS." 32 32)

(def evbuf (alloc-str 512))
(def p (fn xp x y (do (write xp evbuf) (blit-str evbuf x y))))

(def k (open "/keyboard"))
(def maxx 1863)
(def maxy 1031)

(def strlen (fn s (do
  (let i 0)
  (let sz (size s))
  (while (lt i sz) (do
    (let i (+ i 1))
    (if (get str i) 0 (let i sz)) ; stop at 0
  ))
  i
)))

(def eq (fn a b (lt (+ (lt a b) (gt a b)) 1)))

(def main (fn (do
  (let x 32)
  (let y 64)
  (let blink 0)
  (let buffer "")

  (while 1 (do
    (let str (recv k))
    (let c (get str 0))

    (if (gt c 0) (print c) 0)

    (if (gt c 0x1f) (do
      (blit-str str x y)
      (let buffer (concat buffer str))
      (let x (+ x 8)) ) 0)
      
    (if (* (gt c 8) (lt c 10)) ; tab
      (do (blit-char 32 x y) (let y (+ y 16)) (let x 32) (let buffer "")) 0)
      
    (if (* (gt c 9) (lt c 11)) ; return
      (do
        (blit-char 32 x y)
        (let r (eval (list (read buffer))))
        (print r)
        (let buffer "")
        (let x 32) (let y (+ y 16))
        (p r x y) (let y (+ y 16))) 0)

    (if (* (gt c 0x7e) (lt c 0x80)) ; bksp
      (do
        (blit-char 32 x y)
        (let x (- x 16))
        (let nl (- (strlen buffer) 3))
        (let buffer (substr buffer 0 nl))
        (print (cons nl buffer))) 0)

    (if (gt x maxx) (do (let x 32) (let y (+ y 16))) 0)

    (if (gt y maxy) (let y 32) 0)

    (if (lt x 32) (let x 32) 0)
    
    (if (gt blink 9)
      (blit-char 0x275a x y) (blit-char 32 x y))
    (let blink (% (+ blink 1) 20))
    (send scr 0)
    
    (gc)
  ))
)))

(main)
)
