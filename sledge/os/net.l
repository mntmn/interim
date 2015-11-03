
(def net (open "/net"))
(def net-y 32)

(def irc-msg (fn msg (do
  (let ircbuf (concat "PRIVMSG #nodrama.de :" msg))
  (send net ircbuf)
  (send net [0a])
)))

(def cmdbuf (alloc-str 512))
(def cmd-read (list))
(def remote-cmd (fn msg (do
  (let parts (split msg "$"))
  (if (gt (list-size parts) 1) (do
    (let cmd (concat (concat "(" (item parts 1)) ")"))
    (print (list "remote cmd" cmd))
    (def cmdbuf (alloc-str 512))
    (def cmd-read (read cmd))
    (write (eval cmd-read) cmdbuf)
    (print (list "result" cmdbuf))
    (irc-msg cmdbuf)
  ) 0)
  0
)))

(def freenode "/net/tcp/62.231.75.133/6667")
(def sternfreunde "/net/tcp/46.101.207.85/80")
(def interim-os "/net/tcp/91.250.115.15/80")

(def connect (fn net-path (do
  (def net (open net-path))
)))

(def net-task (fn (do
  (add-task (fn (do
    (let packet (recv net))
    (if (size packet) (do
      (def temp-minx minx)
      (def temp-maxx maxx)

      (def minx 1000)
      (def maxx 1700)
      (let msg (bytes->str packet))

      (boxfill 1000 net-y 716 64 0xffff)
      (let ofsy (+ (blit-str msg minx net-y) rune-h))
      
      (def minx temp-minx)
      (def maxx temp-maxx)
      (def net-y (+ 0 ofsy))
      (if (gt net-y maxy) (def net-y miny) 0)

      ; only enable this when you know what you're doing
      ; (remote-cmd msg)
    ) 0)
  )))
  1
)))

(def http-get (fn host path (do
  (boxfill 1000 0 800 1000 0xffff)
  (let header (concat (concat "Host: " host) (bytes->str [0d0a0d0a])))
  (send net (concat (concat (concat (concat "GET " path) " HTTP/1.1") (bytes->str [0d0a])) header))
)))

(def irc-join (fn nick channel (do
  (send net "PASS *")
  (send net [0a])
  (send net (concat "NICK " nick))
  (send net [0a])
  (send net (concat "USER " (concat nick " 8 * :Interim OS")))
  (send net [0a])
  (send net (concat "JOIN " channel))
  (send net [0a])
)))
