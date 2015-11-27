/* Copyright 2001 Chu Min Li
Copyright 2009-2015 Oliver Kullmann
Satz215 is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation and included in the OKlibrary;
either version 3 of the License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/satz215.2.c
  \brief Updated and corrected version
  https://en.wikipedia.org/wiki/Satz_(SAT_solver)
  Compile with

  gcc -Wall -Wno-parentheses -std=c99 -Ofast -o satz satz215.2.c

  <ul>
   <li> Safer input handling. </li>
   <li> Updated output format. </li>
   <li> Improved C usage, and update to C99. </li>
   <li> Corrected time measurement. </li>
  </ul>
*/

/****************************************************************************/
/* Satz214 + Detection of implied literals suggested by Daniel Le Berre     */
/*                                                                          */
/*                  Maintained by Chu Min LI (cli@laria.u-picardie.fr)      */
/*                  LaRIA, Universite de Picardie Jules Verne               */
/*                  March 2001                                              */
/****************************************************************************/

/* Version Satz215.1.c last modified the 25 sept. 2001.
Now only up to 10*INIT_NB_CLAUSE ternary resolvents can be
added into the formule in the preprocessing */

/* Version Satz215.2.c last modified the 18 Oct. 2001.
 my_type is defined to be int
 redundant literal suppression and tautology clause detection
 in -s option (in build_simple_sat_instance)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#include <sys/times.h>
#include <unistd.h>

typedef signed int my_type;
typedef unsigned int my_unsigned_type;

//! The maximal length of words to be read (otherwise segmentation fault!)
#ifndef WORD_LENGTH
# define WORD_LENGTH 100
#endif

#define TRUE 1
#define FALSE 0
#define NONE -1

#define WEIGTH 5
#define T 10

#define EXITCODE_PARAMETER_FAILURE 100
#define EXITCODE_INPUT_ERROR 101
#define EXITCODE_VERIFICATION_FAILED 102
#define EXITCODE_SAT 10
#define EXITCODE_UNSAT 20

#ifndef MAX_NUMBER_VARIABLES
# define MAX_NUMBER_VARIABLES 200000
#endif

#ifndef MAX_NUMBER_CLAUSES
# define MAX_NUMBER_CLAUSES 800000
#endif

#ifndef MAX_CLAUSE_LENGTH
# define MAX_CLAUSE_LENGTH 1000
#endif


/* The tables of variables and clauses are statically allocated. Set macros
   MAX_NUMBER_CLAUSES, MAX_NUMBER_CLAUSES for compilation if necessary.
*/
#define tab_variable_size MAX_NUMBER_VARIABLES
#define tab_clause_size MAX_NUMBER_CLAUSES

#define tab_unitclause_size \
 ((tab_clause_size/4<2000) ? 2000 : tab_clause_size/4)
#define my_tab_variable_size \
 ((tab_variable_size/2<1000) ? 1000 : tab_variable_size/2)
#define my_tab_clause_size \
 ((tab_clause_size/2<2000) ? 2000 : tab_clause_size/2)
#define my_tab_unitclause_size \
 ((tab_unitclause_size/2<1000) ? 1000 : tab_unitclause_size/2)
#define tab_literal_size 2*tab_variable_size
#define double_tab_clause_size 2*tab_clause_size
#define positive(literal) literal<NB_VAR
#define negative(literal) literal>=NB_VAR
#define get_var_from_lit(negative_literal) negative_literal-NB_VAR
#define RESOLVANT_LENGTH 3
#define RESOLVANT_SEARCH_THRESHOLD 5000
#define complement(lit1, lit2) \
 ((lit1<lit2) ? lit2-lit1 == NB_VAR : lit1-lit2 == NB_VAR)
#define pop(stack) stack[--stack ## _fill_pointer]
#define push(item, stack) stack[stack ## _fill_pointer++] = item
#define satisfiable() CLAUSE_STACK_fill_pointer == NB_CLAUSE

#define NEGATIVE 0
#define POSITIVE 1
#define PASSIVE 0
#define ACTIVE 1
#define MAX_NODE_NUMBER 6000

struct node {
    int clause;
    struct node *next;
};

struct ressource {
     struct node *pnode;
     struct ressource *next;
};

int CLAUSE_NODES_pointer=MAX_NODE_NUMBER;

struct node *CLAUSE_NODES;
struct ressource *ressources=NULL;

struct node *allocate_node() {
   struct ressource *pressource;
   if (CLAUSE_NODES_pointer == MAX_NODE_NUMBER) {
      CLAUSE_NODES = (struct node *)malloc(MAX_NODE_NUMBER * sizeof(struct node));
      CLAUSE_NODES_pointer = 0;
      pressource= (struct ressource *)malloc(sizeof (struct ressource));
      pressource->next = ressources;
      ressources=pressource;
      pressource->pnode = CLAUSE_NODES;
   }
   return &(CLAUSE_NODES[CLAUSE_NODES_pointer++]);
}

void free_ressources() {
   struct ressource *pressource;
   while (ressources != NULL) {
      free(ressources->pnode);
      pressource = ressources;
      ressources = ressources->next;
      free(pressource);
   }
}

struct node *node_neg_in[tab_variable_size];
struct node *node_pos_in[tab_variable_size];
struct node *in_resolv[tab_literal_size];

int *neg_in[tab_variable_size];
int *pos_in[tab_variable_size];
my_type var_current_value[tab_variable_size];
my_type var_rest_value[tab_variable_size];
my_type var_state[tab_variable_size];

int saved_clause_stack[tab_variable_size];
int saved_managedclause_stack[tab_variable_size];
my_unsigned_type nb_neg_clause_of_length2[tab_variable_size];
my_unsigned_type nb_neg_clause_of_length3[tab_variable_size];
my_unsigned_type nb_pos_clause_of_length2[tab_variable_size];
my_unsigned_type nb_pos_clause_of_length3[tab_variable_size];

int reduce_if_negative_nb[tab_variable_size];
int reduce_if_positive_nb[tab_variable_size];

int *sat[tab_clause_size];
my_type clause_state[tab_clause_size];
my_type clause_length[tab_clause_size];

int VARIABLE_STACK_fill_pointer = 0;
int CLAUSE_STACK_fill_pointer = 0;
int UNITCLAUSE_STACK_fill_pointer = 0;
int MANAGEDCLAUSE_STACK_fill_pointer = 0;

int VARIABLE_STACK[tab_variable_size];
int CLAUSE_STACK[tab_clause_size];
int UNITCLAUSE_STACK[tab_unitclause_size];
int MANAGEDCLAUSE_STACK[tab_clause_size];

int TESTED_VAR_STACK[tab_variable_size];
int TESTED_VAR_STACK_fill_pointer=0;

int NB_VAR;
int NB_CLAUSE;
int INIT_NB_CLAUSE;
my_type R = 3;

typedef unsigned long StatisticsCount;
StatisticsCount NB_UNIT=0, NB_MONO=0, NB_BRANCHE=0, NB_BACK=0;

#define double_tab_clause_size 2*tab_clause_size

struct var_node {
  int var;
  int weight;
  struct var_node *next;
};

#define VAR_NODES1_nb 6
int VAR_NODES1_index=0;
struct var_node VAR_NODES1[10*VAR_NODES1_nb];
struct var_node *VAR_FOR_TEST1=NULL;

struct var_node *allocate_var_node1() {
  return &VAR_NODES1[VAR_NODES1_index++];
}

int test_flag[tab_variable_size];

int MAX_REDUCED;
int T_SEUIL;
int H_SEUIL;

long NB_SECOND_SEARCH=0;
long NB_SECOND_FIXED = 0;

