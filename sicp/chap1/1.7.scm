(import (rnrs))

(define (square x) (* x x))
(define (average x y)
  (/ (+ x y) 2))
(define (improve guess x)
    (average guess (/ x guess)))

(define (oldsqrt x)
  (define (goodenough? guess x)
    (< (abs (- (square guess) x)) 0.001))
  (define (sqrtiter guess x)
    (display guess)
    (display " ")
    (if (goodenough? guess x)
      guess
      (sqrtiter (improve guess x)
        x)))
  (sqrtiter 1.0 x))

(define (newsqrt x)
  (define (goodenough? guess x)
    (< (/ (abs (- (square guess) x)) guess) 0.001))
  (define (sqrtiter guess x)
    (display (/ (abs (- guess x))))
    (display " ")
    (if (goodenough? guess x)
      guess
      (sqrtiter (improve guess x)
        x)))
  (sqrtiter 1.0 x))
