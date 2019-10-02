# HBST
A C++ implementation of Hidden Binary Search Tree described in in this [paper](https://arxiv.org/abs/1711.07746) by Saulo Queiroz.

HBST maintains its height *O(B) = O(log2n)*, where *B* is a size of each key in bits.
This result requires no special order on the input - as does BST - nor self-balancing procedures, as do AVL and Red-Black.

**HBST.hpp** file contains an implementation itself, **main.cpp** contains some example code.

# Citations

```
@misc{queiroz2017hidden,
    title={The Hidden Binary Search Tree:A Balanced Rotation-Free Search Tree in the AVL RAM Model},
    author={Saulo Queiroz},
    year={2017}
}
```