#include "../lib/prima.h"

void square_and_multiply(mpz_t r, mpz_t a, mpz_t exp, mpz_t modulo)
{
    mpz_set(r,a);
    /**
     * dec to bin
     */
    char * hi = mpz_get_str(NULL,2,exp);
    
    /**
     * pour i = t − 1, . . . , 0 faire
     */
    for(int i = 1; i < strlen(hi);i++)
    {
        mpz_mul(r,r,r);
        mpz_mod(r,r,modulo);
        /**
         * hi = 1 alors
         */
        if(hi[i] == '1')
        {
            mpz_mul(r,r,a);
            mpz_mod(r,r,modulo);
        }
    }
}

int fermat(mpz_t n, int k)
{
    int i;
    mpz_t res, a;
    mpz_t n2, n1;
    
    /**
     * NULL pour eviter un segmentation fault
     */
    mpz_inits(res, a, NULL);
    
    /**
     * initialisation de la seed default pour générer un entier
     */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    
    /**
     * n1 stock n-1
     */
    mpz_init_set(n1, n);
    mpz_sub_ui(n1, n1, 1);
    
    /**
     * n1 stock n-2, car mpz_urandomm génére un entier aléatoire compris entre 0 et n-1 inclus, on doit donc générer entre (0 < a < n-2) + 1 .
     */
    mpz_init_set(n2, n);
    mpz_sub_ui(n2,n2,2);
    
    
    /**
     * si n = 2 ou n = 1, on arrete car on peut pas generer de nombre
     */
    if(mpz_cmp_ui(n, 2) == 0){
        printf("[%sERROR-FERMAT%s] choisir un entier supérieur à 2 ! \n", RED, RESET);
        mpz_clears(n2, res, a, n1, state, NULL);
        return 1;
    }
    
    for(i = 0 ; i < k; i++){
        /* Generation aleatoire entre 0 < a < n-2 */
        mpz_urandomm(a, state, n2);
        /* On rajoute 1 pour avoir 1 < a < n-1 */
        mpz_add_ui(a, a, 1);
        /* On applique l’algorithme square and multiply pour verifier a^n-1 mod n */
        square_and_multiply(res, a, n1, n);
        /* On verifie si a^n-1 != 1 mod n */
        if(mpz_cmp_ui(res, 1) != 0){
            printf("[%sFERMAT%s] le nombre n est composé \n", MAG, RESET);
            mpz_clears(n2, res, a, n1, state, NULL);
            return 0;
        }
    }
    printf("[%sFERMAT%s] le nombre n est premier\n", MAG, RESET);
    mpz_clears(n2, res, a, n1, state, NULL);
    return 1;
}


/* Fonction qui permet de décomposer n - 1  en 2^s * t (t impair )*/
int decomposition(mpz_t n, mpz_t result, mpz_t cst1){
    int i=1;
    /* initialisation des variables t et _n */
    mpz_t t,_n;
    mpz_init(t);
    mpz_init(_n);
    
    
    mpz_set(_n,n); /* on attribut la valeur de n à _n */
    mpz_sub_ui(_n,_n,1); /* on soustrait 1 à _n ^pur avoir un nombre pair */
    mpz_set(t,_n); /* t aura comme valeur de depart _n avant d'être divisé*/
    
    while( mpz_cmp_ui(t,0) > 0 ){
        
        mpz_mul_2exp(t,cst1,i); /* on calcul 2^s avec s = i et on affecte à t*/
        
        mpz_divexact(t, _n, t);/* pon divise _n/t pour avoir un reste */
        
        if ( mpz_odd_p(t) ){ /* si le reste est impair alors on récupère le resultat et on arrête*/
            mpz_set(result,t);
            return i;
            break;
        }
        
        i++;/* sinon on continue tant que _n/t > 0*/
    }
    /*desalouer la mémoire des mpz */
    mpz_clear(t);
    mpz_clear(_n);
    return -1;
}

