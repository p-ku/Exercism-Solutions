(import (rnrs))

(define (leap-year? year)
  (define (divis? x) (zero? (remainder year x)))
  (or (and (divis? 4) (divis? 100) (divis? 400))
      (and (divis? 4) (not (divis? 100)))))