void remove_clauses(register int *clauses) {
   register int clause;
   for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
     if (clause_state[clause] == ACTIVE) {
        clause_state[clause] = PASSIVE;
        push(clause, CLAUSE_STACK);
     }
  }
}

int manage_clauses(const int* clauses) {
   for (int clause=*clauses; clause!=NONE; clause=*(++clauses)) {
      if (clause_state[clause] == ACTIVE) {
         switch (clause_length[clause]) {
         case 1: return FALSE;
         case 2: push(clause, UNITCLAUSE_STACK);
           push(clause, MANAGEDCLAUSE_STACK);
           clause_length[clause]--; break;
         default: clause_length[clause]--;
           push(clause, MANAGEDCLAUSE_STACK);
         }
      }
   }
   return TRUE;
}

void simple_manage_clauses(const int* clauses) {
   for (int clause=*clauses; clause!=NONE; clause=*(++clauses)) {
     if (clause_state[clause] == ACTIVE) {
       switch (clause_length[clause]) {
       case 2: push(clause, UNITCLAUSE_STACK);
         push(clause, MANAGEDCLAUSE_STACK);
         clause_length[clause]--; break;
       default: clause_length[clause]--;
         push(clause, MANAGEDCLAUSE_STACK);
       }
     }
   }
}

void my_simple_manage_clauses(const int* clauses) {
   for (int clause=*clauses; clause!=NONE; clause=*(++clauses)) {
     if (clause_state[clause] == ACTIVE) {
       switch (clause_length[clause]) {
       case 2: push(clause, UNITCLAUSE_STACK);
         clause_length[clause]--; break;
       default: clause_length[clause]--;
         push(clause, MANAGEDCLAUSE_STACK);
       }
     }
   }
}

int my_manage_clauses(const int* clauses) {
  for(int clause=*clauses; clause!=NONE; clause=*(++clauses)) {
    if (clause_state[clause] == ACTIVE) {
      switch (clause_length[clause]) {
      case 1: return FALSE;
      case 2: push(clause, UNITCLAUSE_STACK);
        clause_length[clause]--; break;
      default: clause_length[clause]--;
        push(clause, MANAGEDCLAUSE_STACK);
      }
    }
  }
  return TRUE;
}

void print_values(const int nb_var) {
    FILE* fp_out;
    fp_out = fopen("satx.sol", "w");
    for (int i=0; i<nb_var; i++) {
       if (var_current_value[i] == 1)
          fprintf(fp_out, "%d ", i+1);
       else
          fprintf(fp_out, "%d ", 0-i-1);
    }
    fprintf(fp_out, "\n");
    fclose(fp_out);
}

int backtracking() {
   int var, index;

   UNITCLAUSE_STACK_fill_pointer = 0;
   ++NB_BACK;

   do {
      var = pop(VARIABLE_STACK);
      if (var_rest_value[var] == NONE)
          var_state[var] = ACTIVE;
      else {
          for (index = saved_clause_stack[var];
               index < CLAUSE_STACK_fill_pointer;
               index++)
             clause_state[CLAUSE_STACK[index]] = ACTIVE;
          CLAUSE_STACK_fill_pointer = saved_clause_stack[var];

          for (index = saved_managedclause_stack[var];
               index < MANAGEDCLAUSE_STACK_fill_pointer;
               index++)
             clause_length[MANAGEDCLAUSE_STACK[index]]++;
          MANAGEDCLAUSE_STACK_fill_pointer = saved_managedclause_stack[var];

          var_current_value[var] = var_rest_value[var];
          var_rest_value[var] = NONE;
          push(var, VARIABLE_STACK);
          if (var_current_value[var]==FALSE) {
            simple_manage_clauses(pos_in[var]);
            remove_clauses(neg_in[var]);
          }
          else {
            simple_manage_clauses(neg_in[var]);
            remove_clauses(pos_in[var]);
          }
          return TRUE;
      }
   } while (VARIABLE_STACK_fill_pointer != 0);
   return FALSE;
}


/* test if the new clause is redundant or subsompted by another */
#define OLD_CLAUSE_REDUNDANT 77
#define NEW_CLAUSE_REDUNDANT 7

int smaller_than(const int lit1, const int lit2) {
    return ((lit1<NB_VAR) ? lit1 : lit1-NB_VAR) <
        ((lit2<NB_VAR) ? lit2 : lit2-NB_VAR);
}

my_type redundant(const int* new_clause, const int* old_clause) {
    int lit1, lit2, old_clause_diff=0, new_clause_diff=0;

    lit1=*old_clause; lit2=*new_clause;
    while ((lit1 != NONE) && (lit2 != NONE)) {
       if (smaller_than(lit1, lit2)) {
          lit1=*(++old_clause); old_clause_diff++;
       }
       else
       if (smaller_than(lit2, lit1)) {
          lit2=*(++new_clause); new_clause_diff++;
       }
       else
       if (complement(lit1, lit2)) {
           return FALSE; /* old_clause_diff++; new_clause_diff++; j1++; j2++; */
       }
       else {
          lit1=*(++old_clause);  lit2=*(++new_clause);
       }
    }
    if ((lit1 == NONE) && (old_clause_diff == 0))
   /* la nouvelle clause est redondante ou subsumee */
       return NEW_CLAUSE_REDUNDANT;
    if ((lit2 == NONE) && (new_clause_diff == 0))
         /* la old clause est redondante ou subsumee */
       return OLD_CLAUSE_REDUNDANT;
    return FALSE;
}

my_type get_resolvant(const int* const clause1, const int* const clause2, int* const resolvant) {
    int lit1, lit2, nb_diff1=0,  nb_diff2=0,
      nb_iden=0, nb_opps=0, j1=0, j2=0, j, limited_length;

    while (((lit1=clause1[j1])!=NONE) && ((lit2=clause2[j2]) != NONE)) {
       if (complement(lit1, lit2)) {
          j1++; j2++; nb_opps++;
       }
       else
       if (lit1 == lit2) {
          j1++; j2++; nb_iden++;
       }
       else
       if (smaller_than(lit1, lit2)) {
          nb_diff1++; j1++;
       }
       else {
          nb_diff2++; j2++;
       }
    }
    if (nb_opps ==1) {
       if (clause1[j1] ==NONE) {
          for (; clause2[j2]!= NONE; j2++) nb_diff2++;
       }
       else {
          for (; clause1[j1]!= NONE; j1++) nb_diff1++;
       }
       if ((j1==1) || (j2==1))  limited_length=RESOLVANT_LENGTH;
       else
       if ((j1==2) && (j2==2))  limited_length=1;
       else
       if (j1<j2) limited_length=((j1<RESOLVANT_LENGTH) ? j1 : RESOLVANT_LENGTH);
       else  limited_length=((j2<RESOLVANT_LENGTH) ? j2 : RESOLVANT_LENGTH);

       if (nb_diff1 + nb_diff2 + nb_iden <= limited_length) {
          j1=0; j2=0; j=0;
          while (((lit1 = clause1[j1])!=NONE) && ((lit2 = clause2[j2]) != NONE)) {
             if (lit1 == lit2) {
                resolvant[j] = lit1; j1++; j2++; j++;
             }
             else
             if (smaller_than(lit1, lit2)) {
                resolvant[j] = lit1; j1++; j++;
             }
             else
             if (smaller_than(lit2, lit1)) {
                resolvant[j] = lit2; j2++; j++;
             }
             else {
                j1++; j2++;
             }
          }
          if (clause1[j1] ==NONE) while ((resolvant[j++] = clause2[j2++]) != NONE);
          else while ((resolvant[j++] = clause1[j1++]) != NONE);
          if (j==0) return NONE;
          if (nb_diff2==0) return 2; /* clause1 is redundant */
          return TRUE;
       }
    }
    return FALSE;
}

