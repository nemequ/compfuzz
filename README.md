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

m^2 has written
[a good guide](https://extrememoderate.wordpress.com/2015/11/16/fuzz-testing-compressors/)
for how to fuzz compression codecs.  Some additional notes:

* Use a tmpfs directory.  AFL can be rough on hard drives, and given
  how small test cases are there isn't usually a problem with running
  out of RAM.
* On 64-bit Linux systems you'll probably want to use the
  limit_memory.sh script distributed with AFL (see the
  notes_for_asan.txt document, also distributed with AFL, for
  details).

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

* crashes — Tests which, at one point, caused the codec in question to
  crash.
* outstanding — If a codec hasn't been fixed yet after discovery of a
  crash but the issue has been made public (see the "Disclosure
  Policy" section below), it goes in this directory.  Once the issue
  is resolved it will be moved to the "crashes" directory.
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

Note that "OK" doesn't mean "no issues", it means there has been at
least one attempt at fuzzing, and that there are currently no
**known** issues.

<table>
  <thead>
    <tr>
      <td>Plugin</td>
      <td>Status</td>
	  <td>Patch</td>
	  <td>Results</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><a href="https://github.com/jibsen/brieflz">brieflz</a></td>
      <td>OK</td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/google/brotli">brotli</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://libbsc.com/">bsc</a></td>
      <td>Vulnerable</td>
      <td></td>
      <td>
	    <ul>
          <li><a href="https://extrememoderate.wordpress.com/2015/11/16/fuzz-testing-compressors/">m^2 has had some results</a></li>
        </ul>
  	  </td>
    </tr>

    <tr>
      <td><a href="http://bzip.org/">bzip2</a></td>
      <td>OK</td>
      <td><a href="https://github.com/nemequ/compfuzz/tree/master/libraries/bzip2/patches">Patch</a></td>
      <td>Fuzzed by others</td>
    </tr>

    <tr>
      <td><a href="http://compressme.net/">crush</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/fusiyuan2010/CSC">csc</a></td>
      <td>Vulnerable</td>
      <td></td>
      <td>Lots of issues found and reported, awaiting fixes.</td>
    </tr>

    <tr>
      <td><a href="https://github.com/centaurean/density">density</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://bitbucket.org/attila_afra/doboz">doboz</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/davidcatt/FastARI">fari</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://fastlz.org/">fastlz</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://fastlz.org/">gipfeli</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/atomicobject/heatshrink">heatshrink</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://cyan4973.github.io/lz4/">lz4</a></td>
      <td>OK</td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://liblzf.plan9.de/">lzf</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://liblzg.bitsnbites.eu/">lzg</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/richgel999/lzham_codec/">lzham</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://en.wikipedia.org/wiki/LZJB">lzjb</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://tukaani.org/xz/">lzma</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://www.oberhumer.com/opensource/lzo/">lzo</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/coderforlife/ms-compress/">ms-compress</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://ncompress.sourceforge.net/">ncompress</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/johnezang/pithy">pithy</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://www.quicklz.com/">quicklz</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://google.github.io/snappy/">snappy</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/ShaneWF/wflz">wflz</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://bitbucket.org/tkatchev/yalz77">yalz77</a></td>
      <td>OK</td>
      <td></td>
      <td>Several issues found, <a href="https://bitbucket.org/tkatchev/yalz77/commits/07b5d3df427e981ee5cfb25094af1b731b14ed44">fixed</a>.</td>
    </tr>

    <tr>
      <td><a href="http://zlib.net/">zlib</a></td>
      <td>OK</td>
      <td><a href="https://github.com/nemequ/compfuzz/tree/master/libraries/zlib/patches">Patch</a></td>
      <td>
        <ul>
	      <li>Hanno Böck has done <a href="https://fuzzing-project.org/software.html">some testing</a></li>
        </ul>
	  </td>
    </tr>

    <tr>
      <td><a href="https://github.com/Dead2/zlib-ng/">zlib-ng</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="https://github.com/richox/libzling">zling</a></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>

    <tr>
      <td><a href="http://mattmahoney.net/dc/zpaq.html">zpaq</a></td>
      <td>OK</td>
      <td></td>
      <td>Several issues found, <a href="https://github.com/zpaq/zpaq/commit/176df1f453a9bcebc794bb928e5aff1c9e9d5585">fix released</a>.</td>
    </tr>

    <tr>
      <td><a href="https://github.com/Cyan4973/zstd">zstd</a></td>
      <td>OK</td>
      <td></td>
      <td>
        <ul>
	      <li>Evan Nemerson found some issues, which have been <a href="https://github.com/Cyan4973/zstd/commit/8f86c700cdb9190901613124100c9be4c6e69827">fixed</a></li>
	    </ul>
    </tr>
  </tbody>
</table>
