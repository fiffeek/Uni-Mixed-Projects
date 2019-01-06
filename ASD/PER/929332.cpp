#include <iostream>
#include <vector>

using namespace std;
using ull = unsigned long long;
const int MAX_N = 100005;

ull max(ull a, ull b, ull c) {
    return std::max(std::max(a, b), c);
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;
    vector<ull> numbers;
    ull dp[MAX_N][2];
    ull dp_rev[MAX_N][2];

    for (int i = 0; i < MAX_N; ++i) {
        for (int j = 0; j < 2; ++j) {
            dp[i][j] = 0;
            dp_rev[i][j] = 0;
        }
    }

    std::string t;

    for (int i = 0; i < n; ++i) {
        ull k;
        cin >> k;

        numbers.emplace_back(k);
    }

    if (n < 4) {
        int sum = 0;

        for (int i = 0; i < n; ++i) {
            sum += numbers[i];
        }

        cout << sum << endl;
    } else {
        dp[0][0] = 0;
        dp[0][1] = numbers[0];
        dp[1][0] = numbers[0];
        dp[1][1] = numbers[0] + numbers[1];
        dp[2][0] = dp[1][1];
        dp[2][1] = max(numbers[0] + numbers[1], numbers[1] + numbers[2], numbers[0] + numbers[2]);

        for (int i = 3; i < n; ++i) {
            dp[i][0] = std::max(dp[i - 1][0], dp[i - 1][1]);
            dp[i][1] = max(dp[i - 3][0] + numbers[i - 2] + numbers[i],
                           dp[i - 2][0] +  numbers[i - 1] + numbers[i],
                           dp[i - 2][1] + numbers[i]);
        }

        dp_rev[n - 1][0] = 0;
        dp_rev[n - 1][1] = numbers[n - 1];
        dp_rev[n - 2][0] = numbers[n - 1];
        dp_rev[n - 2][1] = numbers[n - 1] + numbers[n - 2];
        dp_rev[n - 3][0] = dp_rev[n - 2][1];
        dp_rev[n - 3][1] = max(numbers[n - 2] + numbers[n - 1], numbers[n - 2] + numbers[n - 3],
                               numbers[n - 1] + numbers[n - 3]);

        for (int i = n - 4; i >= 0; --i) {
            dp_rev[i][0] = std::max(dp_rev[i + 1][0], dp_rev[i + 1][1]);
            dp_rev[i][1] = max(dp_rev[i + 3][0] + numbers[i + 2] + numbers[i],
                               dp_rev[i + 2][0] + numbers[i + 1] + numbers[i],
                               dp_rev[i + 2][1] + numbers[i]);
        }

        ull max_sum = 0;

        for (int i = 0; i <= n - 3; ++i) {
            ull prev = i - 1 >= 0 ? dp[i - 1][0] : 0;
            ull last = i + 3 < n ? dp_rev[i + 3][0] : 0;

            ull this_sum = prev + numbers[i] + numbers[i + 1] + numbers[i + 2] + last;
            max_sum = std::max(max_sum, this_sum);
        }

        for (int i = 0; i < n; ++i) {
            ull prev_take = i - 1 >= 0 ? std::max(dp[i - 1][1], dp[i - 1][0]) : 0;
            ull next_take = i + 1 >= 0 ? std::max(dp_rev[i + 1][1], dp_rev[i + 1][0]) : 0;

            ull this_sum = next_take + prev_take;
            max_sum = std::max(max_sum, this_sum);
        }

        cout << max_sum << endl;
    }
}
