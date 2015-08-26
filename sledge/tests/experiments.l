
(def net (open "/net"))

(def netblit (fn (do
  (while 1 (do
  (let packet (recv net))
  (let sz 0)
  (let sz (size packet))

  (if sz (do
    (let i 0)
    (let j 0)
    (let b 0)
    (let ofs 0)
    (print sz)
    (while (lt i sz) (do
      (let b (get packet i))
      (put fb ofs b)
      (let i (+ i 1))
      (let ofs (+ ofs 1))
      (let j (+ j 1))
      (if (gt j 127) (do
        (let j 0)
        (let ofs (+ ofs (- screen-pitch 128)))
      ) 0)
    ))
  ) 0)

  (print "about to gc") ; FIXME gc crashes here
  ))
)))


; fixme crashes on device
(def graph (fn center max (do
  (let x 0)
  (while (lt x max) (do
    (print x)
    (set-pixel x (+ center (sin (sin x))) stroke-color)
    (let x (+ x 1))
  ))
  x
  )))
