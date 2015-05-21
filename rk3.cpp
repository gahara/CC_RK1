#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Statement 
{
   int stmt_type, cnst, var;
   struct Statement *next;
} Statement;

typedef struct Prog 
{
   int prog_id;
   Statement *stmt;
   struct Prog *next;
} Prog;

Statement *ScanStatement() 
{
   char str[16];
   Statement *s;

   s = (Statement*)malloc(sizeof(Statement));
   s->next = 0;

   scanf("%s", str);
   if (!strcmp(str, "print")) 
   {
      s->stmt_type = 1;
      scanf("%s", str);
      s->var = *str-'a';
   } 
   else if (!strcmp(str, "lock")) 
   {
      s->stmt_type = 2;
   } 
   else if (!strcmp(str, "unlock")) 
   {
      s->stmt_type = 3;
   }
   else if (!strcmp(str, "end")) 
   {
      s->stmt_type = 4;
   }
   else 
   {
      s->stmt_type = 0;
      s->var = *str - 'a';
      scanf(" = %d", &s->cnst);
   }
   return s;
}

Prog *Add(Prog *q, Prog *prog) 
{
   Prog *head;
   prog->next = 0;
   if (!q) 
   {
      return prog;
   } 
   else 
   {
      head = q;
      while (q->next) q = q->next;
      q->next = prog;
      return head;
   }
}

int main() 
{
   Statement *progs, *stmt;
   Prog *ready_queue = 0, *block_queue, *prog, *tmp_prog;
   int n_prog, num_prog, runtime[5], quantum, from_left;
   int vars[26], locked;

   scanf("%d %d %d %d %d %d %d", &num_prog, runtime, runtime + 1, runtime + 2,
	                                       runtime + 3, runtime + 4, &quantum);
   progs = (Statement*)malloc(sizeof(Statement) * num_prog);
   for (n_prog = 0; n_prog < num_prog; n_prog++) 
   {
      stmt = ScanStatement();
      progs[n_prog] = *stmt;
      stmt = progs + n_prog;
      prog = (Prog*)malloc(sizeof(prog));
      prog->stmt = stmt;
      prog->prog_id = n_prog+1;
      while (stmt->stmt_type != 4) 
      {
	     stmt->next = ScanStatement();
	     stmt = stmt->next;
      }
      ready_queue = Add(ready_queue, prog);
   }

   for (int i = 0; i < 26; i++) 
      vars[i] = 0;
   
   block_queue = 0;
   from_left = quantum;
   locked = 0;

   while (ready_queue) 
   {
      prog = ready_queue;
      ready_queue = ready_queue->next;
      stmt = prog->stmt;
      
      switch (stmt->stmt_type) 
      {
         case 0:
	        vars[stmt->var] = stmt->cnst;
	      break;
       
         case 1:
	        printf("%d: %d\n", prog->prog_id, vars[stmt->var]);
	      break;
       
         case 2:
	        if (locked) 
           {
	           block_queue = Add(block_queue, prog);
	           from_left = quantum;
	           continue;
	        } 
           else 
           {
	           locked = 1;
	        }
	      break;
       
         case 3:
	        locked = 0;
	        if (block_queue) 
           {
	           tmp_prog = block_queue;
	           block_queue = block_queue->next;
	           tmp_prog->next = ready_queue;
	           ready_queue = tmp_prog;
	        }    
	      break;
 
      }
      
      from_left -= runtime[stmt->stmt_type];

      if (stmt->stmt_type != 4) 
      {
	     prog->stmt = stmt->next;
	 
	     if (from_left <= 0) 
        {
	        ready_queue = Add(ready_queue, prog);
	        from_left = quantum;
	     } 
        else 
        {
	        prog->next = ready_queue;
	        ready_queue = prog;
	     }
      }

   }       
	 
   return 0;
}