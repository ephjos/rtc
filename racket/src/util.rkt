#lang racket/base

(provide rough-eq?
         )

(define (rough-eq? a b)
  (let ([eps 0.00001])
    (< (abs (- a b)) eps)))

