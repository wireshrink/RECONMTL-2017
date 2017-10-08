# DVSE Walkthrough

Introduction and the purpose of the document
============================================

This document describes a walkthrough of DVSE exploitation.

DVSE (or Damn Vulnerable SGX Enclave) is a training target for hacking SGX
enclaves.

It was presented first time \@Recon2017 and resides at [this github
repository](https://github.com/wireshrink/RECONMTL-2017).

The presentation which resides in [this
folder](https://github.com/wireshrink/RECONMTL-2017/docs) of this repository
provides background about it and some additional links for the concerned fellow
citizens. Reading the presentation first will ease the reading of this document
significantly.

Application and its workflow
============================

The application consists of client, server and the Evil DRM :) . In the current
configuration variant, it provides a possibility to play some public domain
cartoons. The configuration is stored in so-called EPG, or “electronic program
guide”, and described below.

### Initial EPG (Electronic Program Guide) state

Here is the content of the EPG.

>   Popeye - i don't care,1,2,payment required,

>   Popeye - patriotic popeye,2,2,payment required,

>   Popeye - private eye popeye,3,2,payment required,

>   Popeye - shuteye popeye,4,2,payment required,

>   The big bad wolf,6,2,payment required,

>   woody woodpecker pantry panic,7,2,free view,

It contains 6 public domain cartoons, each cartoon is described in a single
line, parameters separated by comma, where the first string is a cartoon name,
movie id is a number which follows the name, and the last string defines the
payment requirements.

### The movie play workflow

1.  User defines connection parameters and the library directory if needed (on
    first run only)

2.  Application initializes the SGX enclave

3.  On initialization, the enclave connects to the server with usage of enclaved
    OpenSSL, checks the certificate of the server by its certificate’s SHA1
    signature, and downloads from there 3 files

    1.  Blob – template for storing local user payment data (if previous version
        does not exist already)

    2.  Coupons – a list of coupons which allow to use so-called payed movies

    3.  Epg – a list of currently available movies in the text format

4.  Then user opens the EPG window, and double-clicks the movie.

5.  The application asks the enclave to prepare the movie for play.

6.  The enclave downloads the movie from the server and stores it encrypted in
    the library folder

7.  The enclave is checking if one of the following conditions is met:

    1.  The movie is free to play

    2.  The user has enough coins to allow playing the movie now

    3.  The user already paid for this movie and the grace period (2 hours
        specifically) did not pass yet (the current time is obtained from the
        outside via OCALL)

8.  If one of these conditions is met, the enclave opens the corresponding media
    file via OCALL and became ready for playing

9.  The application asks for movie chunks as it needs and plays the movie

    1.  Enclave reads encrypted data, decrypts it and gives to the application

### Coupons and paying “protected movies” workflow

If the user wants to play a movie which is marked as “payment required” in the
EPG {s}he needs a means to pay for it. These means are called “coupons” and
described in “coupons” file on the server side. Each coupon has 32 byte length
and starts from string DVSE_CPN.

If user has a coupon in his possession {s}he can apply it via setup dialog.

When user pastes the coupon into the corresponding edit box and presses “apply”
button the following happens:

1.  The enclave-driving application calls an enclave in order to verify the
    coupon.

2.  Inside an enclave the coupon is compared with the content of coupons file.

3.  If one of the coupon matches the enclave checks if was used previously by
    comparing it with the list of previously used coupons which is stored in the
    blob file.

4.  If it was not used previously user’s coupon balance is increased by 25 coins
    and the used coupon is wrote into the blob as used.

5.  Coupons acquired in this way can be used for purchasing protected movies as
    described in previous chapter.

Deliberately inserted vulnerabilities
=====================================

| Vulnerability                                                                     | How to find and exploit                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          | How to avoid                                                                                                                                                                                                                           | Category           |
|-----------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------|
| No replay protection on the blob file                                             | Can be found by design review/reverse engineering. Can be exploited by simple deleting of the blob file (which will cause to creating empty one without marks of used coupons), or storing the blob file outside after applying all coupons and returning it back when needed. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                  | Store balance data on the server. Insert monotonic counter into the blob which will increase with each rewrite, and delete the library when somebody tampers it. Don’t use coupons as an idea or connect coupon to the specific movie. | Bad design          |
| Strcmp is used to compare coupon values                                           | Can be found by code review/reverse engineering. Can be exploited as any other timing attack on strcmp. **Important note**: Implementations of strcmp in the SGX SDK on Windows and Linux are very much different: Windows implementation is heavily optimized and based on SSE2 instruction set extension, which makes this attack on windows much less practical.                                                                                                                                                                                                                                                              | Don’t use strcmp on sensitive data, it is evil and ill-reputed practice.                                                                                                                                                               | Bad code            |
| Files are downloaded regardless playing allowance                                 | Can be found by code review/reverse engineering. Cannot be exploited directly, but can (and will) be used with other bugs. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                                                                                                                                                      | Don’t store something that you don’t know you really need.                                                                                                                                                                             | Bad code/Bad design |
| The enclave trusts the time got via OCALL                                         | Can be found by code review/design review/reverse engineering. Can be exploited by hijacking the timestamp before it is sent back to the enclave during corresponding OCALL execution. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                                                                                          | Don’t trust OCALLs. In this specific case use time from server provided with via network.                                                                                                                                              | Bad code/bad design |
| The enclave trusts to the file handle got via OCALL                               | Can be found by code review/design review/reverse engineering. Can be exploited by opening another file during OCALL execution (for example in order to make an enclave think that it opens file allowed to play). See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                                                              | Don’t trust OCALLs. In this specific case movie id can be embedded into the data guarded by AES GCM, for example into so called AAD, and checked on reading and opening.                                                               | Bad code/bad design |
| Decrypted coupons are stored in enclave memory decrypted all the enclave lifetime | Can be found by code review/design review/reverse engineering. Cannot be exploited solely, but if some data is leaked your secrets may be inside. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                                                                                                                               | Don’t leave the secrets unattended, decrypt secrets only when they are really needed and encrypt them back afterwards.                                                                                                                 | Bad code/bad design |
| Missing index check in reading the EPG pages                                      | Can be found by code review /reverse engineering/ fuzzing. Can be exploited by reading the non-existing EPG pages, all the heap will probably be leaked. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                                                                                                                        | Write good code, validate user input, conduct code review, fire bad programmers on time.                                                                                                                                               | Bad code            |
| Sealed log file write function is left by mistake in the enclave                  | Can be found by code review/ reverse engineering. Can be exploited by writing enough logs in order to reveal key generation material and AES GCM intermediate results that can be used for decrypting the content without knowledge about exact key value. See “” function in EnclaveTest project for exploitation example.                                                                                                                                                                                                                                                                                                      | Write good code, validate user input, conduct code review, fire bad programmers on time. \#ifdef \_DE**N**UG is a more common mistake that it is usually considered.                                                                   | Bad code            |
| Type cast mistake in sealing replacement function                                 | Can be found by code review/ reverse engineering. Can be exploited altogether with previous vulnerability: this cast mistake leads to significant key derivation material exhaustion. See “” function in EnclaveTest project for exploitation example. Note: this vulnerability was implemented with mistake that significantly hardens its exploitation – yes, 6 bytes of random data for key derivation is significantly less then 44 bytes, but it still requires a lot of time and disk space to generate intermediate GCM results with the previous vulnerability. However, it is possible – see the usage algorithm below. | Write good code, validate user input, conduct code review, fire bad programmers on time.                                                                                                                                               | Bad code            |

Exploitation examples walkthrough
=================================

Exploitation of the vulnerabilities described above was implemented in the
recent changes of EnclaveTest project in the main workspace of this solution.

The following scenarios were implemented (detailed explanation follows):

-   Memory exfiltration attack (was demonstrated on Recon)

-   Key material exhaustion attack

-   File substitution attack

-   Blob reuse attack

Two more possible scenarios were not implemented:

-   Time substitution attack is left for the user exercise.

-   Timing attack on coupon comparison – unfortunately it is not practical on
    Windows because the Windows SGX SDK implements strcmp with SSE2 instruction
    extensions. According to the source of Linux SGX SDK it *should* work on
    Linux, but I didn’t port all this to Linux *yet*.

Memory exfiltration attack
--------------------------

### Requirements for successful attack

* None

### Vulnerabilities used

* Missing index check in reading the EPG
pages
* Decrypted coupons are stored in enclave memory decrypted all the enclave
lifetime

### Exploitation algorithm

Just read the enclave memory as an EPG pages, by this you can read all the heap
including coupon values. Usage example is at void e0_exfiltrate_data(char\*
server_ip, int iport, char\* library_folder) function located at
exfiltrate_data.cpp file – it just reads 20 pages back and forward from the EPG.
You can find the coupons in this memory.

Key material exhaustion attack
------------------------------

### Requirements for successful attack

Some disk space (I had 8 TB and really needed only \~2 TB)

### Some explanations on AES-GCM

AES-GCM is AES used in so-called GCM (Galois/Counter mode).

It is described well in Wikipedia
[here](https://en.wikipedia.org/wiki/Galois/Counter_Mode) .

The main idea here is converting block cypher to stream cypher by the following
procedure (very much simplified, only relevant part of the definition here, all
the topic of GMAC is left behind):

1.  Choose 12 byte of random IV and the key

2.  The rest of 16 byte IV is a block counter (4 bytes). Randomize initial
    counter value.

3.  Keep the key (or key generation material) in secret, and pass the IV with
    the cypher text.

4.  For each block

    1.  Increase counter

    2.  Encrypt the block of zeros with the key and resulting IV.

    3.  XOR the encrypted zeros with the corresponding plain text block to get a
        cypher text

This definition means one very simple thing: If we can find a known plain text
encrypted with the specific IV and key we can decrypt any other block of data
encrypted with the same IV and key.

### Vulnerabilities used

* Sealed log file write function is left by mistake in the
enclave
* Type cast mistake in sealing replacement
function

### Exploitation algorithm

-   Find enough disk space (something like \~2TB was enough for me, I used external disks)
-   Create an enclave
-   Switch the enclave output for file writing to one file (see example of how
    to do it at void e5_pregenerate_logs(char\* server_ip, int port, char\*
    library_folder, char\* file_to_gen_to, size_t num_of_chunks)
-   Use vulnerability [Sealed log file write function is left by mistake in the
    enclave](#sealed-log-file-write-function-is-left-by-mistake-in-the-enclave)
    to write as much as possible sealed log strings with known content (I used
    1K zeros) to some file

-   Request as much as possible coupon files and collide them into one file(see
    example of how to do it at void e5_pregenerate_coupons(char\* server_ip, int
    port, char\* library_folder, char\* file_to_add_to). You’ll probably need to
    run the executable a lot of times (I ran it from windows command line with
    loop and got something like 1975567 unique variants of the same encrypted
    file)

-   Review the sealing implementation in the enclave and the bug, find offsets
    of non-zero key material

-   Load all coupon file variants into memory, index by non-zero key-material

-   Start reading log file block by block. If non-zero key material matches –
    xor the data with the “well known content” (again, I used 1K zeros) and xor
    the resulting data with the matching coupon file (see example of how to do
    it at void e5_crypto_extraction(char\* server_ip, int port, char\*
    library_folder, char\* coupons_file, char\* logs_file).

-   As a result, you’ll see the decrypted coupons.

File substitution attack
------------------------

### Requirements for successful attack

* At least one file in the EPG should be defined as free-to-view

### Vulnerabilities used

* The enclave trusts to the file handle got via
OCALL

### Exploitation algorithm

The main idea here is to make the enclave think that it always reads and
decrypts the free-to-view movie. Here is how it is possible (example at void
e2_file_substitution(char\* server_ip, int iport, char\* library_folder):

-   Create an enclave and initialize it

-   Read the EPG, find free-to-play movie id

-   For each movie

    -   Call prepare movie on it. This will download movie, determine that you
        have no enough coupons and return false

    -   Configure fopen OCALL to open the current sealed movie file instead of
        the file required by the enclave

    -   Read this movie as free-to-play movie. Enclave will think that it opened
        it as free to play, but will decrypt what you need.

Blob reuse attack (using coupons more than once)
------------------------------------------------

### Requirements for successful attack

You need to know coupon values. You can get these values by at least 2 possible
ways (key material exhaustion attack and memory exfiltration attack) on Windows.

### Vulnerabilities used

* No replay protection on the blob file

### Exploitation algorithm

The main idea is very simple – just remove the blob file where all the data
about already used coupons stored when needed. This will cause the enclave to
create a brand shiny new one and will give you a possibility to use the coupons
more then once. You can see the example of this attack in void
e3_blob_reuse(char\* server_ip, int iport, char\* library_folder)

-   Create and initialize an enclave

-   Read the EPG

-   Store all required file ids

-   Unload the enclvale

-   For each needed file:

    -   Delete all the service files (blob, coupons, epg)

    -   Load and initialize the enclave (this will create new files)

    -   Apply already known coupons

    -   Read the file and store it

    -   Unload the enclave
