#lang racket/base

(require rackunit
         racket/function
         "../src/util.rkt"
         "../src/canvas.rkt")

(provide canvas-tests)

(define canvas-tests
  (test-suite
    "Canvas Tests"
    (test-case
      ""
      (check-true #t))
    ))
