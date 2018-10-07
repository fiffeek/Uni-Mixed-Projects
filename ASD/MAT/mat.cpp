#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    string in;
    cin >> in;

    size_t n = in.length();
    size_t min_distance = n - 1;
    size_t actual_distance = 0;
    char last_active = in[0];

    for (auto aux: in) {
        if (last_active == '*' && aux == '*') {
            continue;
        }

        if (aux == '*') {
            actual_distance++;
        } else {
            if (last_active != '*' && last_active != aux) {
                min_distance = min(actual_distance, min_distance);
            }

            actual_distance = 0;
            last_active = aux;
        }
    }

    cout << n - min_distance << endl;
}