void remove_link(const int clause) {
   int *lits;
   struct node *pnode1, *pnode2, *pnode;

   lits = sat[clause];

   for (int lit=*lits; lit != NONE; lit=*(++lits)) {
       pnode = (positive(lit) ? node_pos_in[lit] :
                                node_neg_in[get_var_from_lit(lit)]);
       if (pnode == NULL) return;
       if (pnode->clause == clause) {
          if (positive(lit)) node_pos_in[lit] = pnode->next;
          else node_neg_in[get_var_from_lit(lit)] = pnode->next;
       }
       else
       for (pnode1 = pnode, pnode2 = pnode->next;
            pnode2 != NULL; pnode2=pnode2->next) {
         if (pnode2->clause == clause) {
            pnode1->next = pnode2->next;
            break;
         }
         else
            pnode1 = pnode2;
       }
   }
}

void set_link(const int clause) {
   int *lits;
   struct node *pnode;

   lits = sat[clause];
   for (int lit=*lits; lit != NONE; lit=*(++lits)) {
       pnode = allocate_node();
       pnode->clause = clause;
       if (positive(lit)) {
          pnode->next = node_pos_in[lit];
          node_pos_in[lit] = pnode;
       }
       else {
          pnode->next = node_neg_in[get_var_from_lit(lit)];
          node_neg_in[get_var_from_lit(lit)] = pnode;
       }
   }
}

void set_link_for_resolv(const int resolv) {
   struct node* pnode;
   const int* lits=sat[resolv];
   for (int lit=*lits; lit != NONE; lit=*(++lits)) {
       pnode = allocate_node();
       pnode->clause = resolv;
           pnode->next=in_resolv[lit];
           in_resolv[lit]=pnode;
   }
}

void remove_link_for_resolv(const int resolv) {
   struct node *pnode1, *pnode2, *pnode;

   const int* lits = sat[resolv];
   for (int lit=*lits; lit != NONE; lit=*(++lits)) {
       pnode = in_resolv[lit];
       if (pnode == NULL) return;
       if (pnode->clause == resolv)
                  in_resolv[lit] = pnode->next;
       else
       for (pnode1 = pnode, pnode2 = pnode->next;
            pnode2 != NULL; pnode2=pnode2->next) {
         if (pnode2->clause == resolv) {
            pnode1->next = pnode2->next;
            break;
         }
         else
            pnode1 = pnode2;
       }
   }
}

int INVOLVED_CLAUSE_STACK[tab_clause_size];
int INVOLVED_CLAUSE_STACK_fill_pointer=0;
int CLAUSE_INVOLVED[tab_clause_size];

int already_present(int* const resolvant) {
  int clause, length=0;
  struct node *pnode;
  const int* lits=resolvant;
  for (int lit=*lits; lit != NONE; lit=*(++lits)) {
    length++;
    for (pnode=in_resolv[lit]; pnode != NULL; pnode=pnode->next) {
      clause=pnode->clause;
      CLAUSE_INVOLVED[clause]++;
      if (CLAUSE_INVOLVED[clause]==1)
        push(clause, INVOLVED_CLAUSE_STACK);
      if (clause_length[clause]==CLAUSE_INVOLVED[clause]) {
        for(int i=0; i<INVOLVED_CLAUSE_STACK_fill_pointer; ++i)
          CLAUSE_INVOLVED[INVOLVED_CLAUSE_STACK[i]]=0;
        INVOLVED_CLAUSE_STACK_fill_pointer=0;
        return NEW_CLAUSE_REDUNDANT;
      }
    }
  }
  for (int i=0; i<INVOLVED_CLAUSE_STACK_fill_pointer; ++i) {
    clause=INVOLVED_CLAUSE_STACK[i];
    if ((length==CLAUSE_INVOLVED[clause]) && (length<clause_length[clause])){
      clause_state[clause] = PASSIVE;
      remove_link(clause);
      remove_link_for_resolv(clause);
    }
    CLAUSE_INVOLVED[clause]=0;
  }
  INVOLVED_CLAUSE_STACK_fill_pointer=0;
  return FALSE;
}

int OLD_CLAUSE_SUPPRESED;
#define ACTIVE2 2

int search_redundence(int* lits) {
  struct node *pnode, *pnode1;

  /* if lits is unit, all clauses in the pnode list become redundant and
     will be deleted, so that pnode=pnode->next is not good */
  const int* const new_lits = lits; OLD_CLAUSE_SUPPRESED=FALSE;
  for (int lit=*lits; lit != NONE; lit=*(++lits)) {
    for (pnode = (positive(lit) ? node_pos_in[lit] :
                  node_neg_in[get_var_from_lit(lit)]);
         pnode != NULL; pnode = pnode1) {
      pnode1=pnode->next;
      const int* const old_lits = sat[pnode->clause];
      const int is_red = redundant(new_lits, old_lits);
      if (is_red == OLD_CLAUSE_REDUNDANT) {
        /*          printf("old clause %d is redundant\n",
                    pnode->clause);
        */
        OLD_CLAUSE_SUPPRESED=TRUE;
        clause_state[pnode->clause] = PASSIVE;
        remove_link(pnode->clause);
      }
      else
        if (is_red == NEW_CLAUSE_REDUNDANT) {
          return NEW_CLAUSE_REDUNDANT;
        }
    }
  }
  return OLD_CLAUSE_SUPPRESED;
}

int add_resolvant(int* lits) {
  int resolvant[RESOLVANT_LENGTH+1];
  struct node *pnode, *pnode1;

  /* if lits is unit, all clauses in the pnode list become redundant and
     will be deleted, so that pnode=pnode->next is not good */
  const int* const new_lits = lits;
  for (int lit=*lits; lit != NONE; lit=*(++lits))
    for (pnode = (positive(lit) ?
                  node_neg_in[lit] :
                  node_pos_in[get_var_from_lit(lit)]);
         pnode != NULL; pnode = pnode1) {
      pnode1=pnode->next;
      const int* const old_lits = sat[pnode->clause];
      const my_type is_res = get_resolvant(new_lits, old_lits, resolvant);
      if (is_res == NONE) return NONE;
      if (is_res != FALSE) {
        const int is_red=search_redundence(resolvant);
        if (is_red != NEW_CLAUSE_REDUNDANT) {
          if (already_present(resolvant) == FALSE) {
            int* const res=(int *)malloc((RESOLVANT_LENGTH+1)*sizeof(int));
            if (OLD_CLAUSE_SUPPRESED==TRUE)
              clause_state[NB_CLAUSE]=ACTIVE2;
            else
              clause_state[NB_CLAUSE]=ACTIVE;
            int j=0;
            while ((res[j]=resolvant[j]) != NONE) ++j;
            if (j==0) return NONE;
            sat[NB_CLAUSE] = res;
            set_link_for_resolv(NB_CLAUSE);
            clause_length[NB_CLAUSE++]=j;
          }
          /* new_lits is redundant by resolvant */
          if (is_res == 2) return 2;
        }
      }
    }
  return TRUE;
}

int* copy_clauses(struct node* const node_in) {
    int* in;
    struct node *pnode;
    {int j=0;
     pnode=node_in;
     while (pnode != NULL) {
       if (clause_state[pnode->clause]==ACTIVE) ++j;
       pnode = pnode->next;
     }
     in = (int *)malloc((j+1)*sizeof(int));
    }
    int j=0; pnode=node_in;
    while (pnode != NULL) {
      if (clause_state[pnode->clause]==ACTIVE) {
        in[j] = pnode->clause; ++j;
      }
      pnode = pnode->next;
    }
    in[j] = NONE;
    return in;
}

int unitclause_process();


