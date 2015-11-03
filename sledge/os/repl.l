(

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

(def repl-task-initial-state (list
  0  ; focused
  "" ; buffer
  0  ; blink
  0  ; win-x
  0  ; win-y
  0  ; term-x
  0  ; term-y
  100 ; maxx
  200 ; maxy
  (list) ; history
  (list) ; future
))

(def repl-make (fn focused win-x win-y w h (list
  focused "" 0 win-x win-y 0 0 w h (list) (list)
)))

(def repl-task (fn state (do
  (let s state)
  (let focused (car s)) (let s (cdr s))
  (let buffer  (car s)) (let s (cdr s))
  (let blink   (car s)) (let s (cdr s))
  (let win-x   (car s)) (let s (cdr s))
  (let win-y   (car s)) (let s (cdr s))
  (let term-x  (car s)) (let s (cdr s))
  (let term-y  (car s)) (let s (cdr s))
  (let maxx    (car s)) (let s (cdr s))
  (let maxy    (car s)) (let s (cdr s))
  (let history (car s)) (let s (cdr s))
  (let future  (car s)) (let s (cdr s))

  (let c 0)
  (if focused (do
    (let str (recv keyboard))
    (let c (get str 0))
  ) 0)

  ; TODO (rect)
  ;(line (pt win-x win-y) (pt (+ win-x maxx) win-y))
  ;(line (pt (+ win-x maxx) win-y) (pt (+ win-x maxx) (+ win-y maxy)))
  ;(line (pt win-x win-y) (pt win-x (+ win-y maxy)))
  ;(line (pt win-x (+ win-y maxy)) (pt (+ win-x maxx) (+ win-y maxy)))
  
  (if (gt c 0) (print c) 0)
    (if (and (gt c 0x1f) (not (eq 0x7f c))) (do
      (let term-y (-
        (blit-str str (+ win-x term-x) (+ win-y term-y) (+ win-x maxx) (+ win-y maxy))
        win-y))
      (let buffer (concat buffer str))
      (let term-x (+ term-x rune-spacing))
    ) 0)
      
    (if (eq c 9) (do ; tab
      ; give up focus
      (let focused 0)
    ) 0)
    
    (if (eq c 10) ; return
      (do
        (blit-char 32 (+ win-x term-x) (+ win-y term-y))
        ;(let history (cons buffer history))

        (let buffer-read (list (read buffer)))
        (print (list buffer buffer-read))
        (let result (eval buffer-read))
        
        (let buffer "")
        (let term-x 0)
        (let term-y (+ term-y rune-h))
        (let term-y (- (+ rune-h (p result (+ win-x term-x) (+ win-y term-y) (+ win-x maxx) (+ win-y maxy)) ) win-y))
        0
      ) 0)

    (if (eq c 17) (do ; cursor up
      (let res (repl-history-back history future))
      (let buffer  (car res))
      (let history (car (cdr res)))
      (let future  (car (cdr (cdr res))))
      (let term-x  (* rune-spacing (strlen buffer)))
    ) 0)
        
    (if (eq c 18) (do ; cursor down
      (let res (repl-history-forth history future))
      (let buffer  (car res))
      (let history (car (cdr res)))
      (let future  (car (cdr (cdr res))))
      (let term-x  (* rune-spacing (strlen buffer)))
    ) 0)

    (if (eq c 0x7f) ; bksp
      (if (gt (strlen buffer) 0)
      (do
        (blit-char 32 (+ win-x term-x) (+ win-y term-y))
        (let term-x (- term-x rune-spacing))
        (let nl (- (strlen buffer) 1))
        (let buffer (substr buffer 0 nl))
      ) 0) 0)

    (if (gt term-x maxx) (do
      (let term-x 0)
      (let term-y (+ term-y rune-h))) 0)

    (if (gt term-y maxy) (let term-y 0) 0)

    (if (lt term-x 0) (let term-x 0) 0)
    
    (if (and focused (eq blink (/ cursor-blink-delay 2)))
        (blit-char 0x2588 (+ win-x term-x) (+ win-y term-y)) 0)
    (if (eq blink 0)
        (blit-char 32 (+ win-x term-x) (+ win-y term-y)) 0)

    (let blink (% (+ blink 1) cursor-blink-delay))

    (let new-state (list focused buffer blink win-x win-y term-x term-y maxx maxy history future))
    new-state
)))
)
