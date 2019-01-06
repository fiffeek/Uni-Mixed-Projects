#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);
    int n, k;
    cin >> n >> k;

    vector<int> nums;
    unsigned long long ans = 0;
    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        nums.push_back(aux);
    }

    int previous = 0;
    unordered_map<int, int> elemsSet;
    for (int s = 0; s < n; ++s) {
        ans += previous - s;

        for (previous; previous < n; ++previous) {
            elemsSet[nums[previous]]++;

            if (elemsSet.size() > k) {
                elemsSet[nums[previous]]--;
                if (elemsSet[nums[previous]] == 0) {
                    elemsSet.erase(nums[previous]);
                }

                break;
            }

            ans++;
        }

        elemsSet[nums[s]]--;
        if (elemsSet[nums[s]] == 0) {
            elemsSet.erase(nums[s]);
        }
    }

    cout << ans << endl;
}

