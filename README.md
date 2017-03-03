# A few notes about BigIntLib:

1. The BigIntLib was produced primarily for positive
   integers of arbitrary size.

2. The integers are stored in a vector<char>, so are
   fairly efficient in terms of memory.

3. The multiplication and division methods are very
   fast. Further optimisations can, however, be made,
   by converting big switch statements into a few
   case statements followed by a few calculations.
   This will, however, decrease the understandability
   of the algorithm itself.

4. They can be made to represent negative integer, f-
   ollowing slight modifications. This is is not done
   because the primary purpose of this class was to 
   support calculations in RSA encryption.