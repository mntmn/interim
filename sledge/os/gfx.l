(
(def fb (mmap "/framebuffer"))
(def scr (open "/framebuffer"))
(def screen-pitch (* screen-width screen-bpp)) ; // TODO read from framebuffer

(def stroke-color 0x0000)

(if (gt screen-bpp 1)
; 16 bit
(def set-pixel (fn x y c (do
  (let ofs (+ (* y screen-pitch) (shl x 1)))
  (put fb ofs (shr c 8))
  (put fb (+ 1 ofs) c)
  c
)))
; 8 bit
(def set-pixel (fn x y c (do
  (let ofs (+ (* y screen-pitch) x))
  (put fb ofs c)
  c
)))
)

(def pt list)

(def line (fn a b (do
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
    (set-pixel xa ya stroke-color)
    (let e2 err)
    (if (gt e2 (- 0 dx)) (do (let err (- err dy)) (let xa (+ xa sx))) 0)
    (if (lt e2       dy) (do (let err (+ err dx)) (let ya (+ ya sy))) 0)
  ))
  0
)))

(def f (open "/sd/unifont.bin"))
(def unifont (recv f))
(def unifont-pitch 4096)

(def font unifont)
(def font-pitch unifont-pitch)

(def rune-w 16)
(def rune-spacing 8)
(def rune-h 16)
(def rune-mod 256)

(def set-unifont (fn (do
  (def font unifont)
  (def font-pitch unifont-pitch)
  (def rune-w 16)
  (def rune-spacing 8)
  (def rune-h 16)
  (def rune-mod 256)
)))

