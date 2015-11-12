(
(def mouse (open "/mouse"))
(def mouse-x 0)
(def mouse-y 0)
(def mouse-oldx 0)
(def mouse-oldy 0)
(def mouse-dx 0)
(def mouse-dy 0)
(def mouse-btn 0)
(def mouse-dragging 0)
(def mouse-func (fn (t task) dummy (do
  (blit-char16 fb unifont 32 mouse-x mouse-y)
    
  (let mouse-info (recv mouse))
  (def mouse-x (car (car mouse-info)))
  (def mouse-y (cdr (car mouse-info)))

  (def mouse-dx (- mouse-x mouse-oldx))
  (def mouse-dy (- mouse-y mouse-oldy))
  (def mouse-oldx (+ mouse-x 0))
  (def mouse-oldy (+ mouse-y 0))
  
  ;(def mouse-x (+ mouse-x mouse-dx))
  ;(def mouse-y (+ mouse-y mouse-dy))
  ;(if (lt mouse-x 0) (def mouse-x 0) 0)
  ;(if (lt mouse-y 0) (def mouse-y 0) 0)
  ;(if (gt mouse-x (- screen-width 1))  (def mouse-x (- screen-width 1)) 0)
  ;(if (gt mouse-y (- screen-height 1)) (def mouse-y (- screen-height 1)) 0)
    
  (def mouse-btn (cdr mouse-info))
  (def mouse-dragging mouse-btn)

  (if mouse-btn (blit-char16 fb unifont 0x219c mouse-x mouse-y)
    (blit-char16 fb unifont 0x2196 mouse-x mouse-y))
)))
)
