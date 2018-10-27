#include <iostream>
#include <vector>

using namespace std;

const long long MOD = 1000000000;

struct Tree {
    unsigned long s = 1;
    vector<long long> tree;

    Tree(unsigned long n) {
        while (s < n) {
            s *= 2;
        }
        tree.resize(2 * s, 0);
    }

    void update(unsigned long index, long long sum) {
        index += s;

        while (index) {
            tree[index] += sum;
            tree[index] %= MOD;
            index /= 2;
        }
    }

    long long sum(unsigned long s_index, unsigned long e_index) {
        if (s_index > e_index) return 0;

        s_index += s, e_index += s;
        long long res = 0;

        res += tree[s_index];
        if (e_index != s_index) res += tree[e_index];
        res %= MOD;

        while (s_index / 2 < e_index / 2) {
            if (!(s_index & 1)) res += tree[s_index + 1];
            res %= MOD;
            if (e_index & 1) res += tree[e_index - 1];
            res %= MOD;

            s_index /= 2;
            e_index /= 2;
        }

        return res;
    }
};

int main() {
   unsigned long n, k;
   cin >> n >> k;
   vector<unsigned long> vi;
   vector<Tree> trees;

   for (unsigned long i = 0; i < k; ++i) {
       trees.push_back(Tree(n));
   }

   for (unsigned long i = 0; i < n; ++i) {
       unsigned long aux;
       cin >> aux;

       vi.push_back(aux);
   }

   for (unsigned long x = 0; x < n; ++x) {
       for (long long j = (long) k - 1; j >= 0; j--) {
           if (j == 0) {
               trees[j].update(vi[x] - 1, 1);
               continue;
           }

           long long adder = trees[j - 1].sum(vi[x], n - 1);
           trees[j].update(vi[x] - 1, adder);
       }
   }

   cout << (trees[k - 1].sum(0, n - 1) % MOD) << endl;
   return 0;
}