#lang racket

(require "./util.rkt"
         "./color.rkt")

(provide canvas-w
         canvas-h
         canvas-p
         make-canvas
         canvas-write!
         canvas-write-safe!
         canvas-read
         canvas-to-ppm)

(struct canvas (w h p))

(define (make-canvas w h)
  (let ([p (make-vector (* w h) (make-color 0 0 0))])
    (canvas w h p)))

(define (canvas-write! c x y col)
  (vector-set! (canvas-p c) (index-2d x y (canvas-w c)) col))

(define (canvas-write-safe! c x y col)
  (let ([w (canvas-w c)]
        [h (canvas-h c)])
    (if (and (>= x 0) (< x w) (>= y 0) (< y h))
      (vector-set! (canvas-p c) (index-2d x y (canvas-w c)) col)
      (void))))

(define (canvas-read c x y)
  (vector-ref (canvas-p c) (index-2d x y (canvas-w c))))

(define (canvas-to-ppm c)
  (let* ([w (number->string (canvas-w c))]
         [h (number->string (canvas-h c))]
         [dims (string-append w " " h)]
         [header (string-join (list "P3" dims "255") "\n")]
         [body (string-join
                 (vector->list
                   (vector-map color-to-display (canvas-p c)))
                 "\n")])
    (string-join (list header body "") "\n")))

