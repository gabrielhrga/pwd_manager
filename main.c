#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define CATEGORY_NAME_SIZE 20
#define CRED_USERNAME_SIZE 30
#define CRED_PASSWORD_SIZE 20
#define CRED_DESCRIPTION_SIZE 50
#define CRED_DATE_SIZE 20
#define MAX_BUFFER 1024
#define KEY "KEY"

struct _credential;
typedef struct _credential* credPosition;
typedef struct _credential {
    char username[CRED_USERNAME_SIZE];
    char password[CRED_PASSWORD_SIZE];
    char description[CRED_DESCRIPTION_SIZE];
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
void AddCategory(catPosition p);
void ListCategories(catPosition p);
void AddCredential(catPosition p);
void InsertCredential(catPosition p, credPosition new_credential, const char cat_name[CATEGORY_NAME_SIZE]);
void ListAllCredentials(catPosition p);
void FreeMemory(catPosition p);
credPosition SearchCredential(catPosition p);
void UpdateCredential(catPosition p);
void SortCatAlphabetically(catPosition p);
void ListCatCredential(catPosition category);
void SearchCategory(catPosition p);
void DeleteCredential(catPosition p);

void RandomPassword(catPosition p);
char* GeneratePassword(int length);
void PwdStrengthChecker(catPosition p);
int isValidPassword(const char *password);

char* VigenereCipher(const char* input, const char* key, int encrypt);
void PrintToFileEnc(catPosition p);
void ScanFromFile(catPosition p, const char* filename);

int main() {
    catPosition head = (catPosition)malloc(sizeof(Category));
    int choice = 0;

    head->cat_next = NULL;
    head->cred_next = NULL;

    printf("Welcome to...\n\n");

    printf("888                       888      8888888 88888888888 \n");
    printf("888                       888        888       888     \n");
    printf("888                       888        888       888     \n");
    printf("888      .d88b.   .d8888b 888  888   888       888     \n");
    printf("888     d88  88b d88P     888 .88P   888       888     \n");
    printf("888     888  888 888      888888K    888       888     \n");
    printf("888     Y88..88P Y88b.    888  88b   888       888     \n");
    printf("88888888  Y88P     Y8888P 888  888 8888888     888     \n\n");

    ScanFromFile(head, "pwds.txt");

    while(1) {
        printf("_____________________________________MENU___________________________________\n\n");
        printf(" (1) Add new credential\n (2) List categories\n (3) List all credentials\n (4) Search for credential\n (5) Update credential\n (6) Add new category\n (7) List category credentials\n (8) Sort categories\n (9) Delete credential\n (10) Generate random password\n (11) Check password strength\n (0) Quit\n");
        printf("_____________________________________________________________________________\n");
        printf("Enter your choice: ");
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
        else if(choice == 4) {
            SearchCredential(head->cat_next);
        }
        else if(choice == 5) {
            UpdateCredential(head->cat_next);
        }
        else if(choice == 6) {
            AddCategory(head);
        }
        else if(choice == 7) {
            SearchCategory(head->cat_next);
        }
        else if(choice == 8) {
            SortCatAlphabetically(head);
        }
        else if(choice == 9) {
            DeleteCredential(head);
        }
        else if(choice == 10) {
            RandomPassword(head);
        }
        else if(choice == 11) {
            PwdStrengthChecker(head);
        }
        else if(choice == 0) {
            printf("Thanks for using LockIT!\n");
            break;
        }
        else continue;
    }
    PrintToFileEnc(head->cat_next);
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

void AddCategory(catPosition p) {
    char temp_name[CATEGORY_NAME_SIZE];
    catPosition new_category = NULL;

    printf("Enter new category name: ");
    fgets(temp_name, CATEGORY_NAME_SIZE, stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;

    new_category = CreateCategory(temp_name);

    new_category->cat_next = p->cat_next;
    p->cat_next = new_category;
}

void ListCategories(catPosition p) {

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
    getchar();

    if(choice == 'y' || choice == 'Y') {
        ListCategories(p->cat_next);
        printf("Enter your category: ");
        fgets(temp_category, CATEGORY_NAME_SIZE, stdin);
        temp_category[strcspn(temp_category, "\n")] = 0;

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

        printf("--------------------------------\n");
        printf("Category: %s\n", p->name);
        printf("--------------------------------\n");

        if(temp == NULL) {
            printf("No credentials in this category...\n");
        }

        else {
            
            while(temp != NULL) {
                printf("--------------------------------\n");
                printf(" Description: %s\n Username: %s\n Password: %s\n", temp->description, temp->username, temp->password);
                printf("--------------------------------\n");
                temp = temp->next;
            }
        
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

credPosition SearchCredential(catPosition p) {
    char temp_desc[CRED_DESCRIPTION_SIZE];
    credPosition temp = NULL;

    printf("Enter description: ");
    fgets(temp_desc, CRED_DESCRIPTION_SIZE, stdin);
    temp_desc[strcspn(temp_desc, "\n")] = 0;

    while(p != NULL) {
        temp = p->cred_next;

        while(temp != NULL) {
            if(strcmp(temp->description, temp_desc) == 0) {
                printf("Credential found...\n");
                printf("--------------------------------\n");
                printf("Category: %s\n", p->name);
                printf("--------------------------------\n");
                printf(" Description: %s\n Username: %s\n Password: %s\n", temp->description, temp->username, temp->password);
                printf("--------------------------------\n");

                return temp;
            }
            temp = temp->next;
        }
        p = p->cat_next;
    }

    printf("Credential not found...\n");
    return NULL;
}

void UpdateCredential(catPosition p) {
    credPosition cred_update = NULL;

    cred_update = SearchCredential(p);

    if(cred_update == NULL) {
        printf("Credential could not be updated...\n");
    }
    else {
        printf("Enter your updated username: ");
        fgets(cred_update->username, CRED_USERNAME_SIZE, stdin);
        cred_update->username[strcspn(cred_update->username, "\n")] = 0;

        printf("Enter your updated password: ");
        fgets(cred_update->password, CRED_PASSWORD_SIZE, stdin);
        cred_update->password[strcspn(cred_update->password, "\n")] = 0;

        printf("Enter your updated description: ");
        fgets(cred_update->description, CRED_DESCRIPTION_SIZE, stdin);
        cred_update->description[strcspn(cred_update->description, "\n")] = 0; 
    }

}

void ListCatCredential(catPosition category) {

    if(category->cred_next == NULL) {
        printf("No credentials in this category...\n");
    }

    else {
        credPosition temp = category->cred_next;

        printf("--------------------------------\n");
        printf("Category: %s\n", category->name);
        while(temp != NULL) {
            printf("--------------------------------\n");
            printf(" Description: %s\n Username: %s\n Password: %s\n", temp->description, temp->username, temp->password);
            printf("--------------------------------\n");
            temp = temp->next;
    }
    }
    
}

void SearchCategory(catPosition p) {
    char temp_name[CATEGORY_NAME_SIZE];
    int found = 0;

    printf("Enter category: ");
    fgets(temp_name, CATEGORY_NAME_SIZE, stdin);
    temp_name[strcspn(temp_name, "\n")] = 0;

    while(p != NULL) {
        if(strcmp(p->name, temp_name) == 0) {
            found = 1;
            ListCatCredential(p);
        }
        p = p->cat_next;
    }

    if(found == 0) {
        printf("No category by that name...\n");
    }
}

void SortCatAlphabetically(catPosition p) {
    catPosition j, prev_j, temp, end;

    end = NULL;
    while(p->cat_next != end) {
        prev_j = p;
        j = p->cat_next;

        while(j->cat_next != end) {
            if(strcmp(j->name, j->cat_next->name)>0) {
                temp = j->cat_next;
                prev_j->cat_next = temp;
                j->cat_next = temp->cat_next;
                temp->cat_next = j;

                j = temp;
            }
            prev_j = j;
            j = j->cat_next;
        }
        end = j;
    }
}

void DeleteCredential(catPosition p) {
    char cred_desc[CRED_DESCRIPTION_SIZE];
    int found = 0;

    printf("Enter credential description you want to delete: ");
    fgets(cred_desc, CRED_DESCRIPTION_SIZE, stdin);
    cred_desc[strcspn(cred_desc, "\n")] = 0;

    credPosition temp = NULL;
    credPosition prev = NULL;

    while(p != NULL) {
        prev = NULL;
        temp = p->cred_next;
        while(temp != NULL) {
            if(strcmp(cred_desc, temp->description) == 0) {
                found = 1;
                break;
            }
            prev = temp;
            temp = temp->next;
        }
        
        if(found == 1) {
            if(prev == NULL) {
                p->cred_next = temp->next;
            }
            else {
                prev->next = temp->next;
            }
            free(temp);
            break;
        }

        p = p->cat_next;
    }
}

//random password and strength checker
void RandomPassword(catPosition p) {
    int char_num = 0;
    char choice;
    char* new_password = NULL;

    printf("Enter the number of characters you want: ");
    if (scanf("%d", &char_num) != 1 || char_num <= 0 || char_num > CRED_PASSWORD_SIZE) {
        printf("Invalid input or number out of range...\n");
        return;
    }

    while (getchar() != '\n');

    new_password = GeneratePassword(char_num);

    if (new_password != NULL) {
        printf("\nNew password: %s\n", new_password);

        printf("Do you want to save it in a new credential (Y/N)? ");
        scanf(" %c", &choice);
        getchar();

        if(choice == 'y' || choice == 'Y') {
            char new_username[CRED_USERNAME_SIZE];
            char new_description[CRED_DESCRIPTION_SIZE];  
            char temp_category[CATEGORY_NAME_SIZE];
            char general_cat[CATEGORY_NAME_SIZE] = "General";

            credPosition new_credential = NULL;
            new_credential = (credPosition)malloc(sizeof(Credential));
            if(new_credential == NULL) {
                printf("Memory allocation error...\n");
                return;
            }

            printf("Enter your credential username: ");
            fgets(new_username, CRED_USERNAME_SIZE, stdin);
            new_username[strcspn(new_username, "\n")] = 0;

            printf("Enter your credential description: ");
            fgets(new_description, CRED_DESCRIPTION_SIZE, stdin);
            new_description[strcspn(new_description, "\n")] = 0; 

            strcpy(new_credential->username, new_username);
            strcpy(new_credential->password, new_password);
            strcpy(new_credential->description, new_description);
            new_credential->next = NULL;

            printf("Do you want to store your new credential in a specific category (Y/N)? ");
            scanf(" %c", &choice);
            getchar();

            if(choice == 'y' || choice == 'Y') {
                ListCategories(p->cat_next);
                printf("Enter your category: ");
                fgets(temp_category, CATEGORY_NAME_SIZE, stdin);
                temp_category[strcspn(temp_category, "\n")] = 0;

                InsertCredential(p, new_credential, temp_category);
                
            }
            else {
                InsertCredential(p, new_credential, general_cat);
            }
            free(new_password);
        }
        
    }
}

char* GeneratePassword(int length) {
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$^&*()-_=+[]{}|;:,.<>?";
    int charSetLength = sizeof(characters) - 1;  // Exclude null terminator

    char* password = (char*)malloc((length + 1) * sizeof(char)); // +1 for null terminator
    if (password == NULL) {
        printf("Memory allocation error...\n");
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        password[i] = characters[rand() % charSetLength];
    }

    password[length] = '\0';

    return password;
}

void PwdStrengthChecker(catPosition p) {
    int choice = 0;

    printf("(1) Type a password to check\n");
    printf("(2) Search an existing password by description\n");
    printf("Enter choice: ");

    scanf(" %d", &choice);
    getchar();

    if(choice == 1) {
        char temp_pwd[CRED_PASSWORD_SIZE];

        printf("Enter password: ");
        fgets(temp_pwd, CRED_PASSWORD_SIZE, stdin);
        temp_pwd[strcspn(temp_pwd, "\n")] = 0;

        if(isValidPassword(temp_pwd) == 1) {
            printf("Password is strong...\n");
        }
        else {
            printf("Password is weak...\n");
        }
    }
    else if(choice == 2) {
        credPosition credential = SearchCredential(p);

        if(isValidPassword(credential->password) == 1) {
            printf("Password is strong...\n");
        }
        else {
            printf("Password is weak...\n");
        }
    }
    else {
        printf("Invalid choice...\n");
        return;
    }
}

int isValidPassword(const char *password) {
    int length = strlen(password);
    if (length < 8) {
        return 0;
    }

    // Flags
    int hasUpper = 0, hasLower = 0, hasDigit = 0, hasSpecial = 0;

    for (int i = 0; i < length; i++) {
        if (isupper(password[i])) {
            hasUpper = 1;
        } else if (islower(password[i])) {
            hasLower = 1;
        } else if (isdigit(password[i])) {
            hasDigit = 1;
        } else if (ispunct(password[i])) {
            hasSpecial = 1;
        }
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}

// Cipher
char* VigenereCipher(const char* input, const char* key, int encrypt) {
    int textLength = strlen(input);
    int keyLength = strlen(key);

    char* output = (char*)malloc(textLength + 1);
    if (output == NULL) {
        printf("Memory allocation error...\n");
        return NULL;
    }

    int i, j;
    for (i = 0, j = 0; i < textLength; i++) {
        unsigned char currentChar = input[i];
        unsigned char keyChar = key[j % keyLength];

        if (encrypt) {
            output[i] = ((currentChar + keyChar - 2 * 32) % 95) + 32;
        } else {
            output[i] = ((currentChar - keyChar + 95) % 95) + 32;
        }

        j++;
    }

    output[i] = '\0';
    return output;
}

void PrintToFileEnc(catPosition p) {
    FILE* fp = fopen("pwds.txt", "w");

    if (fp == NULL) {
        printf("Failed to open file...\n");
        return;
    }

    credPosition temp_cred = NULL;

    while (p != NULL) {
        temp_cred = p->cred_next;

        while (temp_cred != NULL) {
            
            char* user_encrypted = VigenereCipher(temp_cred->username, KEY, 1);
            char* pwd_encrypted = VigenereCipher(temp_cred->password, KEY, 1);
            char* desc_encrypted = VigenereCipher(temp_cred->description, KEY, 1);
            char* cat_encrypted = VigenereCipher(p->name, KEY, 1);


            if (user_encrypted && pwd_encrypted && desc_encrypted && cat_encrypted) {                
                fprintf(fp, "%s\t%s\t%s\t%s\n", cat_encrypted, user_encrypted, pwd_encrypted, desc_encrypted);
            }

            free(user_encrypted);
            free(pwd_encrypted);
            free(desc_encrypted);
            free(cat_encrypted);


            temp_cred = temp_cred->next;
        }
        p = p->cat_next;
    }

    fclose(fp);
}

void ScanFromFile(catPosition p, const char* filename) {
    char temp_line[MAX_BUFFER];
    char temp_cat[CATEGORY_NAME_SIZE];
    char temp_user[CRED_USERNAME_SIZE];
    char temp_pwd[CRED_PASSWORD_SIZE];
    char temp_desc[CRED_DESCRIPTION_SIZE];

    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file...\n");
        return;
    }

    while (fgets(temp_line, MAX_BUFFER, fp)) {

        if (sscanf(temp_line, "%19s\t%29s\t%19s\t%49s", temp_cat, temp_user, temp_pwd, temp_desc) == 4) {
            credPosition new_cred = (credPosition)malloc(sizeof(Credential));
            if (new_cred == NULL) {
                printf("Memory allocation error...\n");
                fclose(fp);
                return;
            }

            char* decrypt_cat = VigenereCipher(temp_cat, KEY, 0);
            char* decrypt_user = VigenereCipher(temp_user, KEY, 0);
            char* decrypt_pwd = VigenereCipher(temp_pwd, KEY, 0);
            char* decrypt_desc = VigenereCipher(temp_desc, KEY, 0);

            if (!decrypt_cat || !decrypt_user || !decrypt_pwd || !decrypt_desc) {
                printf("Decryption error...\n");
                free(new_cred);
                continue;
            }

            strncpy(new_cred->username, decrypt_user, CRED_USERNAME_SIZE);
            strncpy(new_cred->password, decrypt_pwd, CRED_PASSWORD_SIZE);
            strncpy(new_cred->description, decrypt_desc, CRED_DESCRIPTION_SIZE);
            new_cred->next = NULL;

            InsertCredential(p, new_cred, decrypt_cat);

            free(decrypt_cat);
            free(decrypt_user);
            free(decrypt_pwd);
            free(decrypt_desc);
        } else {
            printf("Error reading line: %s\n", temp_line);
        }
    }

    fclose(fp);
}