int miller_rabin(mpz_t n , int repetition){
    
    int s; /* variable qui servira de puissance lors de la decomposition */
    
    mpz_t a,t,y;
    mpz_t cst1,cst2; /*constante 1 et 2, uniquement pour travailler sur des fonctions qui n'acceptent que des mpz_t */
    mpz_init(a);
    mpz_init(t);
    mpz_init(y);
    mpz_init(cst1);
    mpz_init(cst2);
    
    mpz_set_ui(cst1,1); /* pour avoir 1 mod n avec cst1 = 1*/
    mpz_set(cst2,n);
    mpz_sub_ui(cst2,cst2,1);  /* pour avoir -1 mod n avec cst2 = n-1 */
    
    /* Initialisation random départ */
    gmp_randstate_t state;
    gmp_randinit_default(state);
    
    /* tester si n est un nombre impair*/
    if ( mpz_odd_p (n) ){
        
        /*on décompose n-1 et on recupere la puissance s et on modifie t (impair)  tel que : 2^s * t = n - 1 */
        s = decomposition(n, t, cst1);
        
        /* boucle sur la répétion donnée en paramètre */
        for(int i = 1; i<=repetition ; i++){
            
            /* on génère un nombre aléatoire compris entre 0 et n exclus */
            mpz_urandomm (a, state, n);
            
            /*on attendant de régler le state à 1*/ if (mpz_cmp_ui(a,0) == 0) mpz_set_ui(a,1);
            
            /* on calcul a^t mod n avec et on affecte à y*/
            square_and_multiply(y,a,t,n);
            
            /* on test si y n'est pas congrut à 1 et -1 mod n */
            if (!( mpz_congruent_p (y,cst1,n) ) & !(mpz_congruent_p (y,cst2,n) ) ){
                
                for(int j = 1; j<s; j++ ){
                    /* on calcul y^2 mod n avec et on affecte à y */
                    mpz_mul(y,y,y);
                    mpz_mod(y,y,n);
                    
                    if (mpz_congruent_p (y,cst1,n) ) return 0;/* n est composé */
                        if (mpz_congruent_p (y,cst2,n) ) break;
                        
                } /*fin du deuxieme for*/
                printf("[%sMILLER-RABIN%s] le nombre n est composé \n", MAG, RESET);
                return 0; /* n est composé*/
                
            } /* fin du if */
            
        } /* fin du premier for*/
        printf("[%sMILLER-RABIN%s] le nombre n est premier \n", MAG, RESET);
        return 1; /* n est premier*/
        
    }/*fin du if*/
    
    /*desalouer la mémoire des mpz et gmp_rand*/
    mpz_clear(a);
    mpz_clear(t);
    mpz_clear(y);
    mpz_clear(cst1);
    mpz_clear(cst2);
    gmp_randclear(state);
    
    if ( mpz_cmp_ui(n,2) == 0 )printf("[%sMILLER-RABIN%s] le nombre n est premier \n", MAG, RESET);
    else  printf("[%sMILLER-RABIN%s] le nombre n est composé \n", MAG, RESET);
    return -1; /* n est pair */
}

int main(){
    system("cat x.org");
    int k;
    mpz_t n;
    mpz_init(n);
    printf("\n");
    printf("[%sINFO%s] dm2crypto - prima - test de primalité", GRN, RESET);
    printf("\n");
    printf("[%sINFO%s] choisir un entier n : ", GRN, RESET);
    gmp_scanf("%Zd", &n);
    
    if(mpz_cmp_ui(n,1)<=0)
    {
        printf("[%sERROR%s] choisir un entier supérieur à 1 !\n", RED, RESET);
    }
    else
    {
        printf("[%sINFO%s] choisir un nombre de répétitions k : ", GRN, RESET);
        scanf("%d",&k);
        /* test de fermat */
        fermat(n,k);
        /* test de miller-rabin */
        miller_rabin(n, k);
    }
    
    mpz_clear(n);
    return 0;
    return 0;
}

