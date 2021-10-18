#lang racket

(require "../src/tuple.rkt"
         "../src/color.rkt"
         "../src/canvas.rkt")

(define-struct projectile (position velocity))
(define-struct environment (gravity wind))

(define (tick p e)
  (let ([new-pos (tuple-add (projectile-position p) (projectile-velocity p))]
        [new-vel (tuple-add (projectile-velocity p) (tuple-add (environment-gravity e) (environment-wind e)))])
    (make-projectile new-pos new-vel)))

(define p (make-projectile (make-point-3d 0 1 0) (tuple-scale 11.25 (tuple-normalize (make-vector-3d 1 1.8 0)))))
(define e (make-environment (make-vector-3d 0 -0.1 0) (make-vector-3d -0.01 0 0)))
(define c (make-canvas 900 550))
(define red (make-color 1 0 0))

(define (run p e)
  (let* ([pos (projectile-position p)]
         [pos-x (exact-round (tuple-x pos))]
         [pos-y (exact-round (- 550 (tuple-y pos)))])
    (println pos)
    (canvas-write-safe! c pos-x pos-y red)
    (if (> (tuple-y (projectile-position p)) 0)
      (run (tick p e) e)
      (begin
        (display "Hit!\n")
        (display
          (canvas-to-ppm c)
          (open-output-file
            "./canvas-demo.ppm" #:exists 'truncate))
        (display "Wrote to ./canvas-demo.ppm\n")))))

(run p e)
