(
(struct surface
        width  0
        height 0
        shift  1
        pitch  0
        x      0
        y      0
        pixels [])

(struct font
        surface (surface)
        rune-w 16
        rune-h 16
        rune-mod 256 ;; ?
        spacing 8) ;; todo kerning table

(def fb (surface))

(sput fb pixels (mmap "/framebuffer"))
(sput fb width  (load "/framebuffer/width"))
(sput fb height (load "/framebuffer/height"))
(sput fb shift  (- (load "/framebuffer/depth") 1))
(sput fb pitch (shl (sget fb width) (sget fb shift)))

(print (list "[gfx] fb " fb))

(def screen (open "/framebuffer"))

(def set-pixel (fn (surf surface) x y c (do
  (let pixels (sget surf pixels))
  (let ofs (+ (* y (sget surf pitch)) (shl x (sget surf shift))))
  
  (put16 pixels ofs c)
  c
)))

(def pt list)

(def line (fn (s surface) a b color (do
  (let xa (car a))
  (let ya (car (cdr a)))
  (let xb (car b))
  (let yb (car (cdr b)))

  (let dx (abs (- xb xa)))
  (let dy (abs (- yb ya)))
  (let sx (if (lt xa xb) 1 -1))
  (let sy (if (lt ya yb) 1 -1))

  (let err (if (gt dx dy) dx (- 0 dy)))
  (let err (/ err 2))
  (let e2 0)

  (while (not (and (eq xa xb) (eq ya yb))) (do
    (set-pixel s xa ya color)
    (let e2 err)
    (if (gt e2 (- 0 dx)) (do (let err (- err dy)) (let xa (+ xa sx))) 0)
    (if (lt e2       dy) (do (let err (+ err dx)) (let ya (+ ya sy))) 0)
  ))
  0
)))

(def unifont-surf (surface))
(sput unifont-surf pixels (load "/sd/unifont.bin"))
(sput unifont-surf width 4096)
(sput unifont-surf height 4096)
(sput unifont-surf pitch 4096)
(sput unifont-surf shift 0)
(def unifont (font))
(sput unifont surface unifont-surf)
(sput unifont rune-w 16)
(sput unifont rune-h 16)
(sput unifont rune-mod 256) ;; 256 chars per row
(sput unifont spacing 8)

(def blit-char16 (fn (surf surface) (font font) rune x y (do
  (let sx 0)
  (let sy 0)
  (let so 0)
  (let do 0)
  (let iy 0)
  (let rune-ww 0)
  (let c 0)
  (let d 0)
  (let rune-mod (sget font rune-mod))
  (let font-surf (sget font surface))
  (let font-pitch (sget font-surf pitch))
  (let screen-pitch (sget surf pitch))
  (let rune-spacing (sget font spacing))
  (let rune-w (sget font rune-w))
  (let rune-h (sget font rune-h))
  (let pixels (sget surf pixels))
  (let font-pixels (sget font-surf pixels))
  
  (let sx (* rune-w (% rune rune-mod)))
  (let sy (* rune-h (/ rune rune-mod)))
  (let so (+ sx (* sy font-pitch)))
  (let do (+ (shl  x 1) (*  y screen-pitch)))

  (let rune-ww (shl rune-spacing 1))

  (while (lt iy rune-h) (do
    (let ix 0)
    (while (lt ix rune-ww) (do
      (let c (get8 font-pixels (+ so (shr ix 1))))
      (put16 pixels (+ do ix) (bitor c (shl c 8)))
      (let ix (+ ix 2))
    ))
    (let so (+ so font-pitch))
    (let do (+ do screen-pitch))
    (let iy (+ iy 1))
  ))
  0
)))

; (def blit-char (fn rune x y (do
;   (let sx 0)
;   (let sy 0)
;   (let so 0)
;   (let do 0)
;   (let iy 0)
;   (let rune-ww 0)
;   (let c 0)
;   (let d 0)
  
;   (let sx (* rune-w (% rune rune-mod)))
;   (let sy (* rune-h (/ rune rune-mod)))
;   (let so (+ sx (* sy font-pitch)))
;   (let do (+ (*  x 2) (*  y screen-pitch)))

;   (let rune-ww rune-spacing)
  
;   (while (lt iy rune-h) (do
;     (let ix 0)
;     (let dx 0)
;     (while (lt ix rune-ww) (do
;       (let c (get font (+ so ix)))
;       (let dx (+ do (shl ix 1)))
;       (put fb dx c)
;       (put fb (+ dx 1) c)
;       (let ix (+ ix 1))
;     ))
;     (let so (+ so font-pitch))
;     (let do (+ do screen-pitch))
;     (let iy (+ iy 1))
;   ))
;   0
; )))

; (def blit-char8 (fn rune x y (do
;   (let sx 0)
;   (let sy 0)
;   (let so 0)
;   (let do 0)
;   (let iy 0)
;   (let rune-ww 0)
;   (let c 0)
;   (let d 0)
  
;   (let sx (* rune-w (% rune rune-mod)))
;   (let sy (* rune-h (/ rune rune-mod)))
;   (let so (+ sx (* sy font-pitch)))
;   (let do (+ x (*  y screen-pitch)))

;   (let rune-ww rune-spacing)
  
;   (while (lt iy rune-h) (do
;     (let ix 0)
;     (let dx 0)
;     (while (lt ix rune-ww) (do
;       (let c (get font (+ so ix)))
;       (let dx (+ do ix))
;       (put fb dx c)
;       (let ix (+ ix 1))
;     ))
;     (let so (+ so font-pitch))
;     (let do (+ do screen-pitch))
;     (let iy (+ iy 1))
;   ))
;   0
; )))

; (if (eq screen-bpp 1) (def blit-char blit-char8) 0)

; (def grab-from fb)
; (def grab-pitch screen-pitch)
; (def grab (fn x y w h (do
;   (let xx 0)
;   (let yy 0)
;   (let di 0)
;   (let yy (+ y 0))
;   (let xw (+ x w))
;   (let yh (+ y h)) 
;   (let res (alloc (* (shl w 1) h)))
;   (let from grab-from)
;   (let pitch grab-pitch)
;   (while (lt yy yh) (do
;     (let xx (+ x 0))
;     (while (lt xx xw) (do
;       (put res di (get from (+ xx (* pitch yy))))
;       (let di (+ di 1))
;       (put res di (get from (+ (+ xx (* pitch yy)) 1)))
;       (let di (+ di 1))
;       (let xx (+ xx 1))
;     ))
;     (let yy (+ yy 1))
;   ))
;   res
; )))
(def boxfill (fn (surf surface) x y w h color (do
  (if (lt x 0) (let x 0) 0)
  (if (lt y 0) (let y 0) 0)
  
  (let dw (sget surf width))
  (let dh (sget surf height))
  (if (gt (+ x w) (- dw 1)) (let w (- dw x)) 0)
  (if (gt (+ y h) (- dh 1)) (let h (- dh y)) 0)
  
  (let ofs 0)
  (let xi 0)
  (let yi 0)
  (let xi (+ x 0)) ; TODO get rid of this
  (let yi (+ y 0))
  (let xx (+ x w))
  (let yy (+ y h))

  (let ww (shl w 1))
  (let pitch (sget surf pitch))
  (let ofs (+ (* y pitch) (shl x 1)))
  (let pixels (sget surf pixels))

  (while (lt yi yy) (do
    (let xi (+ x 0))
    (while (lt xi xx) (do
      (put16 pixels ofs color)
      (let xi (+ xi 1))
      (let ofs (+ ofs 2))
    ))
    (let ofs (- (+ ofs pitch) ww))
    (let yi (+ yi 1))
  ))
  0 ; FIXME crashes x64 if this is not here
)))

(def blit (fn (dest surface) (from surface) (do
  (let xx 0)
  (let yy 0)
  (let di 0)
  (let si 0)
  (let yy (+ (sget from y) 0))
  (let xw (+ (sget from x) (sget from width)))
  (let yh (+ (sget from y) (sget from height)))
  (let to (sget dest pixels))
  (let shift (sget dest shift))
  (let w (sget from width))
  (let c 0)
  (let pixels (sget from pixels))

  ;; clipping
  (let x (sget from x))
  (let xskip 0)
  (if (lt yy 0) (do
    (let si (+ si (* (abs yy) (shl w 1))))
    (let yy 0)
  ) 0)
  (if (gt yh (- (sget dest height) 1)) (do
    (let yh (- (sget dest height) 1))
  ) 0)
  (if (lt x 0) (do
    (let xskip (abs x))
    (let w (- w xskip))
    (let x (+ x xskip))
  ) 0)
  (if (gt xw (- (sget dest width) 1)) (do
    (let xskip (- xw (sget dest width)))
    (let si (- si (shl xskip 1)))
    (let xw (- xw xskip))
    (let w (- w xskip))
  ) 0)
  
  (let pitch (- (sget dest pitch) (shl w shift)))
  (let di (+ (shl x shift) (* yy (sget dest pitch))))
  
  (while (lt yy yh) (do
    (let xx (+ x 0))
    (let si (+ si (shl xskip 1)))
    
    (while (lt xx xw) (do
      (let c (get16 pixels si))
      (put16 to di c)
      (let si (+ si 2))
      (let di (+ di 2))
      (let xx (+ xx 1))
    ))
    (let yy (+ yy 1))
    (let di (+ di pitch))
  ))
  1
)))

(def blit-str (fn (surf surface) (font font) str x y (do
  (let i 0)
  (let xx 0)
  (let yy 0)
  (let xx (+ x 0))
  (let yy (+ y 0))
  (let sz (+ (size str) 0))
  (let c 0)

  (let maxx (sget surf width))
  (let maxy (sget surf height))
  (let rune-spacing (sget font spacing))
  (let rune-h (sget font rune-h))
  
  (while (lt i sz) (do
    (let c (get8 str i))
    (blit-char16 surf font c xx yy)
    (let xx (+ xx rune-spacing))
    ; newline
    (if (or (eq c 10) (gt xx maxx)) (do
      (let xx x)
      (let yy (+ yy rune-h))
      (if (gt yy maxy) (do
        (let yy y)) 0)
    ) 0)
    (let i (+ i 1))
    (if (get8 str i) 0 (let i sz)) ; stop at 0
  ))
  yy
)))


(def triangle (fn (surf surface) a b c color (do
  (line surf a b color)
  (line surf b c color)
  (line surf a c color)
)))

(def box (fn (surf surface) x y x2 y2 color (do
  (let tl (list x y))
  (let tr (list x2 y))
  (let bl (list x y2))
  (let br (list x2 y2))

  (line surf tl tr color)
  (line surf bl br color)
  (line surf tr br color)
  (line surf tl bl color)
  0
)))

(def circle (fn (surf surface) cx cy r color (do
  (let x 0)
  (while (lt x 359) (do
    (set-pixel surf (+ cx (* (sin x) r)) (+ cy (* (cos x) r)) color)
    (let x (+ x 1))
  ))
  x
)))

(def clear (fn (boxfill fb 0 0 (- (sget fb width) 1) (- (sget fb height) 1) 0xffff)))

)
