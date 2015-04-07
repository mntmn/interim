(
(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (if a 1 (if b 1 0) 0)))

(def buf "")

(def item (fn lst idx (do
  (def i 0)
  (while (gt idx i) (do
    (def lst (cdr lst))
    (def i (+ i 1))))
  (car lst)
)))

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
  pos
)))

(def ufind-next (fn buf rune pos (do
  (def sz (usize buf))
  (while (and (lt pos sz) (not (= rune (uget buf pos))))
    (def pos (+ pos 1)))
  pos
)))

(def ufind-prev-ws (fn buf pos (do
  (while (and (gt pos 0) (not (or (= 10 (uget buf pos)) (= 32 (uget buf pos)))))
    (def pos (- pos 1)))
  (if (= pos 0) 0 pos)
)))

(def ufind-next-ws (fn buf pos (do
  (while (and (lt pos (usize buf)) (not (or (= 10 (uget buf pos)) (= 32 (uget buf pos)))))
    (def pos (+ pos 1)))
  pos
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


(def reverse (fn lst (do
  (def res (list))
  (while (or (car lst) (cdr lst)) (do
    (def res (cons (car lst) res))
    (def lst (cdr lst))
  ))
  res
)))

(def replace-item (fn lst pos new-value ((fn res i (do
  (while (or (car lst) (cdr lst)) (do
    (if (= pos i)
      (def res (cons new-value res))
      (def res (cons (car lst) res)))
    (def lst (cdr lst))
    (def i (+ i 1))
  ))
  (reverse res)
  ))
  
  (list) 0)
))

(def buffers (list
  (list (alloc-str 1024) 0   0 800 800 -1 "main")
  (list (alloc-str 1024) 0 900 800 100  0 "minibuf")
))

(def buf-str (fn id (item (item buffers id) 0)))
(def buf-x (fn id (item (item buffers id) 1)))
(def buf-y (fn id (item (item buffers id) 2)))
(def buf-w (fn id (item (item buffers id) 3)))
(def buf-h (fn id (item (item buffers id) 4)))
(def buf-cursor (fn id (item (item buffers id) 5)))
(def buf-name (fn id (item (item buffers id) 6)))

(def set-buf-str (fn id new-str (def buffers (replace-item buffers id (replace-item (item buffers id) 0 new-str)))))
(def set-buf-x (fn id x (def buffers (replace-item buffers id (replace-item (item buffers id) 1 x)))))
(def set-buf-y (fn id y (def buffers (replace-item buffers id (replace-item (item buffers id) 2 y)))))
(def set-buf-w (fn id w (def buffers (replace-item buffers id (replace-item (item buffers id) 3 w)))))
(def set-buf-h (fn id h (def buffers (replace-item buffers id (replace-item (item buffers id) 4 h)))))
(def set-buf-cursor (fn id new-cursor (def buffers (replace-item buffers id (replace-item (item buffers id) 5 new-cursor)))))

(def padding 20)

(def scroll-line (fn buf-id (do
  (def break-at (+ 1 (ufind-next str 0xa 0)))
  (set-buf-str id (substr str break-at (usize str)))
)))

(def buf-render (fn buf-id (do
  (def rendered-upto
    (blit-string unifont
                 (buf-str buf-id)
                 (buf-cursor buf-id)
                 (+ padding (buf-x buf-id)) (+ padding (buf-y buf-id)) (buf-w buf-id) (buf-h buf-id) 0xffffff))
  (def str (buf-str buf-id))
  (def clipped (- (usize str) rendered-upto))
  (if (gt clipped 0) (scroll-line buf-id))
)))

(def buf-append (fn id str (do
  (set-buf-str id (concat (buf-str id) (concat str [0a])))
  (set-buf-cursor id (usize (buf-str id)))
)))

(def clear (fn (do
  (set-buf-str 0 "")
)))

(def word-at (fn buf pos (do
  (def from (ufind-prev-ws buf pos))
  (def to   (ufind-next-ws buf pos))
  (substr buf from (- to from))
)))

(def plumb (fn word (do
  (write buf (cons "plumb" word))
  (if (= (uget word 0) (uchr "/"))
    (def buf (load word)))
)))

(def backspace (fn bufid (if (gt (buf-cursor bufid) 0) (do
  (def cursor (buf-cursor bufid))
  (uremove (buf-str bufid) cursor)
  (set-buf-cursor bufid (- cursor 1))
))))

(def cursor-left (fn bufid (do
  (def cursor (buf-cursor bufid))
  (set-buf-cursor bufid (- cursor 1))
)))

(def cursor-right (fn bufid (do
  (def cursor (buf-cursor bufid))
  (set-buf-cursor bufid (+ cursor 1))
)))

(def cursor-up (fn bufid (do
  (def cursor (ufind-prev buf 13 (- (buf-cursor bufid) 1)))
  (set-buf-cursor bufid cursor)
)))

(def cursor-down (fn bufid (do
  (def cursor (ufind-next buf 13 (+ (buf-cursor bufid) 1)))
  (set-buf-cursor bufid cursor)
)))

(def eval-tmp-buf (alloc-str 1024))

(def handle-command-key (fn k modif (do
  (if (or (= k 13) (= k 10)) (do
    (print (cons "evaling:" (buf-str 1)))
    (def eval-tmp (eval (buf-str 1)))
    (print (cons "eval-tmp:" eval-tmp))
    (write eval-tmp-buf eval-tmp)
    (buf-append 0 eval-tmp-buf)
    0
  ))

  (if (= k 12) (do
    (plumb (word-at buf cursor))
  ))

  (if (= k 127) (backspace 1))
  (if (= k 130) (cursor-left))
  (if (= k 132) (cursor-up))
  (if (= k 133) (cursor-down))
  (if (= k 131) (cursor-right))
  
  0
)))

(def handle-editor-key (fn k modif (do
  (if (and (gt k 13) (lt k 127))
    (do
      (def cursor (buf-cursor 1))
      (uinsert (buf-str 1) cursor k)
      (set-buf-cursor 1 (+ cursor 1))
    )
    (handle-command-key k modif)
  )
  0
)))

(def buffer-ids (fn (do
  (def i -1)
  (map (fn b (def i (+ i 1))) buffers)
)))

(def render-all (fn (do
  (map buf-render (buffer-ids))
)))

(def main (fn (do
  (flip)
  (print "buf-append")
  (print (list "buffers: " buffers))
  (buf-append 0 "welcome to bomber jacket OS (arm7/32bit) ☕ ☕ ☕ ")
  (render-all)
  (print "entering while")
  (while 1 (do
  
    (def k (inkey 0))
    (def modif (inkey 1))

    (if k (do 
      (if keymap (if modif (def k (get keymap (+ 128 k))) (def k (get keymap k))))
    ))
    
    (if (not (= k 0))
      (handle-editor-key k modif))
    
    (gc)
    (flip)
    (render-all)
  ))
)))

(main)
)
