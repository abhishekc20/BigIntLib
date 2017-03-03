# BigIntLib

A BigInt library for arbitrary-sized unsigned integers in C++.

## A few notes

1. The BigIntLib was produced primarily for positive integers of arbitrary size.

2. The integers are stored in a vector<char>, so are fairly efficient in terms of memory.

3. They can be made to represent negative integer, following slight modifications. This is is not done
because the primary purpose of this class was to support calculations in RSA encryption.