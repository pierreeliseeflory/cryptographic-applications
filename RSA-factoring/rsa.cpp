#include <cmath>
#include <gmp.h>
#include <iostream>
#include <string>

void FermatFactorisation(unsigned long int target) {
  mpz_t t, s, n;
  mpz_inits(t, s, n, NULL);
  mpz_set_ui(n, target);

  mpz_sqrt(t, n); // truncated value of sqrt, needs increment

  do {
    mpz_add_ui(t, t, 1); // increment
    mpz_mul(s, t, t);    // s = t²
    mpz_sub(s, s, n);    // s = t² - n
  } while (mpz_perfect_square_p(s) == 0);
  mpz_sqrt(s, s);

  std::cout << n;

  mpz_add(n, t, s);
  mpz_sub(s, t, s);

  std::cout << " = " << n << " * " << s << std::endl;
  mpz_clears(t, s, n, NULL);
}

bool IsPrime(unsigned long int p) {
  if (p <= 3)
    return p > 1;
  if ((p % 2 == 0) || (p % 3 == 0))
    return false;
  int i = 5;
  while (i * i <= p) {
    if ((p % i == 0) || (p % (i + 2) == 0))
      return false;
    i += 6;
  }
  return true;
}

int FastExponentiation(int a, int b) {
  if (b == 1)
    return a;
  if (b % 2 == 0)
    return FastExponentiation(a * a, b / 2);
  return a * FastExponentiation(a * a, (b - 1) / 2);
}

void PollardAux(gmp_randstate_t state, mpz_t n, mpz_t a, unsigned long int B) {
  // Select a value prime with the target
  mpz_urandomm(a, state, n);

  unsigned long int e;
  // Generation of the least common multiple of possible primes
  for (int p = 2; p < sqrt(B); ++p) {
    if (IsPrime(p)) {
      e = (unsigned long int)(FastExponentiation(p, floor(log(B) / log(p))));
      mpz_powm_ui(a, a, e, n);
    }
  }
  // Greatest common divisor
  mpz_sub_ui(a, a, 1);
  mpz_gcd(a, a, n);

  if (mpz_cmp_ui(a, 1) == 0) {
    return PollardAux(state, n, a, 2 * B);
  } else if (mpz_cmp(a, n) == 0) {
    return PollardAux(state, n, a, B);
  } else
    std::cout << "p = " << a << std::endl;
}

void PollardFactorisation(unsigned long int target, unsigned long int B) {
  mpz_t n, a;
  mpz_inits(a, n, NULL);
  mpz_set_ui(n, target);

  gmp_randstate_t state;
  gmp_randinit_default(state);

  PollardAux(state, n, a, B);

  mpz_clears(a, n, NULL);
  gmp_randclear(state);
}

void MultipleModulusFactorisation(char const *modulus_string[],
                                  int number_modulus) {
	mpz_t p, pgcd, tmp;
	mpz_inits(p, pgcd, tmp, NULL);

	mpz_t modulus[number_modulus];

	for (int i = 0; i < number_modulus; ++i)
		mpz_init_set_str(modulus[i], modulus_string[i], 16);

	mpz_set(p, modulus[0]);
	for (int i = 1; i < number_modulus; ++i)
		mpz_mul(p, p, modulus[i]);

	for (int i = 0; i < number_modulus; ++i) {
		mpz_divexact(tmp, p, modulus[i]);
		mpz_gcd(pgcd, tmp, modulus[i]);

		if (mpz_cmp_ui(pgcd, 1) == 0) {
		std::cout << "Pas de résultat pour la clé RSA n°" << i << std::endl;
		} else if (mpz_cmp(pgcd, modulus[i]) == 0) {
		for (int j = 0; j < number_modulus; ++j) {
			std::cout << "aaaa" << std::endl;
			if (j != i) {
				mpz_gcd(pgcd, modulus[j], modulus[i]);
				if (mpz_cmp_ui(pgcd, 1) != 0) {
				mpz_divexact(tmp, modulus[i], pgcd);
				std::cout << "Clé n°" << i << " : " << modulus[i] << " = " << pgcd
						<< " * " << tmp << std::endl;
				break;
			}
			}
		}
		} else {
			mpz_divexact(tmp, modulus[i], pgcd);
			std::cout << "Clé n°" << i << " : " << modulus[i] << " = " << pgcd
					<< " * " << tmp << std::endl;
		}
	}

	for (int i = 0; i < number_modulus; ++i)
		mpz_clear(modulus[i]);
	mpz_clears(p, pgcd, tmp, NULL);
}

int main(int argc, const char **argv) {
  FermatFactorisation(23360947609);

  PollardFactorisation(444853, 200);
  PollardFactorisation(2057574960, 200);
  PollardFactorisation(5270033701, 200);
  PollardFactorisation(1593351640742417, 200);

  char const *modulus_string[5] = {
      "08fdea86945913bebd4fe54c79730bbec8e0c59ef9ee655d4a2dcf617670aa5f",
      "8b9138464a052bb6dec956411e86e09c32d9639080fd4732bfcd1cfa7001a191",
      "0630b5bad68bde511dc9c005001dfcafaf04ad817cfd7b8472fadf17f71aa93d",
      "42c9069934df7b74ff2b128ed9a1ca02708e8fda139bd17d39cc738ad64b5ba9",
      "23fd34c25101e28cb88e42cbe122fa21abfbe62f23f626926bb17bcc016466d1"};

  // char const* modulus_string[5] = {
  //     "AAD7853BF1E1711743AD049CDD5F0D0FE86720401BC5E1106A9DD2E462C1F1CF",
  //     "C76AD671D04F804FCEFE71E90FE0483A714330B7079BD44F763EAB43587F9C37",
  //     "B6B71E6354A2508E2C1C04BE448183F55D1809CDCC5EE86F2D8439043B063505",
  //     "BEA85F9857DC06679AEF6CAAE28E074D647ECA7DED3DA04E72642B8C16CFE31F",
  //     "B1F7C9CB52199620F30307F3A95E1A1EF353AD096634F2F81B6E955DC32A09BD"
  // };

  MultipleModulusFactorisation(modulus_string, 5);

  return 0;
}