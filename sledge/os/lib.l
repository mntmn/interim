(
(def load (fn path (recv (open path))))
(def import (fn path (eval (read (recv (open path))))))

(def and (fn a b (if a (if b 1 0) 0)))
(def or  (fn a b (if a a b)))
(def not (fn a (if a 0 1)))

(def item (fn lst idx (do
  (let i 0)
  (let l lst)
  (while (gt idx i) (do
    (let l (cdr l))
    (let i (+ i 1))))
  (car l)
)))

(def list-size (fn lst (do
  (let i 0)
  (let l lst)
  (while (car l) (do
    (let l (cdr l))
    (let i (+ i 1))))
  i
)))

(def reverse (fn lst (do
  (let l (list))
  (while (car lst) (do
    (let l (cons (car lst) l))
    (let lst (cdr lst))
  ))
  l
)))

(def split (fn str sepstr (do
  (let sep (get8 sepstr 0))
  (let result (quote ()))
  (let sz (size str))
  (let i 0)
  (let i (- sz 1))
  (let last-i 0)
  (let last-i (+ i 1))
  (let partsize 0)
  
  (while (gt i -2) (do
    (if (lt i 0)
      (let c 0)
      (let c (get8 str i)))
    (if (or (eq c sep) (eq i -1)) (do
      (let partsize (- (- last-i i) 1))
  
      (if (gt partsize -1)
        (let result (cons (substr str (+ i 1) partsize) result)) 0)
      (let last-i i)
    ) 0)
    (let i (- i 1))
  ))
  result
)))

(def copy (fn buf from to num (do
  (let i 0)
  (let c 0)
  
  (if (lt from to)
    (do
      (let i (- num 1)) 
      (while (gt i -1) (do
        (let c (get8 buf (+ from i)))
        (put8 buf (+ to i) c)
        (let i (- i 1))
      )) 0)
    (do
      (let i 0)
      (while (lt i num) (do
        (let c (get8 buf (+ from i)))
        (put8 buf (+ to i) c)
        (let i (+ i 1))
      )) 0)
  )
  num
)))

; fixme use substr + concat instead of mutation

(def remove (fn buf pos (do
  (let p (+ pos 0))
  (let from (+ pos 1))
  (let num (- (size buf) pos))
  (copy buf from p num)
  (put8 buf (- (size buf) 1) 0)
  buf
)))

(def insert (fn buf pos k (do
  (let p (+ pos 0))
  (let to (+ pos 1))
  (let c (+ k 0))
  (let num (- (size buf) (+ pos 1)))
  (copy buf p to num)
  (put8 buf p c)
  buf
)))

(def strlen (fn s (if s (do
  (let i 0)
  (let sz (size s))
  (let c (get8 s 0))
  (while (and (gt c 0) (lt i sz)) (do
    (let i (+ i 1))
    (let c (get8 s i))
  ))
  i) 0)
))

(def assoc ; assoc which only returns the cdar
(fn key al
  (do
    (let rest al)
    (let result nil)
    (while rest (do
      (if (eq key (car (car rest)))
        (do
          (let result (cdr (car rest)))
          (let rest nil))
        (let rest (cdr rest)))))
    result)))

(def acons (fn key val lst (cons (cons key val) lst)))

(def sintab [80828486888b8d8f919496989a9c9ea1a3a5a7a9abadafb2b4b6b8babcbec0c1c3c5c7c9cbcdced0d2d3d5d7d8dadcdddfe0e2e3e4e6e7e8eaebecedeeeff1f2f3f4f4f5f6f7f8f9f9fafbfbfcfcfdfdfefefefffffffffffffffffffffffffffffffefefefdfdfcfcfbfbfaf9f9f8f7f6f5f4f4f3f2f1efeeedecebeae8e7e6e4e3e2e0dfdddcdad8d7d5d3d2d0cecdcbc9c7c5c3c1c0bebcbab8b6b4b2afadaba9a7a5a3a19e9c9a989694918f8d8b88868482807d7b79777472706e6b69676563615e5c5a58565452504d4b49474543413f3e3c3a38363432312f2d2c2a2827252322201f1d1c1b1918171514131211100e0d0c0b0b0a0908070606050404030302020101010000000000000000000000000000000101010202030304040506060708090a0b0b0c0d0e1011121314151718191b1c1d1f2022232527282a2c2d2f31323436383a3c3e3f41434547494b4d50525456585a5c5e61636567696b6e70727477797b7d])

(def sin (fn deg (get8 sintab (% deg 360))))
(def cos (fn deg (get8 sintab (% (+ deg 90) 360))))
(def abs (fn a (if (lt a 0) (- 0 a) a)))
)
