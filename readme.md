# C++ assertion library for TDD

Based on the same syntax used by [Jasmine](https://jasmine.github.io/) but waaaay less feature complete.

## Usage

The library just provides some helpers to structure unit tests in C++

Example:

```cpp
#include "./assert.h"

int main() {
  describe("tdd spec", []() {
    xdescribe("skip me", [](){});
    xit("skip me too");

    describe("numbers", []() {
      it("should compare numbers", []() {
        int a = 1;
        int b = 2;

        expect(a + b).toBe(3);
      });
    });

    describe("booleans", []() {
      it("should be true", []() {
        bool a = true;

        expect(a).toBeTrue();
      });

      it("should be false", []() {
        bool a = false;

        expect(a).toBeFalse();
      });
    });

    describe("string", []() {
      it("should compare two strings", []() {
        const char* a = "foo";
        const char* b = "foo";

        expect(a).toEqualString(b);
      });
    });
  });

  testSummary();
}
```