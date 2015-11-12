(
(struct repl
  focused 0
  buffer ""
  term-x 0
  term-y 0
  history (list) 
  future (list) 
  font unifont
)

(def evbuf (alloc-str 4096))
(def p (fn (surf surface) xp x y (do
   (write xp evbuf)
   (blit-str surf unifont evbuf x y)
)))

(def repl-history-back (fn history future (do
  (let line (car history))

  (let future (cons (car history) future))
  (let history (cdr history))
  (print (list "history:" history "future:" future))
  (list line history future)
)))

(def repl-history-forth (fn history future (do
  (let line (car future))
  
  (let history (cons (car future) history))
  (let future (cdr future))
  (print (list "history:" history "future:" future))
  (list line history future)
)))

(def repl-task (fn (task-obj task) (this repl) (do
  (let focused (sget task-obj focused))
  (let buffer  (sget this buffer))
  (let surf (sget task-obj surface))
  (let term-x  (sget this term-x))
  (let term-y  (sget this term-y))
  
  (let maxx    (sget surf width))
  (let maxy    (sget surf height))
  (let history (sget this history))
  (let future  (sget this future))

  (let win-x 2)
  (let win-y 20)
  (let font (sget this font))
  (let rune-spacing (sget font spacing))
  (let rune-h (sget font rune-h))

  (let c 0)
  (if focused (do
    (let str (recv keyboard))
    (let c (get8 str 0))
  ) 0)

  ; TODO new strategy:
  ; - list of lines
  ; - fn to render one line
  ; - fn to render all visible lines
  ; tab = 9
  
  (if (and (gt c 0x1f) (not (eq 0x7f c)))
    (do
      (let term-y (-
        (blit-str surf unifont str (+ win-x term-x) (+ win-y term-y))
        win-y))
      (let buffer (concat buffer str))
      (let term-x (+ term-x rune-spacing))) 0)
    
  (if (eq c 10) ; return
    (do
        (blit-char16 surf unifont 32 (+ win-x term-x) (+ win-y term-y))
        ;(let history (cons buffer history))

        (let buffer-read (list (read buffer)))
        (print (list buffer buffer-read))
        (let result (eval buffer-read))
        
        (let buffer "")
        (let term-x 0)
        (let term-y (+ term-y rune-h))
        (let term-y (- (+ rune-h (p surf result (+ win-x term-x) (+ win-y term-y)) ) win-y))
        0) 0)

  (if (eq c 17)
    (do ; cursor up
      (let res (repl-history-back history future))
      (let buffer  (car res))
      (let history (car (cdr res)))
      (let future  (car (cdr (cdr res))))
      (let term-x  (* rune-spacing (strlen buffer)))
    ) 0)
        
  (if (eq c 18)
    (do ; cursor down
      (let res (repl-history-forth history future))
      (let buffer  (car res))
      (let history (car (cdr res)))
      (let future  (car (cdr (cdr res))))
      (let term-x  (* rune-spacing (strlen buffer)))
    ) 0)

  (if (eq c 0x7f) ; bksp
    (if (gt (strlen buffer) 0)
      (do
        (blit-char16 surf unifont 32 (+ win-x term-x) (+ win-y term-y))
        (let term-x (- term-x rune-spacing))
        (let nl (- (strlen buffer) 1))
        (let buffer (substr buffer 0 nl))
      ) 0) 0)

  (if (gt c 0) (do
    (sput task-obj redrawn 1)
  ) 0)

  (if (gt term-x maxx) (do
    (let term-x 0)
    (let term-y (+ term-y rune-h))) 0)

  (if (gt term-y maxy) (let term-y 0) 0)

  (if (lt term-x 0) (let term-x 0) 0)
    
  (if (and focused (eq cursor-blink (/ cursor-blink-delay 2))) (do
    (blit-char16 surf unifont 0x2588 (+ win-x term-x) (+ win-y term-y))
    (sput task-obj redrawn 1)) 0)
  (if (eq cursor-blink 0) (do
    (blit-char16 surf unifont 32 (+ win-x term-x) (+ win-y term-y))
    (sput task-obj redrawn 1)) 0)

  (sput this buffer buffer)
  (sput this term-x term-x)
  (sput this term-y term-y)
  (sput this history history)
  (sput this future future)
  0
)))
)