(def blit-char16 (fn rune x y (do
  (let sx 0)
  (let sy 0)
  (let so 0)
  (let do 0)
  (let iy 0)
  (let rune-ww 0)
  (let c 0)
  (let d 0)
  
  (let sx (* rune-w (% rune rune-mod)))
  (let sy (* rune-h (/ rune rune-mod)))
  (let so (+ (* sx 2) (* sy font-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))

  (let rune-ww (+ rune-spacing rune-spacing))
  
  (while (lt iy rune-h) (do
    (let ix 0)
    (while (lt ix rune-ww) (do
      (let c (get font (+ so ix)))
      (let d (get font (+ 1 (+ so ix))))
      (put fb (+ do ix) c)
      (put fb (+ (+ do ix) 1) d)
      (let ix (+ ix 2))
    ))
    (let so (+ so font-pitch))
    (let do (+ do screen-pitch))
    (let iy (+ iy 1))
  ))
  0
)))

(def blit-char (fn rune x y (do
  (let sx 0)
  (let sy 0)
  (let so 0)
  (let do 0)
  (let iy 0)
  (let rune-ww 0)
  (let c 0)
  (let d 0)
  
  (let sx (* rune-w (% rune rune-mod)))
  (let sy (* rune-h (/ rune rune-mod)))
  (let so (+ sx (* sy font-pitch)))
  (let do (+ (*  x 2) (*  y screen-pitch)))

  (let rune-ww rune-spacing)
  
  (while (lt iy rune-h) (do
    (let ix 0)
    (let dx 0)
    (while (lt ix rune-ww) (do
      (let c (get font (+ so ix)))
      (let dx (+ do (shl ix 1)))
      (put fb dx c)
      (put fb (+ dx 1) c)
      (let ix (+ ix 1))
    ))
    (let so (+ so font-pitch))
    (let do (+ do screen-pitch))
    (let iy (+ iy 1))
  ))
  0
)))

(def blit-char8 (fn rune x y (do
  (let sx 0)
  (let sy 0)
  (let so 0)
  (let do 0)
  (let iy 0)
  (let rune-ww 0)
  (let c 0)
  (let d 0)
  
  (let sx (* rune-w (% rune rune-mod)))
  (let sy (* rune-h (/ rune rune-mod)))
  (let so (+ sx (* sy font-pitch)))
  (let do (+ x (*  y screen-pitch)))

  (let rune-ww rune-spacing)
  
  (while (lt iy rune-h) (do
    (let ix 0)
    (let dx 0)
    (while (lt ix rune-ww) (do
      (let c (get font (+ so ix)))
      (let dx (+ do ix))
      (put fb dx c)
      (let ix (+ ix 1))
    ))
    (let so (+ so font-pitch))
    (let do (+ do screen-pitch))
    (let iy (+ iy 1))
  ))
  0
)))

(if (eq screen-bpp 1) (def blit-char blit-char8) 0)

(def grab-from fb)
(def grab-pitch screen-pitch)
(def grab (fn x y w h (do
  (let xx 0)
  (let yy 0)
  (let di 0)
  (let yy (+ y 0))
  (let xw (+ x w))
  (let yh (+ y h)) 
  (let res (alloc (* (shl w 1) h)))
  (let from grab-from)
  (let pitch grab-pitch)
  (while (lt yy yh) (do
    (let xx (+ x 0))
    (while (lt xx xw) (do
      (put res di (get from (+ xx (* pitch yy))))
      (let di (+ di 1))
      (put res di (get from (+ (+ xx (* pitch yy)) 1)))
      (let di (+ di 1))
      (let xx (+ xx 1))
    ))
    (let yy (+ yy 1))
  ))
  res
)))

(def paste (fn from x y w h (do
  (let xx 0)
  (let yy 0)
  (let di 0)
  (let si 0)
  (let yy (+ y 0))
  (let xw (+ x w))
  (let yh (+ y h))
  (let to grab-from)
  (let pitch (+ grab-pitch 0))
  (while (lt yy yh) (do
    (let xx (+ x 0))
    (while (lt xx xw) (do
      (let di (+ xx (* pitch yy)))
      (put to di (get from si))
      (put to (+ di 1) (get from (+ si 1)))
      (let si (+ si 2))
      (let di (+ di 2))
      (let xx (+ xx 1))
    ))
    (let yy (+ yy 1))
  ))
  1
)))

(def blit-str (fn str x y maxx maxy (do
  (let i 0)
  (let xx 0)
  (let yy 0)
  (let xx (+ x 0))
  (let yy (+ y 0))
  (let sz (+ (size str) 0))
  (let c 0)
  (while (lt i sz) (do
    (let c (get str i))
    (blit-char c xx yy)
    (let xx (+ xx rune-spacing))
    ; newline
    (if (or (eq c 10) (gt xx maxx)) (do
      (let xx x)
      (let yy (+ yy rune-h))
      (if (gt yy maxy) (do
        (let yy y)) 0)
    ) 0)
    (let i (+ i 1))
    (if (get str i) 0 (let i sz)) ; stop at 0
  ))
  yy
)))

(def boxfill (fn x y w h color (do
  (let ofs 0)
  (let xi 0)
  (let yi 0)
  (let xi (+ x 0))
  (let yi (+ y 0))
  (let xx (+ x w))
  (let yy (+ y h))
  (let chi 0)
  (let clo 0)
  (let chi (shr color 8))
  (let clo (bitand color 0xff))
  (let ofs (+ (* y screen-pitch) (shl x 1)))
  (let ww (shl w 1))

  (while (lt yi yy) (do
    (let xi (+ x 0))
    (while (lt xi xx) (do
      (put fb ofs chi)
      (put fb (+ 1 ofs) clo)
      (let xi (+ xi 1))
      (let ofs (+ ofs 2))
    ))
    (let ofs (- (+ ofs screen-pitch) ww))
    (let yi (+ yi 1))
  ))
  0 ; crashes x64 if this is not here
)))

(def triangle (fn a b c (do
  (line a b)
  (line b c)
  (line a c)
)))

(def box (fn x y x2 y2 color (do
  (let tl (list x y))
  (let tr (list x2 y))
  (let bl (list x y2))
  (let br (list x2 y2))

  (def stroke-color color)
  (line tl tr)
  (line bl br)
  (line tr br)
  (line tl bl)
  0
)))

(def circle (fn cx cy r (do
  (let x 0)
  (while (lt x 359) (do
    (set-pixel (+ cx (* (sin x) r)) (+ cy (* (cos x) r)) stroke-color)
    (let x (+ x 1))
  ))
  x
)))

(def clear (fn (boxfill 0 0 (- screen-width 1) (- screen-height 1) 0xffff)))

)
