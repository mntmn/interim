(def import (fn path (eval (read (recv (open path))))))
(import "/sd/os/lib.l")

(def foo (fn "foo"))
(foo)
(cons 1 2)
(cons "foo" 1)
(cons 1 "foo")
(cons "a" (cons "b" "c"))

(def test (fn tn tx (print (list "test " tn (if tx "OK" "FAIL")))))  
(def = eq)
(def not (fn a (if a 0 1)))

(test 1 (= 1 1))
(test 2 (= 2 2))
(test 3 (not (= 1 0)))
(test 4 (not (= 0 1)))
(test 6 (= + +))
(test 7 (= (+ 1 2) (+ 2 1)))
(test 8 (def a 1))
(test 9 (not (= a (def b (+ a 1)))))
(test 10 (= a a))
(test 11 (= b 2))

(test 12 (= 4 (do 1 2 "foo" 4)))
(test 13 (= 3 (size (do 1 2 "foo"))))

(def fib (fn n 0))
(def fib (fn n (if (lt n 3) 1 (+ (fib (- n 1)) (fib (- n 2))) )))

(test 14 (= (fib 10) 55))

(def foo (fn a b (+ a b)))
(test 15 (do (def a 5) (def b 6) (= (foo (+ a 1) (- b 2)) 10)))

(test 17 (= (get [65] 0) (get "beef" 1)))
(test 18 (= (get [66] 0) (get "beef" 3)))

(test 19 (lt 4 500))
(test 20 (gt -2 -4))
(test 21 (eq 666 (* -1 -666)))

(test 22 (= (get (substr "hellaz" 1 3) 0) 101))

(def fun1 (fn a b c (list a b c)))
(def fun2 (fn a b c (fun1 c a b)))

(print (fun2 2 3 1))

(test 23 (= 5 (+ (+ (+ 1 1) 1) (+ 1 1))))
(test 24 (= 4 (+ (+ 1 1) (+ 1 1))))
(test 25 (= 3 (+ (+ 1 1) 1)))

(def func-a (fn xx yy (* (+ xx 1) (+ yy 1))))
(def func-b (fn x y (func-a x y)))
(test 26 (= (func-b 5 5) 36))

(test 16 (= 12 (strlen (concat "hello" "worlden"))))

(def lett (fn g (do
  (let a 23)
  (let b 46)
  (let c 66)
  (let d 72)
  (let e "foo")
  (list g a b c d e g)
)))

(print (lett 6))

(def spillover (fn a b c d e f (do
  (print (list a b c d e f))
  (print (list f e d c b a))
)))
(spillover 1 2 3 4 5 6)

(def fa (fn x y z w (do
  (let a 8)
  (let b 9)
  (print a)
  (print b)
  (print x) ; gives 2, should give 1
  (print y)
  (print z)
  
  (print (list "fa: " a b x y z w))
)))

(def fb (fn a b (do
  (print a)
  (print b)
  (fa a b "3" "4")
)))

(fb 1 2)
