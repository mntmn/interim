; logic functions  -----------------------------------------------

(def nil (quote ()))
(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (if a 1 (if b 1 0) 0)))

; list functions  -----------------------------------------------

(def item (fn lst idx (do
  (mut item-i 0)
  (while (gt idx item-i) (do
    (def lst (cdr lst))
    (mut item-i (+ item-i 1))))
  (car lst)
)))

; string functions  -----------------------------------------------

(def substr (fn str beg sz (do
  (def res (alloc-str (* 2 sz)))
  (mut ii 0)
  
  (while (lt ii sz) (do
    (uput res ii (uget str (+ beg ii)))
    (mut ii (+ ii 1))
  ))
  res
)))

(def split (fn str sepstr (do
  (mut sep (uget sepstr 0))
  (def result (quote ()))
  (mut sz (usize str))
  (mut i (- sz 1))
  (mut last-i (+ i 1))
  (while (gt i -2) (do
    (if (or (= (uget str i) sep) (= i -1)) (do
      (def partsize (- (- last-i i) 1))
  
      (if (gt partsize 0)
        (def result (cons (substr str (+ i 1) partsize) result)) 0)
      (mut last-i i)
    ) 0)
    (mut i (- i 1))
  ))
  result
)))

(def ufind-prev (fn buf rune pos (do
  (while (* (gt pos 0) (not (= rune (uget buf pos))))
    (mut pos (- pos 1)))
  (+ pos 0)
)))

(def ufind-next (fn buf rune pos (do
  (while (* (lt pos (usize buf)) (not (= rune (uget buf pos))))
    (mut pos (+ pos 1)))
  (+ pos 0)
)))

(def ucopy (fn buf from to num (do
  (if (lt from to)
    (do 
      (mut i (- num 1)) 
      (while (gt (mut i (- i 1)) -1) (do
        (def c (uget buf (+ from i)))
        (uput buf (+ to i) c)
      )))
    (do
      (mut i 0)
      (while (lt (mut i (+ i 1)) num) (do
        (def c (uget buf (+ from i)))
        (uput buf (+ to i) c)
      )))
  )
)))

(def ufill (fn buf from to what (do
  (while (lt from (+ to 1)) (do
    (uput buf from what)
    (mut from (+ from 1))
  ))
)))

(def fill (fn buf from to what (do
  (while (lt from (+ to 1)) (do
    (put buf from what)
    (mut from (+ from 1))
  ))
)))

(def chr (fn s (uget s 0)))

; fonts and colors -----------------------------------------------

(def unifont (load "unifont"))
(def keymap (load "keymap"))

(def white 16777215)
(def cyan 65535)
(def color white)
(def unifont-width 4128)
(def unifont-height 4160)
(def screen-w (/ 1024 16))
(def screen-h (/ 768 16))

(def blit-char (fn rune x y color
  (blit-mono unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ 4128 8) 2 16 x y color)
))

(def blit-char-inv (fn rune x y color (do
  (blit-mono-inv unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ 4128 8) 2 16 x y color)
)))

(def cursor-size 1)

(def palette (quote (
  0x000000
  0xffffff
  0x68372b
  0x70a4b2
  0x6f3d86
  0x588d43
  0x352879
  0xb8c76f
  0x6f4f25
  0x433900
  0x9a6759
  0x444444
  0x6c6c6c
  0x9ad284
  0x6c5eb5
  0x959595
)))

(def blit-str (fn cursor str xx y colormap (do
  (mut l (+ 1 (usize str)))
  (mut i 0)
  (mut x xx)
  (mut cmi 0)
  (while (lt i l) (do
    (mut chr (uget str i))
    
    (if (not (= chr 10))
      (do
        (blit-char-inv chr x y (item palette (get colormap i)))
        (mut x (+ x 16)))
      (do
        (mut y (+ y 16))
        (mut x xx))
    )
    
    (if (gt x 1008) (do
      (mut x xx)
      (mut y (+ y 16))) 0)
      
    (mut i (+ i 1))
  ))
)))

