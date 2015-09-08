(
(def cursor 0)

(def screen-w 40)
(def screen-h 40)

(def screen   (alloc (* screen-w screen-h)))
(def colormap (alloc (* screen-w screen-h)))
(def solidmap (alloc (* screen-w screen-h)))

(def fill (fn buf from to c (do
  (print (list "fill" (size buf) from to c))
  (let i from)
  (while (lt i (+ to 1)) (do
    (put buf i c)
    (let i (+ i 1))
  ))
)))

(def buf-render (fn b bx by (do
  (let y 0)
  (while (lt y screen-h) (do
    (blit-str (substr b (* y screen-w) screen-w) bx (+ by (* y 16)))
    (let y (+ y 1))
  ))
  0
)))

; symbols ------------------------------------------------------

(def sym-block (get "#" 0))
(def sym-umbrella  (get "m" 0))
(def sym-coffee  (get "c" 0))
(def sym-scissors  (get "x" 0))
(def sym-zigzag  (get "z" 0))
(def sym-dog  (get "D" 0))
(def sym-pot  (get "p" 0))
(def sym-pill  (get "." 0))
(def sym-egg  (get "o" 0))
(def sym-ear  (get "e" 0))
(def sym-rabbit  (get "R" 0))
(def sym-table  (get "T" 0))
(def sym-salad  (get "s" 0))
(def sym-hot  (get "~" 0))
(def sym-fish  (get "<" 0))
(def sym-disc  (get "0" 0))
(def sym-potb  (get "p" 0))
(def sym-person  (get "@" 0))
(def sym-blockb  (get "=" 0))

; structures -----------------------------------------------------------

; rune color solid

(def make-tile (fn rune color solid (cons rune (cons color (cons solid nil)))))

(def tile-space (make-tile 32 0 0))
(def tile-wall (make-tile sym-block 12 1))
(def tile-asphalt (make-tile (get "." 0) 11 0))
(def tile-floor-yellow (make-tile (get "_" 0) 7 0))
(def tile-floor-wood (make-tile (get "_" 0) 8 0))
(def tile-floor-woodl (make-tile (get "_" 0) 9 0))
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
  (put screen   (+ x (* screen-w y))  (car tile))
  (put colormap (+ x (* screen-w y)) (car (cdr tile)))
  (put solidmap (+ x (* screen-w y)) (car (cdr (cdr tile))))
)))

(def is-tile-solid (fn tile (do
  (eq 1 (car (cdr (cdr tile))))
)))

(def get-tile-rune (fn x y (do
  (get screen (+ x (* screen-w y)))
)))

(def get-tile-solid (fn x y (do
  (get solidmap (+ x (* screen-w y)))
)))

(def tile-rect (fn x y xx yy tile (do
  (let i y)
  (while (lt y (+ yy 1)) (do
    (put-tile x  y tile)
    (put-tile xx y tile)
    (let y (+ y 1))
  ))

  (let y i)
  (while (lt x (+ xx 1)) (do
    (put-tile x y  tile)
    (put-tile x yy tile)
    (let x (+ x 1))
  ))
)))

(def fill-tile-rect (fn x y xx yy tile (do
  (print (cons "fill-tile-rect" tile))
  (print (list x y xx yy))
  (while (lt y (+ yy 1)) (do
    (let i x)
    (while (lt i (+ xx 1)) (do
      (put-tile i y tile)
      (let i (+ i 1))
    ))
    (let y (+ y 1))
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

(def actors (quote ()))


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

(def player (make-actor tile-player 13 13))

; keyboard -----------------------------------------------------------

(def move-player (fn nx ny (do
  (if (get-tile-solid nx ny)
    0
    (do
      (def player (make-actor (actor-tile player) nx ny))
      (def actors (quote ()))
      (add-actor player)))
)))

(def player-west (fn (do
  (let nx (- (actor-x player) 1))
  (let ny (actor-y player))
  (move-player nx ny)
)))

(def player-east (fn (do
  (let nx (+ (actor-x player) 1))
  (let ny (actor-y player))
  (move-player nx ny)
)))

(def player-north (fn (do
  (let nx (actor-x player))
  (let ny (- (actor-y player) 1))
  (move-player nx ny)
)))

(def player-south (fn (do
  (let nx (actor-x player))
  (let ny (+ (actor-y player) 1))
  (move-player nx ny)
)))

(def handle-game-key (fn k (do
  (if (eq k 20) (player-east)
      (if (eq k 19) (player-west)
        (if (eq k 17) (player-north)
          (if (eq k 18) (player-south)
              
  (if (* (gt k 0) (lt k 64))
    (do
      (put screen 0 k)
    )
  0)
  
  ))))
)))

; --------------------------------------------------------------

(def ox 0)
(def oy 0)

(def actors-render (fn ox oy (do
  (let ad actors)
  (let aa (car ad))
  
  (while aa (do
    (blit-char (actor-rune aa) (* 8 (actor-x aa)) (* 16 (actor-y aa)))
    
    (let ad (cdr ad))
    (let aa (car ad))
    0
  ))
)))

(def main (fn
(while 1 (do

  (if (eq state state-init) (do
    (fill colormap 0 (- screen-size 1) 0)
    (fill screen 0 (- screen-size 1) 32)
    ;(fill screen 0 (- screen-size 1) sym-block)

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
  ) 0)

  (let str (recv keyboard))
  (let c (get str 0))

  (handle-game-key c)
  (buf-render screen 0 0)
  (actors-render 0 0)
  (send scr 0)
  (gc)
))
))

(main)

)
