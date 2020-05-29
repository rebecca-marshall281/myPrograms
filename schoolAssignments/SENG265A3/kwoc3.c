/*
 * kwoc3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Spring 2020
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "emalloc.h"
#include "listy.h"

#define MAX_WORD_LEN 40
#define MAX_LINE_LEN 100


void print_word(node_t *node, void *arg)
{
    char *format = (char *)arg;
    printf(format, node->text);
}

//read input file and store it in a linked list
node_t *store_input_file(node_t *head, char *input_file) {
	FILE *fp;
	fp = fopen(input_file, "r");
	char temp_line[MAX_LINE_LEN] = "";
	node_t *temp_node = NULL;
	while(fgets(temp_line, MAX_LINE_LEN, fp)) {
		temp_line[strlen(temp_line)-1] = '\0';
		temp_node = new_node(temp_line);
		head = add_end(head, temp_node);
	}
	fclose(fp);
	return head;
}

//store words from input file linked list in another linked list
node_t *store_words(node_t *file_p, node_t *word_p, int *num_words) {
	char buffer[MAX_LINE_LEN];
	node_t *temp_node = NULL;
	while(file_p != NULL) {
		strncpy(buffer, file_p->text, MAX_LINE_LEN);
		char *t;
		t = strtok(buffer, " ");
		while (t != NULL) {
			temp_node = new_node(t);
			word_p = add_end(word_p, temp_node);
			*num_words = *num_words + 1;
			t = strtok(NULL, " ");
		}
		file_p = file_p->next;
	}
	return word_p;
}

//remove duplicates of words from word linked list
node_t *remove_duplicates(node_t *word_p, int *num_words) {
	int len = *num_words;	
	for (int i = 0; i < len; i++) {
		int add = 1;
		node_t *p = word_p->next;
		for (int j = i+1; j < len; j++) {
			if (strcmp(word_p->text, p->text)==0) {
				 add = 0;
			}
			p = p->next;
			
		}
		if (add == 1) {
			node_t *temp = new_node(word_p->text);
			word_p = add_end(word_p, temp);
		}
		else {
			*num_words = *num_words - 1;
		}
		node_t *temp = word_p;
		word_p = remove_front(word_p);
		free(temp);
		
	}
	return word_p;

}

//remove excluded words from word linked list
node_t *remove_excluded(node_t *words, node_t *e_words, int *num_words) {
	int len = *num_words;
	for (int i = 0; i < len; i++) {
		int add = 1;
		node_t *p = e_words;
		while(p!=NULL) {
			if (strcmp(words->text, p->text)==0) {
				add = 0;
			}
		p = p->next;
		}
		if (add == 1) {
			node_t *temp = new_node(words->text);
			words = add_end(words, temp);
		}
		else {
			*num_words = *num_words - 1;
		}
		node_t *temp = words;
		words = remove_front(words);
		free(temp);
	}	
	return words;
}

//convert words from an array to uppercase
char *convert_to_upper_word(char *word) {
	char *p = word;
	for (int i = 0; i < strlen(p); i++) {
		p[i] = toupper(p[i]);
	}
	return word; 
}

//convert words from a linked list to uppercase
node_t *convert_to_upper(node_t *words) {
	node_t *head = words;
	while (head != NULL) {
		for (int i = 0; i < strlen(head->text); i++) {
			head->text[i] = toupper(head->text[i]);
		}
		head = head->next;
	}
	return words;
}

//sort the words linked list in alphabetical order
node_t *sort(node_t *head) {
	node_t *p = head;
	while (p != NULL) {
		node_t *nextp = p->next;
		node_t *min = p;
		while (nextp != NULL) {
			if (strcmp(nextp->text, min->text) < 0) {
				min = nextp;
			}
			nextp = nextp->next;
		}
		char temp[MAX_WORD_LEN];
		strncpy(temp, p->text, MAX_WORD_LEN);
		strncpy(p->text, min->text, MAX_WORD_LEN);
		strncpy(min->text, temp, MAX_WORD_LEN);
		p = p->next;
	}
	return head;
}

//find the longest word so we can format the output properly
int max_length(node_t *words) {
	if (words == NULL) {
		return 0;
	}
	int max_l = strlen(words->text);
	while (words->next != NULL) {
		if (max_l < strlen(words->next->text)) {
			max_l = strlen(words->next->text);
		}
		words = words->next;
	}
	return max_l;
	
}

//print the final output 
void print_output(int word_freq, char *word, int max_l, char *line, int line_counter) {
        int spaces = max_l-strlen(word)+2;
        printf("%s", word);
        for (int i = 0; i < spaces; i++) {
                printf(" ");
        }
        if (word_freq == 1) {
                printf("%s (%d)\n", line, line_counter);
        }
        else {
                printf("%s (%d*)\n", line, line_counter);
        }
}

//get the requirements for printing the output (e.g if the word appears in the line, if it appears once or more than once) and call the print_output function to print 
void get_output(node_t *file, node_t *words, int max_l) {
	
	while (words != NULL) {
		int line_counter = 1;
		node_t *file_p = file;
		while (file_p != NULL) {
			int word_freq = 0;
			char *temp_line = (char *)emalloc(sizeof(char)*MAX_LINE_LEN);
			strncpy(temp_line, file_p->text, MAX_LINE_LEN);
			temp_line = convert_to_upper_word(temp_line);
			char buffer[MAX_LINE_LEN];
			strncpy(buffer, temp_line, MAX_LINE_LEN);
			char *t = strtok(buffer, " ");
			while (t != NULL) {
				if (strcmp(words->text, t) == 0) {
					word_freq++;
				} 
				t = strtok(NULL, " ");
			}
			if (word_freq >= 1) {
				print_output(word_freq, words->text, max_l, file_p->text, line_counter);
			}	
			
			line_counter++;
			file_p = file_p->next;
			free(temp_line);			
		}
		words = words->next;
	}
}



int main(int argc, char *argv[])
{
	char *exception_file = NULL;
    	char *input_file = NULL;
    	int i;
	//checks the command line inputted and associates the input_file and exception_file with the correct file that was inputted
    	for (i = 1; i < argc; i++) {
        	if (strcmp(argv[i], "-e") == 0 && i+i <= argc) {
            	exception_file = argv[++i];
        	} else {
            	input_file = argv[i];
        	}
    	}
	
	node_t *head_file = NULL;
	node_t *head_words = NULL;
	node_t *head_exception_words = NULL;
	
	int num_words = 0;
	
	head_file = store_input_file(head_file, input_file);
	head_words = store_words(head_file, head_words, &num_words);
	head_words = convert_to_upper(head_words);
	head_words = remove_duplicates(head_words, &num_words);
	if (exception_file != NULL) {
		head_exception_words = store_input_file(head_exception_words, exception_file);
		head_exception_words = convert_to_upper(head_exception_words);
		head_words = remove_excluded(head_words, head_exception_words, &num_words);
	}
	head_words = sort(head_words);
	int max_length_word = max_length(head_words); 
	get_output(head_file, head_words, max_length_word);

	//FREE MEMORY
	node_t *temp_node = head_file;
	while(temp_node != NULL) {
		assert(temp_node != NULL);
		head_file = remove_front(head_file);
		free(temp_node);
		temp_node = head_file;
	}	
	assert(head_file == NULL);
		
	temp_node = head_words;
        while(temp_node != NULL) {
                assert(temp_node != NULL);
                head_words = remove_front(head_words);
                free(temp_node);
                temp_node = head_words;
        }
        assert(head_words == NULL);
	
	temp_node = head_exception_words;
        while(temp_node != NULL) {
                assert(temp_node != NULL);
                head_exception_words = remove_front(head_exception_words);
                free(temp_node);
                temp_node = head_exception_words;
        }
	assert(head_exception_words == NULL);
	

    exit(0);
}
