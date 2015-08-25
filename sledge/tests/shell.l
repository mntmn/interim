(
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (+ a b)))
(def not (fn a (if a 0 1)))
(def eq (fn a b (lt (+ (lt a b) (gt a b)) 1)))

(def item (fn lst idx (do
  (let i 0)
  (let l lst)
  (while (gt idx i) (do
    (let l (cdr l))
    (let i (+ i 1))))
  (car l)
)))

(def sintab [80828486888b8d8f919496989a9c9ea1a3a5a7a9abadafb2b4b6b8babcbec0c1c3c5c7c9cbcdced0d2d3d5d7d8dadcdddfe0e2e3e4e6e7e8eaebecedeeeff1f2f3f4f4f5f6f7f8f9f9fafbfbfcfcfdfdfefefefffffffffffffffffffffffffffffffefefefdfdfcfcfbfbfaf9f9f8f7f6f5f4f4f3f2f1efeeedecebeae8e7e6e4e3e2e0dfdddcdad8d7d5d3d2d0cecdcbc9c7c5c3c1c0bebcbab8b6b4b2afadaba9a7a5a3a19e9c9a989694918f8d8b88868482807d7b79777472706e6b69676563615e5c5a58565452504d4b49474543413f3e3c3a38363432312f2d2c2a2827252322201f1d1c1b1918171514131211100e0d0c0b0b0a0908070606050404030302020101010000000000000000000000000000000101010202030304040506060708090a0b0b0c0d0e1011121314151718191b1c1d1f2022232527282a2c2d2f31323436383a3c3e3f41434547494b4d50525456585a5c5e61636567696b6e70727477797b7d])

(def sin (fn deg (get sintab (% deg 360))))
(def cos (fn deg (get sintab (% (+ deg 90) 360))))
(def abs (fn a (if (lt a 0) (- 0 a) a)))

(def load (fn path (recv (open path))))
(def ls (fn (load "/sd/")))
(def run (fn path (eval (read (recv (open path))))))

(def scr (open "/framebuffer"))
(def fb (mmap "/framebuffer"))
(def screen-pitch  3840) ; // TODO read from framebuffer

(def stroke-color 0x0000)

(def set-pixel (fn x y c (do
  (let ofs (+ (* y screen-pitch) (shl x 1)))
  (put fb ofs (shr c 8))
  (put fb (+ 1 ofs) c)
  c
)))

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

(def ox 824)
(def oy 550)
(def stroke-color 0xdddd)
(line (pt (+ ox 16) (- oy 38)) (pt (+ ox 16) (- oy 102)))
(line (pt (+ ox 16) (- oy 102)) (pt (+ ox 80) (- oy 38)))
(line (pt (+ ox 80) (- oy 38)) (pt (+ ox 80) (- oy 102)))
(line (pt (+ ox 80) (- oy 102)) (pt (+ ox 144) (- oy 38)))
(line (pt (+ ox 144) (- oy 38)) (pt (+ ox 144) (- oy 102)))
(line (pt (+ ox 144) (- oy 102)) (pt (+ ox 208) (- oy 38)))
(line (pt (+ ox 208) (- oy 38)) (pt (+ ox 208) (- oy 102)))
(line (pt (+ ox 208) (- oy 102)) (pt (+ ox 272) (- oy 102)))

(def f (open "/sd/unifont.565"))
(def unifont (recv f))
(def unifont-pitch 8192)

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

(def fghi 0xff)
(def fglo 0x00)

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

; 112 x 30 chars at scale 2

(def scale 2)
(def maxx (/ 1847 scale))
(def maxy (/ 1015 scale))
(def minx 32)
(def miny 32)

(def blit-str (fn str x y (do
  (let i 0)
  (let xx 0)
  (let yy 0)
  (let xx (+ x 0))
  (let yy (+ y 0))
  (let sz (+ (size str) 0))
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
)))

(blit-str "Welcome to Interim OS." 32 32)

