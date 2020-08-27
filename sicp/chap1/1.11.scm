(import (rnrs))

(define (fn n)
  (if (< n 3)
    n
    (+ (fn (- n 1)) (* 2 (fn (- n 2))) (* 3 (fn (- n 3))))))
(define (fn2 n)
  (define (fniter a b c counter)
    (cond (( counter maxcount) ) 
      result
      (+ (fn2 (- n 1)) (* 2 (fn2 (- n 2))) (* 3 (fn2 (- n 3))))))
  (fniter 3 n) 
    
