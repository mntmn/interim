(
(def uchr (fn s (uget s 0)))

(def substr_ (fn str beg sz (do
  (def res (alloc-str (* 2 sz)))
  (def ii 0)
  (while (lt ii sz) (do
    (uput res ii (uget str (+ beg ii)))
    (def ii (+ ii 1))
  ))
  res
)))

(def ufind-prev (fn buf rune pos (do
  (while (and (gt pos 0) (not (= rune (uget buf pos))))
    (def pos (- pos 1)))
  pos
)))

(def ufind-next (fn buf rune pos (do
  (def sz (usize buf))
  (while (and (lt pos sz) (not (= rune (uget buf pos))))
    (def pos (+ pos 1)))
  pos
)))

(def ufind-prev-ws (fn buf pos (do
  (while (and (gt pos 0) (not (or (= 13 (uget buf pos)) (or (= 10 (uget buf pos)) (= 32 (uget buf pos))))))
    (def pos (- pos 1)))
  (if (= pos 0) 0 pos)
)))

(def ufind-next-ws (fn buf pos (do
  (while (and (lt pos (usize buf)) (not (or (= 13 (uget buf pos)) (or (= 10 (uget buf pos)) (= 32 (uget buf pos))))))
    (def pos (+ pos 1)))
  pos
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

(def uremove (fn buf pos (do
  (ucopy buf (- pos 1) (- pos 2) (- (usize buf) (- pos 1)))
  (uput buf (- (usize buf) 1) 0)
  0
)))

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

(def join (fn lst jstr (do
  (def joined "")
  (while (car lst) (do
    (def tmp (alloc-str 128))
    (write tmp (car lst))
    (def tmp (substr tmp 1 (- (usize tmp) 2)))
    (def joined (concat (concat joined jstr) tmp))
    (print joined)
    (def lst (cdr lst))
  ))
  joined
)))

(def str= (fn str-a str-b (do
  (def si 0)
  (def equal 1)
  (while (and equal (lt si (size str-a))) (do
    (if (not (= (get str-a si) (get str-b si))) (def equal 0)) 
    (def si (+ si 1))
  ))
  equal
)))
)
