#lang racket/base

(require "../src/primitive.rkt")

(define-struct projectile (position velocity))
(define-struct environment (gravity wind))

(define (tick p e)
  (let ([new-pos (primitive-add (projectile-position p) (projectile-velocity p))]
        [new-vel (primitive-add (projectile-velocity p) (primitive-add (environment-gravity e) (environment-wind e)))])
    (make-projectile new-pos new-vel)))

(define p (make-projectile (point 0 1 0) (primitive-normalize (vector 1 1 0))))
(define e (make-environment (vector 0 -0.1 0) (vector -0.01 0 0)))

(define (run p e)
  (println (projectile-position p))
  (if (> (primitive-y (projectile-position p)) 0)
    (run (tick p e) e)
    (println 'Hit!)))

(run p e)
