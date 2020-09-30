#include <cstring>
#include <iostream>
#include <fstream>

#define P1 0.75
#define P2 0.25
#define P3 0.5
#define P4 0.75

int fonction_booleenne[16] = {0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1};

int r1[7] = {1, 0, 0, 0, 0, 0, 1};
int r2[6] = {1, 0, 1, 1, 0, 1};
int r3[9] = {0, 0, 0, 1, 0, 0, 0, 0, 1};
int r4[8] = {0, 1, 1, 1, 0, 0, 0, 1};

int FonctionBooleene(int bits_LSFR[], int nombre_LSFR, int fonction[])
{
	int index = 0;
	for (int i = 0; i < nombre_LSFR; ++i)
	{
		index += bits_LSFR[i] << i;
	}
	return fonction[index];
}

int Clock(int polynome_retroaction[], int taille, int *etat)
{
	int bit = 0;

	for (int i = 0; i < taille; ++i)
	{
		bit += polynome_retroaction[i] * etat[i];
	}
	bit %= 2;
	int bit_pseudo_aleatoire = etat[taille - 1];

	for (int i = taille - 1; i > 0; --i)
	{
		etat[i] = etat[i - 1];
	}
	etat[0] = bit;

	return bit_pseudo_aleatoire;
}

void PrintEtat(int etat[], int n)
{
	for (int i = 0; i < n; ++i)
		std::cout << etat[i];
	std::cout << std::endl;
}

void Initialize(int nombre_bit_produits)
{
	int etat_initial_1[7] = {1, 1, 1, 1, 1, 1, 1};
	int etat_initial_2[6] = {1, 0, 1, 0, 1, 0};
	int etat_initial_3[9] = {0, 0, 1, 1, 0, 0, 1, 1, 0};
	int etat_initial_4[8] = {1, 1, 0, 0, 1, 1, 0, 0};

	int etat_1[7];
	int etat_2[6];
	int etat_3[9];
	int etat_4[8];

	memcpy(etat_1, etat_initial_1, sizeof(etat_initial_1));
	memcpy(etat_2, etat_initial_2, sizeof(etat_initial_2));
	memcpy(etat_3, etat_initial_3, sizeof(etat_initial_3));
	memcpy(etat_4, etat_initial_4, sizeof(etat_initial_4));

	std::ofstream out("suite_chiffrante");
	std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(out.rdbuf());				 //redirect std::cout to out.txt!

	int counter = 0;

	while (counter < nombre_bit_produits)
	{
		int bit_LSFR[4] = {0};

		bit_LSFR[0] = Clock(r1, 7, etat_1);
		bit_LSFR[1] = Clock(r2, 6, etat_2);
		bit_LSFR[2] = Clock(r3, 9, etat_3);
		bit_LSFR[3] = Clock(r4, 8, etat_4);

		std::cout << FonctionBooleene(bit_LSFR, 4, fonction_booleenne);

		++counter;
	}
	std::cout << std::endl;
	std::cout.rdbuf(coutbuf); //reset to standard output again
	// std::cout << "Suite chiffrante initialisee." << std::endl;
}

void TestEtatInitialRegistre(int etat_initial[], int sequence[], int nombre_bit_produits, int registre)
{
	int counter = 0;
	while (counter < nombre_bit_produits)
	{
		switch (registre)
		{
		case 1:
			sequence[counter] = Clock(r1, 7, etat_initial);
			break;
		case 2:
			sequence[counter] = Clock(r2, 6, etat_initial);
			break;
		case 4:
			sequence[counter] = Clock(r4, 8, etat_initial);
			break;
		}

		++counter;
	}
}

void DecrypteEtatInitialRegistre(int polynome_retroaction[],
								 int taille_registre,
								 int taille_sequence, int sequence[], int registre, int seuil, int result[])
{
	unsigned int max = 1 << taille_registre;
	for (unsigned int i = 0; i < max; ++i)
	{
		int etat[taille_registre] = {0};
		int sequence_test[taille_sequence] = {0};
		for (int index = 0; index < taille_registre; ++index)
		{
			etat[index] = (i >> index) & 1;
		}
		TestEtatInitialRegistre(etat, sequence_test, taille_sequence, registre);
		int alpha = 0;
		for (int t = 0; t < taille_sequence; ++t)
		{
			if ((sequence_test[t] ^ sequence[t]) == 0)
				++alpha;
			else
				--alpha;
		}
		if ((seuil > 0 && alpha >= seuil) || (seuil < 0 && alpha <= seuil))
		{
			for (int index = 0; index < taille_registre; ++index)
				result[index] = (i >> index) & 1;
			return;
		}
	}
}