my_type build_sat_instance(const char* const input_file) {
   FILE* const fp_in=fopen(input_file, "r");
   if (fp_in==NULL) return FALSE;

   {char ch=getc(fp_in);
    while (ch!='p') {
      while (ch!='\n') ch=getc(fp_in);
      ch=getc(fp_in);
    }
   }

   {
    char word2[WORD_LENGTH];
    fscanf(fp_in, "%s%d%d", word2, &NB_VAR, &NB_CLAUSE);
   }
   if (NB_VAR > MAX_NUMBER_VARIABLES) {
     fprintf(stderr, "ERROR: Parameter lines says maximal variable index is %d,\n"
      " but MAX_NUMBER_VARIABLES = %d.\n", NB_VAR, MAX_NUMBER_VARIABLES);
     return FALSE;
   }
   if (NB_CLAUSE > MAX_NUMBER_CLAUSES) {
     fprintf(stderr, "ERROR: Parameter lines says the clause-number is %d,\n"
      " but MAX_NUMBER_CLAUSES = %d.\n", NB_CLAUSE, MAX_NUMBER_CLAUSES);
     return FALSE;
   }

   for (int i=0; i<NB_CLAUSE; ++i) {
      int lits[MAX_CLAUSE_LENGTH+1];
      fscanf(fp_in, "%d", &lits[0]);
      int length=0;
      while (length <= MAX_CLAUSE_LENGTH && lits[length] != 0)
        fscanf(fp_in, "%d", &lits[++length]);
      if (length > MAX_CLAUSE_LENGTH) {
        fprintf(stderr, "ERROR: A clause-line contains more than MAX_CLAUSE_LENGTH = %d literals.\n", MAX_CLAUSE_LENGTH);
        return FALSE;
      }

      char tautologie = FALSE;
      /* test if some literals are redundant and sort the clause */
      for (int ii=0; ii<length-1; ++ii) {
         int lit = lits[ii];
         for (int jj=ii+1; jj<length; ++jj) {
            if (abs(lit)>abs(lits[jj])) {
               const int lit1=lits[jj]; lits[jj]=lit; lit=lit1;
            }
            else
            if (lit == lits[jj]) {
               lits[jj--] = lits[--length]; lits[length] = 0;
               printf("literal %d is redundant in clause %d\n", lit, i);
            }
            else
            if (abs(lit) == lits[jj]) {
               tautologie = TRUE; break;
            }
         }
         if (tautologie == TRUE) break;
         else lits[ii] = lit;
      }
      if (tautologie == FALSE) {
        lits[length] = 0;
        sat[i] = (int *)malloc((length+1)*sizeof(int));
        int j=0;
        while (lits[j] != 0) {
           if (lits[j] > 0) sat[i][j] = lits[j]-1;
           else sat[i][j] = abs(lits[j]) + NB_VAR -1;
           ++j;
        }
        sat[i][j] = NONE;
        clause_length[i] = length;
        clause_state[i] = ACTIVE;
 /*       static_clause_length[i] = length;
*/
      }
      else {i--; NB_CLAUSE--;}
   }
   fclose(fp_in);

   for (int i=0; i<NB_VAR; ++i) {
      node_neg_in[i] = NULL;
      node_pos_in[i] = NULL;
      var_state[i]=ACTIVE;
   }

   INIT_NB_CLAUSE= NB_CLAUSE;

   for(int i=0; i<NB_CLAUSE; ++i) set_link_for_resolv(i);

   for(int i=0; i<tab_clause_size; ++i)
           CLAUSE_INVOLVED[INVOLVED_CLAUSE_STACK[i]]=0;
   INVOLVED_CLAUSE_STACK_fill_pointer=0;

   for(int i=0; i<NB_CLAUSE; ++i) {
      remove_link_for_resolv(i);
      int* plit = sat[i];
      const int length = clause_length[i];
      if (length==1) push(i, UNITCLAUSE_STACK);
      if (search_redundence(plit) != NEW_CLAUSE_REDUNDANT) {
        if ((i<INIT_NB_CLAUSE*10) ||
            (length<3) ||
            (clause_state[i]==ACTIVE2)) {
          const char is_res = add_resolvant(plit);
          if (is_res  == NONE) return NONE;
          else
            if (is_res == 2) clause_state[i] = PASSIVE;
            else
              {set_link(i); clause_state[i] = ACTIVE;}
        }
        else clause_state[i] = PASSIVE;
      }
      else clause_state[i] = PASSIVE;
   }
   for(int i=0; i<NB_VAR; ++i) {
      neg_in[i]=copy_clauses(node_neg_in[i]);
      pos_in[i]=copy_clauses(node_pos_in[i]);
   }
   free_ressources();
   if (unitclause_process()==NONE) return NONE;
   int NB_CLAUSE1 = 0;
   for (int i=0; i<NB_CLAUSE; ++i)
     if (clause_state[i] == ACTIVE) ++NB_CLAUSE1;
   NB_CLAUSE = NB_CLAUSE1;

   return TRUE;
}


int verify_solution() {
   int i, lit, *lits, clause_truth;

   for (i=0; i<NB_CLAUSE; i++) {
      clause_truth = FALSE;
      lits = sat[i];
      for(lit=*lits; lit!=NONE; lit=*(++lits))
         if (((negative(lit)) &&
              (var_current_value[get_var_from_lit(lit)] == FALSE)) ||
             ((positive(lit)) &&
              (var_current_value[lit] == TRUE)) ) {
            clause_truth = TRUE;
            break;
         }
      if (clause_truth == FALSE) return FALSE;
   }

   return TRUE;
}

long NB_SEARCH = 0; long NB_FIXED = 0;

int unitclause_process() {
  int var, unitclause, lit, *lits, unitclause_position;

  for (unitclause_position = 0;
       unitclause_position < UNITCLAUSE_STACK_fill_pointer;
       unitclause_position++) {
     unitclause = UNITCLAUSE_STACK[unitclause_position];
     if (clause_state[unitclause] == ACTIVE) {
       ++NB_UNIT;
       lits = sat[unitclause];
       for(lit=*lits; lit!=NONE; lit=*(++lits)) {
          if (positive(lit)) {
             var = lit;
             if (var_state[var] == ACTIVE) {
                var_current_value[var] = TRUE;
                var_rest_value[var] = NONE;
                if (manage_clauses(neg_in[var])==TRUE) {
                   var_state[var] = PASSIVE;
                   push(var, VARIABLE_STACK);
                   remove_clauses(pos_in[var]);
                   break;
                }
                else {
                   return NONE;
                }
             }
          }
          else {
             var = get_var_from_lit(lit);
             if (var_state[var] == ACTIVE) {
                var_current_value[var] = FALSE;
                var_rest_value[var] = NONE;
                if (manage_clauses(pos_in[var])== TRUE) {
                   var_state[var] = PASSIVE;
                   push(var, VARIABLE_STACK);
                   remove_clauses(neg_in[var]);
                    break;
                }
                else {
                   return NONE;
                }
             }
          }
       }
     }
   }
   UNITCLAUSE_STACK_fill_pointer = 0;
   return TRUE;
}

int get_nb_clauses(int var) {
   return ((nb_neg_clause_of_length2[var] +
            nb_pos_clause_of_length2[var]) * WEIGTH) +
           nb_neg_clause_of_length3[var] +
           nb_pos_clause_of_length3[var];
}

