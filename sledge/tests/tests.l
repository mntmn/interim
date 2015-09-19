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
(def fib (fn n (if (lt n 3) 1
                 (+ (fib (- n 1)) (fib (- n 2))) )))

(test 14 (= (fib 10) 55))

(def foo (fn a b (+ a b)))
(test 15 (do (def a 5) (def b 6) (= (foo (+ a 1) (- b 2)) 10)))

(def func-a (fn xx yy (* (+ xx 1) (+ yy 1))))
(def func-b (fn x y (func-a x y)))

(test 16 (= 12 (strlen (concat "hello" "worlden"))))

(test 17 (= (get [65] 0) (get "beef" 1)))
(test 18 (= (get [66] 0) (get "beef" 3)))

(test 19 (lt 4 500))
(test 20 (gt -2 -4))
(test 21 (eq 666 (* -1 -666)))

(test 22 (= (get (substr "hellaz" 1 3) 0) 101))
