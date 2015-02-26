(def = (fn a b (if (- a b) 0 1)))
(def not (fn a (if a 0 1)))

(def unifont (load "unifont"))
(def white 16777215)
(def cyan 65535)
(def color white)
(def unifont-width 4128)
(def unifont-height 4160)

(def blit-char (fn rune x y color 
  (blit-mono unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ unifont-width 8) 2 16 x y color)
))

(def blit-char-inv (fn rune x y color (do
  (blit-mono-inv unifont 
    (* (+ 2 (% rune 256)) 2) 
    (* (+ 4 (/ rune 256)) 16) (/ unifont-width 8) 2 16 x y color)
)))

(def cursor-size 1)

(def debug (fn o (write eval-buf o)))

; TODO refactor
(def blit-str (fn str xx yy color (do
  (def l (size str))
  (def i (+ 1 l))
  (def xxx (+ xx 0))
  (while (def i (- i 1)) (do
    (def chr (uget str (- l i)))
    (def xxx (+ xxx 8))
    (if (* (gt (- l i) (- cursor 1)) (lt (- l i) (+ cursor cursor-size)))
      (blit-char-inv chr xxx yy color)
      (if (= chr 32) 0
        (blit-char chr xxx yy color)))
    (if (- chr 10) 0 (do (def yy (+ yy 16)) (def xxx (+ xx 0))))
    (if (= chr 0) (def i 1) 0)
  ))
)))

(def cursor 0)
(def running 1)
(def cmd-mode 0)

(def buf "                  
                                                                                             
                                                                                             
                                                                                             
                                                                                             
                                                                           
; welcome to sledge/x86. ☕                                                          
                                                                                            
                                                                                              
                                                                                             
                                                                                             
                                                                              ")
(def eval-buf "                                                                                                                                                                                                                                                                                                        ")

(def help-buf "[e] eval buffer [tab] toggle menu")

(def padding 20)

(def buf-render (fn b bx by (do
  (rectfill bx by (- 1024 bx) (- 768 by) color)
  (rectfill bx by 1024 1 0)
  (blit-str b (+ padding bx) (+ padding by) color)
)))

(def exit (fn (do
;  (save buf "buffer")
;  (def running 0)
)))

(def toggle-command (fn (do
  (if (= color cyan)
    (def color white)
    (def color cyan))
  (def cmd-mode (- 1 cmd-mode))
)))

(def chr (fn s (uget s 0)))

(def handle-command-key (fn k (do
  (def kidx k)
  (def kchr (uget keymap kidx))
  
  (if (= kchr (chr "e")) (do
    (def eval-tmp (eval buf))
    (write eval-buf eval-tmp)
  ) 0)
  (if (= k 1) (toggle-command) 0)
)))

(def keymap (load "keymap"))

(def ufind-prev (fn buf rune pos (do
  (while (* (gt pos 0) (not (= rune (uget buf pos))))
    (def pos (- pos 1)))
  (+ pos 0)
)))

(def ufind-next (fn buf rune pos (do
  (while (* (lt pos (usize buf)) (not (= rune (uget buf pos))))
    (def pos (+ pos 1)))
  (+ pos 0)
)))

(def cursor-left (fn modif (do
  (if (= modif 1)
    (def cursor-size (- cursor-size 1))
    (def cursor (- cursor 1)))
  (if (lt cursor-size 1) (def cursor-size 1))
)))


(def cursor-right (fn modif (do
  (if (= modif 1)
    (def cursor-size (+ cursor-size 1))
    (def cursor (+ cursor 1)))
)))

(def handle-editor-key (fn k modif (do
  (if (= k 1) (toggle-command)
    (if (= k 79) (cursor-right)
      (if (= k 80) (cursor-left)
        (if (= k 42) (def cursor (- cursor 1))
          (if (= k 82) (def cursor (ufind-prev buf 10 (- cursor 1)))
            (if (= k 81) (def cursor (+ 1 (ufind-next buf 10 (+ cursor 1))))
              (if (= modif 1024) (handle-command-key k)

  (if (* (gt k 0) (lt k 64))
    (do
      (def kidx (if (+ (= 1 modif) (= 2 modif)) (+ k 128) k))
      (def kchr (uget keymap kidx))
      (uput buf cursor kchr)
      (def cursor (+ cursor 1))
    )
  0)
  
  )))))))
)))

(def network-task (fn (do
  (def u (udp-poll))
  (def us (usize u))
  (def ui 0)
  
  (while (* (lt ui (size buf)) (lt ui us)) (do
    (def ui (+ ui 1))
    (uput eval-buf ui (uget u ui))
  ))
)))

(while running (do
  (buf-render buf 0 0)
  (buf-render eval-buf 0 650)
  (if cmd-mode (buf-render help-buf 0 700) 0)
  (flip)

  (network-task)

  (def k (inkey 0))
  (def modif (inkey 1))

  (if cmd-mode
    (handle-command-key k modif)
    (handle-editor-key k modif))
  
  (if (lt cursor 0) (def cursor 0) 0)
  (if (not (lt cursor (size buf))) (def cursor (- (size buf) 1)) 0)
))