int get_resolvant_nb(int saved_managedclause_fill_pointer) {
  int *lits;
  int lit, var, i, clause, resolvant_nb=0;

  for (i=saved_managedclause_fill_pointer;
       i<MANAGEDCLAUSE_STACK_fill_pointer; i++) {
    clause = MANAGEDCLAUSE_STACK[i];
    if (clause_length[clause] == 2) {
      lits = sat[clause];
      for(lit=*lits; lit!=NONE; lit=*(++lits)) {
        if (positive(lit)) {
          var = lit;
          if (var_state[var] == ACTIVE)
            resolvant_nb += (nb_neg_clause_of_length2[var] * WEIGTH)
              +nb_neg_clause_of_length3[var];
        }
        else {
          var = get_var_from_lit(lit);
          if (var_state[var] == ACTIVE)
            resolvant_nb += (nb_pos_clause_of_length2[var] * WEIGTH)
              +nb_pos_clause_of_length3[var];
        }
      }
    }
  }
  return resolvant_nb;
}

void reset_context(int saved_var_stack_fill_pointer,
                   int saved_managedclause_fill_pointer) {
   int i;

   for (i=0; i<UNITCLAUSE_STACK_fill_pointer; i++)
      clause_length[UNITCLAUSE_STACK[i]]++;
   UNITCLAUSE_STACK_fill_pointer = 0;

   for (i=saved_var_stack_fill_pointer;
        i<VARIABLE_STACK_fill_pointer; i++)
       var_state[VARIABLE_STACK[i]] = ACTIVE;
   VARIABLE_STACK_fill_pointer = saved_var_stack_fill_pointer;

   for (i=saved_managedclause_fill_pointer;
        i<MANAGEDCLAUSE_STACK_fill_pointer; i++)
         clause_length[MANAGEDCLAUSE_STACK[i]]++;
   MANAGEDCLAUSE_STACK_fill_pointer =
     saved_managedclause_fill_pointer;
}

int branch();

int examine1(int tested_var) {
  int generating_fixed_variables_if_positif,
     generating_fixed_variables_if_negatif,
     saved_var_stack_fill_pointer,
     saved_managedclause_fill_pointer;

   saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
   saved_managedclause_fill_pointer=
     MANAGEDCLAUSE_STACK_fill_pointer;

   var_current_value[tested_var] = TRUE;
   var_state[tested_var] = PASSIVE;
   push(tested_var, VARIABLE_STACK);
   my_simple_manage_clauses(neg_in[tested_var]);

   generating_fixed_variables_if_positif = branch();

   if (generating_fixed_variables_if_positif == NONE) {
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
     var_current_value[tested_var] = FALSE;
     var_rest_value[tested_var] = NONE;
     var_state[tested_var] = PASSIVE;
     push(tested_var, VARIABLE_STACK);
     simple_manage_clauses(pos_in[tested_var]);
     remove_clauses(neg_in[tested_var]);
     return NONE;
   }
   else {
     reduce_if_positive_nb[tested_var] =
       get_resolvant_nb(saved_managedclause_fill_pointer);
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
   }
   var_current_value[tested_var] = FALSE;

   var_state[tested_var] = PASSIVE;
   push(tested_var, VARIABLE_STACK);
   my_simple_manage_clauses(pos_in[tested_var]);

   generating_fixed_variables_if_negatif = branch();

   if (generating_fixed_variables_if_negatif == NONE) {
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
     simple_manage_clauses(neg_in[tested_var]);
     var_current_value[tested_var] = TRUE;
     var_rest_value[tested_var] = NONE;
     var_state[tested_var] = PASSIVE;
     push(tested_var, VARIABLE_STACK);
     remove_clauses(pos_in[tested_var]);
         return NONE;
   }
   else {
     reduce_if_negative_nb[tested_var] =
       get_resolvant_nb(saved_managedclause_fill_pointer);
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
   }
   push(tested_var, TESTED_VAR_STACK);
   return TRUE;
}

int examine(int tested_var) {
   int generating_fixed_variables_if_positif = 0,
     generating_fixed_variables_if_negatif = 0,
     saved_var_stack_fill_pointer,
     saved_managedclause_fill_pointer;

     saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
     saved_managedclause_fill_pointer=
       MANAGEDCLAUSE_STACK_fill_pointer;

      var_current_value[tested_var] = TRUE;

      var_state[tested_var] = PASSIVE;
      push(tested_var, VARIABLE_STACK);
      my_simple_manage_clauses(neg_in[tested_var]);

      generating_fixed_variables_if_positif = branch();
      reduce_if_positive_nb[tested_var]=
        MANAGEDCLAUSE_STACK_fill_pointer-
        saved_managedclause_fill_pointer;
      reset_context(saved_var_stack_fill_pointer,
                    saved_managedclause_fill_pointer);

      if (generating_fixed_variables_if_positif == NONE) {
         var_current_value[tested_var] = FALSE;
         var_rest_value[tested_var] = NONE;
         var_state[tested_var] = PASSIVE;
         push(tested_var, VARIABLE_STACK);
         simple_manage_clauses(pos_in[tested_var]);
         remove_clauses(neg_in[tested_var]);
         return NONE;
      }

      var_current_value[tested_var] = FALSE;

      var_state[tested_var] = PASSIVE;
      push(tested_var, VARIABLE_STACK);
      my_simple_manage_clauses(pos_in[tested_var]);

      generating_fixed_variables_if_negatif = branch();
      reduce_if_negative_nb[tested_var]=
        MANAGEDCLAUSE_STACK_fill_pointer-
        saved_managedclause_fill_pointer;
      reset_context(saved_var_stack_fill_pointer,
                    saved_managedclause_fill_pointer);

      if (generating_fixed_variables_if_negatif == NONE) {
         simple_manage_clauses(neg_in[tested_var]);
         var_current_value[tested_var] = TRUE;
         var_rest_value[tested_var] = NONE;
         var_state[tested_var] = PASSIVE;
         push(tested_var, VARIABLE_STACK);
         remove_clauses(pos_in[tested_var]);
         return NONE;
       }
   push(tested_var, TESTED_VAR_STACK);
   return TRUE;
}

int get_nb(const int saved_managedclause_fill_pointer) {
  int nb=0;

  for (int i=saved_managedclause_fill_pointer;
      i<MANAGEDCLAUSE_STACK_fill_pointer; ++i) {
    const int clause=MANAGEDCLAUSE_STACK[i];
    if (clause_length[clause] == 2) {
      const int* lits = sat[clause];
      for (int lit=*lits; lit!=NONE; lit=*(++lits)) {
        if (negative(lit)) {
          const int var=get_var_from_lit(lit);
          if (var_state[var] == ACTIVE) nb+=nb_pos_clause_of_length2[var];
        }
        else {
          const int var=lit;
          if (var_state[var] == ACTIVE) nb+=nb_neg_clause_of_length2[var];
        }
      }
    }
  }
  return nb;
}

int examine2(const int tested_var) {;
   const int saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
   const int saved_managedclause_fill_pointer=
     MANAGEDCLAUSE_STACK_fill_pointer;

   var_current_value[tested_var] = TRUE;

   var_state[tested_var] = PASSIVE;
   push(tested_var, VARIABLE_STACK);
   my_simple_manage_clauses(neg_in[tested_var]);
   const int generating_fixed_variables_if_positif = branch();

   if (generating_fixed_variables_if_positif == NONE) {
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
     var_current_value[tested_var] = FALSE;
     var_rest_value[tested_var] = NONE;
     var_state[tested_var] = PASSIVE;
     push(tested_var, VARIABLE_STACK);
     simple_manage_clauses(pos_in[tested_var]);
     remove_clauses(neg_in[tested_var]);
     return NONE;
   }
   else {
     reduce_if_positive_nb[tested_var]=
       get_nb(saved_managedclause_fill_pointer);
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
   }


   var_current_value[tested_var] = FALSE;

   var_state[tested_var] = PASSIVE;
   push(tested_var, VARIABLE_STACK);
   my_simple_manage_clauses(pos_in[tested_var]);
   const int generating_fixed_variables_if_negatif = branch();

   if (generating_fixed_variables_if_negatif == NONE) {
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
     simple_manage_clauses(neg_in[tested_var]);
     var_current_value[tested_var] = TRUE;
     var_rest_value[tested_var] = NONE;
     var_state[tested_var] = PASSIVE;
     push(tested_var, VARIABLE_STACK);
     remove_clauses(pos_in[tested_var]);
     return NONE;
   }
   else {
     reduce_if_negative_nb[tested_var]=
       get_nb(saved_managedclause_fill_pointer);
     reset_context(saved_var_stack_fill_pointer,
                   saved_managedclause_fill_pointer);
   }

   push(tested_var, TESTED_VAR_STACK);
   return TRUE;
}

