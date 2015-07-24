
(def demo (fn (do
  (def c 0)
  (def a 1024)
  (while 1 (do
    (def c (+ 1 c))
    (def b 1079)
    (while (def b (- b 1)) (do
      (def a 1919)
      (def c (+ 1 c))
      (while (def a (- a 1))
        (pixel a b (* (- (- a b) c) c)))))
    (flip)
    (gc)
  ))
)))

(def frame 0)
(def tridemo (fn (do
  (def tx1 (+ 500 (/ (sin (* frame 100)) 100)))
  (def ty1 (+ 300 (/ (cos (* frame 100)) 100)))
  (def tx2 (+ 800 (/ (cos (* frame 200)) 100)))
  (def ty2 (+ 600 (/ (sin (* frame 200)) 100)))
  (def frame (+ frame 1))
  (flip)
  (gc)
)))

(def update-statusline (fn buf (do
  (def memstat (load "/sys/mem"))
  (def cellusage (/ (* 100 (car (cdr (cdr memstat)))) (car (cdr (cdr (cdr memstat))))))
  (def memusage (/ (* 100 (car memstat)) (car (cdr memstat))))
  (write buf (cons (cons cursor (usize buf)) (cons (cons "heap%" memusage) (cons "cell%" cellusage))))
  0
)))


(def blit-char-inv (fn rune x y color (do
  (blit-mono-inv unifont 
    (* (% rune 256) 16) 
    (* (/ rune 256) 16) 4096 16 16 x y color)
)))

(def blit-char (fn rune x y color
  (blit-mono unifont 
    (* (% rune 256) 16) 
    (* (/ rune 256) 16) 4096 16 16 x y color)
))

(def blit-str (fn cursor str x y color (do
  (def len (+ (usize str) 1))
  (def i -1)
  (def xx x)
  (while (and (lt y 1080) (lt (def i (+ i 1)) len)) (do
    (def chr (uget str i))
    (if (= i cursor)
      (blit-char-inv chr xx y 0x6666ff)
      (if (or (= chr 10) (= chr 32)) 0 (blit-char-inv chr xx y color)))
    (def xx (+ xx 8))
    (if (= chr 10)
      (do
        (def y (+ y 16))
        (def xx x)))
    0
  ))
  0
)))

  (buf-append 0 "(list 
0x0000 0x0000 0x0000 0x0000 0x0000
0x0000 0x0000 0x0000 0x0000 0x0000
0x0000 0x0000 0x0000 0x0000 0x0000
0x0000 0x0000 0x0000 0x0000 0x0000
0x0000 0x0000 0x0000 0x0000 0x0000
)")
