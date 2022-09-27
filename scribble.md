# Hybrid Vector Clocks

- Vector clocks use counters to set timestamps
- Increment can be any number
- Instead of counters, we can use actual clocks, something like (10:00, 8:07, 3:02, ...)
- Assume a clock skew e
    - Difference between max clock and any clock in the HVC is e
- Instead of maintaining actual numbers, maintain offsets and one reference time, to save space
- All offsets are integers
- The new vector clock would have
    - HLC component
    - bitmap
    - offsets
- Find an order(number of 1s) algorithm to find all places where the bitmap is 1

- Discretizing the clock would reduce the offset bits needed to be stored
- But you might miss some events in between
    - Build a counter for events between discrete points
    - Have a bitmap for the offsets and the counters
    - Can we combine both the bitmaps?

## Gabe's notes

- 