# C++ assertion library for TDD

Based on the same syntax used by [Jasmine](https://jasmine.github.io/) but waaaay less feature complete.

## Introduction

The library just provides some helpers to structure unit tests in C++

## Usage

```
clang++ -std=gnu++11 -o ./test.bin example.spec.cpp
./test.bin
```

`example.spec.cpp` :

```cpp
#include "./assert.h"

int main() {
  describe("tdd spec", [] {
    xdescribe("skip me", [] {});
    xit("skip me too");

    describe("numbers", [] {
      it("should compare numbers", [] {
        int a = 1;
        int b = 2;

        expect(a + b).toBe(3);
      });
    });

    describe("booleans", [] {
      it("should be true", [] {
        bool a = true;

        expect(a).toBe(true);
      });

      it("should be false", [] {
        bool a = false;

        expect(a).toBe(false);
      });
    });

    describe("string", [] {
      it("should compare two strings", [] {
        const char* a = "foo";
        const char* b = "foo";

        expect(a).toBe(b);
      });
    });
  });

  testSummary();
}
```

Output:

```
- tdd spec
  - skip me
  - skip me too
  - numbers
    ✔ should compare numbers
  - booleans
    ✔ should be true
    ✔ should be false
  - string
    ✔ should compare two strings


Successful: 4
Failed: 0
Skipped: 2

Total: 6

Done.
```