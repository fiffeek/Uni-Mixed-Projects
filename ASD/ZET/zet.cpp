#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    map<int, int> ms;

    while (n--) {
        int aux;
        cin >> aux;

        ms[aux]++;
    }

    int z;
    cin >> z;

    while (z--) {
        int k;
        cin >> k;

        for (int j = 0; j < k; ++j) {
            pair<int, int> took = *ms.rbegin();
            int how_many = took.second > k - j ? k - j : took.second;
            took.second -= how_many;

            if (took.second == 0) {
                ms.erase(took.first);
            } else {
                ms[took.first] = took.second;
            }

            ms[took.first / 2] += how_many;
            ms[(took.first + 1) / 2] += how_many;
            j += how_many - 1;
        }

        cout << ms.size() << endl;
    }
}