void LectureSequence(int sequence[], int taille_sequence)
{
	std::ifstream fichier_sequence("suite_chiffrante");

	char c;
	int i = 0;
	while (fichier_sequence.get(c))
	{
		sequence[i] = c - '0';
		++i;
	}
}

void SequencePartielle(int registre_1[], int retroaction_1[], int taille_1, int registre_2[], int retroaction_2[], int taille_2, int registre_4[], int retroaction_4[], int taille_4, int taille_sequence, int sequence[])
{

	int counter = 0;
	unsigned int bit_LSFR;
	while (counter < taille_sequence)
	{
		bit_LSFR = 0;
		bit_LSFR ^= (-Clock(retroaction_1, taille_1, registre_1) ^ bit_LSFR) & (1UL << 0);
		bit_LSFR ^= (-Clock(retroaction_2, taille_2, registre_2) ^ bit_LSFR) & (1UL << 1);
		bit_LSFR ^= (-Clock(retroaction_4, taille_4, registre_4) ^ bit_LSFR) & (1UL << 3);

		sequence[counter] = bit_LSFR;
		++counter;
	}
}

bool SequenceExhaustive(int registre[], int retroaction[], int taille, int taille_sequence, int sequence_partielle[], int sequence_relle[])
{
	int counter = 0;
	unsigned int bit_LSFR;
	// PrintEtat(sequence_relle, taille_sequence);
	while (counter < taille_sequence)
	{
		bit_LSFR = sequence_partielle[counter];
		bit_LSFR ^= (-Clock(retroaction, taille, registre) ^ bit_LSFR) & (1UL << 2);
		if (fonction_booleenne[bit_LSFR] != sequence_relle[counter])
			return false;
		++counter;
	}
	return true;
}

void AttaqueExhaustive(int registre_1[], int retroaction_1[], int taille_1, int registre_2[], int retroaction_2[], int taille_2, int registre_3[], int retroaction_3[], int taille_3, int registre_4[], int retroaction_4[], int taille_4, int taille_sequence, int sequence[])
{
	int sequence_partielle[taille_sequence] = {0};
	SequencePartielle(registre_1, retroaction_1, taille_1, registre_2, retroaction_2, taille_2, registre_4, retroaction_4, taille_4, taille_sequence, sequence_partielle);

	unsigned int max = 1 << taille_3;
	for (unsigned int i = 0; i < max; ++i)
	{
		for (int index = 0; index < taille_3; ++index)
		{
			registre_3[index] = (i >> index) & 1;
		}
		if (SequenceExhaustive(registre_3, retroaction_3, taille_3, taille_sequence, sequence_partielle, sequence))
		{
			for (int index = 0; index < taille_3; ++index)
				registre_3[index] = (i >> index) & 1;
			return;
		}
	}
}

int main()
{
	int N = 131;
	// Créé la suite chiffrante à partir d'états d'initailisation choisis arbitrairement supposés inconnus
	Initialize(N);

	// Créé un array de taille max(N_i) de la suite chiffrante z
	int sequence[N] = {0};
	LectureSequence(sequence, N);

	std::cout << "Registre n°1:" << std::endl;
	int etat_decrypte_1[7] = {0};
	DecrypteEtatInitialRegistre(r1, 7, N, sequence, 1, 32, etat_decrypte_1);
	PrintEtat(etat_decrypte_1, 7);
	std::cout << "Registre n°2:" << std::endl;
	int etat_decrypte_2[6] = {0};
	DecrypteEtatInitialRegistre(r2, 6, N, sequence, 2, -31, etat_decrypte_2);
	PrintEtat(etat_decrypte_2, 6);
	std::cout << "Registre n°4:" << std::endl;
	int etat_decrypte_4[8] = {0};
	DecrypteEtatInitialRegistre(r4, 8, N, sequence, 4, 49, etat_decrypte_4);
	PrintEtat(etat_decrypte_4, 8);

	int etat_decrypte_3[9] = {0};
	std::cout << "Attaque exhaustive sur le dernier registre..." << std::endl;
	AttaqueExhaustive(etat_decrypte_1, r1, 7, etat_decrypte_2, r2, 6, etat_decrypte_3, r3, 9, etat_decrypte_4, r4, 8, N, sequence);
	std::cout << "Registre n°3:" << std::endl;
	PrintEtat(etat_decrypte_3, 9);

	return 0;
}
