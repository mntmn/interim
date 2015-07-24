(def item (fn lst idx (do
  (def i 0)
  (while (gt idx i) (do
    (def lst (cdr lst))
    (def i (+ i 1))))
  (car lst)
)))

(def reverse (fn lst (do
  (def res (list))
  (while (car lst) (do
    (def res (cons (car lst) res))
    (def lst (cdr lst))
  ))
  res
)))

(def replace-item (fn lst pos new-value (do
  (def res (list))
  (def i 0)
  (while (car lst) (do
    (if (= pos i)
      (def res (cons new-value res))
      (def res (cons (car lst) res)))
    (def lst (cdr lst))
    (def i (+ i 1))
  ))
  (reverse res)
)))

(def mylist (list 1 2 3 4))

(replace-item mylist 0 23)
(replace-item mylist 1 (cons 1 2))
(replace-item mylist 2 "foo")

mylist

