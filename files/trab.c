#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100
#define MAX_GENRE_LENGTH 50
#define MAX_MAGAZINE_LENGTH 50
#define MAX_PUBLISHER_LENGTH 50
#define MAX_VOLUMES 100

typedef struct {
    char isbn[13];
    char title[MAX_TITLE_LENGTH];
    char authors[MAX_AUTHOR_LENGTH];
    int start_year;
    int end_year;
    char genre[MAX_GENRE_LENGTH];
    char magazine[MAX_MAGAZINE_LENGTH];
    char publisher[MAX_PUBLISHER_LENGTH];
    int edition_year;
    int volume_count;
    int acquired_volume_count;
    int acquired_volumes[MAX_VOLUMES];
} Manga;

void addManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile);
void listMangas(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile);
void updateManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile);
void deleteManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile);
long searchByISBN(FILE *primaryIndexFile, const char *isbn);
long searchByTitle(FILE *secondaryIndexFile, const char *title);

int main() {
    FILE *dataFile = fopen("mangas.dat", "r+b");
    FILE *primaryIndexFile = fopen("primary_index.dat", "r+b");
    FILE *secondaryIndexFile = fopen("secondary_index.dat", "r+b");

    if (!dataFile || !primaryIndexFile || !secondaryIndexFile) {
        dataFile = fopen("mangas.dat", "w+b");
        primaryIndexFile = fopen("primary_index.dat", "w+b");
        secondaryIndexFile = fopen("secondary_index.dat", "w+b");
        if (!dataFile || !primaryIndexFile || !secondaryIndexFile) {
            perror("Failed to open files");
            return 1;
        }
    }

    int choice;
    do {
        printf("\nManga Management System\n");
        printf("1. Add Manga\n");
        printf("2. List Mangas\n");
        printf("3. Update Manga\n");
        printf("4. Delete Manga\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addManga(dataFile, primaryIndexFile, secondaryIndexFile);
                break;
            case 2:
                listMangas(dataFile, primaryIndexFile, secondaryIndexFile);
                break;
            case 3:
                updateManga(dataFile, primaryIndexFile, secondaryIndexFile);
                break;
            case 4:
                deleteManga(dataFile, primaryIndexFile, secondaryIndexFile);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice, please try again.\n");
        }
    } while (choice != 5);

    fclose(dataFile);
    fclose(primaryIndexFile);
    fclose(secondaryIndexFile);

    return 0;
}

void addManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile) {
    Manga manga;
    printf("Enter ISBN: ");
    scanf("%s", manga.isbn);
    getchar(); // consume newline

    printf("Enter Title: ");
    fgets(manga.title, MAX_TITLE_LENGTH, stdin);
    manga.title[strcspn(manga.title, "\n")] = '\0'; // remove newline

    printf("Enter Authors: ");
    fgets(manga.authors, MAX_AUTHOR_LENGTH, stdin);
    manga.authors[strcspn(manga.authors, "\n")] = '\0'; // remove newline

    printf("Enter Start Year: ");
    scanf("%d", &manga.start_year);

    printf("Enter End Year: ");
    scanf("%d", &manga.end_year);

    printf("Enter Genre: ");
    scanf("%s", manga.genre);
    getchar(); // consume newline

    printf("Enter Magazine: ");
    fgets(manga.magazine, MAX_MAGAZINE_LENGTH, stdin);
    manga.magazine[strcspn(manga.magazine, "\n")] = '\0'; // remove newline

    printf("Enter Publisher: ");
    fgets(manga.publisher, MAX_PUBLISHER_LENGTH, stdin);
    manga.publisher[strcspn(manga.publisher, "\n")] = '\0'; // remove newline

    printf("Enter Edition Year: ");
    scanf("%d", &manga.edition_year);

    printf("Enter Volume Count: ");
    scanf("%d", &manga.volume_count);

    printf("Enter Acquired Volume Count: ");
    scanf("%d", &manga.acquired_volume_count);

    for (int i = 0; i < manga.acquired_volume_count; i++) {
        printf("Enter Acquired Volume %d: ", i + 1);
        scanf("%d", &manga.acquired_volumes[i]);
    }

    fseek(dataFile, 0, SEEK_END);
    long position = ftell(dataFile);
    fwrite(&manga, sizeof(Manga), 1, dataFile);

    fprintf(primaryIndexFile, "%s %ld\n", manga.isbn, position);
    fprintf(secondaryIndexFile, "%s %ld\n", manga.title, position);
}

void listMangas(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile) {
    rewind(dataFile);
    Manga manga;

    while (fread(&manga, sizeof(Manga), 1, dataFile)) {
        printf("\nISBN: %s\n", manga.isbn);
        printf("Title: %s\n", manga.title);
        printf("Authors: %s\n", manga.authors);
        printf("Start Year: %d\n", manga.start_year);
        printf("End Year: %d\n", manga.end_year);
        printf("Genre: %s\n", manga.genre);
        printf("Magazine: %s\n", manga.magazine);
        printf("Publisher: %s\n", manga.publisher);
        printf("Edition Year: %d\n", manga.edition_year);
        printf("Volume Count: %d\n", manga.volume_count);
        printf("Acquired Volume Count: %d\n", manga.acquired_volume_count);
        printf("Acquired Volumes: ");
        for (int i = 0; i < manga.acquired_volume_count; i++) {
            printf("%d ", manga.acquired_volumes[i]);
        }
        printf("\n");
    }
}

void updateManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile) {
    char isbn[13];
    printf("Enter ISBN of the manga to update: ");
    scanf("%s", isbn);

    long position = searchByISBN(primaryIndexFile, isbn);
    if (position == -1) {
        printf("Manga with ISBN %s not found.\n", isbn);
        return;
    }

    fseek(dataFile, position, SEEK_SET);
    Manga manga;
    fread(&manga, sizeof(Manga), 1, dataFile);

    printf("Enter new Title (current: %s): ", manga.title);
    getchar(); // consume newline
    fgets(manga.title, MAX_TITLE_LENGTH, stdin);
    manga.title[strcspn(manga.title, "\n")] = '\0'; // remove newline

    printf("Enter new Authors (current: %s): ", manga.authors);
    fgets(manga.authors, MAX_AUTHOR_LENGTH, stdin);
    manga.authors[strcspn(manga.authors, "\n")] = '\0'; // remove newline

    printf("Enter new Start Year (current: %d): ", manga.start_year);
    scanf("%d", &manga.start_year);

    printf("Enter new End Year (current: %d): ", manga.end_year);
    scanf("%d", &manga.end_year);

    printf("Enter new Genre (current: %s): ", manga.genre);
    scanf("%s", manga.genre);

    printf("Enter new Magazine (current: %s): ", manga.magazine);
    getchar(); // consume newline
    fgets(manga.magazine, MAX_MAGAZINE_LENGTH, stdin);
    manga.magazine[strcspn(manga.magazine, "\n")] = '\0'; // remove newline

    printf("Enter new Publisher (current: %s): ", manga.publisher);
    fgets(manga.publisher, MAX_PUBLISHER_LENGTH, stdin);
    manga.publisher[strcspn(manga.publisher, "\n")] = '\0'; // remove newline

    printf("Enter new Edition Year (current: %d): ", manga.edition_year);
    scanf("%d", &manga.edition_year);

    printf("Enter new Volume Count (current: %d): ", manga.volume_count);
    scanf("%d", &manga.volume_count);

    printf("Enter new Acquired Volume Count (current: %d): ", manga.acquired_volume_count);
    scanf("%d", &manga.acquired_volume_count);

    for (int i = 0; i < manga.acquired_volume_count; i++) {
        printf("Enter Acquired Volume %d (current: %d): ", i + 1, manga.acquired_volumes[i]);
        scanf("%d", &manga.acquired_volumes[i]);
    }

    fseek(dataFile, position, SEEK_SET);
    fwrite(&manga, sizeof(Manga), 1, dataFile);
    printf("Manga with ISBN %s updated successfully.\n", isbn);
}

void deleteManga(FILE *dataFile, FILE *primaryIndexFile, FILE *secondaryIndexFile) {
    char isbn[13];
    printf("Enter ISBN of the manga to delete: ");
    scanf("%s", isbn);

    long position = searchByISBN(primaryIndexFile, isbn);
    if (position == -1) {
        printf("Manga with ISBN %s not found.\n", isbn);
        return;
    }

    fseek(dataFile, position, SEEK_SET);
    Manga manga;
    fread(&manga, sizeof(Manga), 1, dataFile);

    printf("Are you sure you want to delete the following manga?\n");
    printf("ISBN: %s\n", manga.isbn);
    printf("Title: %s\n", manga.title);
    printf("Authors: %s\n", manga.authors);
    printf("Enter 'yes' to confirm deletion: ");

    char confirm[10];
    scanf("%s", confirm);
    if (strcmp(confirm, "yes") == 0) {
        // Remove from primary index
        fseek(primaryIndexFile, 0, SEEK_SET);
        FILE *tempFile = fopen("temp_primary_index.dat", "w+b");

        char tempISBN[13];
        long tempPosition;
        while (fscanf(primaryIndexFile, "%s %ld\n", tempISBN, &tempPosition) != EOF) {
            if (strcmp(tempISBN, isbn) != 0) {
                fprintf(tempFile, "%s %ld\n", tempISBN, tempPosition);
            }
        }
        fclose(tempFile);
        fclose(primaryIndexFile);
        remove("primary_index.dat");
        rename("temp_primary_index.dat", "primary_index.dat");
        primaryIndexFile = fopen("primary_index.dat", "r+b");

        // Remove from secondary index
        fseek(secondaryIndexFile, 0, SEEK_SET);
        tempFile = fopen("temp_secondary_index.dat", "w+b");

        char tempTitle[MAX_TITLE_LENGTH];
        while (fscanf(secondaryIndexFile, "%s %ld\n", tempTitle, &tempPosition) != EOF) {
            if (strcmp(tempTitle, manga.title) != 0) {
                fprintf(tempFile, "%s %ld\n", tempTitle, tempPosition);
            }
        }
        fclose(tempFile);
        fclose(secondaryIndexFile);
        remove("secondary_index.dat");
        rename("temp_secondary_index.dat", "secondary_index.dat");
        secondaryIndexFile = fopen("secondary_index.dat", "r+b");

        // Remove from data file
        fseek(dataFile, position, SEEK_SET);
        char deleted = '*'; // Marking the record as deleted
        fwrite(&deleted, sizeof(char), 1, dataFile);
        printf("Manga with ISBN %s deleted successfully.\n", isbn);
    } else {
        printf("Deletion aborted.\n");
    }
}

long searchByISBN(FILE *primaryIndexFile, const char *isbn) {
    rewind(primaryIndexFile);
    char tempISBN[13];
    long position;

    while (fscanf(primaryIndexFile, "%s %ld\n", tempISBN, &position) != EOF) {
        if (strcmp(tempISBN, isbn) == 0) {
            return position;
        }
    }

    return -1; // Not found
}

long searchByTitle(FILE *secondaryIndexFile, const char *title) {
    rewind(secondaryIndexFile);
    char tempTitle[MAX_TITLE_LENGTH];
    long position;

    while (fscanf(secondaryIndexFile, "%s %ld\n", tempTitle, &position) != EOF) {
        if (strcmp(tempTitle, title) == 0) {
            return position;
        }
    }

    return -1; // Not found
}
