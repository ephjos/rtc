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
        (canvas-write! c 2 3 red)
        (check-true (color-eq? (canvas-read c 2 3) red))
        (check-true (color-eq? (canvas-read c 0 0) black))))
    (test-case
      "PPM header"
      (let* ([c (make-canvas 5 3)]
             [ppm (string-split (canvas-to-ppm c) "\n")]
             [header (take ppm 3)])
        (map
          (lambda (x y)
            (check-true (string=? x y)))
          header (list "P3" "5 3" "255"))))
    (test-case
      "PPM pixel data"
      (let* ([c (make-canvas 5 3)]
             [c1 (make-color 1.5 0 0)]
             [c2 (make-color 0 0.5 0)]
             [c3 (make-color -0.5 0 1)])
        (canvas-write! c 0 0 c1)
        (canvas-write! c 2 1 c2)
        (canvas-write! c 4 2 c3)
        (let* ([ppm (string-split (canvas-to-ppm c) "\n")]
               [body (drop ppm 3)])
          (check-true (string=? (list-ref body 0) "255 0 0"))
          (check-true (string=? (list-ref body 7) "0 127 0"))
          (check-true (string=? (list-ref body 14) "0 0 255"))
          )))
    (test-case
      "PPM ends with a newline"
      (let* ([c (make-canvas 5 3)]
             [chars (string->list (canvas-to-ppm c))])
        (check-eq? (last chars) #\newline)))
    ))
