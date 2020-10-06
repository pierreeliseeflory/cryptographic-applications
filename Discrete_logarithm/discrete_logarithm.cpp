#include <cmath>
// #include <gmp.h> 
#include <iostream>
#include <string>
#include <bits/stdc++.h>

int GcdExtended(int a, int b, int *x, int *y) {
   if (a == 0) {
      *x = 0;
      *y = 1;
      return b;
   }
   int x1, y1;
   int gcd = GcdExtended(b%a, a, &x1, &y1);
   *x = y1 - (b/a) * x1;
   *y = x1;
   return gcd;
}

int IsInTable(int T[], int size, int beta) {
    for (int i = 0; i < size; ++i) {
        if (T[i] == beta) return i;
    }
    return -1;
}

int Shanks(int p, int g, int beta) {
    int m = ceil(sqrt(p));
    int x, alpha;

   GcdExtended(p, pow(g, m), &x, &alpha);
   alpha = (alpha + p) % p;

   int T[m];

   for (int i = 0; i < m; ++i) {
       T[i] = ((int)(pow(g, i)) + p) % p;
   }
   int i = 0, j;
   while (IsInTable(T, m, beta) < 0) {
       beta = (beta*alpha) % p;
       ++i;
   }
   j = IsInTable(T, m, beta);
    return m*i+j;
}

int modulo( int value, int m) {
    int mod = value % m;
    if (mod < 0) {
        mod += m;
    }
    return mod;
}

int f (int xi, int beta, int alpha, int mod) {
    switch (modulo(xi, 3))
    {
    case 1:
        return modulo(beta*xi, mod);
    case 0:
        return modulo(xi*xi, mod);
    default:
        return modulo(alpha*xi, mod);
    }
}

int g (int ai, int xi, int mod) {
    switch (modulo(xi, 3))
    {
    case 1:
        return modulo(ai, mod);
    case 0:
        return modulo(2*ai, mod);
    default:
        return modulo(ai+1, mod);
    }
}

int h (int bi, int xi, int mod) {
    switch (modulo(xi, 3))
    {
    case 1:
        return modulo(bi+1, mod);
    case 0:
        return modulo(2*bi, mod);
    default:
        return modulo(bi, mod);
    }
}

int RhoPollard(int taille, int ordre, int alpha, int beta) {
    int ai=0, a2i=0, bi=0, b2i=0, xi=1, x2i=1;
    for (int i = 0; i < taille; ++i) {
        ai = g(ai, xi, ordre);
        bi = h(bi, xi, ordre);
        xi = f(xi, beta, alpha, taille); // x_i+1
        a2i = g(a2i, x2i, ordre);
        b2i = h(b2i, x2i, ordre);
        x2i = f(x2i, beta, alpha, taille); // x_2i+1
        a2i = g(a2i, x2i, ordre);
        b2i = h(b2i, x2i, ordre);
        x2i = f(x2i, beta, alpha, taille); // x_2(i+1)
        if ((xi == x2i) && (b2i != bi) ) {
            int x, binv, b = modulo(bi-b2i, ordre);
            GcdExtended(ordre, b, &x, &binv);
            return modulo((a2i - ai)*binv, ordre);
        }
    }
    return 0;
}

int main(int argc, const char **argv) {
    std::cout << Shanks(113, 3, 31) << std::endl;

    std::cout << RhoPollard(383, 191, 2, 228) << std::endl;
   return 0;
}
