#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <map>

using namespace std;

const int maxn = 560000 + 5;

int u[maxn], v[maxn];
int first[maxn], nxt[maxn];

int data_num = 0;
int point_num = 0;

vector<vector<int> > result;
map<int, int> point_map;
map<int, int> point_map_reverse;

int result_num = 0;

string input_path = "../data/test_data.txt";
char out_path[50] = "../projects/student/result.txt";


void readTxt(string file) {

    ifstream infile;
    infile.open(file.data());

    string s;
    int i = 1;
    while (getline(infile, s)) {
        data_num += 1;
        int from, to, money;
        sscanf(s.data(), "%d,%d,%d", &from, &to, &money);

        auto map_it = point_map.find(from);
        if (map_it == point_map.end()) {
            point_num += 1;
            point_map[from] = point_num;
            point_map_reverse[point_num] = from;
            from = point_num;
        } else {
            from = point_map[from];
        }

        map_it = point_map.find(to);
        if (map_it == point_map.end()) {
            point_num += 1;
            point_map[to] = point_num;
            point_map_reverse[point_num] = to;
            to = point_num;
        } else {
            to = point_map[to];
        }

        u[i] = from;
        v[i] = to;
        nxt[i] = first[u[i]];
        first[u[i]] = i;
        i++;
    }
    infile.close();
}

void dfs(int now, int st_point, vector<int> point_data, int point_len) {
    if (point_len > 8)
        return;
    if (point_map_reverse[st_point] > point_map_reverse[now])
        return;
    if (st_point == now && point_len >= 4) {
        vector<int> temp;
        for (int i = 0; i < point_data.size(); i++) {
            temp.push_back(point_map_reverse[point_data[i]]);
        }
        result.push_back(temp);
        result_num++;
        return;
    }
    if (point_len > 1) {
        auto find_loop = find(point_data.begin(), point_data.end() - 1, now);
        if (find_loop != point_data.end() - 1) { // 存在两个循环了，终止
            return;
        }
    }

    int k = first[now];
    while (k != 0) {
//        printf("%d %d\n",u[k],v[k]);
        point_data.push_back(v[k]);
        dfs(v[k], st_point, point_data, point_len + 1);
        point_data.pop_back();
        k = nxt[k];
    }

}

int cmp(vector<int> a, vector<int> b) {
    if (a.size() != b.size()) {
        return a.size() < b.size();
    }
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i])
            return a[i] < b[i];
    }
}

void sort_result() {
    sort(result.begin(), result.end(), cmp);
}

void print_result() {

    FILE *fp = fopen(out_path, "w+");

    printf("%d\n", result_num);
    fprintf(fp, "%d\n", result_num);

    for (int i = 0; i < result_num; i++) {
        for (int j = 0; j < result[i].size() - 2; j++) {
            printf("%d,", result[i][j]);
            fprintf(fp, "%d,", result[i][j]);
        }
        printf("%d\n", result[i][result[i].size() - 2]);
        fprintf(fp, "%d\n", result[i][result[i].size() - 2]);
    }

    fclose(fp);

}

int main() {

    readTxt(input_path);
    for (int i = 1; i <= point_num; i++) {
        vector<int> temp;
        temp.push_back(i);
        dfs(i, i, temp, 1);
    }

    sort_result();
    print_result();
    return 0;
}