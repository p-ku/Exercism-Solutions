(import (rnrs))

(define (pascal n x)
  (cond ((= x n) 1) 
        ((= x 1) 1)
  (else (+ (pascal (- n 1) x) 
           (pascal (- n 1) (- x 1)))))) 
