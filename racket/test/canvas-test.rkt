#lang racket

(require rackunit
         racket/function
         "../src/util.rkt"
         "../src/color.rkt"
         "../src/canvas.rkt")

(provide canvas-tests)

(define canvas-tests
  (test-suite
    "Canvas Tests"
    (test-case
      "creating a canvas"
      (let* ([c (make-canvas 10 20)]
            [blackPixels
              (vector-filter
                (lambda (x) (color-eq? x (make-color 0 0 0))) (canvas-p c))])
        (check-eq? (canvas-w c) 10)
        (check-eq? (canvas-h c) 20)
        (check-eq? (vector-length blackPixels) 200)))
    (test-case
      "write a pixel"
      (let* ([c (make-canvas 10 20)]
             [black (make-color 0 0 0)]
             [red (make-color 1 0 0)])
        (canvas-write c 2 3 red)
        (check-true (color-eq? (canvas-read c 2 3) red))
        (check-true (color-eq? (canvas-read c 0 0) black))))
    ))
