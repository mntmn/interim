(
(def ls (fn (do
  (split (load "/sd/") [0a])
)))

(print (list "screen" screen-width screen-height screen-bpp))

(def draw-logo (fn ox oy (do
  (def stroke-color 0xff8e)
  (line (pt (+ ox 16) (- oy 38)) (pt (+ ox 16) (- oy 102)))
  (line (pt (+ ox 16) (- oy 102)) (pt (+ ox 80) (- oy 38)))
  (line (pt (+ ox 80) (- oy 38)) (pt (+ ox 80) (- oy 102)))
  (line (pt (+ ox 80) (- oy 102)) (pt (+ ox 144) (- oy 38)))
  (line (pt (+ ox 144) (- oy 38)) (pt (+ ox 144) (- oy 102)))
  (line (pt (+ ox 144) (- oy 102)) (pt (+ ox 208) (- oy 38)))
  (line (pt (+ ox 208) (- oy 38)) (pt (+ ox 208) (- oy 102)))
  (line (pt (+ ox 208) (- oy 102)) (pt (+ ox 272) (- oy 102)))
)))

(draw-logo (- (/ screen-width 2) 140) (/ screen-height 2))
(draw-logo (- (/ screen-width 2) 139) (/ screen-height 2))
(draw-logo (- (/ screen-width 2) 140) (+ 1 (/ screen-height 2)))
(send scr 0)

(def keyboard (open "/keyboard"))

(blit-str "Welcome to Interim OS." 32 32)
(send scr 0)

(def evbuf (alloc-str 4096))
(def p (fn xp x y (do
  (write xp evbuf)
  (blit-str evbuf x y)
)))

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

(def add-task (fn t (do
  (def tasks (cons t tasks))
)))

(def mouse (open "/mouse"))
(def mouse-x 0)
(def mouse-y 0)
(def mouse-dx 0)
(def mouse-dy 0)
(def mouse-btn 0)
(def mouse-task (fn (do
  (add-task (fn (do
    (blit-char 32 mouse-x mouse-y)
    
    (let mouse-info (recv mouse))
    (def mouse-dx (car (car mouse-info)))
    (def mouse-dy (cdr (car mouse-info)))
    (def mouse-x (+ mouse-x mouse-dx))
    (def mouse-y (+ mouse-y mouse-dy))
    (if (lt mouse-x 0) (def mouse-x 0) 0)
    (if (lt mouse-y 0) (def mouse-y 0) 0)
    (if (gt mouse-x maxx) (def mouse-x maxx) 0)
    (if (gt mouse-y maxy) (def mouse-y maxy) 0)
    
    (def mouse-btn (cdr mouse-info))

    (if mouse-btn (blit-char 0x219c mouse-x mouse-y)
      (blit-char 0x2196 mouse-x mouse-y))

  )))
)))

(def net (open "/net"))
(def net-y 32)

(def temp-minx minx)
(def temp-maxx maxx)

(def irc-msg (fn msg (do
  (let ircbuf (concat "PRIVMSG #nodrama.de :" msg))
  (send net ircbuf)
  (send net [0a])
)))

(def cmdbuf (alloc-str 512))
(def cmd-read (list))
(def remote-cmd (fn msg (do
  (let parts (split msg "$"))
  (if (gt (list-size parts) 1) (do
    (let cmd (concat (concat "(" (item parts 1)) ")"))
    (print (list "remote cmd" cmd))
    (def cmdbuf (alloc-str 512))
    (def cmd-read (read cmd))
    (write (eval cmd-read) cmdbuf)
    (print (list "result" cmdbuf))
    (irc-msg cmdbuf)
  ) 0)
  0
)))

(def freenode "/net/tcp/62.231.75.133/6667")
(def sternfreunde "/net/tcp/46.101.207.85/80")
(def interim-os "/net/tcp/91.250.115.15/80")

(def connect (fn net-path (do
  (def net (open net-path))
)))

(def net-task (fn (do
  (add-task (fn (do
    (let packet (recv net))
    (if (size packet) (do
      (def temp-minx minx)
      (def temp-maxx maxx)

      (def minx 1000)
      (def maxx 1700)
      (let msg (bytes->str packet))

      (boxfill 1000 net-y 716 64 0xffff)
      (let ofsy (+ (blit-str msg minx net-y) rune-h))
      
      (def minx temp-minx)
      (def maxx temp-maxx)
      (def net-y (+ 0 ofsy))
      (if (gt net-y maxy) (def net-y miny) 0)

      ; only enable this when you know what you're doing
      ; (remote-cmd msg)
    ) 0)
  )))
  1
)))

(def http-get (fn host path (do
  (boxfill 1000 0 800 1000 0xffff)
  (let header (concat (concat "Host: " host) (bytes->str [0d0a0d0a])))
  (send net (concat (concat (concat (concat "GET " path) " HTTP/1.1") (bytes->str [0d0a])) header))
)))

(def irc-join (fn nick channel (do
  (send net "PASS *")
  (send net [0a])
  (send net (concat "NICK " nick))
  (send net [0a])
  (send net (concat "USER " (concat nick " 8 * :Interim OS")))
  (send net [0a])
  (send net (concat "JOIN " channel))
  (send net [0a])
)))

(def task-func (fn (print "empty task-func")))

(def run-tasks (fn (do
  (let tl tasks)
  (while (car tl) (do
    (def task-func (car tl))
    ; (print (list "run-task " task-func))
    (task-func)
    (let tl (cdr tl))
  ))
)))

(def ed (fn (import "/sd/os/editor.l") ))

(def buffer-read (list))

(def zz (fn (import "/sd/tests/gtn.l")))

(def shell-running 1)

(def main (fn (do
  (let blink 0)

  (while shell-running (do
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

        (def buffer-read (list (read buffer)))
        (let result (eval buffer-read))
        
        (def buffer "")
        (def term-x minx)
        (def term-y (+ term-y rune-h))
        (def term-y (+ rune-h (p result term-x term-y)))
        0
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
        (let nl (- (strlen buffer) 1))
        (def buffer (substr buffer 0 nl)) ) 0) 0)

    (if (gt term-x maxx) (do (def term-x minx) (def term-y (+ term-y rune-h))) 0)

    (if (gt term-y maxy) (def term-y miny) 0)

    (if (lt term-x 32) (def term-x minx) 0)
    
    (if (eq blink 3)
        (blit-char 0x2588 term-x term-y) 0)
    (if (eq blink 0)
        (blit-char 32 term-x term-y) 0)
    (let blink (% (+ blink 1) 5))

    (run-tasks)
    (send scr 0)
    (gc)
  ))
)))

(main)
)
