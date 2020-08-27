(import (rnrs))

(define (cube x) (* x x x))
(define (average x y)
  (/ (+ x y) 2))
(define (improve guess x)
    (/ (+ (* 2 guess) (/ x (* guess guess))) 3))

(define (cbrt x)
  (define (goodenough? guess x)
    (< (abs (- (cube guess) x)) 0.001))
  (define (cbrtiter guess x)
    (display (/ (abs (- guess x))))
    (display " ")
    (if (goodenough? guess x)
      guess
      (cbrtiter (improve guess x)
        x)))
  (cbrtiter 1.0 x))
