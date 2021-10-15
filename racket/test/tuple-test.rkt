#lang racket

(require rackunit
         racket/function
         "../src/util.rkt"
         "../src/tuple.rkt")

(provide tuple-tests)

(define tuple-tests
  (test-suite
    "Primitive Tests"
    (test-case
      "a tuple with w=1.0 is a point"
      (let ([t (list 4.3 -4.2 3.1 1.0)])
        (check-eq? (tuple-x t) 4.3)
        (check-eq? (tuple-y t) -4.2)
        (check-eq? (tuple-z t) 3.1)
        (check-eq? (tuple-w t) 1.0)
        (check-pred point? t)
        (check-pred (negate vector?) t)))
    (test-case
      "a tuple with w=0.0 is a vector"
      (let ([t (list 4.3 -4.2 3.1 0.0)])
        (check-eq? (tuple-x t) 4.3)
        (check-eq? (tuple-y t) -4.2)
        (check-eq? (tuple-z t) 3.1)
        (check-eq? (tuple-w t) 0.0)
        (check-pred (negate point?) t)
        (check-pred vector? t)))
    (test-case
      "point creates tuples with w=1.0"
      (check-true (tuple-eq? (make-point 4 -4 3) (list 4 -4 3 1))))
    (test-case
      "vector creates tuples with w=0.0"
      (check-true (tuple-eq? (make-vector 4 -4 3) (list 4 -4 3 0))))
    (test-case
      "adding two tuples"
      (let ([a1 (list 3 -2 5 1)]
            [a2 (list -2 3 1 0)])
        (check-true (tuple-eq? (tuple-add a1 a2) (list 1 1 6 1)))))
    (test-case
      "subtract two points"
      (let ([p1 (make-point 3 2 1)]
            [p2 (make-point 5 6 7)])
        (check-true (tuple-eq? (tuple-sub p1 p2) (make-vector -2 -4 -6)))))
    (test-case
      "subtract two vectors"
      (let ([v1 (make-vector 3 2 1)]
            [v2 (make-vector 5 6 7)])
        (check-true (tuple-eq? (tuple-sub v1 v2) (make-vector -2 -4 -6)))))
    (test-case
      "subtract vector from zero vector"
      (let ([v1 (make-vector 0 0 0)]
            [v2 (make-vector 1 -2 3)])
        (check-true (tuple-eq? (tuple-sub v1 v2) (make-vector -1 2 -3)))))
    (test-case
      "negate a tuple"
      (let ([t (list 1 -2 3 -4)])
        (check-true (tuple-eq? (tuple-neg t) (list -1 2 -3 4)))))
    (test-case
      "scale a tuple"
      (let ([t (list 1 -2 3 -4)])
        (check-true (tuple-eq? (tuple-scale 3.5 t) (list 3.5 -7 10.5 -14)))))
    (test-case
      "scale a tuple by a fraction"
      (let ([t (list 1 -2 3 -4)])
        (check-true (tuple-eq? (tuple-scale 0.5 t) (list 0.5 -1 1.5 -2)))))
    (test-case
      "divide a tuple by a fraction"
      (let ([t (list 1 -2 3 -4)])
        (check-true (tuple-eq? (tuple-div 2 t) (list 0.5 -1 1.5 -2)))))
    (test-case
      "magnitude of vector (1, 0, 0)"
      (let ([v (make-vector 1 0 0)])
        (check-true (rough-eq? (tuple-magnitude v) 1))))
    (test-case
      "magnitude of vector (0, 1, 0)"
      (let ([v (make-vector 0 1 0)])
        (check-true (rough-eq? (tuple-magnitude v) 1))))
    (test-case
      "magnitude of vector (0, 0, 1)"
      (let ([v (make-vector 0 0 1)])
        (check-true (rough-eq? (tuple-magnitude v) 1))))
    (test-case
      "magnitude of vector (1, 2, 3)"
      (let ([v (make-vector 1 2 3)])
        (check-true (rough-eq? (tuple-magnitude v) (sqrt 14)))))
    (test-case
      "magnitude of vector (-1, -2, -3)"
      (let ([v (make-vector -1 -2 -3)])
        (check-true (rough-eq? (tuple-magnitude v) (sqrt 14)))))
    (test-case
      "normalize vector (4, 0, 0)"
      (let ([v (make-vector 4 0 0)])
        (check-true (tuple-eq? (tuple-normalize v) (make-vector 1 0 0)))))
    (test-case
      "normalize vector (1, 2, 3)"
      (let ([v (make-vector 1 2 3)]
            [r14 (sqrt 14)])
        (check-true (tuple-eq? (tuple-normalize v) (make-vector (/ 1 r14) (/ 2 r14) (/ 3 r14))))))
    (test-case
      "magnitude of normalized vector"
      (let ([v (make-vector 1 2 3)])
        (check-true (rough-eq? (tuple-magnitude (tuple-normalize v)) 1))))
    (test-case
      "dot product of two tuples"
      (let ([a (make-vector 1 2 3)]
            [b (make-vector 2 3 4)])
        (check-true (rough-eq? (tuple-dot a b) 20))))
    (test-case
      "cross product of two tuples"
      (let ([a (make-vector 1 2 3)]
            [b (make-vector 2 3 4)])
        (check-true (tuple-eq? (tuple-cross a b) (make-vector -1 2 -1)))
        (check-true (tuple-eq? (tuple-cross b a) (make-vector 1 -2 1)))))
    ))

