/**
* Kostra programu pro 3. projekt IZP 2015/16
*
* Jednoducha shlukova analyza: 2D nejblizsi soused.
* Single linkage
* http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
* Ladici makra. Vypnout jejich efekt lze definici makra
* NDEBUG, napr.:
*   a) pri prekladu argumentem prekladaci -DNDEBUG
*   b) v souboru (na radek pred #include <assert.h>)
*      #define NDEBUG
*/
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
* Deklarace potrebnych datovych typu:
*
* TYTO DEKLARACE NEMENTE
*
*   struct objt_t - struktura objektu: identifikator a souradnice
*   struct cluster_t - shluk objektu:
*      pocet objektu ve shluku,
*      kapacita shluku (pocet objektu, pro ktere je rezervovano
*          misto v poli),
*      ukazatel na pole shluku.
*/

struct obj_t {
	int id;
	float x;
	float y;
};

struct cluster_t {
	int size;
	int capacity;
	struct obj_t *obj;
};

/*****************************************************************
* Deklarace potrebnych funkci.
*
* PROTOTYPY FUNKCI NEMENTE
*
* IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
*
*/

/*
Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
// TODO
void init_cluster(struct cluster_t *c, int cap)
{
	assert(c != NULL);
	assert(cap >= 0);

	c->size = 0;

	if (cap == 0)
	{
		c->capacity = 0;
		c->obj = NULL;
	}
	else
	{
		c->capacity = cap;
		c->obj = malloc(cap*sizeof(struct obj_t));
	}
}

/*
Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
*/
// TODO
void clear_cluster(struct cluster_t *c)
{
	if (c->obj != NULL)
	{
		free(c->obj);
		init_cluster(c, 0);
	}
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
*/
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
	// TUTO FUNKCI NEMENTE
	assert(c);
	assert(c->capacity >= 0);
	assert(new_cap >= 0);

	if (c->capacity >= new_cap)
		return c;

	size_t size = sizeof(struct obj_t) * new_cap;

	void *arr = realloc(c->obj, size);
	if (arr == NULL)
		return NULL;

	c->obj = arr;
	c->capacity = new_cap;
	return c;
}

/*
Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
nevejde.
*/
// TODO
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if (c->size >= c->capacity)
	{
		resize_cluster(c, c->capacity + CLUSTER_CHUNK);
	}

	c->obj[c->size] = obj;
	c->size += 1;
}

/*
Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
*/
void sort_cluster(struct cluster_t *c);

/*
Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
Shluk 'c2' bude nezmenen.
*/
// TODO
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c2 != NULL);

	for (int i = 0; i < c2->size; i++)
	{
		append_cluster(c1, c2->obj[i]);
	}

	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
(shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
pocet shluku v poli.
*/
// TODO
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
	assert(idx < narr);
	assert(narr > 0);

	clear_cluster(&carr[idx]);

	for (int i = idx; i < narr - 1; i++)
	{
		carr[i] = carr[i + 1];
	}

	return narr - 1;
}

/*
Pocita Euklidovskou vzdalenost mezi dvema objekty.
*/
// TODO
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
	assert(o1 != NULL);
	assert(o2 != NULL);

	float dx = o1->x - o2->x;
	float dy = o1->y - o2->y;

	return sqrt(dx*dx + dy*dy);
}

/*
Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
souseda.
*/
// TODO
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
	assert(c1 != NULL);
	assert(c1->size > 0);
	assert(c2 != NULL);
	assert(c2->size > 0);

	float min = obj_distance(&c1->obj[0], &c2->obj[0]);
	float dist;

	for (int i = 0; i < c1->size; i++)
	{
		for (int j = 0; j < c2->size; j++)
		{
			dist = obj_distance(&c1->obj[i], &c2->obj[j]);

			if (dist < min)
			{
				min = dist;
			}
		}
	}
	return min;
}

