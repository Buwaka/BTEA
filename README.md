# XXTEA encryption implementation.
XXTEA is the 2nd iteration on TEA (Tiny Encryption Algorithm). It is known for how compact, yet effective of an encryption algorithm it is.
https://en.wikipedia.org/wiki/XXTEA


## Introduction

It is a commandline program requiring 4 parameters:
* Action: either "Encrypt" or "Decrypt"
* Input: an input file to be encrypted or decrypted based on the action (has been tested with plaintext, but should in theory also work on binary files)
* Password: a password to encrypt or decrypt the input file with
* Output: a file or location to write the encrypted or decrypted file to.

Example:
```
.\Deencrypt.exe Encrypt "input.txt" "password" "output.txt"

.\Deencrypt.exe Decrypt "output.txt" "password" "decrypt.txt"
```
