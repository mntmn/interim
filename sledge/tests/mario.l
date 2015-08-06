(
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (+ a b)))
(def not (fn a (if a 0 1)))
(def eq (fn a b (lt (+ (lt a b) (gt a b)) 1)))

(def scr (open "/framebuffer"))
(def fb (mmap "/framebuffer"))
(def f (open "/sd/unifont.565"))
(def unifont (recv f))
(def unifont-pitch 8256)

(def f (open "/sd/smb3.565"))
(def mariotiles (recv f))

(def font unifont)
(def font-pitch unifont-pitch)

(def rune-w 16)
(def rune-spacing 8)
(def rune-h 16)
(def rune-mod 256)

(def mario-map [00])

(def font-mario (fn (do
  (def font mariotiles)
  (def font-pitch 986)
  (def rune-w 17)
  (def rune-h 17)
  (def rune-spacing 16)
  (def rune-mod 58)
)))

(def screen-pitch  3840)

(def blit-char (fn rune x y (do
  (let sx (* (+ 2 (% rune rune-mod)) rune-w))
  (let sy (* (+ 2 (/ rune rune-mod)) rune-h))
  (let so (+ (* sx 2) (* sy font-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))
  (let iy 0)
  (let rune-ww (+ rune-w rune-w))
  
  (while (lt iy rune-h) (do
    (let ix 0)
    (while (lt ix rune-ww) (do
      (let c (get font (+ so ix)))
      (put fb (+ do ix) c)
      (put fb (+ (+ do ix) 1) c)
      (let ix (+ ix 2))
    ))
    (let so (+ so font-pitch))
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
    (let xx (+ xx rune-spacing))
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
  (let c (get s 0))
  (while (and (gt c 0) (lt i sz)) (do
    (let i (+ i 1))
    (let c (get s i))
  ))
  i
)))

(def main (fn (do
  (let x 32)
  (let y 64)
  (let blink 0)
  (let buffer "")

  (while 1 (do
    (let str (recv k))
    (let c (get str 0))

    (if (gt c 0) (print c) 0)

    (if (and (gt c 0x1f) (not (eq 0x7f c))) (do
      (blit-str str x y)
      (let buffer (concat buffer str))
      (let x (+ x rune-spacing)) ) 0)
      
    (if (eq c 9) ; tab
      (do (blit-char 32 x y) (let y (+ y 16)) (let x 32) (let buffer "")) 0)
      
    (if (eq c 10) ; return
      (do
        (blit-char 32 x y)
        (let r (eval (list (read buffer))))
        (print r)
        (let buffer "")
        (let x 32) (let y (+ y rune-w))
        (p r x y) (let y (+ y rune-h))) 0)
    
    (if (and (eq c 0x7f) (gt (strlen buffer) 0)) ; bksp
      (do
        (print (cons "strlen" (strlen buffer)))
        (blit-char 32 x y)
        (let x (- x rune-spacing))
        (print buffer)
        (let nl (- (strlen buffer) 1))
        (let buffer (substr buffer 0 nl))
        (print (cons nl buffer))) 0)

    (if (gt x maxx) (do (let x 32) (let y (+ y rune-h))) 0)

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