int IMPLIED_LITS_fill_pointer=0;
int IMPLIED_LITS[tab_variable_size];
int LIT_IMPLIED[tab_variable_size];

int branch() {
   ++NB_SEARCH;
   for (int unitclause_position = 0;
        unitclause_position != UNITCLAUSE_STACK_fill_pointer;
        ++unitclause_position) {
       const int unitclause = UNITCLAUSE_STACK[unitclause_position];
       if (clause_state[unitclause] == ACTIVE) {
          const int* lits = sat[unitclause];
          for(int lit=*lits; lit!=NONE; lit=*(++lits)) {
              if (positive(lit)) {
                 const int var = lit;
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = TRUE;
                    if (my_manage_clauses(neg_in[var]) == TRUE) {
                      if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      ++LIT_IMPLIED[lit];
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                       ++NB_FIXED;
                       return NONE;
                    }
                 }
              }
              else {
                 const int var = get_var_from_lit(lit);
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = FALSE;
                    if (my_manage_clauses(pos_in[var]) == TRUE) {
                      if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      ++LIT_IMPLIED[lit];
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                      ++NB_FIXED;
                      return NONE;
                    }
                 }
              }
          }
       }
   }
   return MANAGEDCLAUSE_STACK_fill_pointer;
}

int satisfy_literal(const int lit) {
  if (positive(lit)) {
    if (var_state[lit]==ACTIVE) {
      if (manage_clauses(neg_in[lit])==FALSE) return NONE;
      var_current_value[lit] = TRUE;
      var_rest_value[lit]=NONE;
      var_state[lit] = PASSIVE;
      push(lit, VARIABLE_STACK);
      remove_clauses(pos_in[lit]);
    }
    else
      if (var_current_value[lit]==FALSE) return NONE;
  }
  else {
    const int var = get_var_from_lit(lit);
    if (var_state[var]==ACTIVE) {
      if (manage_clauses(pos_in[var])==FALSE) return NONE;
      var_current_value[var] = FALSE;
      var_rest_value[var]=NONE;
      var_state[var] = PASSIVE;
      push(var, VARIABLE_STACK);
      remove_clauses(neg_in[var]);
    }
    else
      if (var_current_value[var]==TRUE) return NONE;
  }
  if (unitclause_process()==NONE)
    return NONE;
  else return FALSE;
}

int treat_implied_lits() {
  for (int i=0; i<IMPLIED_LITS_fill_pointer; ++i) {
    const int lit=IMPLIED_LITS[i];
    if (LIT_IMPLIED[lit]==2) if (satisfy_literal(lit)==NONE) return NONE;
  }
  return TRUE;
}

int get_complement(const int lit) {
   if (positive(lit)) return lit+NB_VAR;
   else return lit-NB_VAR;
}

int insert_var_if_necessary1(const int var) {
  const int weight=reduce_if_positive_nb[var]*reduce_if_negative_nb[var]*1024
         + reduce_if_positive_nb[var]
         + reduce_if_negative_nb[var]+1;

  if ((VAR_FOR_TEST1==NULL) ||
      ((VAR_FOR_TEST1 != NULL) && (weight>VAR_FOR_TEST1->weight))) {
     struct var_node* const pvar_node=allocate_var_node1();
     pvar_node->var=var; pvar_node->weight=weight;
     pvar_node->next=VAR_FOR_TEST1; VAR_FOR_TEST1=pvar_node;
  }
  else {
    int nb=1;
    struct var_node* pvar_node1=VAR_FOR_TEST1;
    struct var_node* pvar_node2=pvar_node1->next;
    while (nb<VAR_NODES1_nb) {
      if ((pvar_node2==NULL) ||
          ((pvar_node2!=NULL) && (weight>pvar_node2->weight))) {
        struct var_node* const pvar_node=allocate_var_node1();
        pvar_node->var=var; pvar_node->weight=weight;
        pvar_node->next=pvar_node1->next; pvar_node1->next=pvar_node;
        break;
      }
      ++nb; pvar_node1=pvar_node2; pvar_node2=pvar_node1->next;
    }
  }
  return TRUE;
}

int branch1() {
   ++NB_SEARCH;
   for (int unitclause_position = 0;
        unitclause_position != UNITCLAUSE_STACK_fill_pointer;
        ++unitclause_position) {
       const int unitclause = UNITCLAUSE_STACK[unitclause_position];
       if (clause_state[unitclause] == ACTIVE) {
          const int* lits = sat[unitclause];
          for (int lit=*lits; lit!=NONE; lit=*(++lits)) {
              if (positive(lit)) {
                 const int var = lit;
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = TRUE;
                    if (my_manage_clauses(neg_in[var]) == TRUE) {
                      //if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      //LIT_IMPLIED[lit]+=5;
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                       ++NB_FIXED;
                       return NONE;
                    }
                 }
              }
              else {
                 const int var = get_var_from_lit(lit);
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = FALSE;
                    if (my_manage_clauses(pos_in[var]) == TRUE) {
                      //if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      //LIT_IMPLIED[lit]+=5;
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                      ++NB_FIXED;
                      return NONE;
                    }
                 }
              }
          }
       }
   }
   return MANAGEDCLAUSE_STACK_fill_pointer;
}


int further_examin_var_if_positive(const int var) {
  const int saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
  const int saved_managedclause_fill_pointer=
     MANAGEDCLAUSE_STACK_fill_pointer;

  var_current_value[var] = TRUE;
  var_state[var] = PASSIVE;
  push(var, VARIABLE_STACK);
  my_simple_manage_clauses(neg_in[var]);
  ++NB_SECOND_SEARCH;
  const int nb_reduced_clauses_if_further_positif = branch1();
  reset_context(saved_var_stack_fill_pointer,
                saved_managedclause_fill_pointer);

  if (nb_reduced_clauses_if_further_positif == NONE) {
    ++NB_SECOND_FIXED;
    var_current_value[var] = FALSE;
    var_state[var] = PASSIVE;
    push(var, VARIABLE_STACK);
    my_simple_manage_clauses(pos_in[var]);
    if (branch() == NONE) return NONE;
    const int lit=get_complement(var);
    if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
    ++LIT_IMPLIED[lit];
  }
  else  --NB_SEARCH;
  return TRUE;
}

int further_examin_var_if_negative(const int var) {
  const int saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
  const int saved_managedclause_fill_pointer= MANAGEDCLAUSE_STACK_fill_pointer;

  var_current_value[var] = FALSE;
  var_state[var] = PASSIVE;
  push(var, VARIABLE_STACK);
  my_simple_manage_clauses(pos_in[var]);
  ++NB_SECOND_SEARCH;
  const int nb_reduced_clauses_if_further_negatif = branch1();
  reset_context(saved_var_stack_fill_pointer,
                saved_managedclause_fill_pointer);

  if (nb_reduced_clauses_if_further_negatif == NONE) {
    ++NB_SECOND_FIXED;
    var_current_value[var] = TRUE;
    var_state[var] = PASSIVE;
    push(var, VARIABLE_STACK);
    my_simple_manage_clauses(neg_in[var]);
    if (branch() == NONE) return NONE;
    if (LIT_IMPLIED[var]==0) push(var, IMPLIED_LITS);
    ++LIT_IMPLIED[var];
  }
  else  --NB_SEARCH;
  return TRUE;
}

