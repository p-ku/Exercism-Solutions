(import (rnrs))

(define (leap-year? year)
  (define (divis? x) (zero? (remainder year x)))
  (or (divis? 400)
      (and (divis? 4) (not (divis? 100)))))
