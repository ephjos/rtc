#lang racket

(require rackunit
         rackunit/text-ui
         "./test/all.rkt")

(exit (run-tests all-tests))

