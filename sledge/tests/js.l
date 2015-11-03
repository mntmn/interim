(
(def st-atom 0)
(def st-num  1)
(def st-str  2)
(def st-arr  3)
(def st-sym  4)

(def curlies  0)
(def brackets 0)
(def parens   0)

(def js-pst st-atom)

(def cur-string "")
(def cur-string-idx 0)
(def cur-number 0)
(def cur-symbol nil)
(def js-opstate 0)
(def js-argstate 0)
(def cur-op 0)
(def js-callable 0)
(def js-stack (list))
(def js-args (list))

(def is-letter (fn c
  (or (eq c 95) ; _
      (or
      (and (gt c 96) (lt c 123)) ; a-z
      (and (gt c 64) (lt c 90))  ; A-Z
))))

(def is-digit (fn c
  (and (gt c 47) (lt c 58)) ; 0-9
))

(def is-space (fn c
  (or (eq c 32) (or (eq c 10) (eq c 13)))
))

(def is-operator (fn c
  (if (eq c 43)  (quote +)
    (if (eq c 45)  (quote -)
    (if (eq c 42)  (quote *)
    (if (eq c 47)  (quote /)
    (if (eq c 37)  (quote %)
    (if (eq c 60)  (quote lt)
    (if (eq c 62)  (quote gt)
    (if (eq c 124) (quote bitor)
    (if (eq c 38)  (quote bitand)
    0)))))))))
))

(def js-emit-operator (fn c lhs rhs (do
  (def js-opstate 0)  
  (def js-stack (cons (is-operator c) (list lhs rhs)))
)))

(def push-js (fn x (do
  (if (car js-stack)
    (def js-stack (cons x js-stack))
    (def js-stack x)
  )
)))

(def js-emit-value (fn v (do 
  (if (eq 2 js-opstate) (do
    (js-emit-operator cur-op js-stack v)
  )
  (push-js v))
)))

; if js-argstate, collect args instead
(def js-emit-expr (fn (do
  (if js-argstate (do
    (def js-args (cons js-args js-args)))
    (do
      (print js-stack)
      (print (eval (cons js-stack nil))))
  )
  (def js-stack (list))
  (def js-pst st-atom)
  (def js-opstate 0)
  (def js-callable 0)
)))

(def js-parse-atom (fn c (do
  (if (is-operator c) (do
    (def cur-op c)
    (def js-opstate 2)
  )
  (if (eq c 59) (do
    (js-emit-expr)
  )
  (if (eq c 40) (do
    (def parens (+ parens 1))
    (if js-callable (do
      (def js-argstate 1)
    ) 0)
  )
  (if (eq c 41) (do
    (def parens (- parens 1))
    (if (lt parens 0) (print "syntax error") 0)
    (if js-argstate (do
      (print "function call!")
      (def js-argstate 0)
      (if (car js-args)
        (def js-stack (list js-stack js-args))
        (def js-stack (list js-stack)))
    )
    (do
      (print "parens")
    )
    )
  )
  0))))
)))

(def js-parse-char (fn c (do

  (if (eq js-pst st-atom)
    (if (is-letter c) (do
      (def js-pst st-sym)
      (def cur-string (alloc-str 16))
      (put cur-string 0 c)
      (def cur-string-idx 1)
      (def js-callable 1)
    )
    (if (is-digit c) (do
      (def js-pst st-num)
      (def cur-number (- c 48))
      (def js-callable 0)
    )
    (js-parse-atom c)
    ))

  (if (eq js-pst st-sym)
    (if (or (is-letter c) (is-digit c)) (do
      (put cur-string cur-string-idx c)
      (def cur-string-idx (+ cur-string-idx 1))
    )
    (do
      (js-emit-value (read cur-string))
      (def js-pst st-atom)
      (js-parse-atom c)
    ))

  (if (eq js-pst st-num)
    (if (is-digit c) (do
      (def cur-number (+ (- c 48) (* 10 cur-number)))
    )
    (do
      (js-emit-value cur-number)
      (def js-pst st-atom)
      (js-parse-atom c)
    ))

  0)))
  0
)))

(def js (fn str (do
  (def js-pst st-atom)

  (let l (size str))
  (let i 0)
  (while (lt i l) (do
    (js-parse-char (get str i))
    (let i (+ i 1))
  ))
  (js-parse-char 0)
  (js-emit-expr)
)))

)
