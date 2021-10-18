#lang racket

(provide rough-eq?
         index-2d
         clamp
         )

(define (rough-eq? a b)
  (let ([eps 0.00001])
    (< (abs (- a b)) eps)))

(define (index-2d x y w)
  (+ (* y w) x))

(define (clamp l x h)
  (if (< x l)
    l
    (if (> x h)
      h
      x)))
