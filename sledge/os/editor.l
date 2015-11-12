(
(struct editor
  buffer "hello world!"
  font unifont
  cursor 0
  cursor-char 32
  cursor-x 0
  cursor-y 0
  cursor-abs-x 32
  cursor-abs-y 32
  last-num-lines -1
  scroll-y 0
  scroll-dirty 0
  buf-dirty 1
)

(def find-prev (fn buf rune pos (do
  (let p pos)
  (while (and (gt p 0) (not (eq rune (get8 buf p))))
    (let p (- p 1)))
  (+ p 0)
)))

(def find-next (fn buf rune pos (do
  (let p pos)
  (while (and (lt p (size buf)) (not (eq rune (get8 buf p))))
    (let p (+ p 1)))
  (+ p 0)
)))

(def find-prev-ws (fn buf pos (do
  (let p (+ pos 0))
  (while (and (gt p 0) (not (or (eq 10 (get8 buf p)) (eq 32 (get8 buf p)))))
    (let p (- p 1)))
  (if (eq p 0) 0 (+ p 1))
)))

(def find-next-ws (fn buf pos (do
  (let p (+ pos 0))
  (while (and (lt p (size buf)) (not (or (eq 10 (get8 buf p)) (eq 32 (get8 buf p)))))
    (let p (+ p 1)))
  (+ p 0)
)))

(def buf-render (fn (surf surface) (this editor) (do
  (let buffer (sget this buffer))
  (let lines (split buffer [0a]))
  (let i 0)
  (let ln (car lines))
  (let pos 0)
  (let nextpos 0)
  (let x 2)
  (let y 20)
  (let maxx (sget surf width))
  (let maxy (sget surf height))
  (let cursor (sget this cursor))
  (let scroll-dirty (sget this scroll-dirty))
  (let scroll-y (sget this scroll-y))
  (let render-all 0)
  (let font (sget this font))
  
  ; number of lines changed? then rerender
  (if (or scroll-dirty (not (eq (sget this last-num-lines) (list-size lines)))) (do
      (let render-all 1)
      (print "editor boxfill")
      (boxfill surf 0 0 maxx maxy 0xffff)
      (sput this scroll-dirty 0)
    )
  0)

  (sput this last-num-lines (list-size lines))

  (let i 0)
  (while ln (do
    (let is-current-line 0)
    (let i (+ i 1))
    
    (let nextpos (+ 1 (+ pos (size ln))))

    (if (and (gt cursor (- pos 1)) (lt cursor (+ nextpos 1))) (do
      ; cursor is in this line
      (sput this cursor-abs-y (+ y 0))
      (sput this cursor-x (- cursor pos))
      (sput this cursor-y (- (- i 1) scroll-y ))
      (sput this cursor-abs-x (+ 0 (* (sget font spacing) (sget this cursor-x))))
      (sput this cursor-char (get8 ln (sget this cursor-x)))
      (let is-current-line 1)

      ; blank out the line
      ;(if only-current (do
      ;  (boxfill surf x y maxx (sget font rune-h) 0xffff)
      ;) 0)
    ) 0)
    
    (if (or render-all is-current-line) (do
      (if (and (lt y maxy) (not (gt scroll-y i)))
        (blit-str surf font ln 0 y)
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

(def backspace (fn (this editor) (if (gt (sget this cursor) 0) (do
  (remove (sget this buffer) (- (sget this cursor) 1))
  (sput this cursor (- (sget this cursor) 1))
  (sput this buf-dirty 1)
) 0)))

(def repair-at-cursor (fn (surf surface) (this editor) (do
  (let term-x (sget this cursor-abs-x))
  (let term-y (sget this cursor-abs-y))
  (print (list "repair-at-cursor" term-x term-y))
  (let cursor-char (sget this cursor-char))
  (let font (sget this font))
  (let rune (if (eq cursor-char 0) 32 cursor-char))

  (blit-char16 surf font rune term-x term-y)
)))

(def cursor-left (fn (surf surface) (this editor) (do
  (repair-at-cursor surf this)
  (sput this cursor (- (sget this cursor) 1))
  (sput this buf-dirty 1)
)))

(def cursor-right (fn (surf surface) (this editor) (do
  (repair-at-cursor surf this)
  (sput this cursor (+ (sget this cursor) 1))
  (sput this buf-dirty 1)
)))

(def scroll-speed 5)

(def cursor-up (fn (surf surface) (this editor) (do
  (repair-at-cursor surf this)
  (let buf (sget this buffer))
  
  (sput this cursor (find-prev buf 10 (- (sget this cursor) 1)))
  (if (and (lt (sget this cursor-y) 5) (gt (sget this scroll-y) 0)) (do
    (sput this scroll-y (- (sget this scroll-y) scroll-speed))
    (if (lt (sget this scroll-y) 0) (sput this scroll-y 0) 0)
    (sput this scroll-dirty 1)
  ) 0)
  (sput this buf-dirty 1)
)))

(def cursor-down (fn (surf surface) (this editor) (do
  (repair-at-cursor surf this)
  (let cursor (sget this cursor))
  (let last-num-lines (sget this last-num-lines))
  (let buf (sget this buffer))
  (let font (sget this font))
  (let rune-h (sget font rune-h))
  
  (let nextzero (find-next buf 0 (+ cursor 1)))
  (let nextnl   (find-next buf 10 (+ cursor 1)))

  (let y 0)
  (let maxy (+ y (sget surf height)))
  (let maxlines (- (/ maxy rune-h) 1))
  
  (sput this cursor (if (lt nextzero nextnl) cursor nextnl))
  (if (and (gt (sget this cursor-y) maxlines) (lt (sget this scroll-y) (sget this last-num-lines))) (do
    (sput this scroll-y (+ (sget this scroll-y) scroll-speed))
    (if (gt (sget this scroll-y) (- last-num-lines 1)) (sput this scroll-y (- (sget this last-num-lines) 1)) 0)
    (sput this scroll-dirty 1)
  ) 0)
  (sput this buf-dirty 1)
)))

(def exit-editor (fn (do
  (print "exit-editor")
)))

(def editor-yield-focus (fn (task-obj task) (do
  (print "yield focus")
  (sput task-obj focused 0)
)))

(def handle-editor-key (fn (task-obj task) (this editor) k (do
  (let surf (sget task-obj surface))

  (if (eq k 20) (cursor-right surf this)
    (if (eq k 19) (cursor-left surf this)
      (if (eq k 0x7f) (backspace this)
        (if (eq k 17) (cursor-up surf this)
          (if (eq k 18) (cursor-down surf this)
            (if (eq k 9) (editor-yield-focus task)

  (if (and (gt k 0) (lt k 250))
    (do
      (print (list "handle-editor-key2" k))
      (let buf (concat (sget this buffer) " ")) ; room for new character
      (insert buf (sget this cursor) (+ k 0))
      (sput this buffer buf)
      (sput this cursor (+ (sget this cursor) 1))
      (sput this buf-dirty 1)
    ) 0)
  
  ))))))
  0
)))

(def handle-command-key (fn 0))

(def editor-task (fn (task-obj task) (this editor) (do
  (let cursor (sget this cursor))
  (let buf (sget this buffer))

  (let focused (sget task-obj focused))
  (let surf (sget task-obj surface))
  
  (if focused (do
    (let str (recv keyboard))
    (let k (get8 str 0))
    (handle-editor-key task-obj this k)
    ) 0)
  
  (if (lt cursor 0) (sput this cursor 0) 0)
  (if (gt cursor (size buf)) (sput this cursor (size buf)) 0)

  (if (or (sget task-obj needs-redraw) (sget this buf-dirty)) (do
                    (if (sget task-obj needs-redraw)
                      (sput this scroll-dirty 1)
                      0)
                    (buf-render surf this)
                    (sput this buf-dirty 0)
                    (sput task-obj needs-redraw 0)
                    (sput task-obj redrawn 1)
                  ) 0)

  (let term-x (sget this cursor-abs-x))
  (let term-y (sget this cursor-abs-y))
  (let cursor-char (sget this cursor-char))
  (let font (sget this font))

  (write (list (quote ed) cursor (quote /) (size buf)) (sget task-obj name))

  (if focused
  (if (eq cursor-blink (/ cursor-blink-delay 2))
    (do
      (blit-char16 surf font 0x2588 term-x term-y)
      (sput task-obj redrawn 1))

    (if (eq cursor-blink 0)
      (do
        (blit-char16 surf font (if (eq cursor-char 0) 32 cursor-char) term-x term-y)
        (sput task-obj redrawn 1))
      0)) 0)
  0
)))


;(def edit-file (fn path (do
;  (let content (load path))
;  (edit content)
;)))

;(def edit-new (fn (do
;  (edit (alloc-str 4096))
;)))
)
