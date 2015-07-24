
(def x (fn a (do (let b 2) (+ a b))))

(def y (fn a (do (let b 2) (let c 3) (+ a (+ b c)))))

(def y (fn a (do (let b "foo-") (let c "-bar") (concat b (concat a (concat b c))))))
