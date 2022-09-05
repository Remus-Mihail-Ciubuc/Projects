#include<iostream>
#include<stdlib.h>

struct trieNode{
    struct trieNode* links[27];
    bool isEndOfWord;
    };
    
struct trieNode *getNode(void)
{
	struct trieNode *pNode = NULL;
	pNode = (struct trieNode *)malloc(sizeof(struct trieNode));
	if (pNode)
	{
		int i;
		pNode->isEndOfWord = FALSE;
		for(i=0; i < ALPHABET_SIZE; i++)
			pNode->links[i] = NULL;
	}
	return pNode;
};

void inserare(char cuvant, trieNode root)
{
    int i=0;
    int p=root;
    nextChar=cuvant[i];
    while(nextChar != '0')
    {
        if(p[nextChar - ' a '] == NULL)
              p[nextChar - ' a ' ] = new trieNode;
         
        p=p[nextChar - ' a '];
        i=i+1;
        nextChar=cuvant[i];
    }
     if(i != 0)
    {
          p[' { ' - ' a '] = p;
}

int cautare(char cuvant, trieNode root)
{
    int i=0;
    int p=root;
    nextChar=cuvant[i];
    while(nextChar != '0' && p!=NULL)
    {
        p= p[nextChar - ' a '];
        i=i+1;
        nextChar=cuvant[i];
    }
    
    if( p == NULL || p[' { ' - ' a '] == NULL)
        return FALSE;
    else
        return TRUE;
}
  
  int main()
{
    string keys[] = {"the", "a", "there",
                    "answer", "any", "by",
                     "bye", "their" };
    int n = sizeof(keys)/sizeof(keys[0]);
 
    struct TrieNode *root = getNode();
 
    // Construct trie
    for (int i = 0; i < n; i++)
        insert(root, keys[i]);
 
    // Search for different keys
    search(root, "the")? cout << "Yes\n" :
                         cout << "No\n";
    search(root, "these")? cout << "Yes\n" :
                           cout << "No\n";
    return 0;
}
