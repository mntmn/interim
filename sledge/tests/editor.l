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

(def substr (fn str beg sz (do
  (def res (alloc-str (* 2 sz)))
  (def ii 0)
  
  (while (lt ii sz) (do
    (uput res ii (uget str (+ beg ii)))
    (def ii (+ ii 1))
  ))
  res
)))

(def debug-buf (alloc-str 128))

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

(def unifont (load "unifont"))
(def white 16777215)
(def cyan 65535)
(def color white)
(def unifont-width 4128)
(def unifont-height 4160)

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

(def debug (fn o (write eval-buf o)))

(def blit-str (fn cursor str xx y color (do
  (mut l (+ (size str) 1))
  (mut i -1)
  (mut x xx)
  (while (lt (mut i (+ i 1)) l) (do
    (mut chr (uget str i))
    (if (= i cursor)
      (blit-char-inv chr x y color)
      (if (or (= chr 10) (= chr 32)) 0 (blit-char chr x y color)))
    (mut x (+ x 8))
    (if (= chr 10)
      (do
        (mut y (+ y 16))
        (mut x xx)))
    (if chr 0 (mut i l))
    0
  ))
  0
)))

(def cursor 0)
(def running 1)
(def cmd-mode 0)

(def buf (alloc-str 500))

(def eval-buf "welcome to bomber jacket OS (x86/64bit) ☕ ☕ ☕                                                                                                                                       ")

(def help-buf "[e] eval buffer [tab] toggle menu")

(def focus-buffer buf)
(def padding 20)

(def buf-render (fn focused b bx by (do
  (rectfill bx by (- 1024 bx) (- 768 by) color)
  (rectfill bx by 1024 1 0)
  (blit-str focused b (+ padding bx) (+ padding by) color)
)))

(def toggle-command (fn (do
  (if (= color cyan)
    (def color white)
    (def color cyan))
  (def cmd-mode (- 1 cmd-mode))
)))

(def chr (fn s (uget s 0)))

(def eval-tmp 0)

(def handle-command-key (fn k (do
  (def kidx k)
  (def kchr (uget keymap kidx))

  (if (= kchr 101) (do
    (def eval-tmp (eval buf))
    (def eval-buf (alloc-str 1024))
    (write eval-buf main)
    0
  ))
)))

(def keymap (load "keymap"))

(def ufind-prev (fn buf rune pos (do
  (while (* (gt pos 0) (not (= rune (uget buf pos))))
    (def pos (- pos 1)))
  (+ pos 0)
)))

(def ufind-next (fn buf rune pos (do
  (while (* (lt pos (usize buf)) (not (= rune (uget buf pos))))
    (def pos (+ pos 1)))
  (+ pos 0)
)))

(def cursor-left (fn modif (do
  (if (= modif 1)
    (def cursor-size (- cursor-size 1))
    (def cursor (- cursor 1)))
  (if (lt cursor-size 1) (def cursor-size 1) 0)
)))

