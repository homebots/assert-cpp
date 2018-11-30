#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define lprintf(...) do { leftPadding(); printf(__VA_ARGS__); } while (0);
using namespace std;

const char* OK = "✔";
const char* FAIL = "×";
const char* LEFTPAD = "  ";
const char* EXPECTED_NUMBER = "Expected %s to be %s\n";
const char* EXPECTED_STRING = "Expected string ";
const char* TO_BE_STRING = " to be ";

typedef void (*Spec)();

int total = 0;
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
void compare(T actual, T expected) {
  if (hasErrors) {
    return;
  }

  bool valid = actual == expected;
  hasErrors = !valid;

  if (hasErrors) {
    throw actual;
  }
}

void assert(bool actual) {
  compare(actual, true);
}

void describe(const char* description, Spec spec) {
  paddingLevel++;
  lprintf("- %s\n", description);
  spec();
  paddingLevel--;
}

void runSpec(Spec spec) {
  try {
    spec();
  }
  catch (char* e) {
    lprintf("> Expected %s\n", e);
  }
  catch (int e) {
    lprintf("> Expected %d\n", e);
  }
  catch (...) {
    lprintf("> Error!\n");
    hasErrors = true;
  }
}

void it(const char* description, Spec spec) {
  hasErrors = false;
  paddingLevel++;
  total++;

  runSpec(spec);
  printResult(description);

  if (hasErrors) {
    failed++;
  } else {
    successful++;
  }

  paddingLevel--;
}

void skip(const char* description) {
  paddingLevel++;
  lprintf("- %s\n", description);
  paddingLevel--;
}

void xdescribe(const char* description) { skip(description); }
void xdescribe(const char* description, Spec spec) { skipped++; skip(description); }
void xit(const char* description) { skip(description); }
void xit(const char* description, Spec spec) { skipped++; skip(description); }

void testSummary() {
  printf("\n\nTotal: %d\n", total);
  printf("Successful: %d\n", successful);
  printf("Failed: %d\n", failed);
  printf("Skipped: %d\n", skipped);
  printf("\nDone.\n\n");

  exit(failed);
}

template<typename T>
class Expectation {
  public:
    T actual;

  Expectation(T _actual):
    actual(_actual) {}

  void toBe(T expected) {
    compare(actual, expected);
  }

  void toBeTrue() {
    assert(actual);
  }

  void toBeFalse() {
    compare(actual, false);
  }

  void toEqualString(const char* expected) {
    bool valid = strcmp((const char*) actual, expected);
    assert(valid == 0);
  }
};

template<typename T>
Expectation<T> expect(T actual) {
  Expectation<T> e(actual);
  return e;
}
