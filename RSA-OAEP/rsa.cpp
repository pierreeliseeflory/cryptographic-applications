#include <gmp.h>
#include <string>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <cstdint>
#include <gmpxx.h>

int TailleFichier(std::string nom_fichier) {
    std::ifstream fichier(nom_fichier);
	char c;
	int i = 0;
	while (fichier.get(c))
	{
		++i;
	}
    fichier.close();
    return i;
}

void LectureFichier(std::string nom_fichier, uint8_t message[])
{
	std::ifstream fichier(nom_fichier);

	char c;
	int i = 0;
	while (fichier.get(c))
	{
		message[i] = (uint8_t)(c - ' ' + 32);
		++i;
	}
    fichier.close();
}

void EcritureFichier(std::string nom_fichier, u_int8_t message[], int size_message)
{
	std::ofstream fichier(nom_fichier);

	for (int i = 0; i < size_message; ++i) {
        fichier << (char)(message[i]);
    }
    fichier.close();
}

bool MillerRabin(mpz_t n, int t) {
    mpz_t n_minus_1, r, a, y;
    mpz_inits(n_minus_1, r, a, y, NULL);
    mpz_sub_ui(n_minus_1, n, 1);

    unsigned long int s = 0;
    while(mpz_divisible_2exp_p(n_minus_1, s + 1) != 0) ++s;
    
    mpz_ui_pow_ui(a, 2, s);
    mpz_divexact(r, n_minus_1, a);

    gmp_randstate_t state;
    gmp_randinit_default(state);

    for (int i = 0; i < t; ++i) {
        mpz_urandomm(a, state, n);
        while(mpz_cmp_ui(a, 1) <= 0) mpz_urandomm(a, state, n);
        mpz_powm(y, a, r, n);

        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, n_minus_1) != 0)) {
            unsigned long int j = 1;
            while ((j + 1 <= s) && (mpz_cmp(y, n_minus_1) != 0))
            {
                mpz_powm_ui(y, y, 2, n);
                if (mpz_cmp_ui(y, 1) == 0) return false;
                ++j;
            }
            if (mpz_cmp(y, n_minus_1) != 0) return false;
        }
    }
    mpz_clears(n_minus_1, r, a, y, NULL);
    return true;
}

