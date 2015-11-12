(
(def ls (fn (do
  (split (load "/sd/") [0a])
)))

(struct task
  id 0
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
      (let x (sget task-surf x))
      (let y (sget task-surf y))
      (let w (sget task-surf width))
      (let h (sget task-surf height))
      (if (and (gt w 0) (gt h 0)) (do
        (box task-surf 0 0 (- w 1) (- h 1) 0)
        (box task-surf 0 0 (- w 1) 20 0)
        (box task-surf (- w 16) (- h 16) (- w 1) (- h 1) 0)
        (blit-str task-surf unifont (sget t name) 2 2)
        (blit fb task-surf)
      ) 0)
      (sput t redrawn 0))
    0)
  0
)))

(def null-task (task))
(sput null-task id 0) ; "null" task
(def focused-task null-task)

(def point-in-rect (fn px py x y x2 y2 
  (if (gt px x)
    (if (gt py y)
      (if (lt px x2)
        (if (lt py y2)
          1 0) 0) 0) 0)))

(def mouse-resizing 0)
(def focus-given 0)

(def check-task-focus (fn (t task) (do
  (let surf  (sget t surface))
  (let x (sget surf x))
  (let y (sget surf y))
  (let x2 (+ x (sget surf width)))
  (let y2 (+ y (sget surf height)))
  (let z (sget t z))

  (if (not mouse-dragging) (do
    (def mouse-resizing 0)
    (let already-focused (sget t focused))
    (if (point-in-rect mouse-x mouse-y x y x2 y2)
      (if (not focus-given)
        (do
          (def focus-given 1)
          (sput t focused 1)) 0) 
      (sput t focused 0))) 0)

  (if mouse-dragging
    (if (sget t focused)
      (do
              (boxfill fb x y (- x2 x) (- y2 y) 0xffff)
              (if (or mouse-resizing (and (gt mouse-x (- x2 16)) (gt mouse-y (- y2 16))))
                (do ; resize
                  (let nw (+ (sget surf width) mouse-dx))
                  (let nh (+ (sget surf height) mouse-dy))
                  (sput surf width nw)
                  (sput surf height nh)
                  (sput surf pitch (* 2 nw))
                  (sput surf pixels (alloc (* 2 (* nw nh))))
                  (sput t needs-redraw 1)
                  (def mouse-resizing 1) ; sticky
                  )
                (do ; move
                  (sput surf x (+ mouse-dx x))
                  (sput surf y (+ mouse-dy y))) )
              (sput t redrawn 1) ) 0) 0)
  0
)))

(def focused-at-mouse (fn (t task) (do
  (let surf  (sget t surface))
  (let x (sget surf x))
  (let y (sget surf y))
  (let x2 (+ x (sget surf width)))
  (let y2 (+ y (sget surf height)))
  (if (point-in-rect mouse-x mouse-y x y x2 y2)
    (sget t focused) 0)
)))

(def run-tasks (fn (do
  (let tl tasks)
  (let new-tl (list))
  (let i 0)
  (let highest-z-at-mouse 0)

  (def focus-given 0)
  (while (car tl) (do
    (let task-item  (car tl))
    (let task-obj   (car (cdr task-item)))
    (def focus-given (or focus-given (focused-at-mouse task-obj)))
    (let tl (cdr tl))
  ))
  (let tl tasks)
  
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

(import "/sd/os/repl.l")
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

(def max-task-id 0)

(def spawn-editor (fn x y title (do
  (let my-editor (new editor))
  (let my-editor-task (new task))
  (sput my-editor-task id (+ max-task-id 1))
  (sput my-editor-task name title)
  (sput my-editor buffer " ")
  (sput my-editor-task focused 0)
  (sput my-editor-task surface (make-surface x y 400 240))
  (add-task editor-task my-editor-task my-editor)
  (def mask-task-id (+ max-task-id 1))
)))

(def spawn-repl (fn x y title (do
  (let my-repl (new repl))
  (let my-task (new task))
  (sput my-task id (+ max-task-id 1))
  (sput my-task name title)
  ;(sput my-repl buffer " ")
  (sput my-task focused 0)
  (sput my-task surface (make-surface x y 400 240))
  (let s (sget my-task surface))
  (boxfill s 0 0 400 240 0xffff)
  (sput my-task redrawn 1)
  (add-task repl-task my-task my-repl)
  (def mask-task-id (+ max-task-id 1))
)))

(spawn-editor 32 32 "editor               ")
(spawn-repl 32 300 "repl               ")

(def mouse-task (new task))
(sput mouse-task id (+ max-task-id 1))
(sput mouse-task name "mouse")
(add-task mouse-func mouse-task 0)

(def launched 0)

(def launcher-func (fn (t task) dummy (do
  (let f (sget t focused))
  (if (* f (* (not launched) mouse-btn)) (do
    (def launched 1)
    (spawn-editor 100 100 "new editor")
  ) 0)
  (if (not mouse-btn) (def launched 0) 0)
  0
)))

(def launcher-task (new task))
(sput launcher-task id (+ max-task-id 1))
(sput launcher-task name "icon")
(sput launcher-task surface (make-surface 0 0 32 32))
(sput launcher-task redrawn 1)
;(add-task launcher-func launcher-task 0)

(def main (fn (while 1 (do
  (run-tasks)
  (send screen 0)
  (gc)
  (def cursor-blink (% (+ cursor-blink 1) cursor-blink-delay))
))))

(main)
)
