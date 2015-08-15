(
(def rgb888->565 (fn orig (do
  (let sz (size orig))
  (let i 0)
  (let j 0)
  (let out (alloc (/ sz 2)))
  (while (lt i sz) (do
    (let r (shl (shr (get orig (+ i 2)) 3) 11))
    (let g (shl (shr (get orig (+ i 1)) 2)  5))
    (let b      (shr (get orig (+ i 0)) 3))
    (let rgb (bitor (bitor g b) r))
    (put out (+ j 1) (shr (bitand rgb 0xff00) 8))
    (put out (+ j 0) (bitand rgb 0xff))
    (let i (+ i 4))
    (let j (+ j 2))
  ))
  ; (print out)
  out
)))

(def f (open "/sd/smb3.888"))
(def mariotiles (rgb888->565 (recv f)))

(def sprites (rgb888->565 (load "/sd/mario.888")))

(def mario-palm (fn x y (do
  (let rs rune-spacing)
  
  (blit-str [c5c6c7] (* x rs) (* y rs))
  (blit-char 0x100 (* (+ 1 x) rs) (* (+ 1 y) rs))
)))

(def set-mario-font (fn (do
  (def font mariotiles)
  (def font-pitch 986)
  (def rune-w 17)
  (def rune-h 17)
  (def rune-spacing 16)
  (def rune-mod 58)
)))

(def set-sprite-font (fn (do
  (def font sprites)
  (def font-pitch 400)
  (def rune-w 20)
  (def rune-h 30)
  (def rune-spacing 20)
  (def rune-mod 100)
)))

(def mario (fn (do
  (set-mario-font)
  (let rs rune-spacing)

  ;(blit-str [0102030405060708090a0b0c0d0e] 0 0)
  ;(blit-str [0f101112131415161718191a1b1c] 0 17)

  (let i 0)
  (while (lt i 20) (do
    (blit-str [84848484848484848484848484848484848484848484848484848484848484] 0 (* i rs))
    (let i (+ i 1))
  ))

  (mario-palm 5 5)
  (mario-palm 7 6)
  (mario-palm 10 5)
  (mario-palm 9 8)

  (blit-char 0x42b (* 16 20) (* 16 8))
  (blit-char 0x42c (* 16 21) (* 16 8))
  (blit-char 0x465 (* 16 20) (* 16 9))
  (blit-char 0x466 (* 16 21) (* 16 9))
  
  (let x 0)
  (let tick 0)
  (while (lt x 100) (do
    (set-mario-font)
    (if tick 
      (blit-str "vvvv" (* 16 20) (* 16 10))
      (blit-str "wwww" (* 16 20) (* 16 10))
    )
    (set-sprite-font) 
    (let tick (- 1 tick))
    (blit-char (+ 5 tick) (+ 200 x) 150)
    (let x (+ x 1))
    (send scr 0)
  ))
  (set-unifont)
)))

(mario)
)