int further_examin(const int saved_managedclause_fill_pointer) {
   for (int i=saved_managedclause_fill_pointer;
       i<MANAGEDCLAUSE_STACK_fill_pointer; ++i) {
     const int clause=MANAGEDCLAUSE_STACK[i];
     if (clause_length[clause] == 2) {
       const int* lits = sat[clause];
       for (int lit=*lits; lit!=NONE; lit=*(++lits)) {
         if (positive(lit)) {
           const int var = lit;
           if ((var_state[var] == ACTIVE) &&
               (test_flag[var] < NB_SEARCH)) {
             test_flag[var] = NB_SEARCH;
             if (nb_neg_clause_of_length2[var]>0) {
               if (further_examin_var_if_positive(var)==NONE) {
                 MAX_REDUCED=-1;
                 return NONE;
               }
             }
           }
         }
         else {
           const int var = get_var_from_lit(lit);
           if ((var_state[var] == ACTIVE) &&
               (test_flag[var] < NB_SEARCH))  {
             test_flag[var] = NB_SEARCH;
             if (nb_pos_clause_of_length2[var]>0) {
               if (further_examin_var_if_negative(var)==NONE) {
                 MAX_REDUCED=-1;
                 return NONE;
               }
             }
           }
         }
       }
     }
   }
   return TRUE;
}

int further_testable(const int saved_managedclause_fill_pointer) {
  if ((MANAGEDCLAUSE_STACK_fill_pointer - saved_managedclause_fill_pointer > T_SEUIL) &&
      (MANAGEDCLAUSE_STACK_fill_pointer - saved_managedclause_fill_pointer > MAX_REDUCED)) {
    MAX_REDUCED = MANAGEDCLAUSE_STACK_fill_pointer - saved_managedclause_fill_pointer;
    return TRUE;
  }
  return FALSE;
}

int examine3(const int tested_var) {
  int generating_if_positif, generating_if_negatif,
    saved_var_stack_fill_pointer,
    saved_managedclause_fill_pointer;

  saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
  saved_managedclause_fill_pointer=
    MANAGEDCLAUSE_STACK_fill_pointer;

  var_current_value[tested_var] = TRUE;

  var_state[tested_var] = PASSIVE;
  push(tested_var, VARIABLE_STACK);
  my_simple_manage_clauses(neg_in[tested_var]);
  generating_if_positif = branch();
  reduce_if_positive_nb[tested_var]=
    MANAGEDCLAUSE_STACK_fill_pointer-saved_managedclause_fill_pointer;

  if ((generating_if_positif == NONE) ||
      ((further_testable(saved_managedclause_fill_pointer)==TRUE)
       && (further_examin(saved_managedclause_fill_pointer)==NONE))) {
    reset_context(saved_var_stack_fill_pointer,
                  saved_managedclause_fill_pointer);
    var_current_value[tested_var] = FALSE;
    var_rest_value[tested_var] = NONE;
    var_state[tested_var] = PASSIVE;
    push(tested_var, VARIABLE_STACK);
    simple_manage_clauses(pos_in[tested_var]);
    remove_clauses(neg_in[tested_var]);
    return NONE;
  }
  else {
    reset_context(saved_var_stack_fill_pointer,
                  saved_managedclause_fill_pointer);
  }

  var_current_value[tested_var] = FALSE;

  var_state[tested_var] = PASSIVE;
  push(tested_var, VARIABLE_STACK);
  my_simple_manage_clauses(pos_in[tested_var]);

  generating_if_negatif = branch();
  reduce_if_negative_nb[tested_var]=
    MANAGEDCLAUSE_STACK_fill_pointer-saved_managedclause_fill_pointer;

  if ((generating_if_negatif == NONE) ||
      ((further_testable(saved_managedclause_fill_pointer)==TRUE)
       && (further_examin(saved_managedclause_fill_pointer)==NONE)))  {
    reset_context(saved_var_stack_fill_pointer,
                  saved_managedclause_fill_pointer);
    simple_manage_clauses(neg_in[tested_var]);
    var_current_value[tested_var] = TRUE;
    var_rest_value[tested_var] = NONE;
    var_state[tested_var] = PASSIVE;
    push(tested_var, VARIABLE_STACK);
    remove_clauses(pos_in[tested_var]);
    return NONE;
  }
  else {
    reset_context(saved_var_stack_fill_pointer,
                  saved_managedclause_fill_pointer);
  }
  push(tested_var, TESTED_VAR_STACK);
  return TRUE;
}

int get_neg_clause_nb(const int var) {
    my_type neg_clause3_nb = 0, neg_clause2_nb = 0;
    const int* clauses = neg_in[var];
    for (int clause=*clauses; clause!=NONE; clause=*(++clauses))
      if (clause_state[clause] == ACTIVE)
        if (clause_length[clause] == 2) ++neg_clause2_nb;
        else ++neg_clause3_nb;
    nb_neg_clause_of_length2[var] = neg_clause2_nb;
    nb_neg_clause_of_length3[var] = neg_clause3_nb;
    return neg_clause2_nb + neg_clause3_nb;
}

int get_pos_clause_nb(const int var) {
    my_type pos_clause3_nb = 0, pos_clause2_nb = 0;
    const int* clauses = pos_in[var];
    for (int clause=*clauses; clause!=NONE; clause=*(++clauses))
      if (clause_state[clause] == ACTIVE)
        if (clause_length[clause] == 2) ++pos_clause2_nb;
        else ++pos_clause3_nb;
    nb_pos_clause_of_length2[var] = pos_clause2_nb;
    nb_pos_clause_of_length3[var] = pos_clause3_nb;
    return pos_clause2_nb + pos_clause3_nb;
}

