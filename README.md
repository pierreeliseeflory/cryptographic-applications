# Cryptographic applications

This repository gather various implementations of cryptographic applications and functions I have coded.

## Applications

### [Vigenere](Vigenere/vigenere.cpp)

This code implements the algorithm to code and decode a message using a key according to the polyalphabetic substitution performed in Vigenere encoding.

I also implemented the cryptanalysis using the index of coincidence.

### [Siegenthaler](LFSR-siegenthaler/siegenthaler.cpp)

This code enables to produce a sequence of bits using multiple Linear Feedback Shift Register combined with a boolean function.

The code also contains the implementation of the attack proposed by Siegenthaler to break such systems with statistical bias towards some registers, reducing the size of the state-space to break with brute-force.