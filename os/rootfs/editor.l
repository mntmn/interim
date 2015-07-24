(
(def buffers (list
  (list (alloc-str 1024) "main")
  (list (alloc-str 1024) "mini")
  (list (alloc-str 1024) "scratch")
))

; buf-id    x   y   w   h  cur type (0=text,1=cmd,2=image)

(def views (list
  (list 0   0     0 800 800   0  0)
  (list 1   0   900 800 100   0  1)
))

(def append-mode 1)
(def focused-view-id 1)

(def buf-str (fn id (item (item buffers id) 0)))
(def set-buf-str (fn id new-str (def buffers (replace-item buffers id (replace-item (item buffers id) 0 new-str)))))

(def view-buf-id (fn id (item (item views id) 0)))
(def view-x (fn id (item (item views id) 1)))
(def view-y (fn id (item (item views id) 2)))
(def view-w (fn id (item (item views id) 3)))
(def view-h (fn id (item (item views id) 4)))
(def view-cursor (fn id (item (item views id) 5)))
(def view-type (fn id (item (item views id) 6)))

(def set-view-buf-id (fn id buf-id (def views (replace-item views id (replace-item (item views id) 0 buf-id)))))
(def set-view-x (fn id x (def views (replace-item views id (replace-item (item views id) 1 x)))))
(def set-view-y (fn id y (def views (replace-item views id (replace-item (item views id) 2 y)))))
(def set-view-w (fn id w (def views (replace-item views id (replace-item (item views id) 3 w)))))
(def set-view-h (fn id h (def views (replace-item views id (replace-item (item views id) 4 h)))))
(def set-view-cursor (fn id new-cursor (def views (replace-item views id (replace-item (item views id) 5 new-cursor)))))
(def set-view-type (fn id new-type (def views (replace-item views id (replace-item (item views id) 6 new-type)))))

(def padding 20)

(def new-buffer (fn (do
  (def buf (list (alloc-str 1024) "untitled"))
  (def buffers (reverse (cons buf buffers)))
  buf
)))

(def new-view (fn buf-id x y w h type (do
  (def view (list buf-id x y w h 0 type))
  (def views (reverse (cons view views)))
  view
)))

(def uinsert (fn buf-id pos kchr (do
  (set-buf-str buf-id (concat (buf-str buf-id) " "))
  (def buf (buf-str buf-id))
  (ucopy buf pos (+ pos 1) (- (usize buf) (+ pos 0)))
  (uput buf pos kchr)
  0
)))

(def scroll-line (fn buf-id (do
  (def break-at (+ 1 (ufind-next str 0xa 0)))
  (set-buf-str id (substr str break-at (usize str)))
)))

(def text-view-render (fn view-id (do
  (def buf-id (view-buf-id view-id))
  (def rendered-upto
    (blit-string unifont
                 (buf-str buf-id)
                 (if (= focused-view-id view-id) (view-cursor view-id) -1)
                 (+ padding (view-x view-id)) (+ padding (view-y view-id)) (view-w view-id) (view-h view-id) 0xffffff))
  (def str (buf-str buf-id))
  (def clipped (- (usize str) rendered-upto))
  (if (gt clipped 0) (scroll-line buf-id))
)))

(def image-view-render2 (fn view-id (do
  (def buf-id (view-buf-id view-id))
  (blit (buf-str buf-id) (+ padding (view-x view-id)) (+ padding (view-y view-id)) (view-w view-id) (view-h view-id))
)))

(def pixel-size 64)
(def tbuf "")

(def image-view-render (fn view-id (do
  (def buf-id (view-buf-id view-id))
  (def pixels (buf-str buf-id))
  (def vw (view-w view-id))
  (def rx 0)
  (def ry 0)
  (def vx (+ padding (view-x view-id)))
  (def vy (+ padding (view-y view-id)))
  (def cursor (view-cursor view-id)) ; FIXME clobbers i
  (def i 0)

  (while (lt i (size pixels)) (do
    (def color (+ (+ (* (get pixels (+ i 2)) 65536) (* (get pixels (+ i 1)) 256)) (get pixels i)))
    (rectfill (+ rx vx) (+ ry vy) pixel-size pixel-size color)
    (if (= cursor i) (do
      (rectfill (+ rx vx) (+ (* 3 (/ pixel-size 4)) (+ ry vy)) pixel-size (/ pixel-size 4) 0xffffff)
      (rectfill (+ rx vx) (+ (* 3 (/ pixel-size 4)) (+ ry vy)) pixel-size (/ pixel-size 5) 0x000000)
    ))
      
    ;(write tbuf (cons "i" i))
    ;(blit-string unifont tbuf -1 (+ rx vx) (+ ry vy) 100 16 0xffffff)
    
    (def rx (+ rx pixel-size))
    
    (if (gt rx vw) (do
      (def rx 0)
      (def ry (+ ry pixel-size))))
    (def i (+ i 3))
  ))
  0
)))

(def view-render (fn view-id (do
  (def t (view-type view-id))
  (if (= t 2) (image-view-render view-id))
  (if (= t 3) (image-view-render2 view-id))
  (if (lt t 2) (text-view-render view-id))
)))

(def buf-append (fn id str (do
  (set-buf-str id (concat (buf-str id) (concat str [0a])))
)))

(def word-at (fn buf pos (do
  (def from (ufind-prev-ws buf pos))
  (def to   (ufind-next-ws buf pos))
  (substr buf from (- to from))
)))

(def backspace (fn vid (if (gt (view-cursor vid) 0) (do
  (def cursor (view-cursor vid))
  (uremove (buf-str (view-buf-id vid)) cursor)
  (set-view-cursor vid (- cursor 1))
))))

(def cursor-left (fn vid (do
  (def bufid (view-buf-id vid))
  (def cursor (view-cursor bufid))
  (set-view-cursor vid (- cursor 1))
)))

(def cursor-right (fn vid (do
  (def bufid (view-buf-id vid))
  (def cursor (view-cursor bufid))
  (set-view-cursor vid (+ cursor 1))
)))

(def cursor-up (fn vid (do
  (def bufid (view-buf-id vid))
  (def cursor (ufind-prev (buf-str bufid) 10 (- (view-cursor vid) 1)))
  (set-view-cursor vid cursor)
)))

(def cursor-down (fn vid (do
  (def bufid (view-buf-id vid))
  (def cursor (ufind-next (buf-str bufid) 10 (+ (view-cursor vid) 1)))
  (set-view-cursor vid cursor)
)))

(def eval-tmp-buf (alloc-str 1024))

(def palette (quote (
  [000000]
  [ffffff]
  [68372b]
  [70a4b2]
  [6f3d86]
  [588d43]
  [352879]
  [b8c76f]
  [6f4f25]
  [433900]
  [9a6759]
  [444444]
  [6c6c6c]
  [9ad284]
  [6c5eb5]
  [959595]
)))

(def insert-char (fn k (do
  (def cursor (view-cursor focused-view-id))
  (def buf-id (view-buf-id focused-view-id))
  (def type (view-type focused-view-id))
  (print (cons "type" type))
  (if (= type 2) (do
      (print (cons "k" k))
      (def rgb (item palette (- k 48)))
      (put (buf-str buf-id) cursor (get rgb 0))
      (put (buf-str buf-id) (+ cursor 1) (get rgb 1))
      (put (buf-str buf-id) (+ cursor 2) (get rgb 2))
      (set-view-cursor focused-view-id (+ cursor 3))
    )
    (do
      (uinsert buf-id cursor k)
      (set-view-cursor focused-view-id (+ cursor 1))))
)))

(def focus-next-view (fn (do
  (def focused-view-id (% (+ focused-view-id 1) (length views)))
  (set-view-cursor focused-view-id (usize (buf-str (view-buf-id focused-view-id))))
  (print (list "focused view:" focused-view-id (view-cursor focused-view-id)))
)))

; command handler ----------------------------------------------------------

(def handle-command-key (fn k modif (do
  (print (list "key" k "modif" modif))

  (if (or (= k 13) (= k 10))
    (if (= (view-type focused-view-id) 1)
      (do
        (def buf-id (view-buf-id focused-view-id))
        (print (cons "evaling:" (buf-str buf-id)))
        (def eval-tmp (eval (buf-str buf-id)))
        (print (cons "eval-tmp:" eval-tmp))
        (if append-mode (do
          (write eval-tmp-buf eval-tmp)
          (buf-append 0 eval-tmp-buf)))
        0)
      (insert-char k))
  )

  (if (or (= k 9) (= k 134))
    (if modif
      0
  ;    (plumb (word-at (buf-str (view-buf-id focused-view-id)) (view-cursor focused-view-id)))
      (focus-next-view)))

  (def j 2)
  (if (= (view-type focused-view-id) 2) (def j 4))

  (while (def j (- j 1)) (do
    (if (= k 127) (backspace focused-view-id))
    (if (= k 130) (cursor-left focused-view-id))
    (if (= k 132) (cursor-up focused-view-id))
    (if (= k 133) (cursor-down focused-view-id))
    (if (= k 131) (cursor-right focused-view-id))
    (print (cons "cursor" (view-cursor focused-view-id)))
  ))
  
  0
)))

(def handle-editor-key (fn k modif (do
  (if (and (not (= k 27)) (and (gt k 13) (lt k 127)))
    (insert-char k)
    (handle-command-key k modif)
  )
  0
)))

(def list-ids (fn lst (do
  (def i -1)
  (map (fn b (def i (+ i 1))) lst)
)))

(def render-all (fn (do
  (map view-render (list-ids views))
)))

(def gfx (fn (do
  (def append-mode 0)
  (set-buf-str 0 (alloc (* 3 (* 16 16))))
  (set-view-type 0 2)
  (set-view-cursor 0 0)
  (set-view-w 0 512)
  (set-view-h 0 512)
  (def pixel-size 32)
)))

; welcome to bomber jacket OS (arm7/32bit) ☕ ☕ ☕ -------------------------

(def loadimage (fn (do
  (set-view-type 0 3)
  (set-view-w 0 560)
  (def cat (load "grumpy.data"))
  (print "cat loaded!")
  (blit cat 20 20 560 720)
  (flip)
)))

(def main (fn (do
  (flip)
  (print (list "buffers: " buffers "views: " views))
  (print "render-all")
  (render-all)
  (print "entering while")
  (while 1 (do
  
    (def k (inkey 0))
    (def modif (inkey 1))
    
    (if k (do 
      (if (and (lt k 127) keymap) (if modif (def k (get keymap (+ 128 k))) (def k (get keymap k))))
    ))
    
    (if (not (= k 0))
      (handle-editor-key k modif))
    
    (gc)
    (flip)
    (render-all)
    (udp-poll)
  ))
)))

(main)
)
