#include "stdio.h"
#include "stdlib.h"
#include "sys/stat.h"

int numcmp = 0;
int isF = 0;

int frobcmp(const void* a, const void* b)
{
  numcmp++;
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

int frobcmpNEW(const void* a, const void* b)
{
  numcmp++;
  int i = 0;
  while (((*(char**)a)[i] != ' ') && ((*(char**)b)[i] != ' '))
    {
      if (toupper((*(char**)a)[i] ^ (char)0x2A) > toupper((*(char**)b)[i] ^ (char)0x2A))
        return 1;
      else if (toupper((*(char**)a)[i] ^ (char)0x2A) < toupper((*(char**)b)[i] ^ (char)0x2A))
        return -1;
      i++;
    }
  if ((((*(char**)a)[i] ^ (char)0x2A) == ' ') &&
      (((*(char**)b)[i] ^ (char)0x2A) == ' ')) return 0;
  if (((*(char**)b)[i] ^ (char)0x2A) == ' ') return 1;
  else return -1;
}

int main(int argc, char** argv)
{
  if (argc >= 3)
    {
      char error[] = "error: wrong number of arguments";
      write(1, error, 32);
      exit(1);
    }
  if (argc == 2)
    { 
      if(argv[1][0] != '-' || argv[1][1] != 'f')
        {
	  char error[] = "error: wrong argument";
	  write(1, error, 21);
          exit(1);
        }
      else isF = 1;
    }
  int m_size = 10;
  struct stat buf;
  int ret = fstat(0, &buf);
  //printf ("%d", ret);
  if (ret != 0)
    {
      char error[] = "Cannot get file sizes";
      write(1, error, 21);
    }
  int fileSize = buf.st_size + 1;
  //printf ("%d", fileSize);
  if (fileSize != 1) m_size = fileSize;
  char* m_array = (char*)malloc(m_size);
  if (m_array == NULL)
    {
      char error[] = "Memory is not enough";
      write(1, error, 20);
      exit(1);
    }
  int iter = 0;
  if (ferror(stdin))
    {
      char error[] = "Failed to read from file";
      write(1, error, 24);
      exit(1);
    }
  while (1)
    {
      if (iter == m_size)
	{
          //printf("You failed");
          char* check = m_array;   
          m_array = (char*)realloc(m_array, m_size * 2);
	  if (m_array == NULL)
	    {
	      free(check);
	      char error[] = "Memory is not enough";
	      write(1, error, 20);
	      exit(1);
	    }
	  m_size *= 2;
	}
      char TEST;
      int check = read(0, &TEST, 1);
      if (check == 0)
	{
          if (iter != 0) m_array[iter] = ' ';
	  iter++;
	  break;
	}
      m_array[iter] = TEST;
      iter++;
    }
  int m_num = 4;
  if (fileSize != 1) m_num = fileSize;
  char** new_array = (char**)malloc(m_num * sizeof(char*));
  if (new_array == NULL)
    {
      char error[] = "Memory is not enough";
      write(1, error, 20);
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
	      	  char error[] = "Memory is not enough";
		  write(1, error, 20);
	      	  exit(1);
	      	}
	      m_num *= 2;
	    }
	  new_array[outer] = &m_array[i + 1];
	  outer++;
	}
    }
  if (isF == 1) qsort(new_array, outer - 1, sizeof(char*), frobcmpNEW);
  else qsort(new_array, outer - 1, sizeof(char*), frobcmp);
  for (int i = 0; i != outer - 1; i++)
    {
      for (int j = 0; new_array[i][j] != ' '; j++)
	{
	  write(1, &new_array[i][j], 1);
          if (ferror(stdin))
	    {
	      char error[] = "Failed to read from file";
	      write(1, error, 24);
	      exit(1);
	    }
	}
      char space = ' ';
      write(1, &space, 1);
      if (ferror(stdin))
	{
	  char error[] = "Failed to read from file";
          write(1, error, 24);
	  exit(1);
	}
    }
  free(new_array);
  free(m_array);
  //printf("\n");
  // printf("Number of comparison: ");
  //printf("%d", numcmp);
}
