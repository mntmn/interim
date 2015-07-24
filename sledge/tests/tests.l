(def foo (fn "foo"))
(foo)
(cons 1 2)
(cons "foo" 1)
(cons 1 "foo")
(cons "a" (cons "b" "c"))

(def test (fn tn tx (cons (cons "test " tn) (if tx "OK" "FAIL"))))  
(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))

(test 1 (= 1 1))
(test 2 (= 2 2))
(test 3 (not (= 1 0)))
(test 4 (not (= 0 1)))
(test 5 (not (= "hello" "hello")))
(test 6 (= + +))
(test 7 (= (+ 1 2) (+ 2 1)))
(test 8 (def a 1))
(test 9 (not (= a (def b (+ a 1)))))
(test 10 (= a a))
(test 11 (= b 2))

(test 12 (= 4 (do 1 2 "foo" 4)))
(test 13 (= 3 (size (do 1 2 "foo"))))

;(def fib (fn n (if (lt n 3)
;                   1
;                 (+ (fib (- n 1)) (fib (- n 2))) )))

(test 14 (= (fib 10) 55))

(def foo (fn a b (+ a b)))
(test 15 (do (def a 5) (def b 6) (= (foo (+ a 1) (- b 2)) 10)))

(def func-a (fn xx yy (* (+ xx 1) (+ yy 1))))
(def func-b (fn x y (func-a x y)))

(def tst (fn a b (blit-mono unifont a b 0 0 0 0 0 0)))
