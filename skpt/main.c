#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>  // Include the cJSON header

#define RESULT_SUCCESS 0
#define RESULT_INVARGS 1
#define RESULT_NETERROR 2


// Define the structure to hold manifest data
typedef struct {
    char name[100];
    char version[20];
    char author[100];
} Manifest;

// Function to parse JSON manifest file into a struct
Manifest parse_manifest(const char *filename) {
    Manifest manifest = {"", "", ""};
    cJSON *root = NULL;
    char *data = NULL;

    // Read the JSON file
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return manifest;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    data = (char *)malloc(file_size + 1);
    if (!data) {
        fclose(file);
        fprintf(stderr, "Memory allocation failed.\n");
        return manifest;
    }

    fread(data, 1, file_size, file);
    fclose(file);
    data[file_size] = '\0';

    // Parse JSON data
    root = cJSON_Parse(data);
    if (!root) {
        fprintf(stderr, "JSON parse error: %s\n", cJSON_GetErrorPtr());
        free(data);
        return manifest;
    }

    // Extract data from JSON
    cJSON *name = cJSON_GetObjectItem(root, "name");
    cJSON *version = cJSON_GetObjectItem(root, "version");
    cJSON *author = cJSON_GetObjectItem(root, "author");

    if (name && version && author) {
        strncpy(manifest.name, name->valuestring, sizeof(manifest.name));
        strncpy(manifest.version, version->valuestring, sizeof(manifest.version));
        strncpy(manifest.author, author->valuestring, sizeof(manifest.author));
    } else {
        fprintf(stderr, "Missing or invalid JSON fields.\n");
    }

    // Clean up cJSON and data
    cJSON_Delete(root);
    free(data);

    return manifest;
}

void help()
{
    printf("skpt - The SipaaKernel extension manager\n");
    printf("Usage : skpt [options] command\n");
    printf("\n");
    printf("All commands :\n");
    printf("    help - Show this message\n");
    printf("    ver - Show version info\n");
}

void version()
{
    printf("skpt - The SipaaKernel extension manager\n");
    printf("Version 0.1\n");
    printf("\n");
    printf("This open-source tool is made with <3 by the SipaaKernel contributors\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        help();
        return RESULT_INVARGS;
    }

    if (strcmp(argv[1], "version") == 0)
    {
        version();
        return RESULT_SUCCESS;
    }
    else if (strcmp(argv[1], "help") == 0)
    {
        help();
        return RESULT_SUCCESS;
    }
    else if (strcmp(argv[1], "test") == 0)
    {
        if (argc < 3)
        {
            help();
            return RESULT_INVARGS;
        }
        if (strcmp(argv[2], "manifest") == 0)
        {
            if (argc < 4)
            {
                help();
                return RESULT_INVARGS;
            }
            Manifest m = parse_manifest(argv[3]);
            printf("%s (version %s, made by %s)\n", m.name, m.version, m.author);
            return RESULT_SUCCESS;
        }

        return RESULT_SUCCESS;
    }
    else
    {
        help();
        return RESULT_INVARGS;
    }
}