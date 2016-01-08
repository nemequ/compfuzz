# CompFuzz

This repository houses information and resources on using fuzzers
(especially [american fuzzy lop](http://lcamtuf.coredump.cx/afl/)) on
general-purpose compression software.  Currently, it includes:

* A test suite of small but diverse input files which can be used to
  seed the fuzzer.
* Codec-specific compressed files, most notably those which have
  previously resulted in issues.
* Information on previous fuzzing attempts.

The primary goals are:

* Provide information on which codecs are relatively safe, which are
  not, and which haven't been tested yet.
* Reduce duplicate effort among those of us fuzzing compression
  codecs.
* Help avoid regressions by providing test cases for issues.

If you would like to help, the Wiki contains a
[guide](https://github.com/nemequ/compfuzz/wiki/HowTo).

## Structure

### General-purpose data

The "input" directory contains *uncompressed* files which are used to
seed the fuzzer.  The goal here is a large set of diverse but small
files.  The reality is currently a small set of diverse but small
files.

### Codec-specific data

The "libraries" directory contains one directory for each of the
various libraries.  When there is a Squash plugin for the library the
name corresponds to the name of the Squash plugin.

Each library directory may have a "patches" subdirectory which
contains patches against the library in question.  These are mostly
used to disable checksum verification; with care it is generally
possible to craft a malicious file which passes the checksum tests but
triggers the issue in question, but it is difficult for fuzzers to do
so automatically.

Additionally, each library directory contains one or more codec
directory.  Some libraries support multiple codecs (for example, zlib
supports "zlib", "gzip", and "deflate").  When there is a Squash
plugin for the library the name correpsonds to the name of the codec
in Squash.

Each codec directory may contain the following directories:

* crashes — If a codec hasn't been fixed yet after discovery of a
  crash but the issue has been made public (see the "Disclosure
  Policy" section below), it goes in this directory.  Once the issue
  is resolved it will be moved to the "fixed" directory.
* fixed — Test cases which, at one point, caused the codec to crash.
* inputs — Test cases which do *not* cause the codec in question to
  crash.  These are typically combined with the general-purpose inputs
  to seed the fuzzer.

## Disclosure Policy

These test cases all represent security issues.  Some of them may be
exploitable for code execution, others are simply vectors for DoS
attacks.  As such, a responsible disclosure policy is important.

Currently, our policy is is to wait 1 month after reporting issues
before disclosing failing test cases publicly.  If the author of the
library responds and confirms the issue within that month, we will
wait up to an additional 5 months (for a total of 6 months) before
public disclosure.  Once a fix is publicly released we will disclose
the test cases immediately.

## Using Squash

Squash provides a common interface to many compression libraries.
Using Squash isn't required, but it can be quite convenient.  To use
the Squash CLI as a fuzzing tool you will generally want to pass the
`SQUASH_FUZZ_MODE=yes` environment variable.

If you choose to use Squash, keep in mind that you'll still need to
apply any patches in the relevant "patches" subdirectory manually.

## Results

For the results, please see the
[Results wiki page](https://github.com/nemequ/compfuzz/wiki/Results).
Please feel free to add to it!
