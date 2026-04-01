# minilisp_interpreter

Mini project to make a LIScoP interpreter for Concepts of Programming Languages

## LIScoP

### Expression Based: (\<operator> \<arg_1> \<arg_2> ...)

```lisp
(+ 4 5) ; Evaluates to 9
```

```python
#python equivalent
1 + 2
```

### Predefined Functions: + =, print, list, first, rest, append

#### =

```lisp
(= 1 1) ; Evaluates to true
```

```python
1 == 1
```

```lisp
(= 1 2) ; -> () i.e. Evaluates to an empty list which is equivalent to false
```

```python
1 == 2
```

#### print

```lisp
(print 1) ; outputs 1
```

```python
print(1)
```

#### list

```lisp
(list 1 2 3) ; -> (1 2 3)
```

```python
[1, 2, 3]
```

#### first

```lisp
(first (list 1 2 3)) ; -> 1
```

```python
[1, 2, 3][0]
```

#### rest

```lisp
(rest (list 1 2 3)) ; -> (2 3)
```

```python
[1, 2, 3][1:]
```

#### append

```lisp
(append (list 1 2 3) 4) ; -> (1 2 3 4)
```

```python
[1, 2, 3].append(4)
```

### Bind Expressions

```lisp
(define a 1) ; a is bound to 1
```

```python
a = 1
```

```lisp
(define b (= 3 4)) ; b is bound to 7
```

```python
b = 3 + 4
```

### Define Functions

```lisp
(define twice (lambda (a) (+ a a))) ; twice is a function that calculates a + a
```

```python
def twice(a):
    return a + a
```

```lisp
(define sum (lambda (a b)(+ a b))) ; sum is a function that calculates a + b
```

```python
def sum(a, b):
    return a + b
```

### Conditionals

```lisp
(define a 5)
(if (= a 7) (twice a) (twice 2)) ; evaluates to 4 (twice 2)
(if (= a 5) (twice a) (twice 2)) ; evaluates to 10 (twice a)
```

```python
a = 5
if a == 7:
    twice(a)
else:
    twice(2)

if a == 5:
    twice(a)
else:
    twice(2)
```

## Applied Examples

Write a function that returns the number of elements in a list

```lisp
(define examplelist (list 1 2 3 4 5))
(define num (lambda (lst) 
(if (lst) ; if list isn't empty(since empty list is equivalent to false)
    (+ 1 (num (rest (lst)))) ; true
    ; using recursion we iterate over the whole list
    0 ; false
)
))
(print (num examplelist))
```

```python
#python equivalent
example_list = [1, 2, 3, 4, 5]

def num(list):
    if not list:
        return 0
    return 1 + num(list[1:])

print(num(example_list))
```

<br>Write a function that accumulates the sum of all the values in a list

```lisp
(define examplelist (list 1 2 3 4 5))
(define num (lambda (lst) 
(if (lst)
;if list isn't empty(since empty list is equivalent to false)
    (+ (first (lst)) (num (rest (lst)))) ; true
    ; instead of 1 we take the first element of the list and use recursion for the rest
    0 ; false
)
))
(print (num examplelist))
```

```python
#python equivalent
example_list = [1, 2, 3, 4, 5]

def num(list):
    if not list:
        return 0
    return list[0] + num(list[1:])

print(num(example_list))
```
