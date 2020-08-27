(import (rnrs))

(define (exponent b n)
  (define (exptiter a b counter)
    (define (even? n)
      (= (remainder n 2) 0))
    (cond ((= counter 0) a)
          ((even? counter) (* a (exptiter 1 b (- (- counter 1) 2))))
          (else (* b (exptiter 1 b (- (- counter 1) 2))))))
  (exptiter (* b b) b n))

