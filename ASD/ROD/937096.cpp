#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main() {
    std::ios_base::sync_with_stdio(false);

    unordered_set<int> elems;
    unordered_set<int> ans;
    int n, d;
    cin >> n >> d;

    while (n--) {
        int action, number;
        cin >> action >> number;

        if (action == 1) {
            elems.insert(number);

            if (elems.count(number + d) > 0 || elems.count(number - d) > 0) {
                ans.insert(number);
            }

            int starter = number - d;
            while (elems.count(starter) > 0 && ans.count(starter) == 0) {
                ans.insert(starter);
                starter -= d;
            }

            starter = number + d;
            while (elems.count(starter) > 0 && ans.count(starter) == 0) {
                ans.insert(starter);
                starter += d;
            }

        } else {
            int starter = number - d;
            while (elems.count(starter) > 0) {
                if (elems.count(starter - d) > 0) break;
                ans.erase(starter);
                starter -= d;
            }

            starter = number + d;
            while (elems.count(starter) > 0) {
                if (elems.count(starter + d) > 0) break;
                ans.erase(starter);
                starter += d;
            }

            elems.erase(number);
            ans.erase(number);
        }

        if (ans.empty()) {
            cout << "NIE" << endl;
        } else {
            cout << "TAK" << endl;
        }
    }
}