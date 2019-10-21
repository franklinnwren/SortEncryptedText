#include "stdio.h"
#include "stdlib.h"

int frobcmp(const void* a, const void* b)
{
  int i = 0;
  while (((*(char**)a)[i] != ' ') && ((*(char**)b)[i] != ' '))
    {
      if (((*(char**)a)[i] ^ (char)0x2A) > ((*(char**)b)[i] ^ (char)0x2A))
  	return 1;
      else if (((*(char**)a)[i] ^ (char)0x2A) < ((*(char**)b)[i] ^ (char)0x2A))
        return -1;
      i++;
    }
  if ((((*(char**)a)[i] ^ (char)0x2A) == ' ') &&
  (((*(char**)b)[i] ^ (char)0x2A) == ' ')) return 0;
  if (((*(char**)b)[i] ^ (char)0x2A) == ' ') return 1;
  else return -1;
}

int main()
{
  int m_size = 10;
  char* m_array = (char*)malloc(m_size);
  if (m_array == NULL)
    {
      fprintf(stderr, "Memory is not enough");
      exit(1);
    }
  int iter = 0;
  if (ferror(stdin))
    {
      fprintf(stderr, "Failed to read from file");
      exit(1);
    }
  while (1)
    {
      if (iter == m_size)
	{
          char* check = m_array;   
	  m_array = (char*)realloc(m_array, m_size * 2);
	  if (m_array == NULL)
	    {
	      free(check);
	      fprintf(stderr, "Memory is not enough");
	      exit(1);
	    }
	  m_size *= 2;
	}
      char TEST = getchar();
      if (TEST == EOF)
	{
          if (iter != 0) m_array[iter] = ' ';
	  iter++;
	  break;
	}
      m_array[iter] = TEST;
      iter++;
    }
  int m_num = 4;
  char** new_array = (char**)malloc(m_num * sizeof(char*));
  if (new_array == NULL)
    {
      fprintf(stderr, "Memory is not enough");
      exit(1);
    }
  new_array[0] = &m_array[0];
  int outer = 1;
  for (int i = 0; i != iter; i++)
    {
      if (m_array[i] == ' ')
	{
	  if (outer == m_num)
	    {
	      char** check = new_array;
       	      new_array = (char**)realloc(new_array, m_num*2*sizeof(char*));
	      if (new_array == NULL)
	      	{
	      	  free(check);
	      	  fprintf(stderr, "Memory is not enough");
	      	  exit(1);
	      	}
	      m_num *= 2;
	    }
	  new_array[outer] = &m_array[i + 1];
	  outer++;
	}
    }
  qsort(new_array, outer - 1, sizeof(char*), frobcmp);
  for (int i = 0; i != outer - 1; i++)
    {
      for (int j = 0; new_array[i][j] != ' '; j++)
	{
	  putchar(new_array[i][j]);
          if (ferror(stdin))
	    {
	      fprintf(stderr, "Failed to read from file");
	      exit(1);
	    }
	}
      putchar(' ');
      if (ferror(stdin))
	{
	  fprintf(stderr, "Failed to read from file");
	  exit(1);
	}
    }
  free(new_array);
  free(m_array);
}
