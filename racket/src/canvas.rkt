#lang racket

(require "./util.rkt"
         "./color.rkt")

(provide canvas-w
         canvas-h
         canvas-p
         make-canvas
         canvas-write
         canvas-read)

(struct canvas (w h p))

(define (make-canvas w h)
  (let ([p (make-vector (* w h) (make-color 0 0 0))])
    (canvas w h p)))

(define (canvas-write c x y col)
  (vector-set! (canvas-p c) (index-2d x y (canvas-w c)) col))

(define (canvas-read c x y)
  (vector-ref (canvas-p c) (index-2d x y (canvas-w c))))