(def cursor-right (fn modif (do
  (if (= modif 1)
    (def cursor-size (+ cursor-size 1))
    (def cursor (+ cursor 1)))
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

(def backspace (fn (if (gt cursor 0) (do
  (def cursor (- cursor 1))
  (ucopy buf cursor (- cursor 1) (- (usize buf) (+ cursor 1)))
) 0)))

(def handle-editor-key (fn k modif (do
    (if (= k 79) (cursor-right)
      (if (= k 80) (cursor-left)
        (if (= k 42) (backspace)
          (if (= k 82) (def cursor (ufind-prev buf 10 (- cursor 1)))
            (if (= k 81) (def cursor (+ 1 (ufind-next buf 10 (+ cursor 1))))
              (if (= modif 1024) (handle-command-key k)

  (if (* (gt k 0) (lt k 64))
    (do
      (def kidx (if (+ (= 1 modif) (= 2 modif)) (+ k 128) k))
      (def kchr (uget keymap kidx))
      (ucopy buf cursor (+ cursor 1) (- (usize buf) (+ cursor 1)))
      (uput buf cursor kchr)
      (def cursor (+ cursor 1))
    )
  0)
  
  ))))))
)))

(def parse-ip (fn ipstr (do
  (def s (usize ipstr))
  (def ipbytes (alloc 4))
  (def i 0)
  (def j 0)
  (while (and (lt j 4) (lt i s)) (do
    (def c (uget ipstr i))
    (if (= c 46)
        (def j (+ j 1))
      (put ipbytes j (+ (- c 48) (* 10 (get ipbytes j)))))
    (def i (+ i 1))
  ))
  ipbytes
)))


(def tcp-open (fn path opened-fn data-fn (do
  (def parts (split path "/"))
  (def host (item parts 0))
  (def host (parse-ip host))
  (def port (eval (item parts 1)))
)))


(def network-task (fn buf (do
  (def u (udp-poll))
  (def us (usize u))
  (def ui 0)

  (while (* (lt ui (size buf)) (lt ui us)) (do
    (def ui (+ ui 1))
    (uput buf ui (uget u ui))
  ))
)))

(def network-input "")

(def freenode [5bd9bd2a])
(def mntmn [5bfa730f])
(def server [0a000001])
(def dwigns [5eba9793])
(def facebook [1f0d5d03])

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
  (def html-tag (alloc-str 10))

  (while (lt i rsz) (do
    (def c (uget raw i))
    (if (= html-parse-state 0)
      (if (= c 60)
        (def html-parse-state 1)
      (if (or (= c 10) (= c 32))
        (do 
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
          (def html-parse-state 2)
        )
      (do
        (uput out j c)
        (def j (+ j 1))
        (def l (+ l 1))
      )))
    (if (= html-parse-state 1)
      (if (= c 62)
        (def html-parse-state 0)
        0)
    (if (= html-parse-state 2)
      (if (not (or (= c 32) (= c 10)))
        (if (= c 60)
          (def html-parse-state 1)
        (do
          (def html-parse-state 0)
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
        )) 0)
    0)))
    (def i (+ i 1))
    
    (if (and (= c 32) (gt l 80)) (do
      (uput out j 10)
      (def j (+ j 1))
      (def l 0)
    ) 0)
  ))
  out
)))

(def http-handler (fn (do
  (def eval-buf (concat eval-buf (html-parse network-input)))
)))

(def hget (fn ip hostname path (do
  (def eval-buf (concat "loading… " [0a]))
  (def html-parse-state 0)
  (def http-path path)
  (def http-hostname hostname)
  (tcp-connect ip 80 http-connect-handler http-handler))))

(def webtest (fn (hget dwigns "news.dieweltistgarnichtso.net" "/notes/nokia-n900.html")))

;;; IRC ------------------------------------------------------------------

(def irc-connect-handler (fn (do
  (tcp-send "PASS *")
  (tcp-send [0a])
  (tcp-send "NICK bomberjacket_")
  (tcp-send [0a])
  (tcp-send "USER bomberjacket_ 8 * :Bomberjacket OS")
  (tcp-send [0a])
  (tcp-send "JOIN #nodrama.de")
  (tcp-send [0a])
)))

(def irc-handler (fn (do
  (def eval-buf (concat eval-buf network-input))
  
  (if (gt (usize eval-buf) 1200) (def eval-buf (substr eval-buf (- (usize eval-buf) 1200) 1200)) 0)
)))

(def irc-msg (fn msg (do
  (def ircbuf (concat "PRIVMSG #nodrama.de :" msg))
  (tcp-send ircbuf)
  (tcp-send [0a])
  (def eval-buf (concat (concat eval-buf ircbuf) [0a]))
  (uput buf 10 34)
  (uput buf 11 41)
  (uput buf 12 59)
  (def i 13)
  (while (lt i (usize buf)) (do (uput buf i 32) (def i (+ i 1))))
  (def cursor 10)
  (toggle-command)
)))

(def irc (fn
  (tcp-connect freenode 6667 irc-connect-handler irc-handler)))

(def k 0)
(def modif 0)

(def main (fn
(while running (do
  (def k (inkey 0))
  (def modif (inkey 1))

  (if (= modif 1024)
    (handle-command-key k modif)
    (handle-editor-key k modif))

  (if (lt cursor 0) (mut cursor 0))
  (if (gt cursor (usize buf)) (mut cursor (- (usize buf) 1)))

  (buf-render cursor buf 0 0)
  (buf-render 0 eval-buf 0 100)
  (flip)
  (gc)
))))

(main)
