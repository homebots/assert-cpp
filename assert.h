#include <string.h>
#include <stdlib.h>
#include <iostream>

#define lprintf(...) do { leftPadding(); printf(__VA_ARGS__); } while (0);

using namespace std;

const char* OK = "✔";
const char* FAIL = "×";
const char* LEFTPAD = "  ";

typedef void (*Spec)();

int failed = 0;
int successful = 0;
int skipped = 0;
int paddingLevel = 0;
int hasErrors = false;

void leftPadding() {
  for (int i = 1; i < paddingLevel; i++) {
    printf("%s", LEFTPAD);
  }
}

void printResult(const char* description) {
  const char* prefix = OK;
  if (hasErrors) {
    prefix = FAIL;
  }

  int length = strlen(description) + 5;
  char* message = (char*)malloc(length);

  snprintf(message, length, "%s %s", prefix, description);
  lprintf("%s\n", message);
  free(message);
}

template<typename T>
bool compare(T actual, T expected) {
  return actual == expected;
}

bool compare(unsigned char* actual, const char* expected) {
  return 0 == strcmp((const char*) actual, expected);
}

void runSpec(Spec spec) {
  try {
    spec();
  }
  catch (...) {
    hasErrors = true;
  }
}

void skip(const char* description) {
  paddingLevel++;
  lprintf("- %s\n", description);
  paddingLevel--;
}

/////////

void describe(const char* description, Spec spec) {
  paddingLevel++;
  lprintf("- %s\n", description);
  spec();
  paddingLevel--;
}

void it(const char* description, Spec spec) {
  hasErrors = false;
  paddingLevel++;

  runSpec(spec);
  printResult(description);

  if (hasErrors) {
    failed++;
  } else {
    successful++;
  }

  paddingLevel--;
}

void xdescribe(const char* description) { skip(description); }
void xdescribe(const char* description, Spec spec) { skipped++; skip(description); }
void xit(const char* description) { skipped++; skip(description); }
void xit(const char* description, Spec spec) { skipped++; skip(description); }

void testSummary() {
  printf("\n\nSuccessful: %d\n", successful);
  printf("Failed: %d\n", failed);
  printf("Skipped: %d\n", skipped);
  printf("\nTotal: %d\n", skipped + failed + successful);
  printf("\nDone.\n\n");

  if (failed > 0) {
    exit(1);
  }
}

template<typename T>
class Expectation {
  public:
    T actual;

    Expectation(T _actual):
      actual(_actual) {}

    void toBe(T expected) {
      _toBe(!compare(actual, expected), expected);
    }

    void toBe(const char* expected) {
      _toBe(!compare(actual, expected), (unsigned char*) expected);
    }

  private:
    void _toBe(bool value, T expected) {
      hasErrors = value;

      if (hasErrors) {
        cout << "\n";
        leftPadding();
        cout << "  Expected " << expected << " but got " << actual << "\n";

        throw "error";
      }
    }
};

template<typename T>
Expectation<T> expect(T actual) {
  Expectation<T> e(actual);
  return e;
}
