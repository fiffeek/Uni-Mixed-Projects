#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);

    int n, l;
    cin >> n >> l;

    vector<unordered_map<int, int>> maps;
    for (int i = 0; i < l; ++i) {
        unordered_map<int, int> v;
        maps.push_back(v);
    }

    int pads[n][l];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < l; ++j) {
            int aux;
            cin >> aux;

            pads[i][j] = aux;
        }
    }

    int previous_max = 1;
    for (int j = 0; j < l; ++j) {
        for (int i = 0; i < n; ++i) {
            if (j != 0) {
                maps[j][pads[i][j]] = max(maps[j - 1][pads[i][j]] + 1, 1);
                previous_max = max(previous_max, maps[j][pads[i][j]]);
            } else {
                maps[j][pads[i][j]] = 1;
            }
        }
    }

    cout << previous_max << endl;
}