/*
Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
*/
// TODO
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
	assert(narr > 0);

	if (narr > 1)
	{
		float min = cluster_distance(&carr[0], &carr[1]);
		float dist;

		for (int i = 0; i < narr; i++)
		{
			for (int j = 0; j < narr; j++)
			{
				if (i != j)
				{
					dist = cluster_distance(&carr[i], &carr[j]);

					if (dist < min)
					{
						*c1 = i;
						*c2 = j;
						min = dist;
					}
				}
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
	// TUTO FUNKCI NEMENTE
	const struct obj_t *o1 = a;
	const struct obj_t *o2 = b;
	if (o1->id < o2->id) return -1;
	if (o1->id > o2->id) return 1;
	return 0;
}

/*
Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
	// TUTO FUNKCI NEMENTE
	for (int i = 0; i < c->size; i++)
	{
		if (i) putchar(' ');
		printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
	}
	putchar('\n');
}

/*
Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
// TODO
int load_clusters(char *filename, struct cluster_t **arr)
{
	assert(arr != NULL);

	FILE *datafile = NULL;

	datafile = fopen(filename, "r");

	if (datafile == NULL)
	{
		// Failed to open file
		// *arr pointer is set to NULL
		*arr = NULL;
		fprintf(stderr, "In function load_clusters:\nFailed to open the file %s.\n", filename);
		return -1;
	}

	int objectCount = 0;
	int loadCount = 0;
	float x = 0;
	float y = 0;
	char buffer[100];

	fgets(buffer, 99, datafile);

	sscanf(buffer, "count=%d", &objectCount);

	*arr = malloc(objectCount*sizeof(struct cluster_t));

	for (int i = 0; i < objectCount; i++)
	{
		init_cluster(&(*arr)[i], 1);

		if (fgets(buffer, 99, datafile) == NULL)
		{
			// Reached end of file - count given in file was greater than the amount of objects in file.
			fprintf(stderr, "In load_clusters:\nObject count stated by the first line (%d) is greater than the amount of objects in the file.\n", objectCount);
			return -1;
		}

		sscanf(buffer, "%d %f %f", &(*arr)[i].obj[0].id, &x, &y);

		if ((x >= 0 && x <= 1000) && (y >= 0 && y <= 1000))
		{
			(*arr)[i].obj[0].x = x;
			(*arr)[i].obj[0].y = y;
		}
		else
		{
			// Loaded coordinate is out of range
			// *arr still has the adress of first member of the array
			fprintf(stderr, "In function load_clusters:\nThe #%d object's coordinates are out of range (0 <= COORDINATE <= 1000).\n", i);
			return -1;
		}

		(*arr)[i].size = 1;

		loadCount++;
	}

	if (fclose(datafile) == EOF)
	{
		// Failed to close file
		fprintf(stderr, "In function load_clusters:\nFailed to close the file %s.\n", filename);
		return -1;
	}

	return loadCount;
}

/*
Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
	printf("Clusters:\n");
	for (int i = 0; i < narr; i++)
	{
		printf("cluster %d: ", i);
		print_cluster(&carr[i]);
	}
}

// TODO
int main(int argc, char *argv[])
{
	if (argc > 1 && argc < 4)
	{
		struct cluster_t *clusters;

		int cluster1;
		int cluster2;
		int loadedClusters = load_clusters(argv[1], &clusters);
		if (loadedClusters == -1)
		{
			// Error loading clusters from file
			return 1;
		}
		int userInput = 1;

		if ((argc == 3) && (atoi(argv[2]) > userInput))
		{
			userInput = atoi(argv[2]);
		}

		while (loadedClusters > userInput)
		{
			find_neighbours(clusters, loadedClusters, &cluster1, &cluster2);

			merge_clusters(&clusters[cluster1], &clusters[cluster2]);

			loadedClusters = remove_cluster(clusters, loadedClusters, cluster2);
		}

		print_clusters(clusters, loadedClusters);

		for (int i = 0; i < loadedClusters; i++)
		{
			free(clusters[i].obj);
		}

		free(clusters);

		return 0;
	}
	else
	{
		// Uncorrect arguments
		fprintf(stderr, "In function main:\nProgram was launched with %d arguments. Please launch the program with arguments ./proj3 filename.txt numberOfDesiredClusters (optional).\n", argc);
		return 1;
	}
}