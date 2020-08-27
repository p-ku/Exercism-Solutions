(import (rnrs))
(define (inc n) (+ n 1))
(define (cube x) (* x x x))

(define (product term a next b)
  (if (> a b)
     1 
     (* (term a)
        (product term (next a) next b))))
