#lang racket

(require rackunit
         rackunit/text-ui
         rackunit/gui
         "./test/all.rkt")

(test/gui
  all-tests
  #:wait? #t)