bool TestMillerRabin() {
    mpz_t int_to_test;
    mpz_init(int_to_test);
    mpz_set_str(int_to_test, "165662768476331452655463142159307594726439888048973551481125708299309926199937567887055952337053096253593328774393992891113177041034077391405996371956458581599453752211961308257919645235694661320798832362087215820368197623535119747320298409877175812147325203553853631499255757240996852309713216289118288455411",10);
    if (!(MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "166723068679764627327978587507833490991394924391606845214685735121501657989679185890322229545321845122290632941071644307450125281017074569451914040877907883153970695224220123060489829333090158523652510367384580802090937431624802152470191907831031402899256593419457768580979202399045364685862329132978316653033",10);
    if (!(MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "92466841930913404195392813980497818014855282761651298641161504996921657055612674743291274023848573474358374150774384177544205714985837416519641789143011219127583875859272887299279901281082541650625445930501274236670105246021352853313839350139389241459712730976150989543870171187822514258219756759227593447037",10);
    if (!(MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "108795338282625425486258914942688432278517623007266503732414472014492509098982966838891605019211115230393922661664463424759567013458342174426412287934725682832109360152092601376583611159811265785948705626089763877539532056377648619698146361835077023751441021830424745202677689122432638593977625725358631285809",10);
    if (!(MillerRabin(int_to_test, 30))) return false;

    mpz_set_str(int_to_test, "169194386999145244301578334235525013775755685493497405817414669671929355477611746617897998298375918029966885759120094202249012119003813514613632085568118760498152954403188135209862793730146549530274471665788978059673183010349122565029914451680989381155267322956265856693864167063761287467611165529988318817806",10);
    if ((MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "175432243105048059632795349970136950686971246699177412525682414510998795331674846389306042923975962839831263186196011237427765407334341815276722374497209224721909798501268345263282558326869474343582988159054088388293658109121891350270097156113835253916831416539796818931768952949211463943877788967763463234045",10);
    if ((MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "95007167882701898259467326653106726279217353575675849029496217437922870947536462140335982042575983588868226116614075438366516347591791263873973123522296647655987193343751649005600135755860553854091993964443961181555899345287041533943124982290746716388212986514794628428905772867116663697773006370829092886441",10);
    if ((MillerRabin(int_to_test, 30))) return false;
    mpz_set_str(int_to_test, "169400179702825113297718837524564738843617841664969850128874709057028562667346073839183995211982521726802153477992826669289025462018324838131743128686860025277660170453431714126349585342254478888243762789223330947856811192314667808289971186896420809003712664405280749679485270976470239981903351884263668208173",10);
    if ((MillerRabin(int_to_test, 30))) return false;

    mpz_clear(int_to_test);
    return true;
}

void ConcatenationHash(int maskLen, int output_size, mpz_t Y, mpz_t r) {
    // maskLen = 160
    // output_size = 1888
    mpz_t tmp, input;
    mpz_inits(tmp, input, NULL);

    int input_size = (maskLen/8) + 4;

    unsigned char ibuf[input_size];
    unsigned char obuf[20];

    size_t passes = (output_size/maskLen) - 1;
    size_t bits_restant = output_size%maskLen;

    mpz_set(input, r);
    mpz_mul_2exp(input, input, 32);
    mpz_export(ibuf, NULL, 1, sizeof(unsigned char), 0, 0, input);
    SHA1(ibuf, input_size, obuf);
    mpz_import(tmp, 20, 1, sizeof(unsigned char), 0, 0, obuf);
    mpz_set(Y, tmp);

    for (size_t counter = 1; counter < passes; ++counter) {
        mpz_add_ui(input, input, 1);
        mpz_export(ibuf, NULL, 1, sizeof(uint8_t), 0, 0, tmp);
        SHA1(ibuf, input_size, obuf);
        mpz_import(tmp, 20, 1, sizeof(unsigned char), 0, 0, obuf);
        mpz_mul_2exp(Y, Y, 160);
        mpz_add(Y, Y, tmp);
    }


    mpz_add_ui(input, input, 1);
    mpz_export(ibuf, NULL, 1, sizeof(uint8_t), 0, 0, input);
    SHA1(ibuf, input_size, obuf);
    mpz_import(tmp, 20, 1, sizeof(unsigned char), 0, 0, obuf);
    mpz_tdiv_q_2exp(tmp, tmp, bits_restant);
    mpz_mul_2exp(Y, Y, bits_restant);
    mpz_add(Y, Y, tmp);
    mpz_clears(tmp, input, NULL);
}

void SimpleHash(int maskLen, mpz_t X, mpz_t Y) {
    unsigned char ibuf[maskLen];
    unsigned char obuf[20];
    mpz_export(ibuf, NULL, 1, sizeof(uint8_t), 0, 0, X);
    SHA1(ibuf, maskLen, obuf);
    mpz_import(Y, 20, 1, sizeof(unsigned char), 0, 0, obuf);
    // std::cout << "test - Y = " << Y << std::endl;
    // std::cout << "test - X = " << X << std::endl;
}

size_t LectureGMP(mpz_t integer, std::string input) {
    size_t mLen;
    mLen = TailleFichier(input);
    uint8_t message[mLen];
    LectureFichier(input, message);
    mpz_import(integer, mLen, 1, sizeof(uint8_t), 0, 0, message);
    return mLen*8;
}

void EcritureGMP(mpz_t integer, std::string output) {
    int mLen = mpz_sizeinbase(integer, 2) / 8;
    if (mpz_sizeinbase(integer, 2) % 8 != 0) mLen++;
    uint8_t message_dechiffre[mLen];
    mpz_export(message_dechiffre, NULL, 1, sizeof(uint8_t), 0, 0, integer);
    EcritureFichier(output, message_dechiffre, mLen);
}

void Format(std::string nom_fichier, mpz_t OAEP, size_t nLen, int maskLen){
    mpz_t m, r, X, Y;
    mpz_inits(m, r, X, Y, NULL);

    size_t mLen = LectureGMP(m, nom_fichier);

    if (mLen > nLen - maskLen) {
        std::cout << "Message too long." << std::endl;
        return;
    }

    mpz_class t(m);
    size_t padding = nLen - mLen - maskLen;
    mpz_mul_2exp(m, m, padding);
    mpz_setbit(m, padding - 1);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_urandomb(r, state, 160);
    while(mpz_sizeinbase(r, 2) != (size_t)maskLen) {
        mpz_urandomb(r, state, 160);
    }


    ConcatenationHash(maskLen, nLen - maskLen, Y, r);
    mpz_xor(X, m, Y);

    SimpleHash((nLen - maskLen)/8, X, Y);

    mpz_xor(Y, Y, r);

    mpz_mul_2exp(OAEP, X, maskLen);
    mpz_add(OAEP, OAEP, Y);

    mpz_clears(m, r, X, Y, NULL);
}

void UnFormat (std::string nom_fichier, mpz_t OAEP, size_t nLen, int maskLen) {
    mpz_t X, Y, tmp;
    mpz_inits(X, Y, tmp, NULL);

    mpz_tdiv_q_2exp(X, OAEP, maskLen);
    mpz_tdiv_r_2exp(Y, OAEP, maskLen);

    SimpleHash((nLen - maskLen)/8, X, tmp);
    mpz_xor(Y, Y, tmp);

    ConcatenationHash(maskLen, nLen - maskLen, tmp, Y);
    mpz_xor(X, X, tmp);

    while(mpz_tstbit(X, 0) == 0) {
        mpz_tdiv_q_2exp(X, X, 1);
    }
    mpz_tdiv_q_2exp(X, X, 1);

    int mLen = mpz_sizeinbase(X, 2) / 8;
    if (mpz_sizeinbase(X, 2) % 8 != 0) mLen++;
    uint8_t message_dechiffre[mLen];
    mpz_export(message_dechiffre, NULL, 1, sizeof(uint8_t), 0, 0, X);
    EcritureFichier("dechiffre.txt", message_dechiffre, mLen);

    mpz_clears(X, Y, tmp, NULL);
}

void test () {
    mpz_t a, b, c;
    mpz_inits(a, b, c, NULL);

    mpz_set_str(a, "010101", 2);
    mpz_set_str(b, "1010101", 2);
    mpz_xor(c, a, b);

    mpz_class t1(a);
    mpz_class t2(b);
    mpz_class t3(c);

    std::cout << t1.get_str(2) << std::endl;
    std::cout << t2.get_str(2) << std::endl;
    std::cout << t3.get_str(2) << std::endl;

    mpz_clears(a, b, c, NULL);
}

void ExpMod(mpz_t x, mpz_t e, mpz_t n, mpz_t z) {
    //computes z = x^e mod n
    mpz_set_ui(z, 1);

    int taille_exposant = mpz_sizeinbase(e, 2);

    for (int i = taille_exposant; i >= 0; --i) {
        mpz_powm_ui(z, z, 2, n);
        if (mpz_tstbit(e, i) == 1) {
            mpz_mul(z, z, x);
            mpz_mod(z, z, n);
        }
    }
}

void testExp() {
    mpz_t a, b, c, d;
    mpz_inits(a, b, c, d, NULL);

    mpz_set_str(a, "153135", 10);
    mpz_set_str(b, "516", 10);
    mpz_set_str(c, "35468", 10);


    ExpMod(a, b, c, d);

    mpz_class t1(a);
    mpz_class t2(b);
    mpz_class t3(c);
    mpz_class t4(d);

    std::cout << t4 << " = " << a << " ^ " << b << " mod " << c << std::endl;

    mpz_clears(a, b, c, d, NULL);
}

void CreationCle(mpz_t p, mpz_t q, size_t cle_privee_rsa_taille) {
    gmp_randstate_t state;
    gmp_randinit_default(state);

    mpz_urandomb(p, state, cle_privee_rsa_taille);
    while((mpz_sizeinbase(p, 2) != cle_privee_rsa_taille) || (MillerRabin(p, 30) == false)) {
        mpz_urandomb(p, state, cle_privee_rsa_taille);
    }
    mpz_urandomb(q, state, cle_privee_rsa_taille);
    while((mpz_sizeinbase(q, 2) != cle_privee_rsa_taille) || (MillerRabin(q, 30) == false)) {
        mpz_urandomb(q, state, cle_privee_rsa_taille);
    }
}

void RSAChiffrement(std::string input, std::string output, mpz_t e, mpz_t n, int cle_publique_rsa_taille, int maskLen) {
    mpz_t message, cipher;
    mpz_inits(message, cipher, NULL);
    
    Format(input, message, cle_publique_rsa_taille, maskLen);

    ExpMod(message, e, n, cipher);

    EcritureGMP(cipher, output);

    mpz_clears(message, cipher, NULL);
}


void RSADechiffrement(std::string input, std::string output, mpz_t d, mpz_t n, int cle_publique_rsa_taille, int maskLen) {
    mpz_t cipher, decipher;
    mpz_inits(cipher, cipher, NULL);

    LectureGMP(cipher, input);

    ExpMod(cipher, d, n, decipher);

    UnFormat(output, decipher, cle_publique_rsa_taille, maskLen);

    EcritureGMP(decipher, output);

    mpz_clears(decipher, cipher, NULL);
}

void RSA(size_t cle_publique_rsa_taille) {
    mpz_t p, q, n, e, d, phi, message, cipher, decipher, tmp1, tmp2;
    mpz_inits(p, q, n, e, d, phi, message, cipher, decipher, tmp1, tmp2, NULL);

    if (cle_publique_rsa_taille%2 != 0) {
        std::cout << "Taille de clé publique incorrecte" << std::endl;
        return;
    }

    int cle_privee_rsa_taille = cle_publique_rsa_taille / 2;

    CreationCle(p, q, cle_privee_rsa_taille);

    mpz_mul(n, p, q);
    mpz_sub_ui(tmp1, p, 1);
    mpz_sub_ui(tmp2, q, 1);
    mpz_mul(phi, tmp1, tmp2);

    mpz_set_ui(e, 65537);
    mpz_invert(d, e, phi);

    Format("message.txt", message, cle_publique_rsa_taille, 160);

    ExpMod(message, e, n, cipher);

    ExpMod(cipher, d, n, decipher);

    UnFormat("dechiffre.txt", decipher, cle_publique_rsa_taille, 160);

    // RSAChiffrement("message.txt", "chiffre", e, n, cle_publique_rsa_taille, 160);
    // RSADechiffrement("chiffre", "dechiffre.txt", d, n, cle_publique_rsa_taille, 160);

    mpz_clears(p, q, n, e, d, phi, message, cipher, decipher, tmp1, tmp2, NULL);
}

int main(int argc, char const *argv[])
{
    // if (TestMillerRabin() == true) std::cout << "Le test de Miller-Rabin semble fonctionner" << std::endl;

    // test();
    // testExp();

    RSA(2048);

    return 0;
}
