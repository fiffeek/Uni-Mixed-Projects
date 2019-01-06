#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> elems;
    unsigned long long left[n][3], right[n][3];

    for (int i = 0; i < n; ++i) {
        fill_n(left[i], 3, 0);
        fill_n(right[i], 3, 0);
    }

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        elems.push_back(aux);
    }

    unsigned long long answer = 0;
    answer += n;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (i != 0) {
                left[i][j] += left[i - 1][j];
            }
        }

        left[i][elems[i] - 1]++;
    }

    for (int i = n - 1; i >= 0; --i) {
        for (int j = 0; j < 3; ++j) {
            if (i != n - 1) {
                right[i][j] += right[i + 1][j];
            }
        }

        right[i][elems[i] - 1]++;
    }

    for (int i = 0; i < n; ++i) {
        if (elems[i] == 2) {
            answer += left[i][0];
            answer += left[i][0] * right[i][2];
        } else if (elems[i] == 3) {
            answer += left[i][0] + left[i][1];
        }
    }

    cout << answer << endl;
}