(def evbuf (alloc-str 512))
(def p (fn xp x y (do
  (write xp evbuf)
  (blit-str evbuf x y)
)))

(def keyboard (open "/keyboard"))

(def strlen (fn s (if s (do
  (let i 0)
  (let sz (size s))
  (let c (get s 0))
  (while (* (gt c 0) (lt i sz)) (do
    (let i (+ i 1))
    (let c (get s i))
  ))
  i) 0)
))

(def term-x minx)
(def term-y (+ miny 32))

(def history (list))
(def future (list))

(def buffer "")

(def history-back (fn (do
  (def buffer (car history))

  (def future (cons (car history) future))
  (def history (cdr history))
  (print (list "history:" history "future:" future))

  (def term-x (+ minx (* rune-spacing (strlen buffer))))
  (blit-str buffer minx term-y)
)))

(def history-forth (fn (do
  (def buffer (car future))
  
  (def history (cons (car future) history))
  (def future (cdr future))
  (print (list "history:" history "future:" future))
  
  (def term-x (+ minx (* rune-spacing (strlen buffer))))
  (blit-str buffer minx term-y)
)))

(def tasks (list))

(def test (fn (do
  (print "hello from test")
  (p history 0 0)
)))

(def add-task (fn t (do
  (def tasks (cons t tasks))
)))

(def task-func (fn (print "hello from placeholder")))

(def run-tasks (fn (do
  (let tl tasks)
  (while (car tl) (do
    (def task-func (car tl))
    (print (list "run-task " task-func))
    (task-func)
    (let tl (cdr tl))
  ))
)))

(def triangle (fn a b c (do
  (line a b)
  (line b c)
  (line a c)
)))

(def box (fn tl br (do
  (let tr (list (car br) (car (cdr tl))))
  (let bl (list (car tl) (car (cdr br))))
  
  (line tl tr)
  (line bl br)
  (line tr br)
  (line tl bl)
)))

(def circle (fn cx cy r (do
  (let x 0)
  (while (lt x 359) (do
    (set-pixel (+ cx (* (sin x) r)) (+ cy (* (cos x) r)) stroke-color)
    (let x (+ x 1))
  ))
  x
)))

(def graph (fn center max (do
  (let x 0)
  (while (lt x max) (do
    (print x)
    (set-pixel x (+ center (sin (sin x))) stroke-color)
    (let x (+ x 1))
  ))
  x
)))

(def ed (fn (run "/sd/tests/editlite.l") ))

(def main (fn (do
  (let blink 0)
  (let running 1)

  (while running (do
    (let str (recv keyboard))
    (let c (get str 0))

    (if (gt c 0) (print c) 0)

    ; FIXME this aint working
    (if (* (gt c 0x1f) (not (eq 0x7f c))) (do
      (def term-y (blit-str str term-x term-y))
      (def buffer (concat buffer str))
      (def term-x (+ term-x rune-spacing)) ) 0)
      
    (if (eq c 9) ; tab
      (do
        (blit-char 32 term-x term-y)
        (def term-y (+ term-y 16))
        (def term-x 32) (def buffer "")) 0)
      
    (if (eq c 10) ; return
      (do
        (blit-char 32 term-x term-y)
        (def history (cons buffer history))
        (print history)
        
        (let r (eval (list (read buffer))))
        
        (def buffer "")
        (def term-x minx) (def term-y (+ term-y rune-w))
        (def term-y (+ rune-h (p r term-x term-y)))
      ) 0)

    (if (eq c 17) ; cursor up
      (history-back) 0)
        
    (if (eq c 18) ; cursor down
      (history-forth) 0)

    (if (eq c 0x7f) ; bksp
      (if (gt (strlen buffer) 0)
      (do
        (blit-char 32 term-x term-y)
        (def term-x (- term-x rune-spacing))
        (print buffer)
        (let nl (- (strlen buffer) 1))
        (def buffer (substr buffer 0 nl)) ) 0) 0)

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

(main)
)
