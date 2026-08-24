# Palagarism_Checker

Library-style **document similarity** checker used with **Google Test**. After stop-word removal, each document is a **bag-of-words frequency vector**; pairwise **cosine similarity** is reported as a **percentage** (`similarityIn`). There is no GUI, no LCS, no k-grams, and no hashing — the algorithm in `Sample-Test1/Header.h` is TF-style counts plus cosine.

The GitHub repository name is **Palagarism_Checker** (spelling kept).

**Logic:** `Sample-Test1/Header.h`  
**Tests:** `Sample-Test1/test.cpp`  
**Solution:** `Sample-Test1.sln`  
**Author:** Mohammad Rohaan (22I-2327) · [rohaan2802](https://github.com/rohaan2802)

---

## Table of contents

1. [What this repo does](#what-this-repo-does)
2. [Architecture](#architecture)
3. [Caps and globals](#caps-and-globals)
4. [`Input_File` class](#input_file-class)
5. [Pipeline (functions)](#pipeline-functions)
6. [Cosine formula](#cosine-formula)
7. [Control-file format](#control-file-format)
8. [Fixture files](#fixture-files)
9. [Google Test cases](#google-test-cases)
10. [File-by-file](#file-by-file)
11. [Build and run](#build-and-run)
12. [Limitations](#limitations)
13. [Author](#author)

---

## What this repo does

A test driver calls the API in a fixed order:

1. `readInput("inputN.txt")` — parse stop words, document count, paths; slurp each document into `text_read`.
2. `removePunctuationMarks()` — keep letters and single spaces.
3. `convertUpperToLowerCase()` — ASCII `'A'`–`'Z'` → `+ 32`.
4. `removeStopWords()` — drop tokens listed after the 12-character prefix on the first control line.
5. `generateFrequencies(...)` — unique vocabulary + per-document counts.
6. `calculateAllCosineSimilarities(...)` — full N×N matrix in `simil`.
7. `similarityIn(i, j)` — `simil[i-1][j-1] * 100` (1-based document numbers).

`getText(n)` returns `text_read[n - 1]`. `getNumberOfDocument()` forwards to `Input_File`.

---

## Architecture

```text
input*.txt          stop-word line, count, paths
    │
    ▼
readInput()         Input_File f1  +  text_read[10][MAX_WORDS]
    │
    ▼
removePunctuationMarks / convertUpperToLowerCase / removeStopWords
    │
    ▼
generateFrequencies → uniqueWords[], documentFrequency[][], frequency[][]
    │
    ▼
calculateAllCosineSimilarities → similarities[][] and simil[][]
    │
    ▼
similarityIn(d1, d2) → percent
```

All processing is in-process C strings (`strcpy`, `strtok`, `strcmp`, `atoi`). No `main()` beyond Google Test’s.

---

## Caps and globals

From `Header.h`:

```text
MAX_WORDS        = 1000
MAX_LINE_LENGTH  = 1000   // declared; getline uses MAX_WORDS
text_read[10][MAX_WORDS]  // at most 10 documents in RAM
unique_words[10][100]
frequency[100][100]
simil[100][100]
unique_word_count
Input_File f1
```

`text_read[10]` is the hard cap that matters: a control file may list more paths, but only ten slots exist.

---

## `Input_File` class

Private: `stop_words[MAX_WORDS][MAX_WORDS]`, `doc_path[10][100]`, `NumberOfDocument`, `stop_word_count`.

| Method | Behaviour |
|--------|-----------|
| `set_stopWords(const char* str)` | Starts at **index 12**, then splits on spaces |
| `get_stopWords(int x)` | One stop token, or `nullptr` |
| `get_stopWords_count()` | Count |
| `setNumberOfDocument` / `getNumberOfDocument` | Document count from line 2 |
| `set_doc_path` / `get_doc_path` | Path strings (max 10) |

The 12-character skip matches a prefix of length 12 such as `Stop Words:` (including the space after the colon). Tokens after that become the stop list.

---

## Pipeline (functions)

**`readInput(const char* pathofInputFile)`**  
`getline` line 1 → `set_stopWords`. Line 2 → `atoi` → `setNumberOfDocument`. Next N lines → `set_doc_path`. Then each path is opened; whitespace is collapsed to single spaces; leading whitespace skipped; result stored in `text_read[i]`.

**`removePunctuationMarks()`**  
Copy only `a–z`/`A–Z`; insert a space when a non-letter follows a word. Trailing space stripped. Result copied back with `strcpy`.

**`convertUpperToLowerCase()`**  
If `65 ≤ c ≤ 90`, add 32.

**`isStopWord(const char* ch)`**  
Manual string compare against each stop token (no `strcmp` here).

**`removeStopWords()`**  
Split `text_read[i]` on spaces into `temp_array`, keep tokens where `isStopWord` is false, join with spaces.

**`generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency)`**  
Allocates `uniqueWords` (`MAX_WORDS` pointers) and `documentFrequency[numDocs][100]`. Tokenizes with `strtok(..., " ")`. On first sight of a word, copies into heap + `unique_words`; increments counts in both `documentFrequency` and global `frequency`. Sets `unique_word_count`.

**`getFrequency(char* word, int documentNum)`**  
Looks up `unique_words`; returns `frequency[documentNum-1][i]`, or **-1** if missing. Tests also expect **0** when a vocab word is absent from that document (after it was seen in another doc).

**`calculateAllCosineSimilarities(double**& similarities, int** documentFrequency)`**  
Magnitude \( \|d_i\| = \sqrt{\sum_j f_{ij}^2} \). Diagonal 1.0. Off-diagonal \(\frac{d_i \cdot d_j}{\|d_i\| \|d_j\|}\). Copies into `simil`.

**`similarityIn(int documentNum1, int documentNum2)`**  
Out of range → `0`. Else `simil[doc1][doc2] * 100`.

---

## Cosine formula

For frequency vectors **A** and **B** (`<cmath>` `pow` / `sqrt`):

```text
cos θ = (A · B) / ( ||A||  ||B|| )
percent = cos θ × 100
```

Self-similarity is forced to **1.0** (100%). There is **no** division-by-zero guard if a document has all-zero magnitude.

---

## Control-file format

Each `input*.txt`:

```text
Stop Words: <space-separated tokens>
<integer document count>
<path to document 1>
<path to document 2>
...
```

Working directory must be `Sample-Test1` so relative paths such as `document1.txt` resolve.

---

## Fixture files

| File | Role in tests |
|------|----------------|
| `input.txt` | Stop words `they in street the while`; 2 docs → `document3.txt`, `document4.txt` |
| `input1.txt` | Stop words `remove these Words`; 2 docs → `document1.txt`, `document2.txt` |
| `input2.txt` | Stop words `in the she vowels`; **4** docs → `document1`–`document4.txt` |
| `input3.txt` | Empty stop list after prefix; count **1002** (stress; test commented out) |
| `input4.txt` | Count **10**; repeats `Document1.txt` / `Document2.txt` |
| `input5.txt` | 2 docs → `document5.txt`, `document6.txt` (line breaks) |
| `document1.txt` | Regex-like punctuation string (vowels A–E / I–O / U) |
| `document2.txt` | `vowels: a-`~e-i-O-##&U` |
| `document3.txt` | Shells / sea sentence with mixed case and symbols |
| `document4.txt` | `She Sell SeaShellS in the sea` with decoration |
| `document5.txt` | Multi-line `Line after LiNe` |
| `document6.txt` | Letters of “finished” spread across lines |

After punctuation + lowercasing, `document1` becomes `aeiou` and `document2` becomes `vowels aeiou` (see `RemovePunctucationMarks`).

---

## Google Test cases

Suite name **`PC`** in `test.cpp` (`#include "pch.h"`, `Header.h`).

| Test | Fixture | Assertions |
|------|---------|------------|
| `FileReading` | `input1.txt` | `getText(1)` equals the raw document1 string; `getNumberOfDocument() == 2` |
| `RemovePunctucationMarks` | `input1.txt` | After punct + case: `"aeiou"` and `"vowels aeiou"` |
| `CaseChange` | `input.txt` | Doc 2 → `"she sell seashells in the sea"` |
| `RemoveStopWords` | `input.txt` | `"sell shells she sees shells sea"` and `"she sell seashells sea"` |
| `FrequencyCheck` | `input1.txt` | `getFrequency("vowels", 1) == 0`; `getFrequency("aeiou", 2) == 1`; `uniqueCount == 2` |
| `PlagCheck1` | `input.txt` | Self 100%; pair **~53.00** (`EXPECT_NEAR`, abs 0.3) |
| `PlagCheck2` | `input1.txt` | Pair **~70.71** |
| `PlagCheck3` | `input2.txt` | `(2,3) == 0`; `(1,2) == 100` |
| `MaxfileRead` | `input3.txt` | **Commented out** (would expect 1002 documents) |
| `HalfFilesRead` | `input4.txt` | `getText(10)` is `"vowels aeiou"`; count 10 |
| `lineBreaking_prop` | `input5.txt` | Doc 2 → `"f i n i s h e d"` |

Typical API usage (same order as `PlagCheck1`):

```cpp
readInput("input.txt");
removePunctuationMarks();
convertUpperToLowerCase();
removeStopWords();
char** uniqueWords; int uniqueCount; int** documentFrequency;
generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
double** similarities;
calculateAllCosineSimilarities(similarities, documentFrequency);
double pct = similarityIn(1, 2);
```

---

## File-by-file

| Path | Role |
|------|------|
| `Sample-Test1.sln` | VS solution |
| `Sample-Test1/Header.h` | All algorithm functions + `Input_File` |
| `Sample-Test1/test.cpp` | `TEST(PC, …)` cases |
| `Sample-Test1/pch.h` / `pch.cpp` | Precompiled header for GTest |
| `Sample-Test1/packages.config` | NuGet: `Microsoft.googletest.v140.windesktop.msvcstl.static.rt-dyn` **1.8.1.7** |
| `Sample-Test1/Sample-Test1.vcxproj` | Test project |
| `document1.txt` … `document6.txt` | Corpora |
| `input.txt`, `input1.txt` … `input5.txt` | Control files |

---

## Build and run

1. Open `Sample-Test1.sln` in Visual Studio (v140-compatible GTest package).
2. Restore NuGet (`packages.config`).
3. Set the debugger / Test Explorer working directory to **`Sample-Test1`**.
4. Run all tests (Test Explorer) or the GTest binary.

There is no separate `main.cpp` application that prints a similarity matrix.

---

## Limitations

- **10-document** `text_read` / `doc_path` arrays; `input3.txt` (1002) is not a realistic run.
- Heap from `generateFrequencies` / `calculateAllCosineSimilarities` is never `delete`d.
- `strtok` mutates `text_read` (cannot re-tokenize the same buffer).
- No TF–IDF, no stemming, no n-grams, no hash signatures.
- Cosine can divide by zero on empty vectors.
- `getFrequency` returns `-1` for unknown words; tests use in-vocab zeros.
- Typo in test name: `RemovePunctucationMarks`.

**Extend:** CLI `main`; free allocations; raise caps; optional TF–IDF.

---

## Author

**Mohammad Rohaan** — 22I-2327  
[https://github.com/rohaan2802](https://github.com/rohaan2802)
