#lang racket

(require "./util.rkt")

(provide make-color
         color-eq?
         color-add
         color-sub
         color-mul
         color-scale
         color-to-display)

(define (make-color r g b)
  (list r g b))

(define (color-r v) (car v))
(define (color-g v) (cadr v))
(define (color-b v) (caddr v))

(define (color-eq? a b)
  (and
    (rough-eq? (color-r a) (color-r b))
    (rough-eq? (color-g a) (color-g b))
    (rough-eq? (color-b a) (color-b b))))

(define (color-add a b)
  (map + a b))

(define (color-sub a b)
  (map - a b))

(define (color-mul a b)
  (map * a b))

(define (color-scale s c)
  (map (lambda (x) (* s x)) c))

(define (color-to-display c)
  (string-join (map (lambda (x)
         (number->string (exact-floor (clamp 0 (* x 255) 255)))) c)))

