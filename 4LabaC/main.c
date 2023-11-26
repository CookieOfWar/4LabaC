#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#define MAXLEN 20

typedef struct row {
	int len;
	char* string;
	char* mainvar;
	int shouldcheck;
	struct row* next;
}row;

int readstring(row** head, row** temp);
int comparestrings(char* source, char* with);
row* newlist(char* str, int len);
row* addrow(row* prev, char* str, int len);
void printlist(row* head);
int checksyntax(row* list);
int includedstr(char str, char* check);
void takemainvar(row** tmp);
void checktwins(row* main, row** head);
int deletetwins(row* prevp, row* mainp, row* twin, row** head);
char* strslice(char* str, int begin, int end);
void deleteitem(row* prev, row* delv, row** head);
row* reverserows(row* tmp);

int main() {
	setlocale(0, "rus");

	row* head = NULL, * temp = NULL;

	printf("input:\n");

	while (1) {
		if (!readstring(&head, &temp))
			break;
	}

	if (!head)
		return 0;

	if (!checksyntax(head))
		return 0;

	temp = head;
	while (temp) {
		takemainvar(&temp);
		temp = temp->next;
	}
	temp = head;
	checktwins(temp, &head);

	head = reverserows(head);
	printf("\nКонечная программа:\n");
	printlist(head);
}


int readstring(row** head, row** temp) {
	int a;
	char c;
	char* tmpstr = malloc(sizeof(char));
	if (!tmpstr)
		return 0;
	tmpstr[0] = '\0';

	for (a = 0; a < 100 && (c = getchar()) != '\n'; a++)
	{
		tmpstr = realloc(tmpstr, (a + 2) * sizeof(char));
		if (!tmpstr)
			return 0;
		if (c != ' ') {
			tmpstr[a] = (char)c;
			tmpstr[a + 1] = '\0';
		}
		else {
			a--;
		}
	}

	if (tmpstr) {
		if (comparestrings(tmpstr, "END")) {
			return 0;
		}
	}

	if (tmpstr != '\0')
		if (*temp == NULL) {
			*temp = newlist(tmpstr, a);
			*head = *temp;
			return 1;
		}
		else {
			*temp = addrow(*temp, tmpstr, a);
			*head = *temp;
		}

	return 1;
}

int comparestrings(char* source, char* with) {
	int i = 0;
	while (1) {
		if (with[i]) {
			if (source[i]) {
				if (source[i] != with[i])
					return 0;
			}
			else
				return 0;
		}
		else
			if (!source[i])
				return 1;
		i++;
	}
}

row* newlist(char* str, int len) {
	row* tmp = (row*)malloc(sizeof(row));
	if (tmp) {
		tmp->next = NULL;
		tmp->len = len;
		tmp->string = str;
		tmp->shouldcheck = 0;
		return tmp;
	}
	else
		return NULL;
}

row* addrow(row* prev, char* str, int len) {
	row* tmp = (row*)malloc(sizeof(row));
	if (tmp) {
		tmp->next = prev;
		tmp->len = len;
		tmp->string = str;
		tmp->shouldcheck = 0;
		return tmp;
	}
	else
		return NULL;
}

void printlist(row* head) {
	while (head) {
		printf("%s\n", head->string);
		head = head->next;
	}
}

int checksyntax(row* list) {
	while (list) {
		int i = 0;
		int prevsym = 1;
		int SawIs = 0;
		while ((list->string)[i] != ';') {
			if (prevsym) {
				if (!(list->string)[i])
					break;
				if (includedstr((list->string)[i], "+-*/")) {
					printf("\ninvalid syntax");
					return 0;
				}
				else if (SawIs) {
					if (includedstr((list->string)[i], "=")) {
						printf("\nOperation can't include second equating");
						return 0;
					}
					if (!includedstr((list->string)[i], "+-*/")) {
						prevsym = 0;
					}
				}
				else
					prevsym = 0;
			}
			else {
				if (!(list->string)[i])
					break;
				if (includedstr((list->string)[i], (SawIs) ? "+-*/" : "=")) {
					SawIs = 1;
					prevsym = 1;
				}
				else {
					if (SawIs) {
						if (includedstr((list->string)[i], "=")) {
							printf("\nOperation can't include second equating");
							return 0;
						}
					}
					else {
						if (includedstr((list->string)[i], "+-*/")) {
							printf("\nYou should use equating before doing operations with variables");
							return 0;
						}
					}
				}
			}
			i++;
		}
		if ((list->string)[i] != ';') {
			printf("\ndoesn't has ;");
			return 0;
		}
		list = list->next;
	}
	return 1;
}

