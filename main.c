//Pravi se vise General kategorija, ne ispisuje se dobro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CATEGORY_NAME_SIZE 20
#define CRED_USERNAME_SIZE 30
#define CRED_PASSWORD_SIZE 20
#define CRED_DESCRIPTION_SIZE 20
#define CRED_DATE_SIZE 20
#define MAX_BUFFER 1024

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
credPosition CreateCredential();
void ListCategories(catPosition p);
void AddCredential(catPosition p);
void InsertCredential(catPosition p, credPosition new_credential, const char cat_name[CATEGORY_NAME_SIZE]);
void ListAllCredentials(catPosition p);
void FreeMemory(catPosition p);

void ListCatCredentials(catPosition category) {
    credPosition temp = category->cred_next;
    while(temp != NULL) {
        printf("%s\n", temp->description);
        temp = temp->next;
    }
}

int main() {
    catPosition head = (catPosition)malloc(sizeof(Category));
    int choice = 0;

    head->cat_next = NULL;
    head->cred_next = NULL;

    printf("- Welcome to LockIT -\n");
    while(1) {
        printf("Enter your operation (1) Add new credential (2) List categories, (3) List credentials, (0) Quit): ");
        scanf(" %d", &choice);
        getchar();
        if(choice == 1) {
            AddCredential(head);
        }
        else if(choice == 2) {
            ListCategories(head->cat_next);
        }
        else if(choice == 3) {
            ListAllCredentials(head->cat_next);
        }
        else if(choice == 0) {
            printf("Thanks for using LockIT!\n");
            break;
        }
        else continue;
    }

    FreeMemory(head);

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

credPosition CreateCredential() {

    char new_username[CRED_USERNAME_SIZE];
    char new_password[CRED_PASSWORD_SIZE];
    char new_description[CRED_DESCRIPTION_SIZE];  

    credPosition new_credential = NULL;
    new_credential = (credPosition)malloc(sizeof(Credential));
    if(new_credential == NULL) {
        printf("Memory allocation error...\n");
        return NULL;
    }

    printf("Enter your credential username: ");
    fgets(new_username, CRED_USERNAME_SIZE, stdin);
    new_username[strcspn(new_username, "\n")] = 0;

    printf("Enter your credential password: ");
    fgets(new_password, CRED_PASSWORD_SIZE, stdin);
    new_password[strcspn(new_password, "\n")] = 0;

    printf("Enter your credential description: ");
    fgets(new_description, CRED_DESCRIPTION_SIZE, stdin);
    new_description[strcspn(new_description, "\n")] = 0; 

    strcpy(new_credential->username, new_username);
    strcpy(new_credential->password, new_password);
    strcpy(new_credential->description, new_description);
    new_credential->next = NULL;

    return new_credential;

}

void ListCategories(catPosition p) { //provjeru if p == NULL prebaci u main

    printf("Available categories: \n");
    if(p == NULL) {
        printf("None\n");
    }
    else {
        while(p != NULL) {        
            printf(" - %s\n", p->name);
            p = p->cat_next;
    }
    }
}

void AddCredential(catPosition p) {
    char temp_category[CATEGORY_NAME_SIZE];
    char choice;
    credPosition new_credential = NULL;
    char general_cat[CATEGORY_NAME_SIZE] = "General";

    new_credential = CreateCredential();

    printf("Do you want to store your new credential in a specific category (Y/N)? ");
    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y') {
        ListCategories(p->cat_next);
        printf("Enter your category: ");
        scanf("%s", temp_category);
        InsertCredential(p, new_credential, temp_category);
    }
    else {
        InsertCredential(p, new_credential, general_cat);
    }
    
}

void InsertCredential(catPosition p, credPosition new_credential, const char cat_name[CATEGORY_NAME_SIZE]) {
    catPosition current = p->cat_next;
    int found = 0;

    while(current != NULL) {
        if(strcmp(current->name, cat_name) == 0) {
            found = 1;
            break;
        }
        current = current->cat_next;
    }

    if(found == 1) {
        new_credential->next = current->cred_next;
        current->cred_next = new_credential;
    }
    else {
        catPosition new_category = CreateCategory(cat_name);
        new_category->cat_next = p->cat_next;
        p->cat_next = new_category;
        new_credential->next = new_category->cred_next;
        new_category->cred_next = new_credential;
    }
}

void ListAllCredentials(catPosition p) {    

    printf("Listing all credentials...\n");
    
    while(p != NULL) {
        credPosition temp = p->cred_next;
        if(temp == NULL) {
            printf("No credentials in this category...\n");
        }

        printf("--------------------------------\n");
        printf("Category: %s\n", p->name);
        while(temp != NULL) {
            printf("--------------------------------\n");
            printf(" Description: %s\n Username: %s\n Password: %s\n", temp->description, temp->username, temp->password);
            printf("--------------------------------\n");
            temp = temp->next;
        }
        p = p->cat_next;
    }
}

void FreeMemory(catPosition p) {
    while(p != NULL) {
        catPosition temp_cat = p;
        p = p->cat_next;

        credPosition cred_temp = temp_cat->cred_next;
        while(cred_temp != NULL) {
            credPosition temp_cred = cred_temp;
            cred_temp = cred_temp->next;
            free(temp_cred);
        }

        free(temp_cat);
    }
}
