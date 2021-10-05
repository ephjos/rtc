#lang racket/base

(require rackunit
         racket/function
         "../src/primitive.rkt")

(provide primitive-tests)

(define primitive-tests
  (test-suite
    "Primitive Tests"
    (test-case
      "a tuple with w=1.0 is a point"
      (let ([t (list 4.3 -4.2 3.1 1.0)])
        (check-eq? (primitive-x t) 4.3)
        (check-eq? (primitive-y t) -4.2)
        (check-eq? (primitive-z t) 3.1)
        (check-eq? (primitive-w t) 1.0)
        (check-pred point? t)
        (check-pred (negate vector?) t)
        )
      )
    (test-case
      "a tuple with w=0.0 is a vector"
      (let ([t (list 4.3 -4.2 3.1 0.0)])
        (check-eq? (primitive-x t) 4.3)
        (check-eq? (primitive-y t) -4.2)
        (check-eq? (primitive-z t) 3.1)
        (check-eq? (primitive-w t) 0.0)
        (check-pred (negate point?) t)
        (check-pred vector? t)
        )
      )
    (test-case
      "point creates tuples with w=1.0"
      (check-match (point 4 -4 3) (list 4 -4 3 1))
      )
    (test-case
      "vector creates tuples with w=0.0"
      (check-match (vector 4 -4 3) (list 4 -4 3 0))
      )
    ))

