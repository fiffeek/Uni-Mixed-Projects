#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

class Tree {
private:
    vector<vector<int>> tree;
    vector<bool> visited;

public:
    Tree(int n) {
      tree.resize((unsigned long long) (n + 1));
      visited.resize((unsigned long long) (n + 1));
      fill_n(visited.begin(), n + 1, false);
    }

    void addNode(int node, int child) {
        tree[node].push_back(child);
        tree[child].push_back(node);
    }

    vector<int> children(int node) {
        return tree[node];
    }

    void clearVisited() {
        fill_n(visited.begin(), visited.size(), false);
    }

    bool isVisited(int n) {
        return visited[n];
    }

    void visit(int n) {
        assert(visited[n] == false);
        visited[n] = true;
    }
} tree(500000);

class Offline {
private:
    vector<vector<int>> queries;
    vector<vector<int>> whichQuery;
    vector<int> answers;

public:
    Offline(int n, int m) {
        queries.resize((unsigned long long) (n + 1));
        whichQuery.resize((unsigned long long) (n + 1));
        answers.resize((unsigned long long) m);
        fill_n(answers.begin(), m, -1);
    }

    void addQuery(int n, int k, int queryNum) {
        queries[n].push_back(k);
        whichQuery[n].push_back(queryNum);
    }

    void fillQuery(int n, int k, int answer) {
        answers[whichQuery[n][k]] = answer;
    }

    vector<int> takeQueries(int n) {
        return queries[n];
    }

    int returnAnswer(int n) {
        return answers[n];
    }

} queries{500000, 500000};

struct Answer {
    int depth, node;
};

Answer dfs(int current, int depth) {
    if (tree.isVisited(current)) {
        return {-1, -1};
    }

    tree.visit(current);
    Answer local{depth, current};
    auto treeChildren = tree.children(current);

    for (int i = 0; i < treeChildren.size(); ++i) {
        Answer child = dfs(treeChildren[i], depth + 1);

        if (child.depth > local.depth) {
            local = child;
        }
    }

    return local;
}

void dfsWithVector(vector<int> &path, int current) {
    if (tree.isVisited(current)) {
        return;
    }

    tree.visit(current);
    const auto queriesCurrent = queries.takeQueries(current);
    path.push_back(current);

    for (int i = 0; i < queriesCurrent.size(); ++i) {
        if (queriesCurrent[i] < path.size()) {
            queries.fillQuery(current, i, path[path.size() - queriesCurrent[i] - 1]);
        }
    }

    for (const int &child: tree.children(current)) {
        dfsWithVector(path, child);
    }

    path.pop_back();
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int n;
    cin >> n;

    for (int i = 1; i <= n; ++i) {
        int left, right;
        cin >> left >> right;

        if (left != -1) {
            tree.addNode(i, left);
        }

        if (right != -1) {
            tree.addNode(i, right);
        }
    }

    int m;
    cin >> m;

    for (int i = 0; i < m; ++i) {
        int left, right;
        cin >> left >> right;

        queries.addQuery(left, right, i);
    }

    int diameterLeft = dfs(1, 0).node;
    tree.clearVisited();
    int diameterRight = dfs(diameterLeft, 0).node;
    tree.clearVisited();
    vector<int> path;
    dfsWithVector(path, diameterLeft);
    tree.clearVisited();
    dfsWithVector(path, diameterRight);

    for (int i = 0; i < m; ++i) {
        cout << queries.returnAnswer(i) << endl;
    }
}