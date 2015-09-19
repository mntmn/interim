(def demo (fn
(do (let c 0)
    (let a 1024)
  (while 1 (do
    (let c (+ 1 (% (+ c 1) 256)))
    (let b 768)
    (while (let b (- b 1))
      (do
        (let a 512)
        (let c (+ 1 c))
        (while (let a (- a 1))
          (set-pixel (+ a 512) b (* (+ c a) (/ (* a b) c))))))
    (send scr 0))))
))
