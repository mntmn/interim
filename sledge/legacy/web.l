(

;;; Hosts ----------------------------------------------------------------

(def mntmn [5bfa730f])
(def facebook [1f0d5d03])
(def tokyo [c0a80168])
(def freenode [c0ba9d2b])
(def dwigns [5eba9793])

;;; HTTP -----------------------------------------------------------------

(def http-get (fn (do
  (def header (concat (concat "Host: " http-hostname) [0d0a0d0a]))
  (tcp-send (concat (concat (concat (concat "GET " http-path) " HTTP/1.1") [0d0a]) header))
)))

(def http-connect-handler (fn (http-get path)))

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
  (def html-attr (alloc-str 10))
  (def html-tag (alloc-str 10))
  (def attr-value (alloc-str 50))
  (def attr-i 0)
  (def tag-i 0)
  (def value-i 0)
  (def output-attr 0)

  (while (lt i rsz) (do
    (def c (uget raw i))
    (if (= html-parse-state 0) ; text
      (if (= c 60)
        (def html-parse-state 1)
      (if (or (= c 10) (= c 32))
        (do 
          (uput out j c)
          (def j (+ j 1)) (def l (+ l 1))
          (def html-parse-state 2)
        )
      (do
        (uput out j c)
        (def j (+ j 1))
        (def l (+ l 1))
      )))
    (if (= html-parse-state 1) (do ; tag
      (if (= c 62) (do
        (print (list "html parsed tag: " html-tag))
        (def tag-i 0)
        (def html-parse-state 0)
      ))
      (if (and (gt c 96) (lt c 123)) (do ; tag name
        (put html-tag tag-i c)
        (put html-tag (+ tag-i 1) 0)
        (def tag-i (+ tag-i 1))
      ))
      (if (= c 32) (do
        (print (list "html parsed tag: " html-tag))
        (def tag-i 0)
        (def attr-i 0)
        (def html-parse-state 3)))
      )
    (if (= html-parse-state 2) ; whitespace
      (if (not (or (= c 32) (= c 10)))
        (if (= c 60)
          (def html-parse-state 1)
        (do
          (def html-parse-state 0)
          (uput out j c)
          (def j (+ j 1))
          (def l (+ l 1))
        )))
    (if (= html-parse-state 3) (do ; space between attrs
      (if (= c 62) (do ; tag closed
        (def html-parse-state 0)))
      (if (= c 32) (do ; end of attr
        (print (list "html parsed attr: " html-attr))
        (def attr-i 0)
        (put html-attr 0 0)
      ))
      (if (= c 61) (do ; '=' attr value follows
        (print (list "html parsed attr=: " html-attr))
        (def attr-i 0)
        (def html-parse-state 4)
      ))
      (if (and (gt c 96) (lt c 123)) (do ; attr name
        (put html-attr attr-i c)
        (put html-attr (+ attr-i 1) 0)
        (def attr-i (+ attr-i 1))
      )))
    (if (= html-parse-state 4) (do ; beginning of attr value
      (if (str= "href" html-attr) (do
        (print "href attr!")
        (uput out j (get "~" 0)) (def j (+ j 1))
        (def output-attr 1) ; we want to output the link
      ) (def output-attr 0)) ; else don't output
      (def value-i 0)
      (if (or (= c 39) (= c 34))
        (def attr-sep c)
        (def attr-sep 32) ; else
      )
      (def html-parse-state 5) ; go to the actual value
    )
    (if (= html-parse-state 5) (do ; attr value
      (if (= c attr-sep) (do ; value closed
          (print (list "attr value parsed: " attr-value))
          (if output-attr (do
            (uput out j 32) (def j (+ j 1))
          ))
          (def html-parse-state 3)
          (def value-i 0)
        )
        (if (and (= attr-sep 32) (= c 62)) (do ; tag closed
            (if output-attr (do
              (uput out j 32) (def j (+ j 1))
            ))
            (def html-parse-state 0)
          )
          (do ; append to attr value
            (put attr-value value-i c)
            (put attr-value (+ value-i 1) 0)
            (def value-i (+ value-i 1))

            (if output-attr (do
              (uput out j c) (def j (+ j 1))
            ))
            
          ))
      ))
    ))))))
    
    (def i (+ i 1))
    
    (if (and (= c 32) (gt l 80)) (do
      (uput out j 10) ; wrap and linefeed
      (def j (+ j 1))
      (def l 0)
    ) 0)
  ))
  out
)))

(def http-handler (fn (do
  (buf-append 0 (html-parse network-input))
)))

(def hget (fn ip hostname path (do
  (set-buf-str 0 (concat "loading… " [0a]))
  (def html-parse-state 0)
  (def http-path path)
  (def http-hostname hostname)
  (tcp-connect ip 80 http-connect-handler http-handler))))

(def webtest (fn (hget dwigns "news.dieweltistgarnichtso.net" "/notes/nokia-n900.html")))

(def uri-parse (fn str (do
  (def uri-parts (split str ":"))
  (def uri-proto (car uri-parts))
  (def uri-parts2 (split (car (cdr uri-parts)) "/"))
  (def uri-host (car uri-parts2))
  (def uri-path (cdr uri-parts2))
  (list uri-proto uri-host uri-path)
)))


(def plumb (fn word (do
  (print (cons "plumb" word))
  (if (= (uget word 0) (uchr "/"))
    (set-buf-str 0 (load word)))
  (if (= (uget word 0) (uchr "~")) (do
    (def plumb-uri (uri-parse (substr word 1 (usize word))))
    (if (str= "http" (car plumb-uri)) (do
      (def plumb-host (car (cdr plumb-uri)))
      (def path (join (car (cdr (cdr plumb-uri))) "/"))
      (if (lt (usize path) 1) (def path "/"))
      (buf-append 0 (join (list "hget" plumb-host path "…") " "))
      (hget mntmn plumb-host path)
      )
      (do ; FIXME total hack
      (def path (concat "/" (substr word 1 (usize word))))
      (hget mntmn plumb-host path)
      )
    )
  ))
)))

)
