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

(def evbuf (alloc-str 4096))
(def p (fn xp x y maxx maxy (do
  (write xp evbuf)
  (blit-str evbuf x y maxx maxy)
)))

(def tasks (list))
(def new-tasks (list))

(def add-task (fn t initial-state (do
  (def new-tasks (cons (cons t initial-state) new-tasks))
  (list-size new-tasks)
)))

(def task-func (fn state (print "empty task-func")))
(def give-focus 0)

(def run-tasks (fn (do
  (let tl tasks)
  (let new-tl (list))
  (let i 0)
  
  (while (car tl) (do
    (def task-func (car (car tl)))
    (let task-state (cdr (car tl)))
    (let had-focus (car task-state))
    (let had-focus (or had-focus give-focus))
    (let task-state (cons had-focus (cdr task-state)))
    (let new-state (task-func task-state))
    (def give-focus (and had-focus (not (car new-state))))
    (if give-focus
      (print (list "task yields focus " i)) 0)
    (let tl (cdr tl))
    (let new-tl (cons (cons task-func new-state) new-tl))
    (let i (+ i 1))
  ))

  ; spawn new tasks
  (let tl new-tasks)
  (while (car tl) (do
    (let new-tl (cons (car tl) new-tl))
    (let tl (cdr tl))
  ))
  (def new-tasks (list))
  (def tasks (reverse new-tl))
)))

;(def zz (fn (import "/sd/tests/gtn.l")))

(def cursor-blink 0)
(def cursor-blink-delay 20)

(import "/sd/os/repl.l")
(import "/sd/os/editor.l")

(add-task repl-task (repl-make 1 432 32 200 300))
(add-task editor-task (list 1 (new editor)))
;(def ed2 (new editor))
;(sput ed2 win-y 300)
;(add-task editor-task (list 0 ed2))

;(def newshell (fn x y 
;  (add-task repl-task (repl-make 0 x y 400 100))
;))

(def main (fn (while 1 (do
  (run-tasks)
  (send scr 0)
  (gc)
  (def cursor-blink (% (+ cursor-blink 1) cursor-blink-delay))
))))

(main)
)