int choose_and_instantiate_variable_in_clause() {
    int chosen_var=NONE;
    unsigned long max_nb_clauses = 0;

    ++NB_BRANCHE;
    TESTED_VAR_STACK_fill_pointer=0;
    VAR_FOR_TEST1=NULL; VAR_NODES1_index=0; MAX_REDUCED=-1;

    for (int var = 0; var < NB_VAR; ++var) {
       if (var_state[var] == ACTIVE) {
           reduce_if_negative_nb[var]=0;
           reduce_if_positive_nb[var]=0;

           if (get_neg_clause_nb(var) == 0) {
               ++NB_MONO;
               var_current_value[var] = TRUE;
               var_rest_value[var] = NONE;
               var_state[var] = PASSIVE;
               push(var, VARIABLE_STACK);
               remove_clauses(pos_in[var]);
           }
           else
           if (get_pos_clause_nb(var) == 0) {
               ++NB_MONO;
               var_current_value[var] = FALSE;
               var_rest_value[var] = NONE;
               var_state[var] = PASSIVE;
               push(var, VARIABLE_STACK);
               remove_clauses(neg_in[var]);
           }
           else {
              const my_type pos2 = nb_pos_clause_of_length2[var];
              const my_type neg2 = nb_neg_clause_of_length2[var];
              if ((neg2>0) && (pos2>0) && ((neg2+pos2)>3) ) {
                for(int i=0; i<IMPLIED_LITS_fill_pointer; ++i)
                  LIT_IMPLIED[IMPLIED_LITS[i]]=0;
                IMPLIED_LITS_fill_pointer=0;
                if (examine3(var) == NONE) {
                   if (unitclause_process() == NONE) return NONE;
                }
                else
                  if (treat_implied_lits()==NONE) return NONE;
              }
           }
       }
    }
    if (TESTED_VAR_STACK_fill_pointer < T) {
      TESTED_VAR_STACK_fill_pointer=0;
      for (int var=0; var<NB_VAR; ++var)
        if (var_state[var] == ACTIVE) {
          const my_type pos2 = nb_pos_clause_of_length2[var];
          const my_type neg2 = nb_neg_clause_of_length2[var];
          if ((neg2>0) && (pos2 > 0) &&
              ((neg2 > 1) || (pos2 > 1) ) ) {
            for(int i=0; i<IMPLIED_LITS_fill_pointer; ++i)
              LIT_IMPLIED[IMPLIED_LITS[i]]=0;
            IMPLIED_LITS_fill_pointer=0;
            if (examine2(var) == NONE) {
              if (unitclause_process() == NONE) return NONE;
            }
            else
              if (treat_implied_lits()==NONE) return NONE;
          }
        }

      if (TESTED_VAR_STACK_fill_pointer < T) {
        TESTED_VAR_STACK_fill_pointer = 0;
        for (int var=0; var<NB_VAR; ++var)
          if (var_state[var] == ACTIVE) {
            for(int i=0; i<IMPLIED_LITS_fill_pointer; ++i)
              LIT_IMPLIED[IMPLIED_LITS[i]]=0;
            IMPLIED_LITS_fill_pointer=0;
            if (examine1(var) == NONE) {
              if (unitclause_process() == NONE) return NONE;
            }
            else
              if (treat_implied_lits()==NONE) return NONE;
          }
      }
    }
    for (int i=0; i<TESTED_VAR_STACK_fill_pointer; ++i) {
      const int var=TESTED_VAR_STACK[i];
      if (var_state[var] == ACTIVE) {
        const int posi=reduce_if_positive_nb[var];
        const int nega=reduce_if_negative_nb[var];
        const unsigned long nb_clauses = posi*nega*128 + posi + nega +1;
        if (nb_clauses > max_nb_clauses) {
          chosen_var = var;
          max_nb_clauses = nb_clauses;
        }
      }
    }

    if (chosen_var == NONE) return FALSE;
    var_state[chosen_var] = PASSIVE;
    saved_clause_stack[chosen_var] = CLAUSE_STACK_fill_pointer;
    saved_managedclause_stack[chosen_var] = MANAGEDCLAUSE_STACK_fill_pointer;
    push(chosen_var, VARIABLE_STACK);
    var_current_value[chosen_var] = TRUE;
    var_rest_value[chosen_var] = FALSE;
    simple_manage_clauses(neg_in[chosen_var]);
    remove_clauses(pos_in[chosen_var]);
    return TRUE;
}


void reset_all() {
   UNITCLAUSE_STACK_fill_pointer = 0; NB_BACK=0; NB_BRANCHE=0;
   NB_MONO=0; NB_UNIT=0;  NB_SEARCH=0; NB_FIXED=0;
   NB_SECOND_SEARCH=0; NB_SECOND_FIXED=0;
   for (int index=0; index<VARIABLE_STACK_fill_pointer; ++index)
     var_state[VARIABLE_STACK[index]] = ACTIVE;
   VARIABLE_STACK_fill_pointer = 0;

   for (int index = 0; index < CLAUSE_STACK_fill_pointer; ++index)
     clause_state[CLAUSE_STACK[index]] = ACTIVE;
   CLAUSE_STACK_fill_pointer = 0;

   for (int index = 0; index < MANAGEDCLAUSE_STACK_fill_pointer; ++index)
     clause_length[MANAGEDCLAUSE_STACK[index]]++;
   MANAGEDCLAUSE_STACK_fill_pointer = 0;
}

void dpl() {
  reset_all();
  for (int var=0; var<NB_VAR; ++var) test_flag[var]=0;
  do {
    if (UNITCLAUSE_STACK_fill_pointer==0)
      if (choose_and_instantiate_variable_in_clause()==NONE) backtracking();
    if (unitclause_process()==NONE) backtracking();
  } while ((VARIABLE_STACK_fill_pointer != 0) && (!(satisfiable())));
}


int main(const int argc, const char* const argv[]) {
   if (argc!=2) {
      printf("Usage format: \"satz215 input_instance\"\n");
      return EXITCODE_PARAMETER_FAILURE;
   }
   char saved_input_file[WORD_LENGTH];
   for (int i=0; i<WORD_LENGTH; ++i) saved_input_file[i]=argv[1][i];

   struct tms* a_tms = malloc(sizeof(struct tms));
   times(a_tms);
   const clock_t begintime = a_tms->tms_utime;

   int exit_value;
   switch (build_sat_instance(argv[1])) {
      case FALSE: printf("Input file error.\n"); return EXITCODE_INPUT_ERROR;
      case TRUE:
        VARIABLE_STACK_fill_pointer=0;
        CLAUSE_STACK_fill_pointer = 0;
        MANAGEDCLAUSE_STACK_fill_pointer = 0;
        T_SEUIL= NB_VAR/6;
        H_SEUIL=3*T/2;
        dpl();
        break;
      case NONE:
        exit_value = EXITCODE_UNSAT;
        break;
   }
   times(a_tms);
   const clock_t endtime = a_tms->tms_utime;

   const bool sat_decision = satisfiable();
   printf("s ");
   if (sat_decision) {
     exit_value = EXITCODE_SAT;
     printf("SATISFIABLE");
     if (verify_solution()) print_values(NB_VAR);
     else {
       exit_value = EXITCODE_VERIFICATION_FAILED;
       printf ("**** Solution verification failed! ****\n");
     }
   }
  else {
    exit_value = EXITCODE_UNSAT;
    printf ("UNSATISFIABLE");
  }
  printf("\n");
  const int diff_c = NB_CLAUSE-INIT_NB_CLAUSE;
  const long EPS = sysconf(_SC_CLK_TCK);
  const double elapsed = (double)(endtime-begintime)/EPS;
  printf("c sat_status                            %d\n"
         "c number_of_variables                   %u\n"
         "c initial_number_of_clauses             %u\n"
         "c reddiff_number_of_clauses             %d\n"
         "c running_time(sec)                     %1.2f\n"
         "c number_of_nodes                       %lu\n"
         "c number_of_binary_nodes                %lu\n"
         "c number_of_pure_literals               %lu\n"
         "c number_of_1-reductions                %lu\n"
         "c number_of_2-look-ahead                %lu\n"
         "c number_of_2-reductions                %lu\n"
         "c number_of_3-look-ahead                %lu\n"
         "c number_of_3-reductions                %lu\n"
         "c file_name                             %s\n",
       sat_decision, NB_VAR, INIT_NB_CLAUSE, diff_c, elapsed, NB_BRANCHE,
       NB_BACK, NB_MONO, NB_UNIT, NB_SEARCH, NB_FIXED, NB_SECOND_SEARCH,
       NB_SECOND_FIXED, saved_input_file);

  {FILE* fp_time;
   if (! (fp_time = fopen("satz215_timetable", "r"))) {
     fp_time = fopen("satz215_timetable", "w");
     fprintf(fp_time, " rn rc t sat nds r1 r2 pls file bnds r2la r3 r3la dc\n");
   }
   fclose(fp_time);
  }
  FILE* const fp_time = fopen("satz215_timetable", "a");
  fprintf(fp_time, "%d %d %1.2f %d %lu %lu %lu %lu \"%s\" %lu %lu %lu %lu %d\n",
    NB_VAR, INIT_NB_CLAUSE, elapsed, sat_decision, NB_BRANCHE, NB_UNIT,
    NB_FIXED, NB_MONO, saved_input_file, NB_BACK, NB_SEARCH, NB_SECOND_FIXED,
    NB_SECOND_SEARCH, diff_c);
  fclose(fp_time);
  return exit_value;
}
