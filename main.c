#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CATEGORY_NAME_SIZE 20
#define CRED_USERNAME_SIZE 30
#define CRED_PASSWORD_SIZE 20
#define CRED_DESCRIPTION_SIZE 20
#define CRED_DATE_SIZE 20

struct _credential;
typedef struct _credential* credPosition;
typedef struct _credential {
    char username[CRED_USERNAME_SIZE];
    char password[CRED_PASSWORD_SIZE];
    char description[CRED_DESCRIPTION_SIZE];
    char date[CRED_DATE_SIZE];
    credPosition next;
}Credential;

struct _category;
typedef struct _category* catPosition;
typedef struct _category {
    char name[CATEGORY_NAME_SIZE];
    credPosition cred_next;
    catPosition cat_next;
}Category;

catPosition CreateCategory(const char* cat_name);
credPosition CreateCredential(const char* new_username, const char* new_pwd, const char* new_desc);

int main() {
    printf("Hello world\n");

    char name[] = "abc";

    catPosition head = (catPosition)malloc(sizeof(Category));
    head->cat_next = CreateCategory(name);

    free(head);

    return EXIT_SUCCESS;
}

catPosition CreateCategory(const char* cat_name) {
    catPosition new_category = NULL;
    new_category = (catPosition)malloc(sizeof(Category));
    if(new_category == NULL) {
        printf("Memory allocation error...\n");
        return NULL;
    }

    strcpy(new_category->name, cat_name);
    new_category->cred_next = NULL;
    new_category->cat_next = NULL;
    
    return new_category;
}

credPosition CreateCredential(const char* new_username, const char* new_pwd, const char* new_desc) {
    credPosition new_credential = NULL;
    new_credential = (credPosition)malloc(sizeof(Credential));
    if(new_credential == NULL) {
        printf("Memory allocation error...\n");
        return NULL;
    }

    strcpy(new_credential->username, new_username);
    strcpy(new_credential->password, new_pwd);
    strcpy(new_credential->description, new_desc);
    new_credential->next = NULL;

    return new_credential;
}