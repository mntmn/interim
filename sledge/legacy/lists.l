(
(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))
(def and (fn a b (if a (if b 1 0) 0)))
(def or (fn a b (if a 1 (if b 1 0) 0)))

(def length (fn lst (do
  (def i 0)
  (while (cdr lst) (do
    (def i (+ i 1))
    (def lst (cdr lst))
  ))
  i
)))

(def reverse (fn lst (do
  (def res (list))
  (while (cdr lst) (do
    (def res (cons (car lst) res))
    (def lst (cdr lst))
  ))
  res
)))

(def item (fn lst idx (do
  (def i 0)
  (while (gt idx i) (do
    (def lst (cdr lst))
    (def i (+ i 1))))
  (car lst)
)))

(def replace-item (fn lst pos new-value ((fn res i (do
  (while (cdr lst) (do
    (if (= pos i)
      (def res (cons new-value res))
      (def res (cons (car lst) res)))
    (def lst (cdr lst))
    (def i (+ i 1))
  ))
  (reverse res)
  ))
  
  (list) 0)
))
)
