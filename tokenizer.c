/*
 *  * tokenizer.c
 *   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  * Tokenizer type.  You need to fill in the type as part of your implementation.
 *   */

struct TokenizerT_
{
	char *currTok;
	char *stream;
	char *separ;
};

typedef struct TokenizerT_ TokenizerT;

char *TKGetNextToken();


/*
 *  * TKCreate creates a new TokenizerT object for a given set of separator
 *   * characters (given as a string) and a token stream (given as a string).
 *    * 
 *     * TKCreate should copy the two arguments so that it is not dependent on
 *      * them staying immutable after returning.  (In the future, this may change
 *       * to increase efficiency.)
 *        *
 *         * If the function succeeds, it returns a non-NULL TokenizerT.
 *          * Else it returns NULL.
 *           *
 *            * You need to fill in this function as part of your implementation.
 *             */

TokenizerT *TKCreate(char *separators, char *ts)
{
	
	/* mallocs the tokenizer required, also setting aside variables for separators, the string to be tokenized, 
	 * as well as room for the next token in the stream */
	TokenizerT *tokOb = malloc(sizeof(TokenizerT));
	
	tokOb->stream = ts;
	tokOb->separ = separators;
	tokOb->currTok = TKGetNextToken(tokOb);
  	
	return tokOb;
}

/*
 *  * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 *   * allocated memory that is part of the object being destroyed.
 *    *
 *     * You need to fill in this function as part of your implementation.
 *      */

void TKDestroy(TokenizerT *tk)
{
	free(tk);
}

/*
 *  * TKGetNextToken returns the next token from the token stream as a
 *   * character string.  Space for the returned token should be dynamically
 *    * allocated.  The caller is responsible for freeing the space once it is
 *     * no longer needed.
 *      *
 *       * If the function succeeds, it returns a C string (delimited by '\0')
 *        * containing the token.  Else it returns 0.
 *         *
 *          * You need to fill in this function as part of your implementation.
 *           */

char *TKGetNextToken(TokenizerT *tk)
{
	/*printf("TKGetNextToken:\n");*/

	char *streamCheck = malloc(strlen(tk->stream)*sizeof(char));;
	int len = strlen(streamCheck);

        char *token;
        char *delim = tk->separ;
	
	int tokSize = 0;
        
	int a = 0;
	int i = 0;
        int j = 0;
	/* Checks to see if a character has been removed while merging literals into escape characters */
        int isDel = 0;

	memcpy(streamCheck, tk->stream, strlen(tk->stream) + 1);
        streamCheck[strlen(tk->stream)] = '\0';

	/* Incrementing two pointers: one to the stream of characters to be tokenized and the other the string 
	 * containing all the delimiters 
	 */

	while(*(streamCheck+i) != '\0')
        {
		/*printf("While stream isn't empty \n");*/
		printf("streamCheck+i: %c\n",*(streamCheck+i));

               	j = 0;
               	/* Checking for Null Terminator */
               	while(*(delim+j) != '\0')
               	{
			printf("delim+j: %c\n",*(delim+j));
			printf("delim+j+1: %c\n",*(delim+j+1));
			/*printf("While delim isn't empty\n");*/
			/* Checking for special characters and handling them per specification of the guidelines */
			if(*(delim+j) == '\\' && !(*(delim+j+1) == 'a' || *(delim+j+1) == 'b' || *(delim+j+1) == 'f' || *(delim+j+1) == 'n' || *(delim+j+1) == 'r' || *(delim+j+1) == 't' || *(delim+j+1) == 'v' || *(delim+j+1) == '\\' || *(delim+j+1) == '\"'))
			{
				/* Delim+j is \\ and delim+j+1 is not valid escape char\n" */
				
				j++;
				if(*(streamCheck+i) != '\\')
				{
					if(*(streamCheck+i) == *(delim+j))
                                	{
                                        /* delim is not a valid escape char, stream is not \\ and matches\n */

						isDel = 1;
                                        	break;
                                	}
				}
				else if(*(streamCheck+i) == '\\')
                                {
					i++;
                                        if(*(streamCheck+i) == *(delim+j))
                                        {
                                                /* delim is not a valid escape char, stream is \\\n" */

                                                isDel = 1;
                                                break;
                                        }
                                }

			}
			else if(*(delim+j) == '\\'  && (*(delim+j+1) == 'a' || *(delim+j+1) == 'b' || *(delim+j+1) == 'f' || *(delim+j+1) == 'n' || *(delim+j+1) == 'r' || *(delim+j+1) == 't' || *(delim+j+1) == 'v' || *(delim+j+1) == '\\' || *(delim+j+1) == '\"'))
			{
				/* Delim+j is \\ and delim+j+1 is a valid escape char\n */

				j++;
				if(*(streamCheck+i) == '\\')
                                {
					i++;
                                        if(*(streamCheck+i) == *(delim+j))
                                        {
                                                /* delim is a valid escape char, stream is matching\n */

                                                isDel = 1;
                                                break;
                                        }
				}
			}
			else
			{	
                       		if(*(streamCheck+i) == *(delim+j))
                       		{
					/*printf("Char in stream is a delim\n");*/

                               		isDel = 1;
                               		break;
				}
                       	}
                       	j++;
               	}

               	if(isDel == 1)
               	{
                       	if(tokSize != 0)
                       	{
				token = malloc(tokSize+1);
				a = 0;
				while(a < tokSize)
				{
					token[a] = streamCheck[i - tokSize + a];
					a++;
				}
									
				tk->currTok = token;
	                        tk->stream = (streamCheck+i);
				return token;
                       	}
               	}
		else
		{
			tokSize++;
		}
		
		/* Incrementing */
		/*printf("Curr char: %c\n", *(streamCheck+i));	*/
		i++;
	
		/*printf("Next char: %c\n",*(streamCheck+i));*/

		isDel = 0;
       	}

	/* Checking for the last token (in which there is no delimiter to end the string */
	
	if(tokSize != 0)
	{
		token = malloc(tokSize+1);
		a = 0;
               	while(a < tokSize)
                {
                       	token[a] = streamCheck[i - tokSize + a];
                        a++;
                }

       		tk->currTok = token;
       		tk->stream = (streamCheck+i);
		return token;
	}
	else
	{
		tk->stream = "";
		return 0;
	}
}

