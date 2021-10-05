#lang racket/base

(provide point
         vector
         primitive-x
         primitive-y
         primitive-z
         primitive-w
         point?
         vector?
         )

(define (point x y z)
  (list x y z 1))

(define (vector x y z)
  (list x y z 0))

(define (primitive-x v) (car v))
(define (primitive-y v) (cadr v))
(define (primitive-z v) (caddr v))
(define (primitive-w v) (cadddr v))

(define (point? p) (eq? (primitive-w p) 1.0))
(define (vector? p) (eq? (primitive-w p) 0.0))
