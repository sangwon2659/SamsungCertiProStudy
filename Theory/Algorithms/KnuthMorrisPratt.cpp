#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Function to compute the longest prefix-suffix (LPS) array
// lps[i] = length of the longest proper prefix which is also a suffix in pattern[0..i]
vector<int> computeLPS(const string &pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);
    int len = 0; // length of the previous longest prefix suffix
    int i = 1;

    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // fallback to the previous possible prefix
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

// KMP pattern searching function
void KMPSearch(const string &text, const string &pattern) {
    int n = text.size();
    int m = pattern.size();

    vector<int> lps = computeLPS(pattern);

    int i = 0; // index for text
    int j = 0; // index for pattern

    while (i < n) {
        if (text[i] == pattern[j]) {
            i++;
            j++;
        }

        if (j == m) {
            cout << "Pattern found at index " << i - j << endl;
            j = lps[j - 1]; // continue to search for next match
        } else if (i < n && text[i] != pattern[j]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }
}

// Example usage
int main() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";

    cout << "Text: " << text << endl;
    cout << "Pattern: " << pattern << endl << endl;

    KMPSearch(text, pattern);

    return 0;
}
