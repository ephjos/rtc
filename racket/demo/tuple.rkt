#lang racket/base

(require "../src/tuple.rkt")

(define-struct projectile (position velocity))
(define-struct environment (gravity wind))

(define (tick p e)
  (let ([new-pos (tuple-add (projectile-position p) (projectile-velocity p))]
        [new-vel (tuple-add (projectile-velocity p) (tuple-add (environment-gravity e) (environment-wind e)))])
    (make-projectile new-pos new-vel)))

(define p (make-projectile (make-point-3d 0 1 0) (tuple-normalize (make-vector-3d 1 1 0))))
(define e (make-environment (make-vector-3d 0 -0.1 0) (make-vector-3d -0.01 0 0)))

(define (run p e)
  (println (projectile-position p))
  (if (> (tuple-y (projectile-position p)) 0)
    (run (tick p e) e)
    (display "Hit!\n")))

(run p e)
