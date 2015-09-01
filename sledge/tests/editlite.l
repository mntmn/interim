(
(def editor-running 1)

(def buf (alloc-str 1024))

(def find-prev (fn buf rune pos (do
  (let p pos)
  (while (and (gt p 0) (not (eq rune (get buf p))))
    (let p (- p 1)))
  (+ p 0)
)))

(def find-next (fn buf rune pos (do
  (let p pos)
  (while (and (lt p (size buf)) (not (eq rune (get buf p))))
    (let p (+ p 1)))
  (+ p 0)
)))

(def find-prev-ws (fn buf pos (do
  (let p (+ pos 0))
  (while (and (gt p 0) (not (or (eq 10 (get buf p)) (eq 32 (get buf p)))))
    (let p (- p 1)))
  (if (eq p 0) 0 (+ p 1))
)))

(def find-next-ws (fn buf pos (do
  (let p (+ pos 0))
  (while (and (lt p (size buf)) (not (or (eq 10 (get buf p)) (eq 32 (get buf p)))))
    (let p (+ p 1)))
  (+ p 0)
)))

(def copy (fn buf from to num (do
  (let i 0)
  (let c 0)

  ;(print (list "copy: " buf from to num))
  
  (if (lt from to)
    (do 
      (let i (- num 1)) 
      (while (gt i -1) (do
        (let c (get buf (+ from i)))
        (put buf (+ to i) c)
        (let i (- i 1))
      )) 0)
    (do
      (let i 0)
      (while (lt i num) (do
        (let c (get buf (+ from i)))
        (put buf (+ to i) c)
        (let i (+ i 1))
      )) 0)
  )
  num
)))

; fixme clobbered reg loading broken on x64?
; fixme use substr instead of mutation

(def remove (fn buf pos (do
  (let p (+ pos 0))
  (let from (+ pos 1))
  (let num (- (size buf) pos))
  (copy buf from p num)
  (put buf (- (size buf) 1) 0)
  buf
)))

(def insert (fn buf pos k (do
  (let p (+ pos 0))
  (let to (+ pos 1))
  (let c (+ k 0))
  (let num (- (size buf) (+ pos 1)))
  (copy buf p to num)
  (put buf p c)
  buf
)))

(def buf-render-x 32)
(def buf-render-y 32)
(def cursor-char 32)
(def cursor-x 0)
(def cursor-y 0)
(def last-num-lines 0)
(def scroll-y 0)
(def scroll-dirty 0)

(def buf-render (fn cursor b (do
  (let lines (split b [0a]))
  (let i 0)
  (let y 0)
  (let ln (car lines))
  (let pos 0)
  (let nextpos 0)
  (let y buf-render-y)
  (let render-all 0)
  
  ; number of lines changed? then rerender
  (if (or scroll-dirty (not (eq last-num-lines (list-size lines)))) (do
      (let render-all 1)
      (boxfill minx miny maxx maxy 0xffff)
      (def scroll-dirty 0)
    )
  0)

  (def last-num-lines (list-size lines))

  (let i 0)
  
  (while ln (do
    (let is-current-line 0)
    (let i (+ i 1))
    
    (let nextpos (+ 1 (+ pos (size ln))))

    (if (and (gt cursor (- pos 1)) (lt cursor (+ nextpos 1))) (do
      ; cursor is in this line
      (def term-y (+ y 0))
      (def cursor-x (- cursor pos))
      (def cursor-y (- (- i 1) scroll-y ))
      (def term-x (+ buf-render-x (* rune-spacing cursor-x)))
      (def cursor-char (get ln cursor-x))
      (let is-current-line 1)

      ; blank out the line
      ;(if only-current (do
      ;  (boxfill buf-render-x term-y (- maxx buf-render-x) rune-h 0xffff)
      ;) 0)
    ) 0)

    ; this crashes arm
    ; (or is-current-line (not only-current))
    
    (if (or render-all is-current-line) (do
      (if (and (lt y maxy) (not (gt scroll-y i)))
        (blit-str ln buf-render-x y)
        0)
    ) 0)
    
    (let pos (+ nextpos 0))
    
    (if (not (gt scroll-y i))
      (let y (+ y 16))
      0)
    
    (let lines (cdr lines))
    (let ln (car lines))
  ))
  0
)))

(def word-at (fn buf pos (do
  (let from (find-prev-ws buf pos))
  (let to   (find-next-ws buf pos))
  (substr buf from (- to from))
)))

(def cursor 0)
(def buf-dirty 0)

(def backspace (fn (if (gt cursor 0) (do
  (remove buf (- cursor 1))
  (def cursor (- cursor 1))
  (def buf-dirty 1)
) 0)))

(def repair-at-cursor (fn (do
  (print "repair-at-cursor")
  (blit-char (if (eq cursor-char 0) 32 cursor-char) term-x term-y)
)))

(def cursor-left (fn (do
  (repair-at-cursor)
  (def cursor (- cursor 1))
  (def buf-dirty 1)
)))

(def cursor-right (fn (do
  (repair-at-cursor)
  (def cursor (+ cursor 1))
  (def buf-dirty 1)
)))

(def scroll-speed 10)

(def cursor-up (fn (do
  (repair-at-cursor)
  (def cursor (find-prev buf 10 (- cursor 1)))
  (if (and (lt cursor-y 5) (gt scroll-y 0)) (do
    (def scroll-y (- scroll-y scroll-speed))
    (if (lt scroll-y 0) (def scroll-y 0) 0)
    (def scroll-dirty 1)
  ) 0)
  (def buf-dirty 1)
)))

(def cursor-down (fn (do
  (repair-at-cursor)
  (let nextzero (find-next buf 0 (+ cursor 1)))
  (let nextnl (find-next buf 10 (+ cursor 1)))
  (def cursor (if (lt nextzero nextnl) cursor nextnl))
  (if (and (gt cursor-y 30) (lt scroll-y last-num-lines)) (do
    (def scroll-y (+ scroll-y scroll-speed))
    (if (gt scroll-y (- last-num-lines 1)) (def scroll-y (- last-num-lines 1)) 0)
    (def scroll-dirty 1)
  ) 0)
  (def buf-dirty 1)
)))

(def exit-editor (fn (do
  (print "exit-editor")
  (def editor-running 0)
)))

(def handle-editor-key (fn k (do
  (if (eq k 20) (cursor-right)
    (if (eq k 19) (cursor-left)
      (if (eq k 0x7f) (backspace)
        (if (eq k 17) (cursor-up)
          (if (eq k 18) (cursor-down)
            (if (eq k 27) (exit-editor)

  (if (and (gt k 0) (lt k 250))
    (do
      (print (list "handle-editor-key2" k))
      (def buf (concat buf " ")) ; room for new character
      (insert buf cursor (+ k 0))
      (def cursor (+ cursor 1))
      (def buf-dirty 1)
    ) 0)
  
  ))))))

  (if buf-dirty (do
    (buf-render cursor buf)
    (def buf-dirty 0)
  ) 0)
  0
)))

(def handle-command-key (fn 0))

(def edit (fn edit-buf (do
  (def buf edit-buf)
  (def editor-running 1)
  (def last-num-lines -1)
  (def cursor 0)

  (let blink 0)
  (clear)
  (gc)
  (def term-x buf-render-x)
  (def term-y buf-render-y)
  (def buf-dirty 1)
  
  (while (eq 1 editor-running) (do
    (let str (recv keyboard))
    (let k (get str 0))

    (handle-editor-key k)

    (if (lt cursor 0) (def cursor 0) 0)
    (if (gt cursor (size buf)) (def cursor (size buf)) 0)

    ; (print (list "term-x/y" term-x term-y))
    (if (gt blink 9)
      (blit-char 0x2588 term-x term-y)
      (do
        (blit-char (if (eq cursor-char 0) 32 cursor-char) term-x term-y)
      ))
      
    (let blink (% (+ blink 1) 20))
    
    (send scr 0)
    (gc)
  ))
  (concat buf "")
)))

(def edit-file (fn path (do
  (let content (load path))
  (edit content)
)))

(def edit-new (fn (do
  (edit (alloc-str 4096))
)))
)
