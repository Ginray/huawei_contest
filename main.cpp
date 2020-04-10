#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <map>
#include <ctime>
#include <stack>
#include <vector>

#define DEBUG true

using namespace std;


const int maxn = 560000 + 5;

int visit[maxn];
int visit_whole[maxn];

int u[maxn], v[maxn];
int first[maxn], nxt[maxn];
int banned[maxn];
int rudu[maxn];

int data_num = 0;
int point_num = 0;

vector<vector<int> > result;
map<int, int> point_map;
map<int, int> point_map_reverse;

int result_num = 0;

string input_path = "../data/test_data_sp.txt";
char out_path[50] = "../projects/student/result_sp.txt";


void readTxt(string file) {

    ifstream infile;
    infile.open(file.data(), ios::in);

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

        rudu[to] += 1;
        u[i] = from;
        v[i] = to;
        nxt[i] = first[u[i]];
        first[u[i]] = i;
        i++;
    }
    infile.close();
}


void TopologicalSort() {
    stack<int> s;
    for (int i = 1; i <= point_num; i++) {
        if (rudu[i] == 0)
            s.push(i);
    }

    while (!s.empty()) {
        int now = s.top();
        s.pop();
        banned[now] = 1;
        int k = first[now];
        while (k != 0) {
            if (--rudu[v[k]] == 0)
                s.push(v[k]);
            k = nxt[k];
        }
    }

}

void
dfs(int now, vector<int> point_data, int search_number, int search_flag, int search_deep, int sp_num, int pre_num) {
    if (visit_whole[now] >= 1 && visit_whole[now] < search_number)
        return;

    if (search_deep <= 0)
        return;
    int now_search_deep = search_deep;
    int now_search_flag = search_flag;
    int now_sp_num = sp_num;
    if (search_flag) { //搜索之前已经搜索过的点
        now_search_deep--;
    } else {
        if (visit_whole[now] == search_number) {//在当前连通分量中且已经访问过，那么只需要访问下面6层就行了。
            now_search_flag = 1;
            now_search_deep = 7;
            now_sp_num = pre_num;  //之后找到的环中必须包含pre_num，不然找到的环是重复的
        }
    }

    visit_whole[now] = search_number;


    if (banned[now])
        return;
    if (visit[now]) {
        stack<int> temp;

        int temp_len = 0;
        int finded = 0;
        int if_find_sp_num = 0;

        for (int i = point_data.size() - 2; i >= 0; i--) {
            if (point_data[i] == sp_num)
                if_find_sp_num = 1;
            temp.push(point_map_reverse[point_data[i]]);
            temp_len++;
            if (point_data[i] == now) {
                finded = 1;
                break;
            }
            if (temp_len > 6)
                break;
        }

        if (finded && temp_len >= 3) {
            vector<int> temp_v;
            while (!temp.empty()) {
                temp_v.push_back(temp.top());
                temp.pop();
            }
            if (search_flag == 1) {
                if (if_find_sp_num == 1) {
                    result.push_back(temp_v);
                    result_num++;
                }
            } else {
                result.push_back(temp_v);
                result_num++;
            }
            temp_v.clear();
        }
        return;
    }
    visit[now] = 1;

    int k = first[now];
    while (k != 0) {
//        printf("%d %d\n",u[k],v[k]);

        point_data.push_back(v[k]);
        dfs(v[k], point_data, search_number, now_search_flag, now_search_deep, now_sp_num, now);
        point_data.pop_back();
        k = nxt[k];
    }
    visit[now] = 0;

}

void adjust_result() {
    vector<int> temp;
    for (int i = 0; i < result.size(); i++) {
        temp.clear();
        int min = result[i][0];
        int pos = 0;
        for (int j = 1; j < result[i].size(); j++) {
            if (result[i][j] < min) {
                min = result[i][j];
                pos = j;
            }
        }
        if (pos != 0) {
            for (int k = pos; k < result[i].size(); k++)
                temp.push_back(result[i][k]);
            for (int k = 0; k < pos; k++)
                temp.push_back(result[i][k]);
            result[i] = temp;
        }
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

    FILE *fp = fopen(out_path, "w");

    if (DEBUG)
        printf("%d\n", result_num);
    fprintf(fp, "%d\n", result_num);

    for (int i = 0; i < result_num; i++) {
        for (int j = 0; j < result[i].size() - 1; j++) {
            if (DEBUG)
                printf("%d,", result[i][j]);
            fprintf(fp, "%d,", result[i][j]);
        }
        if (DEBUG)
            printf("%d\n", result[i][result[i].size() - 1]);
        fprintf(fp, "%d\n", result[i][result[i].size() - 1]);
    }

    fclose(fp);

}

int main() {

    clock_t startTime, endTime;
    if (DEBUG) {
        startTime = clock();
    }

    readTxt(input_path);
    if (DEBUG) {
        printf("point_num = %d\n", point_num);
    }
    TopologicalSort();
    for (int i = 1; i <= point_num; i++) {
        if (banned[i] == 0 && visit_whole[i] == 0) {
            printf("%d\n", i);
            vector<int> temp;
            temp.push_back(i);
            dfs(i, temp, i, 0, 99999999, -1, -1);
        }
    }

    adjust_result();
    sort_result();
    print_result();

    if (DEBUG) {
        endTime = clock();
        cout << "The run time is: " << (double) (endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    }
    return 0;
}