/* you may assume a maximum word length of 512 characters*/
#define MAX_LENGTH 512

/* insert your function prototypes here */

int frequency(std::string target);

int edit_distance(std::string a, std::string b);

bool spell_correct(const std::string word, char* fixed);
