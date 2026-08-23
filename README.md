# Palagarism_Checker

Document **similarity / plagiarism checker** using bag-of-words frequencies and **cosine similarity**. Logic lives in `Sample-Test1/Header.h`; Google Test cases in `Sample-Test1/test.cpp`.

## Overview

Pipeline for each run:

1. `readInput(path)` — parse a control file: stop-word line, document count, then document paths  
2. Load each document into `text_read[]`  
3. `removePunctuationMarks()` → `convertUpperToLowerCase()` → `removeStopWords()`  
4. `generateFrequencies()` — unique vocabulary + per-document counts  
5. `calculateAllCosineSimilarities()` — pairwise cosine similarity  
6. `similarityIn(i, j)` — returns similarity as a **percentage** (0–100)

Designed as a unit-tested library API rather than an interactive CLI.

## Features

- Configurable stop words and multi-document batches via input files  
- Punctuation stripping and lowercasing  
- Stop-word filtering  
- Term frequency vectors and uniqueness tracking  
- Full similarity matrix; self-similarity is 100%  
- Sample documents: `document1.txt` … `document6.txt`  
- Input fixtures: `input.txt`, `input1.txt` … `input5.txt`  
- Google Test suite (`TEST(PC, …)`): file reading, punctuation, case, stop words, frequencies, plagiarism checks, multi-file reads, line-breaking

Example expectations from tests: documents in `input.txt` score ~**53%** similarity; `input1.txt` pair ~**70.71%**; identical content → **100%**.

## Tech stack

| Component | Technology |
|-----------|------------|
| Language | C++ |
| Testing | Google Test (`pch.h`, `packages.config`, `Sample-Test1.sln`) |
| Math | `<cmath>` (`pow`, `sqrt`) |

## Project structure

```
Palagarism_Checker/
├── Sample-Test1.sln
└── Sample-Test1/
    ├── Header.h          # Input_File + NLP/similarity API
    ├── test.cpp          # Google Test cases
    ├── pch.h / pch.cpp
    ├── document1.txt … document6.txt
    ├── input.txt … input5.txt
    └── Sample-Test1.vcxproj
```

## How to build / run

1. Open `Sample-Test1.sln` in Visual Studio (with Google Test / NuGet packages restored via `packages.config`).  
2. Set working directory to `Sample-Test1` so relative paths like `input1.txt` resolve.  
3. Build and run tests (Test Explorer or debug the test project).

Command-line GTest builds are possible if you compile `Header.h` usage with the same test runner and link gtest.

## Usage

From tests / your own harness:

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

double pct = similarityIn(1, 2);  // percent similarity between docs 1 and 2
```

**Input file format** (as parsed):

1. Line starting with stop words (parser skips a fixed prefix then splits on spaces)  
2. Integer document count  
3. One path per document  

## How to extend / modify

- Raise `MAX_WORDS` / document caps for larger corpora.  
- Add TF–IDF weighting instead of raw counts.  
- Expose a `main()` CLI that prints the similarity matrix.  
- Free dynamically allocated `uniqueWords` / `similarities` arrays to avoid leaks in long runs.

## Author

**rohaan2802** — [https://github.com/rohaan2802](https://github.com/rohaan2802)
