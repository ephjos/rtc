#lang racket/base

(require rackunit
         racket/function
         "../src/util.rkt"
         "../src/color.rkt")

(provide color-tests)

(define color-tests
  (test-suite
    "Color Tests"
    (test-case
      "colors are (r, g, b) tuples"
      (check-true (color-eq? (color -0.5 0.4 1.7) (list -0.5 0.4 1.7))))
    (test-case
      "adding colors"
      (let ([c1 (color 0.9 0.6 0.75)]
            [c2 (color 0.7 0.1 0.25)])
        (check-true (color-eq? (color-add c1 c2) (list 1.6 0.7 1.0)))))
    (test-case
      "subtracting colors"
      (let ([c1 (color 0.9 0.6 0.75)]
            [c2 (color 0.7 0.1 0.25)])
        (check-true (color-eq? (color-sub c1 c2) (list 0.2 0.5 0.5)))))
    (test-case
      "scaling colors"
      (let ([c (color 0.2 0.3 0.4)])
        (check-true (color-eq? (color-scale 2 c) (list 0.4 0.6 0.8)))))
    (test-case
      "multiplying colors"
      (let ([c1 (color 1 0.2 0.4)]
            [c2 (color 0.9 1 0.1)])
        (check-true (color-eq? (color-mul c1 c2) (list 0.9 0.2 0.04)))))
    ))
