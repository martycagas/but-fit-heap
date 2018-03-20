
/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Bohuslav Křena, říjen 2016
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu před prvním použitím,
**      DisposeList ... zrušení všech prvků seznamu,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
void Error()
{
  printf ("*ERROR* The program has performed an illegal operation.\n");
  errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
void InitList (tList *L)
{
	L->Act = NULL;
  L->First = NULL;
}

/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
**/
void DisposeList (tList *L)
{
	tElemPtr freePtr = L->First;
  tElemPtr tempPtr;

  L->Act = NULL;
  L->First = NULL;

  while (freePtr != NULL)
  {
    tempPtr = freePtr->ptr;

    free(freePtr);

    freePtr = tempPtr;
  }
}

/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/
void InsertFirst (tList *L, int val)
{
  tElemPtr secondPtr = L->First;
  tElemPtr newPtr;

  newPtr = (tElemPtr) malloc(sizeof(struct tElem));

  if (newPtr == NULL)
  {
    Error();
    // fprintf(stderr, "Error: Unable to allocate required memory.\nIn: void InsertFirst (tList *L, int val)");

    return;
  }

  newPtr->ptr = secondPtr;
  newPtr->data = val;
  L->First = newPtr;
}

/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
void First (tList *L)
{
	L->Act = L->First;
}

/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/
void CopyFirst (tList *L, int *val)
{
	if (L->First == NULL)
  {
    Error();
    // fprintf(stderr, "Error: Unable retrieve the requested data.\nDetails: List is empty\nIn: void CopyFirst (tList *L, int *val)");

    return;
  }

  * val = L->First->data;
}

/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
void DeleteFirst (tList *L)
{
  if (L != NULL && L->First != NULL)
  {
    if (L->Act == L->First)
    {
      L->Act = NULL;
    }

    tElemPtr freePtr = L->First;

    L->First = L->First->ptr;

    free(freePtr);
  }
}

/*
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/
void PostDelete (tList *L)
{
  if (L->Act != NULL && L->Act->ptr != NULL)
  {
    tElemPtr freePtr = L->Act->ptr;

    L->Act->ptr = L->Act->ptr->ptr;

    free(freePtr);
  }
}

/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
void PostInsert (tList *L, int val)
{
  if (L->Act != NULL)
  {
    tElemPtr nextPtr = L->Act->ptr;
    tElemPtr newPtr;

    newPtr = (tElemPtr) malloc(sizeof(struct tElem));

    if (newPtr == NULL)
    {
      Error();
      // fprintf(stderr, "Error: Unable to allocate required memory.\nIn: void PostInsert (tList *L, int val)");

      return;
    }

    newPtr->ptr = nextPtr;
    newPtr->data = val;
    L->Act->ptr = newPtr;
  }
}

/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
void Copy (tList *L, int *val)
{
  if (L->Act == NULL)
  {
    Error();
    // fprintf(stderr, "Error: Unable retrieve the requested data.\nDetails: List is not active\nIn: void Copy (tList *L, int *val)");

    return;
  }

  * val = L->Act->data;
}

/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
void Actualize (tList *L, int val)
{
  if (L->Act != NULL)
  {
    L->Act->data = val;
  }
}

/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
void Succ (tList *L)
{
  if (L->Act != NULL)
  {
    L->Act = L->Act->ptr;
  }
}

/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return.
**/
int Active (tList *L)
{
  return (L->Act != NULL);
}

/* Konec c201.c */