/*
 *  * main will have two string arguments (in argv[1] and argv[2]).
 *   * The first string contains the separator characters.
 *    * The second string contains the tokens.
 *     * Print out the tokens in the second string in left-to-right order.
 *      * Each token should be printed on a separate line.
 *       */

int main(int argc, char **argv)
{
	TokenizerT *tokenizer = malloc(sizeof(TokenizerT));
	char *delims;
	char *textStream;
	char *theTok;
	int m = 0;

	
	/* Checking input */
	
	if (argc != 3)
	{
                fprintf(stderr, "ERROR: INVALID NUMBER OF INPUTS\n");
                exit(EXIT_FAILURE);
        }


        if (strcmp(argv[2], "") == 0)
	{
                printf("NO STRING TO TOKENIZE\n");
        }

	
	delims = argv[1];
	textStream = argv[2];

	tokenizer = TKCreate(delims, textStream);
	theTok = tokenizer->currTok;
	
	/* Printing escape characters as hex values per instructions, done in the main routine */
	
		
	while(tokenizer->stream != '\0' && theTok != '\0')
	{
		m = 0;
		while(*(theTok+m) != '\0')
        	{
			if(*(theTok+m) == '\\' && (*(theTok+m+1) == 'a' || *(theTok+m+1) == 'b' || *(theTok+m+1) == 'f' || *(theTok+m+1) == 'n' || *(theTok+m+1) == 'r' || *(theTok+m+1) == 't' || *(theTok+m+1) == 'v' || *(theTok+m+1) == '"'))
			{
				printf("[0x%.2x]",*(theTok+m+1));
				m++;
			}
			else if(*(theTok+m) == '\\' && *(theTok+m+1) == '\0')
				;
			else
                		printf("%c", *(theTok+m));
                	m++;
        	}
        	printf("\n");
		printf("\n");
		
		theTok = TKGetNextToken(tokenizer);
	}

	/* Freeing memory */
	TKDestroy(tokenizer);

  	return 0;
}
