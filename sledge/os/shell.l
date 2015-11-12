(
(def ls (fn (do
  (split (load "/sd/") [0a])
)))

(struct task
  name "untitled task"
  focused 0
  z 0
  needs-redraw 0
  redrawn 0
  surface (surface))

(def draw-logo (fn ox oy (do
  (let c 0xff8e)
  (line fb (pt (+ ox 16) (- oy 38)) (pt (+ ox 16) (- oy 102)) c)
  (line fb (pt (+ ox 16) (- oy 102)) (pt (+ ox 80) (- oy 38)) c)
  (line fb (pt (+ ox 80) (- oy 38)) (pt (+ ox 80) (- oy 102)) c)
  (line fb (pt (+ ox 80) (- oy 102)) (pt (+ ox 144) (- oy 38)) c)
  (line fb (pt (+ ox 144) (- oy 38)) (pt (+ ox 144) (- oy 102)) c)
  (line fb (pt (+ ox 144) (- oy 102)) (pt (+ ox 208) (- oy 38)) c)
  (line fb (pt (+ ox 208) (- oy 38)) (pt (+ ox 208) (- oy 102)) c)
  (line fb (pt (+ ox 208) (- oy 102)) (pt (+ ox 272) (- oy 102)) c)
)))

(def desktop-task (fn (t task) (do
  (if (sget t needs-redraw) (do
    (clear)
    (draw-logo (- (/ (sget fb width) 2) 140) (/ (sget fb height) 2))
    (draw-logo (- (/ (sget fb width) 2) 139) (/ (sget fb height) 2))
    (draw-logo (- (/ (sget fb width) 2) 140) (+ 1 (/ (sget fb height) 2)))
    (send screen 0)
  ) 0)
  0
)))

(def keyboard (open "/keyboard"))

(import "/sd/os/mouse.l")

;; (def evbuf (alloc-str 4096))
;; (def p (fn xp x y maxx maxy (do
;;   (write xp evbuf)
;;   (blit-str evbuf x y maxx maxy)
;; )))

(draw-logo (- (/ (sget fb width) 2) 140) (/ (sget fb height) 2))
(blit-str fb unifont "hello" 100 100)

(def tasks (list))
(def new-tasks (list))

(def add-task (fn task-func task-obj task-state (do
  (def tasks (cons (list task-func task-obj task-state) tasks))
  (list-size tasks)
)))

(def task-func (fn task-obj task-state (print "empty task-func")))

(def paint-task (fn (t task) (do
  (if (sget t redrawn)
    (do
      (let task-surf (sget t surface))
      (blit fb task-surf)
      (let x (- (sget task-surf x) 1))
      (let y (- (sget task-surf y) 1))
      (let w (+ (sget task-surf width) 2))
      (let h (+ (sget task-surf height) 2))
      (box fb x y (+ x w) (+ y h) 0)
      (box fb x (- y 20) (+ x w) y 0)
      (blit-str fb unifont (sget t name) (+ x 2) (- y 18))
      (sput t redrawn 0))
    0)
  0
)))

(def check-task-focus (fn (t task) (do
  (let surf  (sget t surface))
  (let x (sget surf x))
  (let y (sget surf y))
  (let x2 (+ x (sget surf width)))
  (let y2 (+ y (sget surf height)))

  (let dbg "            ")
  (write mouse-btn dbg)
  (blit-str fb unifont dbg 0 0)

  (sput t focused 0)
  (if (gt mouse-x x)
    (if (gt mouse-y y)
      (if (lt mouse-x x2)
        (if (lt mouse-y y2)
          (do
            (sput t focused 1)
            (if mouse-dragging (do
              (boxfill fb (- x 2) (- y 22) (+ (- x2 x) 4) (+ (- y2 y) 24) 0xffff)
              (if (lt mouse-y (- y2 16))
                (do ; move
                  (sput surf x (+ mouse-dx x))
                  (sput surf y (+ mouse-dy y)))
                (do ; resize
                  (let nw (+ (sget surf width) mouse-dx))
                  (let nh (+ (sget surf height) mouse-dy))
                  (sput surf width nw)
                  (sput surf height nh)
                  (sput surf pitch (* 2 nw))
                  (sput surf pixels (alloc (* 2 (* nw nh))))
                  ))
              (sput t redrawn 1)
            ) 0)
          ) 0) 0) 0) 0)
  0
)))

(def run-tasks (fn (do
  (let tl tasks)
  (let new-tl (list))
  (let i 0)
  
  (while (car tl) (do
    (let task-item  (car tl))
    (def task-func  (car task-item))
    (let task-obj   (car (cdr task-item)))
    (let task-state (car (cdr (cdr task-item))))

    (check-task-focus task-obj)

    (task-func task-obj task-state)
    (paint-task task-obj)
    
    (let i (+ i 1))
    (let tl (cdr tl))
  ))
)))

;(def zz (fn (import "/sd/tests/gtn.l")))

(def cursor-blink 0)
(def cursor-blink-delay 20)

;(import "/sd/os/repl.l")
(import "/sd/os/editor.l")

(def make-surface (fn x y w h (do
  (let surf (new surface))
  (sput surf pixels (alloc (* 2 (* w h))))
  (sput surf x x)
  (sput surf y y)
  (sput surf width w)
  (sput surf height h)
  (sput surf pitch (shl w 1))
  surf
)))

;(add-task repl-task (repl-make 1 432 32 200 300))

(def spawn-editor (fn x y title focused (do
  (let my-editor (new editor))
  (let my-editor-task (new task))
  (sput my-editor-task name title)
  (sput my-editor buffer " ")
  (sput my-editor-task focused focused)
  (sput my-editor-task surface (make-surface x y 400 240))
  (add-task editor-task my-editor-task my-editor)
)))

(spawn-editor 32 32 "editor               " 0)
(spawn-editor 32 300 "editor               " 1)

(def mouse-task (new task))
(sput mouse-task name "mouse")
(add-task mouse-func mouse-task 0)

(def main (fn (while 1 (do
  (run-tasks)
  (send screen 0)
  (gc)
  (def cursor-blink (% (+ cursor-blink 1) cursor-blink-delay))
))))

(main)
)
