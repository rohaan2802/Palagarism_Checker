# Palagarism_Checker

Document **similarity** tool: bag-of-words frequencies + **cosine similarity** as a **percentage**. Library-style API (Google Test), not a fancy GUI.

**Logic:** `Sample-Test1/Header.h` (also extracted as `_sources/Plag_Header.h`)  
**Tests:** `Sample-Test1/test.cpp`  
[rohaan2802](https://github.com/rohaan2802)

---

## Table of contents

1. [Caps and globals](#caps-and-globals)
2. [Pipeline](#pipeline)
3. [Cosine formula](#cosine-formula)
4. [Input file format](#input-file-format)
5. [Test expectations](#test-expectations)
6. [Build](#build)
7. [API sketch](#api-sketch)

---

## Caps and globals

From the header:

```text
MAX_WORDS = 1000
MAX_LINE_LENGTH = 1000
text_read[10][MAX_WORDS]     // up to 10 documents
unique_words, frequency[][], simil[][]
```

`Input_File` holds stop-word list, document paths, `NumberOfDocument`.  
`set_stopWords` **skips the first 12 characters** of the control line (`for (int i = 12; …)`), then splits on spaces — the input file prefix must match what tests use (e.g. a label like `StopWords: `).

---

## Pipeline

1. `readInput(path)` — stop-word line, document count, paths → `text_read[]`  
2. `removePunctuationMarks()`  
3. `convertUpperToLowerCase()`  
4. `removeStopWords()`  
5. `generateFrequencies()` — vocab + per-doc counts  
6. `calculateAllCosineSimilarities()` — full matrix  
7. `similarityIn(i, j)` — **0–100** percent (self-similarity 100%)

---

## Cosine formula

For frequency vectors **A**, **B**:

```text
cos θ = (A·B) / (||A|| ||B||)
percent = cos θ * 100
```

Implemented with `<cmath>` `pow` / `sqrt`. Empty vectors / zeros need the same guards as in `Header.h` (avoid div-by-zero).

---

## Input file format

1. Stop-word line (parser skips a **fixed 12-char prefix**, then space-separated words).  
2. Integer document count.  
3. One filesystem path per document.

Fixtures: `input.txt`, `input1.txt` … `input5.txt`  
Docs: `document1.txt` … `document6.txt`  
Working directory must be `Sample-Test1`.

---

## Test expectations

Google Test `TEST(PC, …)` covers: file read, punctuation, case, stop words, frequencies, plagiarism checks, multi-file, line-breaking.

Examples from the suite:

| Fixture | Approx. similarity |
|---------|-------------------|
| `input.txt` pair | **~53%** |
| `input1.txt` pair | **~70.71%** |
| Identical text | **100%** |

---

## Build

Open `Sample-Test1.sln` (NuGet `packages.config` → Google Test). Test Explorer. pch: `pch.h` / `pch.cpp`.

---

## API sketch

```cpp
readInput("input.txt");
removePunctuationMarks();
convertUpperToLowerCase();
removeStopWords();

char** uniqueWords;
int uniqueCount;
int** documentFrequency;
generateFrequencies(uniqueWords, uniqueCount, documentFrequency);

double** similarities;
calculateAllCosineSimilarities(similarities, documentFrequency);

double pct = similarityIn(1, 2);
```

**Extend:** TF–IDF; CLI `main` printing the matrix; raise `MAX_WORDS`; free heap arrays; SHA not needed (this is IR, not hashing).

---

## Author

**rohaan2802** · [https://github.com/rohaan2802](https://github.com/rohaan2802)
