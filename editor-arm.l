(
(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (if a 1 (if b 1 0) 0)))

(def item (fn lst idx (do
  (def i 0)
  (while (gt idx i) (do
    (def lst (cdr lst))
    (def i (+ i 1))))
  (car lst)
)))

(def buf (alloc-str 1024))

(def uchr (fn s (uget s 0)))

(def substr (fn str beg sz (do
  (def res (alloc-str (* 2 sz)))
  (def ii 0)
  (while (lt ii sz) (do
    (uput res ii (uget str (+ beg ii)))
    (def ii (+ ii 1))
  ))
  res
)))

(def ufind-prev (fn buf rune pos (do
  (while (and (gt pos 0) (not (= rune (uget buf pos))))
    (def pos (- pos 1)))
  (+ pos 0)
)))

(def ufind-next (fn buf rune pos (do
  (while (and (lt pos (usize buf)) (not (= rune (uget buf pos))))
    (def pos (+ pos 1)))
  (+ pos 0)
)))

(def ufind-prev-ws (fn buf pos (do
  (while (and (gt pos 0) (not (or (= 10 (uget buf pos)) (= 32 (uget buf pos)))))
    (def pos (- pos 1)))
  (if (= pos 0) 0 (+ pos 1))
)))

(def ufind-next-ws (fn buf pos (do
  (while (and (lt pos (usize buf)) (not (or (= 10 (uget buf pos)) (= 32 (uget buf pos)))))
    (def pos (+ pos 1)))
  (+ pos 0)
)))

(def ucopy (fn buf from to num (do
  (if (lt from to)
    (do 
      (def i (- num 1)) 
      (while (gt (def i (- i 1)) -1) (do
        (def c (uget buf (+ from i)))
        (uput buf (+ to i) c)
      )))
    (do
      (def i 0)
      (while (lt (def i (+ i 1)) num) (do
        (def c (uget buf (+ from i)))
        (uput buf (+ to i) c)
      )))
  )
)))

(def uremove (fn buf pos (do
  (ucopy buf (- pos 1) (- pos 2) (- (usize buf) (- pos 1)))
  (uput buf (- (usize buf) 1) 0)
  0
)))

(def uinsert (fn buf pos kchr (do
  (ucopy buf pos (+ pos 1) (- (usize buf) (+ pos 1)))
  (uput buf pos kchr)
  0
)))

(def split (fn str sepstr (do
  (def sep (uget sepstr 0))
  (def result (quote ()))
  (def sz (usize str))
  (def i (- sz 1))
  (def last-i (+ i 1))
  (while (gt i -2) (do
    (if (or (= (uget str i) sep) (= i -1)) (do
      (def partsize (- (- last-i i) 1))
  
      (if (gt partsize 0)
        (def result (cons (substr str (+ i 1) partsize) result)) 0)
      (def last-i i)
    ) 0)
    (def i (- i 1))
  ))
  result
)))

(def white 0xffffff)
(def cyan 65535)
(def color white)
(def unifont-width 4128)
(def unifont-height 4160)
(def xx 0)
(def bx 0)
(def by 0)
(def x 0)
(def y 0)
(def i 0)
(def rune 0)
(def chr 0)

(def cursor 0)
(def blit-char-inv (fn rune x y color (do
  (blit-mono-inv unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ 4128 8) 2 16 x y color)
)))

(def blit-char (fn rune x y color
  (blit-mono unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ 4128 8) 2 16 x y 0xffffff)
))

(def blit-str (fn cursor str xx y color (do
  (def l (+ (usize str) 1))
  (mut i -1)
  (mut x (+ 0 xx))
  (while (lt (mut i (+ i 1)) l) (do
    (mut chr (uget str i))
    (if (= i cursor)
      (blit-char-inv chr x y color)
      (if (= chr 32) 0 (blit-char chr x y color)))
    (mut x (+ x 8))
    (if (= chr 10)
      (do
        (mut y (+ y 16))
        (mut x xx)))
    0
  ))
  0
)))

