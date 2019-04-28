#include <stdio.h>  // All needed libraries.
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1000;  // Define the buffer size.

// ************************************ Function Declarations *********************************************************
int getLength(const char *string);
char* fixString(char* input);
int compareEqualOrSimilar(const char *str1, const char *str2);
int compareEqual(const char* str1, const char* str2);
// ************************************ Main Function *****************************************************************
int main(int argc, char* argv[]) {
    int size = SIZE;  // Get the buffer size from the macro,
    char buff1[size];  // Create the buffers to read from the files.
    char buff2[size];
    char copy1[size];  // Used to copy the buffers for removing the spaces and newlines.
    char copy2[size];
    int k = 0;  // Buffer initialization index.
    for (k = 0; k < size; k ++) {  // Initialize the buffers and copy char arrays.
        buff1[k] = '\0';
        buff2[k] = '\0';
        copy1[k] = '\0';
        copy2[k] = '\0';
    }
    int fdin1; // Create the input file descriptors.
    int fdin2;
    char* path1 = argv[1];  // Move the file paths from the command line to local variables.
    char* path2 = argv[2];
    fdin1 = open(path1, O_RDONLY);  // Open the first file for reading.
    if (fdin1 < 0) {   // Handle opening error.
        perror("Failed to open first file\n");  // Print out a message and exit.
        exit(EXIT_FAILURE);
    }
    fdin2 = open(path2, O_RDONLY);  // Open second file.
    if (fdin2 < 0) {  // Handle opening error.
        perror("Failed to open second file\n");  // Print out a message and exit.
        exit(EXIT_FAILURE);
    }
    int i = 0;  // Index for first buffer reading.
    while (read(fdin1, &buff1[i], 1) == 1) {  // Read first file into buffer.
        i ++;
    }
    int j = 0;  // Index for second buffer reading.
    while (read(fdin2, &buff2[j], 1) == 1) {  // Read second file into buffer.
        j ++;
    }
    int result = 1;  // Will be the result value.
    int length1 = getLength(buff1);  // Get the lengths of both buffers.
    int length2 = getLength(buff2);
    snprintf(copy1, 1000, "%s", buff1);  // Copy the buffers for further use.
    snprintf(copy2, 1000, "%s", buff2);
    char* fixed1 = fixString(copy1);  // Keep the fixed strings after removing the spaces and newlines.
    char* fixed2 = fixString(copy2);
    if (getLength(fixed1) != getLength(fixed2)) { // If the fixed lengths are different they are obviously different
        result = 2;
    }
    else {
        if (compareEqual(fixed1,fixed2) == 1) {  // If they have no different characters we suspect they are similar.
            result = compareEqualOrSimilar(buff1, buff2);  // Otherwise compare to see if similar or equal.
        }
        else {
            result = 2;  // Otherwise they are different.
        }
    }
    if (close(fdin1) < 0) {  // Close first file and handle error.
        perror("Failed to close first file.\n");  // Print out a message and exit.
        exit(EXIT_FAILURE);
    }
    if (close(fdin2) < 0) {  // Close second file and handle error.
        perror("Failed to close second file.\n");  // Print out a message and exit.
        exit(EXIT_FAILURE);
    }
    return result;  // Return the final result.
}
// ****************************************** General Functions *******************************************************
// Get the length of the string.
int getLength(const char *string) {
    int i = 0;
    int count = 0;
    while(string[i] != '\0') {  // Count the characters until null terminator is reached.
        count++;
        i++;
    }
    return count;
}
// Remove spaces and newlines.
char* fixString(char* input) {
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i < getLength(input); i++,j++) {  // Don't insert the spaces and newlines into the output.
        if (input[i]  != ' ' && input[i] != '\n')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}
// Return 3 if strings are mismatched or 1 if not once the loop is exited.
int compareEqualOrSimilar(const char *str1, const char *str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] + 32 == str2[i] || str1[i] - 32 == str2[i]) {  // If they are uppercased or lowercased.
            return 3;
        }
        if (str1[i] == ' ' && str2[i] != ' ') {  // If one has a space and the other does not.
            return 3;
        }
        if (str1[i] != ' ' && str2[i] == ' ') {
            return 3;
        }
        if (str1[i] == '\n' && str2[i] != '\n') {  // If one has a newline and the other does not.
            return 3;
        }
        if (str1[i] != '\n' && str2[i] == '\n') {
            return 3;
        }
        if (str1[i + 1] == '\0' && str2[i + 1] != '\0') {  // If one has ended and the other has not.
            return 3;
        }
        if (str1[i + 1] != '\0' && str2[i + 1] == '\0') {
            return 3;
        }
        i++;
    }
    return 1;  // If no mismatches occurred, return 1 as the strings are equal.
}
// Compare strings to see if they are exactly equal.
int compareEqual(const char* str1, const char* str2) {
    int i = 0;
    while(str1[i] != '\0' && str2[i] != '\0') {  // Iterate the characters until null terminator is reached.
        if (str1[i] != str2[i]) {
            if((str1[i] + 32) != str2[i] && (str1[i] - 32) != str2[i]) {  // If not different by lower or upper case
                return 2;  // The strings are different and we return 2.
            }
        }
        i++;
    }
    return 1;
}
