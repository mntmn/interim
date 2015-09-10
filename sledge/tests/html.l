(def html-parse-state 0)
; 0: in text
; 1: in tag
; 2: whitespace

(def html-parse (fn raw (do
  (def i 0)
  (def j 0)
  (def k 0)
  (def l 0)
  (def rsz (usize raw))
  (def out (alloc-str rsz))
  (def html-tag (alloc-str 10))

  (while (lt i rsz) (do
    (def c (uget raw i))
    (if (= html-parse-state 0)
      (if (= c 60)
        (def html-parse-state 1)
      (if (or (= c 10) (= c 32))
        (do 
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
          (def html-parse-state 2)
        )
      (do
        (uput out j c)
        (def j (+ j 1))
        (def l (+ l 1))
      )))
    (if (= html-parse-state 1)
      (if (= c 62)
        (def html-parse-state 0)
        0)
    (if (= html-parse-state 2)
      (if (not (or (= c 32) (= c 10)))
        (if (= c 60)
          (def html-parse-state 1)
        (do
          (def html-parse-state 0)
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
        )) 0)
    0)))
    (def i (+ i 1))
    
    (if (and (= c 32) (gt l 80)) (do
      (uput out j 10)
      (def j (+ j 1))
      (def l 0)
    ) 0)
  ))
  out
)))
