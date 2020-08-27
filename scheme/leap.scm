(import (rnrs))

(define (leap-year? year)
  (define (divis? x) (= (remainder year x) 0))
  (or (and (divis? 4) (divis? 100) (divis? 400))
      (and (divis? 4) (not (divis? 100)))))
