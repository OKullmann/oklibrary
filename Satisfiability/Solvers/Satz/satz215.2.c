/* Copyright 2001 Chu Min Li
Satz215 is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation and included in the OKlibrary;
either version 3 of the License, or any later version. */

/*!
  \file Satisfiability/Solvers/Satz/satz215.2.c
  \brief Updated and corrected version

  <ul>
   <li> Updated output conventions and C usage. </li>
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

#define WORD_LENGTH 100
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

/* the tables of variables and clauses are statically allocated. Modify the
   parameters tab_variable_size and tab_clause_size before compilation if
   necessary */
#define tab_variable_size  200000
#define tab_clause_size 800000
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

int manage_clauses(register int *clauses) {
   register int clause;
   for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
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

void simple_manage_clauses(register int *clauses) {
   register int clause;
   for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
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

void my_simple_manage_clauses(register int *clauses) {
   register int clause;
   for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
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

int my_manage_clauses(register int *clauses) {
  register int clause;
  for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
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

void print_values(int nb_var) {
    FILE* fp_out;
    int i;
    fp_out = fopen("satx.sol", "w");
    for (i=0; i<nb_var; i++) {
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

int smaller_than(int lit1, int lit2) {
    return ((lit1<NB_VAR) ? lit1 : lit1-NB_VAR) <
        ((lit2<NB_VAR) ? lit2 : lit2-NB_VAR);
}

my_type redundant(int *new_clause, int *old_clause) {
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

my_type get_resolvant(int *clause1, int *clause2, int *resolvant) {
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

void remove_link(int clause) {
   int lit;
   int *lits;
   struct node *pnode1, *pnode2, *pnode;

   lits = sat[clause];

   for (lit=*lits; lit != NONE; lit=*(++lits)) {
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

void set_link(int clause) {
   int lit;
   int *lits;
   struct node *pnode;

   lits = sat[clause];
   for (lit=*lits; lit != NONE; lit=*(++lits)) {
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

void set_link_for_resolv(int resolv) {
   int lit, *lits;
   struct node *pnode;
   lits=sat[resolv];
   for (lit=*lits; lit != NONE; lit=*(++lits)) {
       pnode = allocate_node();
       pnode->clause = resolv;
           pnode->next=in_resolv[lit];
           in_resolv[lit]=pnode;
   }
}

void remove_link_for_resolv(int resolv) {
        int lit, *lits;
   struct node *pnode1, *pnode2, *pnode;

   lits = sat[resolv];
   for (lit=*lits; lit != NONE; lit=*(++lits)) {
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

int already_present(int *resolvant) {
  int lit, *lits, clause, length=0, i;
  struct node *pnode;
  lits=resolvant;
  for (lit=*lits; lit != NONE; lit=*(++lits)) {
    length++;
    for (pnode=in_resolv[lit]; pnode != NULL; pnode=pnode->next) {
      clause=pnode->clause;
      CLAUSE_INVOLVED[clause]++;
      if (CLAUSE_INVOLVED[clause]==1)
        push(clause, INVOLVED_CLAUSE_STACK);
      if (clause_length[clause]==CLAUSE_INVOLVED[clause]) {
        for(i=0; i<INVOLVED_CLAUSE_STACK_fill_pointer; i++)
          CLAUSE_INVOLVED[INVOLVED_CLAUSE_STACK[i]]=0;
        INVOLVED_CLAUSE_STACK_fill_pointer=0;
        return NEW_CLAUSE_REDUNDANT;
      }
    }
  }
  for(i=0; i<INVOLVED_CLAUSE_STACK_fill_pointer; i++) {
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

int search_redundence(int *lits) {
  int lit, is_red;
  int *old_lits, *new_lits;
  struct node *pnode, *pnode1;

  /* if lits is unit, all clauses in the pnode list become redundant and
     will be deleted, so that pnode=pnode->next is not good */
  new_lits = lits; OLD_CLAUSE_SUPPRESED=FALSE;
  for (lit=*lits; lit != NONE; lit=*(++lits)) {
    for (pnode = (positive(lit) ? node_pos_in[lit] :
                  node_neg_in[get_var_from_lit(lit)]);
         pnode != NULL; pnode = pnode1) {
      pnode1=pnode->next;
      old_lits = sat[pnode->clause];
      is_red = redundant(new_lits, old_lits);
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

int add_resolvant(int *lits) {
  int j, lit, is_red, resolvant[RESOLVANT_LENGTH+1];
  my_type is_res;
  int *old_lits, *new_lits, *res;
  struct node *pnode, *pnode1;

  /* if lits is unit, all clauses in the pnode list become redundant and
     will be deleted, so that pnode=pnode->next is not good */
  new_lits = lits;
  for (lit=*lits; lit != NONE; lit=*(++lits))
    for (pnode = (positive(lit) ?
                  node_neg_in[lit] :
                  node_pos_in[get_var_from_lit(lit)]);
         pnode != NULL; pnode = pnode1) {
      pnode1=pnode->next;
      old_lits = sat[pnode->clause];
      is_res = get_resolvant(new_lits, old_lits, resolvant);
      if (is_res == NONE) return NONE;
      if (is_res != FALSE) {
        is_red=search_redundence(resolvant);
        if (is_red != NEW_CLAUSE_REDUNDANT) {
          if (already_present(resolvant) == FALSE) {
            res=(int *)malloc((RESOLVANT_LENGTH+1)*sizeof(int));
            if (OLD_CLAUSE_SUPPRESED==TRUE)
              clause_state[NB_CLAUSE]=ACTIVE2;
            else
              clause_state[NB_CLAUSE]=ACTIVE;
            j=0;
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

int* copy_clauses(struct node *node_in) {
    int j=0, *in; struct node *pnode;

    pnode=node_in;
    while (pnode != NULL) {
      if (clause_state[pnode->clause]==ACTIVE) j++;
      pnode = pnode->next;
    }
    in = (int *)malloc((j+1)*sizeof(int));
    j=0; pnode=node_in;
    while (pnode != NULL) {
      if (clause_state[pnode->clause]==ACTIVE) {
        in[j] = pnode->clause; j++;
      }
      pnode = pnode->next;
    }
    in[j] = NONE;
    return in;
}

int unitclause_process();

/* a clause should not contain more than 1000 literals */
my_type build_sat_instance(char *input_file) {
   FILE* fp_in=fopen(input_file, "r");
   char ch, tautologie, word2[WORD_LENGTH];
   int i, j, length, ii, jj, lit1,
       lits[1000], *plit, lit, NB_CLAUSE1, is_res;

   if (fp_in==NULL) return FALSE;

   ch=getc(fp_in);
  while (ch!='p') {
    while (ch!='\n') ch=getc(fp_in);
    ch=getc(fp_in);
  }

   fscanf(fp_in, "%s%d%d", word2, &NB_VAR, &NB_CLAUSE);

   for (i=0; i<NB_CLAUSE; i++) {
      length=0;
      fscanf(fp_in, "%d", &lits[length]);
      while (lits[length] != 0) {
        length++;
        fscanf(fp_in, "%d", &lits[length]);
      }
      tautologie = FALSE;
      /* test if some literals are redundant and sort the clause */
      for (ii=0; ii<length-1; ii++) {
         lit = lits[ii];
         for (jj=ii+1; jj<length; jj++) {
            if (abs(lit)>abs(lits[jj])) {
               lit1=lits[jj]; lits[jj]=lit; lit=lit1;
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
        j=0;
        while (lits[j] != 0) {
           if (lits[j] > 0) sat[i][j] = lits[j]-1;
           else sat[i][j] = abs(lits[j]) + NB_VAR -1;
           j++;
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

   for (i=0; i<NB_VAR; i++) {
      node_neg_in[i] = NULL;
      node_pos_in[i] = NULL;
      var_state[i]=ACTIVE;
   }

   INIT_NB_CLAUSE= NB_CLAUSE;

   for(i=0; i<NB_CLAUSE; i++)
           set_link_for_resolv(i);

   for(i=0; i<tab_clause_size; i++)
           CLAUSE_INVOLVED[INVOLVED_CLAUSE_STACK[i]]=0;
   INVOLVED_CLAUSE_STACK_fill_pointer=0;

   for(i=0; i<NB_CLAUSE; i++) {
          remove_link_for_resolv(i);
      plit = sat[i];
      length = clause_length[i];
      if (length==1) push(i, UNITCLAUSE_STACK);
      if (search_redundence(plit) != NEW_CLAUSE_REDUNDANT) {
        if ((i<INIT_NB_CLAUSE*10) ||
            (length<3) ||
            (clause_state[i]==ACTIVE2)) {
          is_res = add_resolvant(plit);
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
   for(i=0; i<NB_VAR; i++) {
      neg_in[i]=copy_clauses(node_neg_in[i]);
      pos_in[i]=copy_clauses(node_pos_in[i]);
   }
   free_ressources();
   if (unitclause_process()==NONE) return NONE;
   NB_CLAUSE1 = 0;
   for (i=0; i<NB_CLAUSE; i++) {
     if (clause_state[i] == ACTIVE) NB_CLAUSE1++;
   }
   NB_CLAUSE = NB_CLAUSE1;

   return TRUE;
}

my_type build_simple_sat_instance(char *input_file) {
   FILE* fp_in=fopen(input_file, "r");
   char ch, word2[WORD_LENGTH];
   int i, j, length, NB_CLAUSE1, ii, jj, tautologie,
       lits[1000], *lits1, lit, lit1, var, *pos_nb, *neg_nb;

   if (fp_in == NULL) return FALSE;

   ch=getc(fp_in);
  while (ch!='p') {
    while (ch!='\n') ch=getc(fp_in);
    ch=getc(fp_in);
  }

   fscanf(fp_in, "%s%d%d", word2, &NB_VAR, &NB_CLAUSE);
   INIT_NB_CLAUSE = NB_CLAUSE;
   neg_nb=reduce_if_negative_nb;
   pos_nb=reduce_if_positive_nb;

   for (i=0; i<NB_VAR; i++) {
       nb_neg_clause_of_length3[i] = 0;
       nb_pos_clause_of_length3[i] = 0;
       nb_neg_clause_of_length2[i] = 0;
       nb_pos_clause_of_length2[i] = 0;
       neg_nb[i] = 0;
       pos_nb[i] = 0;
   }
   for (i=0; i<NB_CLAUSE; i++) {
      length=0;
      fscanf(fp_in, "%d", &lits[length]);
      while (lits[length] != 0) {
        length++;
        fscanf(fp_in, "%d", &lits[length]);
      }
      tautologie = FALSE;
      /* test if some literals are redundant and sort the clause */
      for (ii=0; ii<length-1; ii++) {
         lit = lits[ii];
         for (jj=ii+1; jj<length; jj++) {
            if (abs(lit)>abs(lits[jj])) {
               lit1=lits[jj]; lits[jj]=lit; lit=lit1;
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
        sat[i]= (int *)malloc((length+1) * sizeof(int));
        for (j=0; j<length; j++) {
          if (lits[j] < 0) {
            var=abs(lits[j]) - 1;
            if (length==3)
              nb_neg_clause_of_length3[var]++;
            else
              if (length==2)
                nb_neg_clause_of_length2[var]++;
              else
                if (length==1)
                  push(i, UNITCLAUSE_STACK);
            neg_nb[var]++;
            sat[i][j] = var + NB_VAR ;
          }
          else {
            sat[i][j] = lits[j]-1;
            pos_nb[sat[i][j]]++;
            if (length==3)
              nb_pos_clause_of_length3[sat[i][j]]++;
            else
              if (length==2)
                nb_pos_clause_of_length2[sat[i][j]]++;
              else
                if (length==1)
                  push(i, UNITCLAUSE_STACK);
          }
        }
        clause_length[i]=length;
        clause_state[i] = ACTIVE;
        sat[i][length]=NONE;
      }
      else {NB_CLAUSE--; i--;}
   }
   fclose(fp_in);

   for (i=0; i<NB_VAR; i++) {

      neg_in[i] = (int *)
                  malloc((neg_nb[i]+1) * sizeof(int));
      pos_in[i] = (int *)
                  malloc((pos_nb[i]+1) * sizeof(int));
      neg_in[i][neg_nb[i]]=NONE;
      pos_in[i][pos_nb[i]]=NONE;
      neg_nb[i] = 0;
      pos_nb[i] = 0;
      var_state[i] = ACTIVE;
   }
   for (i=0; i<NB_CLAUSE; i++) {
      lits1 = sat[i];
      for(lit=*lits1; lit!=NONE; lit=*(++lits1)) {
         if (positive(lit))
            pos_in[lit][pos_nb[lit]++] = i;
         else
            neg_in[get_var_from_lit(lit)]
                  [neg_nb[get_var_from_lit(lit)]++] = i;
      }
   }
   if (unitclause_process()==NONE) return NONE;
   NB_CLAUSE1 = 0;
   for (i=0; i<NB_CLAUSE; i++) {
     if (clause_state[i] == ACTIVE) NB_CLAUSE1++;
   }
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

int get_nb(int saved_managedclause_fill_pointer) {
  int i, clause, *lits, lit, var, nb=0;

  for(i=saved_managedclause_fill_pointer;
      i<MANAGEDCLAUSE_STACK_fill_pointer; i++) {
    clause=MANAGEDCLAUSE_STACK[i];
    if (clause_length[clause] == 2) {
      lits = sat[clause];
      for(lit=*lits; lit!=NONE; lit=*(++lits)) {
        if (negative(lit)) {
          var=get_var_from_lit(lit);
          if (var_state[var] == ACTIVE) {
            nb+=nb_pos_clause_of_length2[var];
          }
        }
        else {
          var=lit;
          if (var_state[var] == ACTIVE) {
            nb+=nb_neg_clause_of_length2[var];
          }
        }
      }
    }
  }
  return nb;
}

int examine2(int tested_var) {
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
   int var, lit, *lits, unitclause, unitclause_position;
   NB_SEARCH++;
   for (unitclause_position = 0;
        unitclause_position != UNITCLAUSE_STACK_fill_pointer;
        unitclause_position++) {
       unitclause = UNITCLAUSE_STACK[unitclause_position];
       if (clause_state[unitclause] == ACTIVE) {
          lits = sat[unitclause];
          for(lit=*lits; lit!=NONE; lit=*(++lits)) {
              if (positive(lit)) {
                 var = lit;
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = TRUE;
                    if (my_manage_clauses(neg_in[var]) == TRUE) {
                      if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      LIT_IMPLIED[lit]++;
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                       NB_FIXED++;
                       return NONE;
                    }
                 }
              }
              else {
                 var = get_var_from_lit(lit);
                 if (var_state[var] == ACTIVE) {
                    var_current_value[var] = FALSE;
                    if (my_manage_clauses(pos_in[var]) == TRUE) {
                      if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
                      LIT_IMPLIED[lit]++;
                      var_state[var] = PASSIVE;
                      push(var, VARIABLE_STACK);
                      break;
                    }
                    else {
                      NB_FIXED++;
                      return NONE;
                    }
                 }
              }
          }
       }
   }
   return MANAGEDCLAUSE_STACK_fill_pointer;
}

int satisfy_literal(int lit) {
  int var;
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
    var = get_var_from_lit(lit);
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
  int i, lit;
  for (i=0; i<IMPLIED_LITS_fill_pointer; i++) {
    lit=IMPLIED_LITS[i];
    if (LIT_IMPLIED[lit]==2)
      if (satisfy_literal(lit)==NONE)
        return NONE;
  }
  return TRUE;
}

int get_complement(int lit) {
   if (positive(lit)) return lit+NB_VAR;
   else return lit-NB_VAR;
}

int insert_var_if_necessary1(int var) {
  int weight, nb;
  struct var_node *pvar_node, *pvar_node1, *pvar_node2;

  weight=reduce_if_positive_nb[var]*reduce_if_negative_nb[var]*1024
         + reduce_if_positive_nb[var]
         + reduce_if_negative_nb[var]+1;

  if ((VAR_FOR_TEST1==NULL) ||
      ((VAR_FOR_TEST1 != NULL) && (weight>VAR_FOR_TEST1->weight))) {
     pvar_node=allocate_var_node1();
     pvar_node->var=var; pvar_node->weight=weight;
     pvar_node->next=VAR_FOR_TEST1; VAR_FOR_TEST1=pvar_node;
  }
  else {
    nb=1; pvar_node1=VAR_FOR_TEST1; pvar_node2=pvar_node1->next;
    while (nb<VAR_NODES1_nb) {
      if ((pvar_node2==NULL) ||
          ((pvar_node2!=NULL) && (weight>pvar_node2->weight))) {
        pvar_node=allocate_var_node1();
        pvar_node->var=var; pvar_node->weight=weight;
        pvar_node->next=pvar_node1->next; pvar_node1->next=pvar_node;
        break;
      }
      nb++; pvar_node1=pvar_node2; pvar_node2=pvar_node1->next;
    }
  }
  return TRUE;
}

int branch1() {
   int var, lit, *lits, unitclause, unitclause_position;
   NB_SEARCH++;
   for (unitclause_position = 0;
        unitclause_position != UNITCLAUSE_STACK_fill_pointer;
        unitclause_position++) {
       unitclause = UNITCLAUSE_STACK[unitclause_position];
       if (clause_state[unitclause] == ACTIVE) {
          lits = sat[unitclause];
          for(lit=*lits; lit!=NONE; lit=*(++lits)) {
              if (positive(lit)) {
                 var = lit;
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
                       NB_FIXED++;
                       return NONE;
                    }
                 }
              }
              else {
                 var = get_var_from_lit(lit);
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
                      NB_FIXED++;
                      return NONE;
                    }
                 }
              }
          }
       }
   }
   return MANAGEDCLAUSE_STACK_fill_pointer;
}


int further_examin_var_if_positive(int var) {
  int  lit, nb_reduced_clauses_if_further_positif,
         saved_var_stack_fill_pointer,
     saved_managedclause_fill_pointer;

     saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
     saved_managedclause_fill_pointer=
       MANAGEDCLAUSE_STACK_fill_pointer;

    var_current_value[var] = TRUE;
    var_state[var] = PASSIVE;
    push(var, VARIABLE_STACK);
    my_simple_manage_clauses(neg_in[var]);
    NB_SECOND_SEARCH++;
    nb_reduced_clauses_if_further_positif = branch1();
    reset_context(saved_var_stack_fill_pointer,
                  saved_managedclause_fill_pointer);

    if (nb_reduced_clauses_if_further_positif == NONE) {
      NB_SECOND_FIXED++;
      var_current_value[var] = FALSE;
      var_state[var] = PASSIVE;
      push(var, VARIABLE_STACK);
      my_simple_manage_clauses(pos_in[var]);
      if (branch() == NONE) return NONE;
      lit=get_complement(var);
      if (LIT_IMPLIED[lit]==0) push(lit, IMPLIED_LITS);
      LIT_IMPLIED[lit]++;
    }
    else  NB_SEARCH--;
  return TRUE;
}

int further_examin_var_if_negative(int var) {
  int  nb_reduced_clauses_if_further_negatif,
     saved_var_stack_fill_pointer,
     saved_managedclause_fill_pointer;

  saved_var_stack_fill_pointer=VARIABLE_STACK_fill_pointer;
  saved_managedclause_fill_pointer=
    MANAGEDCLAUSE_STACK_fill_pointer;

  var_current_value[var] = FALSE;
  var_state[var] = PASSIVE;
  push(var, VARIABLE_STACK);
  my_simple_manage_clauses(pos_in[var]);
  NB_SECOND_SEARCH++;
  nb_reduced_clauses_if_further_negatif = branch1();
  reset_context(saved_var_stack_fill_pointer,
                saved_managedclause_fill_pointer);

  if (nb_reduced_clauses_if_further_negatif == NONE) {
    NB_SECOND_FIXED++;
    var_current_value[var] = TRUE;
    var_state[var] = PASSIVE;
    push(var, VARIABLE_STACK);
    my_simple_manage_clauses(neg_in[var]);
    if (branch() == NONE) return NONE;
    if (LIT_IMPLIED[var]==0) push(var, IMPLIED_LITS);
    LIT_IMPLIED[var]++;
  }
  else  NB_SEARCH--;
  return TRUE;
}

int further_examin(int saved_managedclause_fill_pointer) {
   int var, i, *lits, lit, clause;

   for(i=saved_managedclause_fill_pointer;
       i<MANAGEDCLAUSE_STACK_fill_pointer; i++) {
     clause=MANAGEDCLAUSE_STACK[i];
     if (clause_length[clause] == 2) {
       lits = sat[clause];
       for(lit=*lits; lit!=NONE; lit=*(++lits)) {
         if (positive(lit)) {
           var = lit;
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
           var = get_var_from_lit(lit);
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

    for(int clause=*clauses; clause!=NONE; clause=*(++clauses))
     if (clause_state[clause] == ACTIVE)
       if (clause_length[clause] == 2) ++neg_clause2_nb;
       else ++neg_clause3_nb;

    nb_neg_clause_of_length2[var] = neg_clause2_nb;
    nb_neg_clause_of_length3[var] = neg_clause3_nb;
    return neg_clause2_nb + neg_clause3_nb;
}

int get_pos_clause_nb(const int var) {

    my_type pos_clause3_nb = 0, pos_clause2_nb = 0;
    int *clauses, clause;
    clauses = pos_in[var];

    for(clause=*clauses; clause!=NONE; clause=*(++clauses)) {
        if (clause_state[clause] == ACTIVE) {
            if (clause_length[clause] == 2)
                pos_clause2_nb++;
            else
                pos_clause3_nb++;
        }
    }
    nb_pos_clause_of_length2[var] = pos_clause2_nb;
    nb_pos_clause_of_length3[var] = pos_clause3_nb;
    return pos_clause2_nb + pos_clause3_nb;
}

int choose_and_instantiate_variable_in_clause() {
    int var, chosen_var=NONE;
    long  i, posi, nega;
    unsigned long nb_clauses, max_nb_clauses = 0;
    my_type pos2, neg2;

    ++NB_BRANCHE;
    TESTED_VAR_STACK_fill_pointer=0;
    VAR_FOR_TEST1=NULL; VAR_NODES1_index=0; MAX_REDUCED=-1;

    for (var = 0; var < NB_VAR; var++) {
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
              pos2 = nb_pos_clause_of_length2[var];
              neg2 = nb_neg_clause_of_length2[var];
              if ((neg2>0) && (pos2>0) && ((neg2+pos2)>3) ) {
                for(i=0; i<IMPLIED_LITS_fill_pointer; i++)
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
      for (var=0; var<NB_VAR; var++)
        if (var_state[var] == ACTIVE) {
          pos2 = nb_pos_clause_of_length2[var];
          neg2 = nb_neg_clause_of_length2[var];
          if ((neg2>0) && (pos2 > 0) &&
              ((neg2 > 1) || (pos2 > 1) ) ) {
            for(i=0; i<IMPLIED_LITS_fill_pointer; i++)
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
        for (var=0; var<NB_VAR; var++)
          if (var_state[var] == ACTIVE) {
            for(i=0; i<IMPLIED_LITS_fill_pointer; i++)
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
    for (i=0; i<TESTED_VAR_STACK_fill_pointer; i++) {
      var=TESTED_VAR_STACK[i];
      if (var_state[var] == ACTIVE) {
        posi=reduce_if_positive_nb[var];
        nega=reduce_if_negative_nb[var];
        nb_clauses = posi*nega*128 + posi + nega +1;
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

my_type build(const int argc, char* const argv[]) {
    if (argc ==3) {
       if ((argv[2][0] == '-') && (argv[2][1] == 's'))
          return build_simple_sat_instance(argv[1]);
       else if ((argv[1][0] == '-') && (argv[1][1] == 's'))
               return build_simple_sat_instance(argv[2]);
       else if (argv[1][0] == '-')
              return build_sat_instance(argv[2]);
       else return build_sat_instance(argv[1]);
    }
    else return build_sat_instance(argv[1]);
}

void reset_all() {
   int index;

   UNITCLAUSE_STACK_fill_pointer = 0; NB_BACK=0; NB_BRANCHE=0;
   NB_MONO=0; NB_UNIT=0;  NB_SEARCH=0; NB_FIXED=0;
   NB_SECOND_SEARCH=0; NB_SECOND_FIXED=0;
   for (index=0; index<VARIABLE_STACK_fill_pointer; index++)
     var_state[VARIABLE_STACK[index]] = ACTIVE;
   VARIABLE_STACK_fill_pointer = 0;

   for (index = 0; index < CLAUSE_STACK_fill_pointer; index++)
     clause_state[CLAUSE_STACK[index]] = ACTIVE;
   CLAUSE_STACK_fill_pointer = 0;

   for (index = 0; index < MANAGEDCLAUSE_STACK_fill_pointer; index++)
     clause_length[MANAGEDCLAUSE_STACK[index]]++;
   MANAGEDCLAUSE_STACK_fill_pointer = 0;
}

void dpl() {
  reset_all();
  for(int var=0; var<NB_VAR; ++var) test_flag[var]=0;
  do {
    if (UNITCLAUSE_STACK_fill_pointer==0)
      if (choose_and_instantiate_variable_in_clause()==NONE) backtracking();
    if (unitclause_process()==NONE) backtracking();
  } while ((VARIABLE_STACK_fill_pointer != 0) && (!(satisfiable())));
}


int main(const int argc, char* const argv[]) {
   if ((argc!=2) && (argc !=3)) {
      printf("Usage format: \"satz input_instance [-s]\"\n");
      return EXITCODE_PARAMETER_FAILURE;
   }
   char saved_input_file[WORD_LENGTH];
   for (int i=0; i<WORD_LENGTH; ++i) saved_input_file[i]=argv[1][i];

   struct tms* a_tms = malloc(sizeof(struct tms));
   times(a_tms);
   const clock_t begintime = a_tms->tms_utime;

   int exit_value;
   switch (build(argc, argv)) {
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
        printf("An empty resolvent was found.\n");
        exit_value = EXITCODE_UNSAT;
        break;
   }
   times(a_tms);
   const clock_t endtime = a_tms->tms_utime;

   const bool sat_decision = satisfiable();
   if (sat_decision) {
     exit_value = EXITCODE_SAT;
     printf ("**** The instance is satisfiable. *****\n");
     if (verify_solution()) print_values(NB_VAR);
     else {
       exit_value = EXITCODE_VERIFICATION_FAILED;
       printf ("**** Solution verification failed (for unknown reasons)! ****\n");
     }
   }
  else {
    exit_value = EXITCODE_UNSAT;
    printf ("**** The instance is unsatisfiable. *****\n");
  }

  printf("NB_MONO= %lu, NB_UNIT= %lu, NB_BRANCHE= %lu, NB_BACK= %lu\n",
         NB_MONO, NB_UNIT, NB_BRANCHE, NB_BACK);

  const long EPS = sysconf(_SC_CLK_TCK);
  const double elapsed = (double)(endtime-begintime)/EPS;
  printf ("Program terminated in %5.3f seconds.\n", elapsed);

  FILE* const fp_time = fopen("satz215_timetable", "a");
  fprintf(fp_time, "\"%s\" %5.3f %lu %lu %ld %ld %d %d %d %d %ld %ld\n",
          saved_input_file, elapsed,
          NB_BRANCHE, NB_BACK,  NB_SEARCH, NB_FIXED,
          sat_decision, NB_VAR, INIT_NB_CLAUSE, NB_CLAUSE-INIT_NB_CLAUSE,
          NB_SECOND_SEARCH, NB_SECOND_FIXED);
  printf("\"%s\" %5.3f %lu %lu %ld %ld %d %d %d %d %ld %ld\n",
          saved_input_file, elapsed,
          NB_BRANCHE, NB_BACK,  NB_SEARCH, NB_FIXED,
          sat_decision, NB_VAR, INIT_NB_CLAUSE, NB_CLAUSE-INIT_NB_CLAUSE,
          NB_SECOND_SEARCH, NB_SECOND_FIXED);
  fclose(fp_time);
  return exit_value;
}
