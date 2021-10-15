#lang racket

(require "./util.rkt")

(provide make-point
         make-vector
         tuple-x
         tuple-y
         tuple-z
         tuple-w
         tuple-eq?
         point?
         vector?
         tuple-add
         tuple-sub
         tuple-scale
         tuple-div
         tuple-neg
         tuple-dot
         tuple-cross
         tuple-magnitude
         tuple-normalize
         )

(define (make-point x y z)
  (list x y z 1))

(define (make-vector x y z)
  (list x y z 0))

(define (tuple-x v) (car v))
(define (tuple-y v) (cadr v))
(define (tuple-z v) (caddr v))
(define (tuple-w v) (cadddr v))
(define (tuple-eq? a b)
  (and
    (rough-eq? (tuple-x a) (tuple-x b))
    (rough-eq? (tuple-y a) (tuple-y b))
    (rough-eq? (tuple-z a) (tuple-z b))
    (rough-eq? (tuple-w a) (tuple-w b))))

(define (point? p) (eq? (tuple-w p) 1.0))
(define (vector? p) (eq? (tuple-w p) 0.0))

(define (tuple-add a b)
  (map + a b))

(define (tuple-sub a b)
  (map - a b))

(define (tuple-scale s p)
  (map (lambda (x) (* x s)) p))

(define (tuple-div s p)
  (map (lambda (x) (/ x s)) p))

(define (tuple-neg p)
  (tuple-scale -1 p))

(define (tuple-dot a b)
  (foldr + 0 (map * a b)))

(define (tuple-cross a b)
  (let ([ax (tuple-x a)]
        [ay (tuple-y a)]
        [az (tuple-z a)]
        [bx (tuple-x b)]
        [by (tuple-y b)]
        [bz (tuple-z b)])
    (make-vector
      (- (* ay bz) (* az by))
      (- (* az bx) (* ax bz))
      (- (* ax by) (* ay bx)))))

(define (tuple-magnitude p)
  (sqrt (foldr + 0 (map * p p))))

(define (tuple-normalize p)
  (let ([m (tuple-magnitude p)])
    (tuple-scale (/ 1 m) p)))

