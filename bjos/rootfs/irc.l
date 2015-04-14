(
;;; IRC ------------------------------------------------------------------

(def irc-connect-handler (fn (do
  (buf-append 0 "connected!")
  (tcp-send "PASS *")
  (tcp-send [0a])
  (tcp-send "NICK bomberjacket_")
  (tcp-send [0a])
  (tcp-send "USER bomberjacket_ 8 * :Bomberjacket OS")
  (tcp-send [0a])
  (tcp-send "JOIN #bomberjacket")
  (tcp-send [0a])
)))

(def irc-handler (fn (do
  (print "IRC-HANDLER!")
  (print (list "input:" network-input))
  (buf-append 0 network-input)
)))

(def irc-msg (fn msg (do
  (def ircbuf (concat "PRIVMSG #bomberjacket :" msg))
  (tcp-send ircbuf)
  (tcp-send [0a])
)))

(def irc (fn
  (tcp-connect freenode 6667 irc-connect-handler irc-handler)))

)
