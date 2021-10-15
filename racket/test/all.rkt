#lang racket

(require rackunit
         rackunit/text-ui
         "./tuple-test.rkt"
         "./color-test.rkt"
         "./canvas-test.rkt")

(provide all-tests)

(define all-tests
  (test-suite
    "rtc tests"
    tuple-tests
    color-tests
    canvas-tests
    ))

