/* File: indexPage.c */
/* Author: Britton Wolfe */
/* Date: September 3rd, 2010 */

/* This program indexes a web page, printing out the counts of words on that page */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* TODO: structure definitions */
struct Trie
{
  char letter;
  struct Trie *Trie[26];
  int occurs;
};

/* NOTE: int return values can be used to indicate errors (typically non-zero)
   or success (typically zero return value) */

/* TODO: change this return type */
struct Trie *indexPage(const char *url);

int addWordOccurrence(const char *word, int wordPos, struct Trie *rootNode);

void printTrieContents(struct Trie *rootNode, char str[300], int pos);

int freeTrieMemory(struct Trie *rootNode);

int getText(const char *srcAddr, char *buffer, const int bufSize);

int main(int argc, char **argv)
{

  if (argc != 2)
  {
    printf("Usage: <url>");
    return -1;
  }
  printf("%s\n", argv[1]);
  char str[300];
  struct Trie *returnValue;
  returnValue = indexPage(argv[1]);

  printTrieContents(returnValue, str, 0);
  if (!freeTrieMemory(returnValue))
  {
    printf("Memory not freed succesfully\n");
  }
  return 0;
}

struct Trie *indexPage(const char *url)
{
  const int BUFFER_SIZE = 300000;
  char str[300000];
  char *buffer = str;
  int bytes = getText(url, buffer, BUFFER_SIZE);
  int wordPos = 0, bufferPos = 0, error = 0;
  char word[300];
  struct Trie *root = malloc(sizeof(struct Trie));
  if (root == NULL)
  {
    printf("failed to allocate memory\n");
  }

  for (int i = 0; i < 26; i++)
  {
    root->Trie[i] = NULL;
  }
  while (str[bufferPos] != '\0' && wordPos < 300 && bufferPos < bytes)
  {
    if ((str[bufferPos] >= 97 && str[bufferPos] <= 122) || (str[bufferPos] >= 65 && str[bufferPos] <= 90))
    {
      word[wordPos] = tolower(str[bufferPos]);
      wordPos += 1;
      bufferPos += 1;
    }
    else
    {
      word[wordPos + 1] = '\0';
      bufferPos += 1;
      if (word[0] != '\0')
      {
        printf("\t%s\n", word);
        error = addWordOccurrence(word, 0, root);
        for (int i = 0; i < 300; i++)
        {
          word[i] = '\0';
          ;
        }
      }
      wordPos = 0;
    }
  }
  bufferPos += 1;
  if (error != -1)
  {
    return root;
  }
  else
  {
    printf("ERROR adding word");
    return root;
  }
}

int addWordOccurrence(const char *word, int wordPos, struct Trie *rootNode)
{
  int error = 0;
  int numeric = word[wordPos] - 97;
  if (rootNode->Trie[numeric] == NULL)
  {
    struct Trie *leaf = malloc(sizeof(struct Trie));
    for (int i = 0; i < 26; i++)
    {
      leaf->Trie[i] = NULL;
    }
    if (leaf == NULL)
    {
      printf("failed to allocate memory\n");
      return -1;
    }
    leaf->occurs = 0;
    leaf->letter = word[wordPos];
    rootNode->Trie[numeric] = leaf;
    if (word[wordPos + 1] == '\0')
    {
      rootNode->Trie[numeric]->occurs += 1;
      return 0;
    }
  }
  else
  {
    if (word[wordPos + 1] == '\0')
    {
      rootNode->Trie[numeric]->occurs += 1;
      return 0;
    }
  }
  error = addWordOccurrence(word, wordPos + 1, rootNode->Trie[numeric]);

  return error;
}

void printTrieContents(struct Trie *rootNode, char str[300], int pos)
{
  for (int i = 0; i < 26; i++)
  {
    if (rootNode->Trie[i] == NULL)
    {
      continue;
    }
    else
    {
      str[pos] = rootNode->Trie[i]->letter;
      if (rootNode->Trie[i]->occurs)
      {
        str[pos + 1] = '\0';
        printf("%s: %d\n", str, rootNode->Trie[i]->occurs);
        rootNode->Trie[i]->occurs = 0;
        i = -1;
      }
      else
      {
        printTrieContents(rootNode->Trie[i], str, pos + 1);
      }
    }
  }
}

int freeTrieMemory(struct Trie *rootNode)
{
  int freed;
  for (int i = 0; i < 26; i++)
  {
    if (rootNode->Trie[i] == NULL)
    {
      if (i == 25)
      {
        free(rootNode);
        return 1;
      }
      else
      {
        continue;
      }
    }
    else
    {
      freed = freeTrieMemory(rootNode->Trie[i]);
    }
  }
  free(rootNode);
  return freed;
}

/* You should not need to modify this function */
int getText(const char* srcAddr, char* buffer, const int bufSize){
  FILE *pipe;
  int bytesRead;

  snprintf(buffer, bufSize, "curl -s \"%s\" | python3 getText.py", srcAddr);

  pipe = popen(buffer, "r");
  if(pipe == NULL){
    fprintf(stderr, "ERROR: could not open the pipe for command %s\n",
	    buffer);
    return 0;
  }

  bytesRead = fread(buffer, sizeof(char), bufSize-1, pipe);
  buffer[bytesRead] = '\0';

  pclose(pipe);

  return bytesRead;
}