(def running 1)
(def cmd-mode 0)

(def eval-buf "welcome to bomber jacket OS (arm7/32bit) ☕ ☕ ☕    ")

(def help-buf (alloc-str 80))

(def focus-buffer buf)
(def padding 20)

(def buf-render (fn focused b bx by (do
  (rectfill bx by 1024 1 0)
  (blit-str focused b (+ padding bx) (+ padding by) color)
)))

(def clear (fn (do
  (rectfill 0 0 1024 768 color)
)))

(def toggle-command (fn (do
  (if (= color cyan)
    (def color white)
    (def color cyan))
  (def cmd-mode (- 1 cmd-mode))
)))

(def word-at (fn buf pos (do
  (mut from (ufind-prev-ws buf pos))
  (mut to   (ufind-next-ws buf pos))
  (substr buf from (- to from))
)))

(def plumb (fn word (do
  (write eval-buf (cons "plumb" word))
  (if (= (uget word 0) (uchr "/"))
    (def buf (load word)))
)))

(def backspace (fn (if (gt cursor 0) (do
  (uremove buf cursor)
  (mut cursor (- cursor 1))
) 0)))

(def cursor-left (fn (do
  (mut cursor (- cursor 1))
)))

(def cursor-right (fn (do
  (mut cursor (+ cursor 1))
  0
)))

(def cursor-up (fn (do
  (mut cursor (ufind-prev buf 13 (- cursor 1)))
  0
)))

(def cursor-down (fn (do
  (mut cursor (ufind-next buf 13 (+ cursor 1)))
  0
)))

(def handle-command-key (fn k (do
                                  
  (if (= k 11) (do
    (def eval-tmp (eval buf))
    (write eval-buf eval-tmp)
    0
  ))

  (if (= k 12) (do
    (plumb (word-at buf cursor))
  ))

  (if (= k 127) (backspace))
  (if (= k 130) (cursor-left))
  (if (= k 132) (cursor-up))
  (if (= k 133) (cursor-down))
  (if (= k 131) (cursor-right))

  (if (= k 10) (do
                   (uput buf cursor 10)
                   (mut cursor (+ cursor 1))
                   (flip)))
  
  0
)))

(def handle-editor-key (fn k modif (do
  (if (and (gt k 12) (lt k 127))
    (do
      (uput buf cursor k)
      (mut cursor (+ cursor 1))
    )
    (handle-command-key k)
  )
  0
  )))

(def demo (fn (do
  (mut c 0)
  (def a 1024)
  (while (not (inkey 0)) (do
    (mut c (+ 1 c))
    (mut b 1079)
    (while (mut b (- b 1)) (do
      (mut a 1919)
      (mut c (+ 1 c))
      (while (mut a (- a 1))
        (pixel a b (* (- (- a b) c) c)))))
  ))
)))

(def buf-load (fn path (def buf (load path))))

(def eval-buf-x 0)
(def eval-buf-y 600)

(def update-statusline (fn (do
  (def memstat (load "/sys/mem"))
  (def cellusage (/ (* 100 (car (cdr (cdr memstat)))) (car (cdr (cdr (cdr memstat))))))
  (def memusage (/ (* 100 (car memstat)) (car (cdr memstat))))
  (write help-buf (cons (cons cursor (usize buf)) (cons (cons "heap%" memusage) (cons "cell%" cellusage))))
  0
)))

(flip)

(def main (fn
(while running (do
  (buf-render cursor buf 0 0)
  (buf-render 0 eval-buf eval-buf-x eval-buf-y)
  (buf-render 0 help-buf 0 700)
  (update-statusline)

  (mut k (inkey 0))
  (mut modif (inkey 1))

  (handle-editor-key k modif)

  (if (lt cursor 0) (mut cursor 0))
  (if (gt cursor (usize buf)) (mut cursor (usize buf)))

  (gc)
))
))

(main)
)
