(
(def mouse (open "/mouse"))
(def mouse-x 0)
(def mouse-y 0)
(def mouse-dx 0)
(def mouse-dy 0)
(def mouse-btn 0)
(def mouse-task (fn (do
  (add-task (fn (do
    (blit-char 32 mouse-x mouse-y)
    
    (let mouse-info (recv mouse))
    (def mouse-dx (car (car mouse-info)))
    (def mouse-dy (cdr (car mouse-info)))
    (def mouse-x (+ mouse-x mouse-dx))
    (def mouse-y (+ mouse-y mouse-dy))
    (if (lt mouse-x 0) (def mouse-x 0) 0)
    (if (lt mouse-y 0) (def mouse-y 0) 0)
    (if (gt mouse-x (- screen-width 1))  (def mouse-x (- screen-width 1)) 0)
    (if (gt mouse-y (- screen-height 1)) (def mouse-y (- screen-height 1)) 0)
    
    (def mouse-btn (cdr mouse-info))

    (if mouse-btn (blit-char 0x219c mouse-x mouse-y)
      (blit-char 0x2196 mouse-x mouse-y))

  )) (list))
)))
)
