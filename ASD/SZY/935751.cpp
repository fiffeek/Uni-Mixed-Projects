#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

constexpr int MAX_N = 500005;

int main() {
    std::ios_base::sync_with_stdio(false);
    unordered_map<int, int> exist;
    vector<int> last;
    int dp[MAX_N][2];
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        dp[i][0] = 0;
        dp[i][1] = 0;
    }

    vector<int> numbers;

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        numbers.emplace_back(aux);
    }

    int index = 0;
    for (const auto& number: numbers) {
        if (exist.count(number) > 0) {
            last.push_back(exist[number]);
        } else {
            last.push_back(index);
        }

        exist[number] = index;
        index++;
    }

    for (int i = 0; i < n; ++i) {
        if (i - 1 >= 0) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][1]);
        }

        if (last[i] != i) {
            dp[i][1] = 1 + dp[last[i]][0];
        }
    }

    cout << max(dp[n - 1][0], dp[n - 1][1]) << endl;
}