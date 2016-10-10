# Cache Performance Outline

  1. Make array `int64_t times[(step + max_cache_size) / step]`
  2. Loop, increasing initial array size by `step`:

    a. Fill array with random values
    b. Scan through the array 10x: Have a random initial value `i` that gets
    `xor`ed with `array[i]` (this will warm the cache).
    c. Start timer.
    d. Repeat (b).
    e. Stop timer
    f. Put time in `times` array
  3. Export `times` to CSV where column one records the size in bytes (8 times
     the length of the array) and column two records the time.
  4. Analyze data with R/Python
