#include <iostream>
#include <map>

using namespace std;

int main() {
    int n;
    cin >> n;
    map<int, int> numbers;
    string a;

    while (n--) {
      int aux;
      cin >> aux;

      numbers[aux]++;
    }

    int ans = 0;
    int actual = 0;
    int last_item = -1;

    for (const auto &numb: numbers) {
        if (last_item == -1) {
            last_item = numb.first;
        }

        if (abs(last_item - numb.first) > 1) {
            ans = max(ans, actual);
            actual = numb.second;
        } else {
            actual += numb.second;
        }

        last_item = numb.first;
    }

    ans = max(ans, actual);
    cout << ans << endl;
}