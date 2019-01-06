#include <iostream>

using namespace std;

#define MAX_N 800005

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;
    int ans = 0;
    int binary[MAX_N];
    fill_n(binary, MAX_N, 0);

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        binary[aux]++;

        if (binary[aux] == 2) {
            binary[aux] = 0;
            ans--;

            for (int j = aux + 1; j < MAX_N; ++j) {
                if (binary[j] == 0) {
                    ans++;
                    binary[j]++;
                    break;
                } else {
                    binary[j] = 0;
                    ans--;
                }
            }
        } else {
            ans++;
        }

        cout << ans << endl;
    }
}