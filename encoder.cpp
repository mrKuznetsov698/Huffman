#include <bits/stdc++.h>
#include <cstdio>

using namespace std;

struct Node {
    int left{};
    int right{};
    uint8_t c = 0;
};

Node nodes[513];
int nodes_ind = 0;

int newNode(int left, int right, uint8_t c = 0) {
    nodes[++nodes_ind] = {.left = left, .right = right, .c = c};
    return nodes_ind;
}

array<int, 256> count(vector<uint8_t> &data) {
    array<int, 256> result{};
    for (uint8_t byte : data) {
        result[byte]++;
    }
    return result;
}

int build_huffman(vector<uint8_t> &data) {
    array<int, 256> dict = count(data);
    set<pair<int, int>> s;
    for (int i = 0; i < 256; ++i) {
        if (dict[i] != 0) s.insert({dict[i], newNode(0, 0, i)});
    }
    while (s.size() > 1) {
        auto [p1, l] = *s.begin(); s.erase(s.begin());
        auto [p2, r] = *s.begin(); s.erase(s.begin());
        s.insert({p1 + p2, newNode(l, r)});
    }
    return s.begin()->second;
}

map<uint8_t, string> get_table(int root) {
    map<uint8_t, string> res;
    queue<pair<int, string>> q;
    q.emplace(root, "");
    while (!q.empty()) {
        auto [v, s] = q.front();
        q.pop();
        if (nodes[v].left == 0 && nodes[v].right == 0) {
            res[nodes[v].c] = s;
            cout << (int)nodes[v].c << " -> " << s << '\n';
        }
        if (nodes[v].left != 0) {
            q.emplace(nodes[v].left, s + '0');
        }
        if (nodes[v].right != 0) {
            q.emplace(nodes[v].right, s + '1');
        }
    }
    return res;
}

string encode_to_binary(map<uint8_t, string> &dict, vector<uint8_t> &data) {
    string tmp;
    for (uint8_t c : data) {
        tmp += dict[c];
    }
    return tmp;
}

vector<uint8_t> encode(string tmp) {
    int n = tmp.size();
    if (n % 8 != 0) {
        for (int i = 0, x = 8 - (n % 8); i < x; ++i) tmp.push_back('0');
        n += 8 - (n % 8);
    }
    vector<uint8_t> res(n / 8);
    for (int i = 0; i < res.size(); ++i) {
        for (int j = 0; j < 8; ++j) {
            res[i] ^= ((tmp[8 * i + j] - '0') << (7 - j));
        }
    }
    return res;
}

vector<uint8_t> read_bytes(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Cannot open file: " + filename);
    }

    // Get file size
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    // Read all bytes
    vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw runtime_error("Failed to read file: " + filename);
    }

    return buffer;
}

void write_result_to_file(uint8_t data_size, int v, vector<uint8_t> &data, const string &path) {
    ofstream file(path, ios::binary);
    file.put(v >> 1); // root == node_ind, but root < 512 so we need 9 bits
    file.put((v << 7) | (data_size << 4)); // data_size < 8 so we need 3 bits
    for (int i = 1; i <= nodes_ind; ++i) {
        auto node = nodes[i];
        file.put(node.left >> 1);
        file.put(((node.left & 1) << 7) | (node.right >> 2));
        file.put((node.right & 0b11) << 6);
        file.put(node.c);
    }
    for (uint8_t chunk: data) {
        file.put(chunk);
    }
    file.close();
}

int main() {
    string path;
    cin >> path;

    vector<uint8_t> data = read_bytes(path);

    int v = build_huffman(data);
    auto dict = get_table(v);
    string bin_string = encode_to_binary(dict, data);
    auto bytes = encode(bin_string);

    write_result_to_file(8 - bin_string.size() % 8, v, bytes, path + ".compressed");

    return 0;
}
