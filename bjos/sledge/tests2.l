
(def s "abcdefghijklmnopqrstuvwxyz0123456789ABCD")
(def i 0)
(while (lt i 40) (do (print (cons (get s (+ i 1)) (get s i))) (def i (+ i 1))))


(def i 0)
(def j 0)
(def s "........................................")

(def loopit (fn
(while 1 (do
(def i 0)
(def j 0)
(while (lt j 40) (do
(def s "........................................")
(def i 0)
(while (lt i 40) (do (put s i (get "*" 0)) (put s (- i j) (get "." 0)) (def i (+ i 1)) (print s)))
(def j (+ j 1))
))
(gc)
))))

(def test (fn a b (print (+ a (+ a b)))))

(def test (fn x y color (print (+ (* y 1920) x))))


# todo: put32   put64
# todo: sane for/range/generator

(def fbfile (open "/framebuffer"))
(def pixel (fn x y color (put32 fb (* 4 (+ x (* y 1920))) color)))

(def y 50)
(def x 50)
(def c 0)
(def paint (fn (do
(def y 50)
(def x 50)
(while (lt y 640) (do
  (def x 0)
  (while (lt x 480) (do
    (pixel x y (+ c (* x y)))
    (def x (+ x 1))
  ))
  (def y (+ y 1))
))
(def c (+ c 0x020202))
(send fbfile 0)
(gc)
)))

(while 1 (paint))


(def fbfile (open "/framebuffer"))
(def pixel (fn x y color (do (put fb (* 4 (+ x (* y 1920))) color) (send fbfile 0))))


(def con (open "/console"))

(def buf (alloc-str 128))
(def c 32)
(def i 0)

(def readline (fn (do
  (def buf (alloc-str 128))
  (def c 32)
  (def i 0)
  (while (gt c 10) (do (def c (recv con)) (put buf i c) (def i (+ i 1))))
  buf
)))


(def f1 (fn a b (* a b)))
(def f2 (fn a b c d (+ (f1 b a) (f1 d c))))
(f2 4 3 2 1)

(let a 3)
