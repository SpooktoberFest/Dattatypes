

# To do:

Now:
- Make tests for enum_flags
- Make tests for iternal_ptr
- Make tests for unlock_map
- Make trigonometric functions for Prec

----


Soon:
- Test Prec with std::complex


----


Later:
- Test Prec with eigen::quaternion

# Memo:

```


=== Unlock map (range) insertion: ===

 _____________________________
 | start | even i | !(i & 1) |
 | end   | odd i  | (i & 1)  |



GENERALLY
    find: i, j;
    if there is stuff between i and j
        modify ends and erase content
    else
        insert upper and lower at i

EDGE CASES

    already exists      -> early return yay!
    data empty          -> cannot deref j or i, early return yay!
    i & j before first  -> deref j to detect, early return yay!

    i & j at end        -> cannot deref j or i
    j is at end         -> cannot deref j

    i is before first   -> deref i to detect
    i touches other     -> deref i to detect & handle
    j touches other     -> deref j to detect & handle








```