(def cursor 0)

(def screen (alloc-str (* 3 (* screen-w screen-h))))
(def colormap (alloc (* screen-w screen-h)))
(def solidmap (alloc (* screen-w screen-h)))

(def buf-render (fn focused b bx by (do
  (blit-str focused b bx by colormap)
)))

; symbols ------------------------------------------------------

(def sym-block 0x2b1b)
(def sym-umbrella 0x2614)
(def sym-coffee 0x2615)
(def sym-scissors 0x2700)
(def sym-zigzag 0x2307)
(def sym-dog 0x2620)
(def sym-pot 0x1013)
(def sym-pill 0x1005)
(def sym-egg 0x1810)
(def sym-ear 0x1028)
(def sym-rabbit 0x0c20)
(def sym-table 0x213f)
(def sym-salad 0x2e19)
(def sym-hot 0x2668)
(def sym-fish 0x1864)
(def sym-disc 0x105c)
(def sym-potb 0x1053)
(def sym-person 0x3020)
(def sym-blockb 0x2b12)

; structures -----------------------------------------------------------

; rune color solid

(def make-tile (fn rune color solid (cons rune (cons color (cons solid nil)))))

(def tile-space (make-tile sym-block 0 0))
(def tile-wall (make-tile sym-block 12 1))
(def tile-asphalt (make-tile sym-block 11 0))
(def tile-floor-yellow (make-tile sym-block 7 0))
(def tile-floor-wood (make-tile sym-block 8 0))
(def tile-floor-woodl (make-tile sym-block 9 0))
(def tile-dog (make-tile sym-dog 1 0))
(def tile-pot (make-tile sym-pot 1 0))
(def tile-pill (make-tile sym-pill 1 0))
(def tile-coffee (make-tile sym-coffee 1 0))
(def tile-window (make-tile sym-blockb 12 0))

(def tile-player (make-tile sym-person 10 1))

; rat
; pills
; money
; coffee

(def put-tile (fn x y tile (do
  (uput screen (+ x (* screen-w y))  (car tile))
  (put colormap (+ x (* screen-w y)) (car (cdr tile)))
  (put solidmap (+ x (* screen-w y)) (car (cdr (cdr tile))))
)))

(def is-tile-solid (fn tile (do
  (= 1 (car (cdr (cdr tile))))
)))

(def get-tile-rune (fn x y (do
  (uget screen (+ x (* screen-w y)))
)))

(def get-tile-solid (fn x y (do
  (get solidmap (+ x (* screen-w y)))
)))

(def tile-rect (fn x y xx yy tile (do
  (def i y)
  (while (lt y (+ yy 1)) (do
    (put-tile x  y tile)
    (put-tile xx y tile)
    (def y (+ y 1))
  ))

  (def y i)
  (while (lt x (+ xx 1)) (do
    (put-tile x y  tile)
    (put-tile x yy tile)
    (def x (+ x 1))
  ))
)))

(def fill-tile-rect (fn x y xx yy tile (do
  (print (cons "fill-tile-rect" tile))
  (def y (+ 0 y))
  (def x (+ 0 x))
  (while (lt y (+ yy 1)) (do
    (mut i x)
    (while (lt i (+ xx 1)) (do
      (put-tile i y tile)
      (mut i (+ i 1))
    ))
    (mut y (+ y 1))
  ))
)))

; game ------------------------------------------------------------------------

(def make-room (fn x y xx yy (do              
  (tile-rect      x y xx yy tile-wall)
  (fill-tile-rect (+ x 1) (+ y 1) (- xx 1) (- yy 1) tile-floor-yellow)
  (put-tile       (/ (+ x xx) 2) y tile-floor-yellow)
)))

(def state-init 1)
(def state-playing 2)
(def state state-init)
(def screen-size (* screen-w screen-h))

