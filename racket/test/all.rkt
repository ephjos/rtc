#lang racket/base

(require rackunit
         rackunit/text-ui
         "./primitive-test.rkt")

(provide all-tests)

(define all-tests
  (test-suite
    "rtc tests"
    primitive-tests
    ))

