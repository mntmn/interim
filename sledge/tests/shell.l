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

(def rgb888->565 (fn orig (do
  (let sz (size orig))
  (let i 0)
  (let j 0)
  (let out (alloc (/ sz 2)))
  (while (lt i sz) (do
    (let r (shl (shr (get orig (+ i 2)) 3) 11))
    (let g (shl (shr (get orig (+ i 1)) 2)  5))
    (let b      (shr (get orig (+ i 0)) 3))
    (let rgb b)
    ;(put out j       (shr (bitand rgb 0xff00) 8))
    (put out j 0xff)
    (let i (+ i 4))
    (let j (+ j 2))
  ))
  out
)))

(def f (open "/sd/smb3.888"))
(def mariotiles (rgb888->565 (recv f)))

(def font unifont)
(def font-pitch unifont-pitch)

(def rune-w 16)
(def rune-spacing 8)
(def rune-h 16)
(def rune-mod 256)

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

; 112 x 30 chars at scale 2

(def scale 2)
(def maxx (/ 1847 scale))
(def maxy (/ 1015 scale))
(def minx 32)
(def miny 32)

(def blit-str (fn str x y (do
  (let i 0)
  (let xx x)
  (let yy y)
  (let sz (size str))
  (while (lt i sz) (do
    (blit-char (get str i) xx yy)
    (let xx (+ xx rune-spacing))
    (if (gt xx maxx) (do
      (let xx minx)
      (let yy (+ yy rune-h))
      (if (gt yy maxy) (do
        (let yy miny)) 0)
    ) 0)
    (let i (+ i 1))
    (if (get str i) 0 (let i sz)) ; stop at 0
  ))
  yy
)))

(blit-str "Welcome to Interim OS." 32 32)

(def evbuf (alloc-str 512))
(def p (fn xp x y (do
  (write xp evbuf)
  (blit-str evbuf x y)
)))

(def k (open "/keyboard"))

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

(def load (fn path (do
  (recv (open path))
)))

(def term-x minx)
(def term-y (+ miny 32))

(def history (list))

(def main (fn (do
  (let blink 0)
  (let buffer "")
  (let running 1)

  (put32 fb 3998 0xf800001f)
  (put32 fb 4002 0x07e0001f)

  (while running (do
    (let str (recv k))
    (let c (get str 0))

    (if (gt c 0) (print c) 0)

    (if (and (gt c 0x1f) (not (eq 0x7f c))) (do
      (def term-y (blit-str str term-x term-y))
      (let buffer (concat buffer str))
      (def term-x (+ term-x rune-spacing)) ) 0)
      
    (if (eq c 9) ; tab
      (do
        (blit-char 32 term-x term-y)
        (def term-y (+ term-y 16))
        (def term-x 32) (let buffer "")) 0)
      
    (if (eq c 10) ; return
      (do
        (blit-char 32 term-x term-y)
        (let r (eval (list (read buffer))))
        (print r)
        (def history (cons buffer history))
        (let buffer "")
        (def term-x 32) (def term-y (+ term-y rune-w))
        (def term-y (+ rune-h (p r term-x term-y)))
      ) 0)
    
    (if (and (eq c 0x7f) (gt (strlen buffer) 0)) ; bksp
      (do
        ; (print (cons "strlen" (strlen buffer)))
        (blit-char 32 term-x term-y)
        (def term-x (- term-x rune-spacing))
        (print buffer)
        (let nl (- (strlen buffer) 1))
        (let buffer (substr buffer 0 nl)) ) 0)

    (if (gt term-x maxx) (do (def term-x minx) (def term-y (+ term-y rune-h))) 0)

    (if (gt term-y maxy) (def term-y miny) 0)

    (if (lt term-x 32) (def term-x minx) 0)
    
    (if (gt blink 9)
      (blit-char 0x2588 term-x term-y) (blit-char 32 term-x term-y))
    (let blink (% (+ blink 1) 20))
    (send scr 0)
    
    (gc)
  ))
)))


(def mario-map [00])

(def mario (fn (do
  (def font mariotiles)
  (def font-pitch 986)
  (def rune-w 17)
  (def rune-h 17)
  (def rune-spacing 16)
  (def rune-mod 58)

  (blit-str [0102030405060708090a0b0c0d0e] 0 0)
  (blit-str [0f101112131415161718191a1b1c] 0 17)
)))

(main)
)
