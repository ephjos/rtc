#lang racket/base

(require rackunit
         rackunit/text-ui
         "./test/primitive-test.rkt")

(define tests
  (test-suite
    "rtc tests"
    primitive-tests
    ))

(run-tests tests)