(def show-palette (fn (do
    (put colormap 0 0)
    (put colormap 1 1)
    (put colormap 2 2)
    (put colormap 3 3)
    (put colormap 4 4)
    (put colormap 5 5)
    (put colormap 6 6)
    (put colormap 7 7)
    (put colormap 8 8)
    (put colormap 9 9)
    (put colormap 10 10)
    (put colormap 11 11)
    (put colormap 12 12)
    (put colormap 13 13)
    (put colormap 14 14)
    (put colormap 15 15)
)))

(def actors nil)

(def make-actor (fn tile x y
  (cons tile (cons x (cons y nil)))
))

(def add-actor (fn a (do
  (def actors (cons a actors))
)))

(def actor-tile (fn a (car a)))
(def actor-rune (fn a (car (actor-tile a))))
(def actor-color (fn a (car (cdr (actor-tile a)))))
(def actor-x (fn a (car (cdr a))))
(def actor-y (fn a (car (cdr (cdr a)))))



; keyboard -----------------------------------------------------------

(def cursor-left (fn modif (do
  (if (= modif 1)
    (mut cursor-size (- cursor-size 1))
    (mut cursor (- cursor 1)))
  (if (lt cursor-size 1) (def cursor-size 1) 0)
)))

(def cursor-right (fn modif (do
  (if (= modif 1)
    (mut cursor-size (+ cursor-size 1))
    (mut cursor (+ cursor 1)))
)))

(def backspace (fn (if (gt cursor 0) (do
  (mut cursor (- cursor 1))
  (ucopy buf cursor (- cursor 1) (- (usize buf) (+ cursor 1)))
) 0)))

(def player-west (fn (do
  (def player-x (- player-x 1))
)))

(def move-player (fn nx ny (do
  (if (get-tile-solid nx ny)
    0
    (def player (make-actor (actor-tile player) nx ny)))
  (def actors nil)
  (add-actor player)
)))

(def player-west (fn (do
  (def nx (- (actor-x player) 1))
  (def ny (actor-y player))
  (move-player nx ny)
)))

(def player-east (fn (do
  (def nx (+ (actor-x player) 1))
  (def ny (actor-y player))
  (move-player nx ny)
)))

(def player-north (fn (do
  (def nx (actor-x player))
  (def ny (- (actor-y player) 1))
  (move-player nx ny)
)))

(def player-south (fn (do
  (def nx (actor-x player))
  (def ny (+ (actor-y player) 1))
  (move-player nx ny)
)))

(def handle-game-key (fn k modif (do
  (if (= k 79) (player-east)
      (if (= k 80) (player-west)
        (if (= k 82) (player-north)
          (if (= k 81) (player-south)
              
  (if (* (gt k 0) (lt k 64))
    (do
      (mut kidx (if (+ (= 1 modif) (= 2 modif)) (+ k 128) k))
      (mut kchr (uget keymap kidx))
      (uput screen 0 kchr)
    )
  0)
  
  ))))
)))

; --------------------------------------------------------------

(def actors-render (fn ox oy (do
  (def d actors)
  (def a (car d))
  (while a (do
    (blit-char-inv (actor-rune a) (* 16 (actor-x a)) (* 16 (actor-y a)) (item palette (actor-color a)))

    (def d (cdr d))
    (def a (car d))
  ))
)))

(while 1 (do

  (if (= state state-init) (do
    (fill colormap 0 (- screen-size 1) 0)
    (fill screen 0 (* (- screen-size 1) 2) 32)
    (ufill screen 0 (- screen-size 1) sym-block)

    (fill-tile-rect 0 0 60 12 tile-asphalt)

    (make-room 10 10 25 22)
    (put-tile  15 15 tile-dog)
    (put-tile  17 17 tile-pill)
    (put-tile  18 17 tile-coffee)

    (make-room 25 15 35 40)

    (print (cons "state" state))
    (print (cons "screen-size" screen-size))
    (print (cons "sym-block" sym-block))
    
    (def player (make-actor tile-player 13 13))
    (add-actor player)

    (def state state-playing)
  ))

  (buf-render 0 screen 0 0)
  (flip)
))