int includedstr(char str, char* check) {
	int ci = 0;
	while (1) {
		if (check[ci]) {
			if (str == check[ci])
				return 1;
		}
		else
			return 0;
		ci++;
	}
}

void takemainvar(row** tmp) {
	char* tmpstr = malloc(sizeof(char));
	if (!tmpstr)
		return;
	tmpstr[0] = '\0';
	int i = 0;
		while ((((*tmp)->string)[i]) != '=')
		{
			tmpstr = realloc(tmpstr, (i + 2) * sizeof(char));
			if (!tmpstr)
				break;
			tmpstr[i] = (char)(((*tmp)->string)[i]);
			tmpstr[i + 1] = '\0';
			i++;
		}
		(*tmp)->mainvar = tmpstr;
}

void checktwins(row* main, row** head) {
	row* prevp = NULL, *nextp;
	while (main) {
		int wc = 0;
		row* tmp = ((main)->next);
		while (tmp) {
			if (comparestrings((tmp)->string, (main)->string)) {
				wc = 1;
				nextp = main->next;
				if (deletetwins(prevp, main, tmp, &(*head))) {
					prevp = NULL;
					main = nextp;
					break;
				}
				else {
					prevp = main;
					main = (main)->next;
					break;
				}
			}
			tmp = (tmp)->next;
		}
		if (!wc) {
			main = main->next;
		}
	}
}

void deleteitem(row* prev, row* delv, row** head) {
	if (prev == NULL) {
		*head = delv->next;
	}
	else {
		prev->next = delv->next;
	}
	free(delv);
}

char* strslice(char* str, int begin, int end) {
	char* tmpstr = malloc(sizeof(char));
	if (!tmpstr)
		return '\0';
	tmpstr[0] = '\0';
	for (int i = begin; i < end; i++){
		tmpstr = realloc(tmpstr, (i-begin + 2) * sizeof(char));
		if (!tmpstr)
			break;
		tmpstr[i-begin] = (char)(str[i]);
		tmpstr[i-begin + 1] = '\0';
	}
	return tmpstr;
}

int deletetwins(row* prevp, row* mainp, row* twin, row** head) {
	int i = 0, start = 0, end = 0, waschanges = 0;
	while ((mainp->string)[i]) {
		if ((mainp->string)[i] == '=')
			start = i + 1;
		if (includedstr((mainp->string)[i], "+-*/;")) {
			end = i;
			row* tmp = mainp->next;
			while (tmp!=twin && tmp) {
				if (comparestrings(tmp->mainvar, strslice(mainp->string, start, end))) {
					waschanges = 1;
					break;
				}
				tmp = tmp->next;
			}
			start = end + 1;
		}
		i++;
	}
	if (!waschanges) {
		printf("\n\nСтрока %s повторяется и не зависит от измененных переменных\n\n", mainp->string);
		deleteitem(prevp, mainp, &(*head));
		return 1;
	}
	return 0;
}

row* reverserows(row* tmp) {
	int first = 1;
	row* prev = tmp;
	row* nextp;
	while (tmp) {
		if (first) {
			nextp = tmp->next;
			tmp->next = NULL;
			first = 0;
		}
		else {
			nextp = tmp->next;
			tmp->next = prev;
		}
		prev = tmp;
		tmp = nextp;
	}
	return prev;
}