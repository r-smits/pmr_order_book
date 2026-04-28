Purpose of project

Order book backed by polymorphic memory resource used to create an arena allocator.
This is a monotonic allocator meaning it can only be deallocated in its entirety.

However, a single allocation at beginning of lifetime of a program should be faster than allocation with default allocator.
Therefore we should be able to benchmark this and notice the difference in latency.
This repository is meant to test this.

Limitations:
- The benchmark is yet to be added.
- Currently only supports PMR, to be extended to default allocator for benchmark.
- Toy allocation of a page of 2MB.
- Memory alloaction differs per operating system. This code ran on Mac OS X and so differs from the default Linux mmap impl.

Features:
- Matching
- Adding in O(log N) due to black red tree data structure backing std::map.
- and deleting orders in O(1) time complexity due to storage of pointers 



