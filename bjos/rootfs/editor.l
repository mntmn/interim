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

(def substr_ (fn str beg sz (do
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
  (while (and (gt pos 0) (not (or (= 13 (uget buf pos)) (or (= 10 (uget buf pos)) (= 32 (uget buf pos))))))
    (def pos (- pos 1)))
  (if (= pos 0) 0 pos)
)))

(def ufind-next-ws (fn buf pos (do
  (while (and (lt pos (usize buf)) (not (or (= 13 (uget buf pos)) (or (= 10 (uget buf pos)) (= 32 (uget buf pos))))))
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

(def join (fn lst jstr (do
  (def joined "")
  (while (car lst) (do
    (def tmp (alloc-str 128))
    (write tmp (car lst))
    (def tmp (substr tmp 1 (- (usize tmp) 2)))
    (def joined (concat (concat joined jstr) tmp))
    (print joined)
    (def lst (cdr lst))
  ))
  joined
)))

(def str= (fn str-a str-b (do
  (def si 0)
  (def equal 1)
  (while (and equal (lt si (size str-a))) (do
    (if (not (= (get str-a si) (get str-b si))) (def equal 0)) 
    (def si (+ si 1))
  ))
  equal
)))

(def length (fn lst (do
  (def i 0)
  (while (cdr lst) (do
    (def i (+ i 1))
    (def lst (cdr lst))
  ))
  i
)))

(def reverse (fn lst (do
  (def res (list))
  (while (cdr lst) (do
    (def res (cons (car lst) res))
    (def lst (cdr lst))
  ))
  res
)))

(def replace-item (fn lst pos new-value ((fn res i (do
  (while (cdr lst) (do
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

(def pixel-size 48)

(def image-view-render (fn view-id (do
  (def buf-id (view-buf-id view-id))
  (def str (buf-str buf-id))
  (def pixels (eval str))
  (def vw (view-w view-id))
  (def rx 0)
  (def ry 0)
  (def vx (+ padding (view-x view-id)))
  (def vy (+ padding (view-y view-id)))
  
  (while (cdr pixels) (do
    (def color (car pixels))
    (rectfill (+ rx vx) (+ ry vy) pixel-size pixel-size color)
    (def pixels (cdr pixels))
    (def rx (+ rx pixel-size))
    (if (gt rx vw) (do
      (def rx 0)
      (def ry (+ ry pixel-size))))
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

(def insert-char (fn k (do
  (def cursor (view-cursor focused-view-id))
  (def buf-id (view-buf-id focused-view-id))
  (uinsert buf-id cursor k)
  (set-view-cursor focused-view-id (+ cursor 1))
)))

(def focus-next-view (fn (do
  (def focused-view-id (% (+ focused-view-id 1) (length views)))
  (set-view-cursor focused-view-id (usize (buf-str (view-buf-id focused-view-id))))
  (print (list "focused view:" focused-view-id (view-cursor focused-view-id)))
)))

; -------------------------------------------------------------------


;;; Hosts ----------------------------------------------------------------

(def mntmn [5bfa730f])
(def facebook [1f0d5d03])
(def tokyo [c0a80168])
(def freenode [c0ba9d2b])
(def dwigns [5eba9793])

;;; IRC ------------------------------------------------------------------

(def irc-connect-handler (fn (do
  (buf-append 0 "connected!")
  (tcp-send "PASS *")
  (tcp-send [0a])
  (tcp-send "NICK bomberjacket_")
  (tcp-send [0a])
  (tcp-send "USER bomberjacket_ 8 * :Bomberjacket OS")
  (tcp-send [0a])
  (tcp-send "JOIN #bomberjacket")
  (tcp-send [0a])
)))

(def irc-handler (fn (do
  (print "IRC-HANDLER!")
  (print (list "input:" network-input))
  (buf-append 0 network-input)
)))

(def irc-msg (fn msg (do
  (def ircbuf (concat "PRIVMSG #bomberjacket :" msg))
  (tcp-send ircbuf)
  (tcp-send [0a])
)))

(def irc (fn
  (tcp-connect freenode 6667 irc-connect-handler irc-handler)))

;;; HTTP -----------------------------------------------------------------

(def http-get (fn (do
  (def header (concat (concat "Host: " http-hostname) [0d0a0d0a]))
  (tcp-send (concat (concat (concat (concat "GET " http-path) " HTTP/1.1") [0d0a]) header))
)))

(def http-connect-handler (fn (http-get path)))

(def html-parse-state 0)
; 0: in text
; 1: in tag
; 2: whitespace

(def html-parse (fn raw (do
  (def i 0)
  (def j 0)
  (def k 0)
  (def l 0)
  (def rsz (usize raw))
  (def out (alloc-str rsz))
  (def html-attr (alloc-str 10))
  (def html-tag (alloc-str 10))
  (def attr-value (alloc-str 50))
  (def attr-i 0)
  (def tag-i 0)
  (def value-i 0)
  (def output-attr 0)

  (while (lt i rsz) (do
    (def c (uget raw i))
    (if (= html-parse-state 0) ; text
      (if (= c 60)
        (def html-parse-state 1)
      (if (or (= c 10) (= c 32))
        (do 
          (uput out j c)
          (def j (+ j 1)) (def l (+ l 1))
          (def html-parse-state 2)
        )
      (do
        (uput out j c)
        (def j (+ j 1))
        (def l (+ l 1))
      )))
    (if (= html-parse-state 1) (do ; tag
      (if (= c 62) (do
        (print (list "html parsed tag: " html-tag))
        (def tag-i 0)
        (def html-parse-state 0)
      ))
      (if (and (gt c 96) (lt c 123)) (do ; tag name
        (put html-tag tag-i c)
        (put html-tag (+ tag-i 1) 0)
        (def tag-i (+ tag-i 1))
      ))
      (if (= c 32) (do
        (print (list "html parsed tag: " html-tag))
        (def tag-i 0)
        (def attr-i 0)
        (def html-parse-state 3)))
      )
    (if (= html-parse-state 2) ; whitespace
      (if (not (or (= c 32) (= c 10)))
        (if (= c 60)
          (def html-parse-state 1)
        (do
          (def html-parse-state 0)
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
        )))
    (if (= html-parse-state 3) (do ; space between attrs
      (if (= c 62) (do ; tag closed
        (def html-parse-state 0)))
      (if (= c 32) (do ; end of attr
        (print (list "html parsed attr: " html-attr))
        (def attr-i 0)
        (put html-attr 0 0)
      ))
      (if (= c 61) (do ; '=' attr value follows
        (print (list "html parsed attr=: " html-attr))
        (def attr-i 0)
        (def html-parse-state 4)
      ))
      (if (and (gt c 96) (lt c 123)) (do ; attr name
        (put html-attr attr-i c)
        (put html-attr (+ attr-i 1) 0)
        (def attr-i (+ attr-i 1))
      )))
    (if (= html-parse-state 4) (do ; beginning of attr value
      (if (str= "href" html-attr) (do
        (print "href attr!")
        (uput out j (get "~" 0)) (def j (+ j 1))
        (def output-attr 1) ; we want to output the link
      ) (def output-attr 0)) ; else don't output
      (def value-i 0)
      (if (or (= c 39) (= c 34))
        (def attr-sep c)
        (def attr-sep 32) ; else
      )
      (def html-parse-state 5) ; go to the actual value
    )
    (if (= html-parse-state 5) (do ; attr value
      (if (= c attr-sep) (do ; value closed
          (print (list "attr value parsed: " attr-value))
          (if output-attr (do
            (uput out j 32) (def j (+ j 1))
          ))
          (def html-parse-state 3)
          (def value-i 0)
        )
        (if (and (= attr-sep 32) (= c 62)) (do ; tag closed
            (if output-attr (do
              (uput out j 32) (def j (+ j 1))
            ))
            (def html-parse-state 0)
          )
          (do ; append to attr value
            (put attr-value value-i c)
            (put attr-value (+ value-i 1) 0)
            (def value-i (+ value-i 1))

            (if output-attr (do
              (uput out j c) (def j (+ j 1))
            ))
            
          ))
      ))
    ))))))
    
    (def i (+ i 1))
    
    (if (and (= c 32) (gt l 80)) (do
      (uput out j 10) ; wrap and linefeed
      (def j (+ j 1))
      (def l 0)
    ) 0)
  ))
  out
)))

(def http-handler (fn (do
  (buf-append 0 (html-parse network-input))
)))

(def hget (fn ip hostname path (do
  (set-buf-str 0 (concat "loading… " [0a]))
  (def html-parse-state 0)
  (def http-path path)
  (def http-hostname hostname)
  (tcp-connect ip 80 http-connect-handler http-handler))))

(def webtest (fn (hget dwigns "news.dieweltistgarnichtso.net" "/notes/nokia-n900.html")))

(def uri-parse (fn str (do
  (def uri-parts (split str ":"))
  (def uri-proto (car uri-parts))
  (def uri-parts2 (split (car (cdr uri-parts)) "/"))
  (def uri-host (car uri-parts2))
  (def uri-path (cdr uri-parts2))
  (list uri-proto uri-host uri-path)
)))

(def plumb (fn word (do
  (print (cons "plumb" word))
  (if (= (uget word 0) (uchr "/"))
    (set-buf-str 0 (load word)))
  (if (= (uget word 0) (uchr "~")) (do
    (def plumb-uri (uri-parse (substr word 1 (usize word))))
    (if (str= "http" (car plumb-uri)) (do
      (def plumb-host (car (cdr plumb-uri)))
      (def path (join (car (cdr (cdr plumb-uri))) "/"))
      (if (lt (usize path) 1) (def path "/"))
      (buf-append 0 (join (list "hget" plumb-host path "…") " "))
      (hget mntmn plumb-host path)
      )
      (do ; FIXME total hack
      (def path (concat "/" (substr word 1 (usize word))))
      (hget mntmn plumb-host path)
      )
    )
  ))
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
      (plumb (word-at (buf-str (view-buf-id focused-view-id)) (view-cursor focused-view-id)))
      (focus-next-view)))

  (if (= k 127) (backspace focused-view-id))
  (if (= k 130) (cursor-left focused-view-id))
  (if (= k 132) (cursor-up focused-view-id))
  (if (= k 133) (cursor-down focused-view-id))
  (if (= k 131) (cursor-right focused-view-id))
  
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

; welcome to bomber jacket OS (arm7/32bit) ☕ ☕ ☕ -------------------------

(def loadimage (fn (do
  (set-view-type 0 3)
  (set-view-w 0 560)
  (def cat (load "../../rootfs/grumpy.data"))
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
