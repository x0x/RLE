#include <bits/stdc++.h>
#include <divsufsort.h>
#include <iostream>
#include <memory>

class Encoder {

public:
  Encoder(std::string data) : m_data(data) {}

  // private:
  std::string m_data;

  void Encode() {
    std::string suffixString = getBWST();
    std::vector<std::string> verticalBytes = readVerticalBytes(suffixString);
    std::vector<int> finalEncoded = runLengthEncode(verticalBytes);
  }

  std::string getBWST() {
    int len = m_data.length();
    int *SA = new int[len * sizeof(int)];
    const char *Text = m_data.c_str();
    divsufsort((unsigned char *)Text, SA, len);

    std::string result = "";
    // output
    for (int i = 0; i < len; ++i) {
      // printf("SA[%2d] = %2d: ", i, SA[i]);
      result += m_data[(SA[i] + len - 1) % len];
      // for (int j = SA[i]; j < len; ++j) {
      //   printf("%c", Text[j]);
      // }
      // printf("$\n");
    }
    return result;
  }

  std::map<char, int> dynamicByteRemapping(const std::string &data) {

    std::map<char, int> mp;
    for (auto el : data) {
      mp[el]++;
    }
    std::vector<std::pair<int, char>> result;
    for (auto el : mp) {
      result.push_back({el.second, el.first});
    }
    sort(result.rbegin(), result.rend(),
         [](std::pair<int, char> &a, std::pair<int, char> &b) {
           if (a.first != b.first)
             return a.first < b.first;
           else
             return a.second > b.second;
         });

    std::map<char, int> order;
    int cur = 0;
    for (auto el : result) {
      order[el.second] = cur++;
    }

    // LOG AT dynamic byte remapping
    // for (auto el : order) {
    //   std::cout << el.first << " " << el.second << "\n";
    // }
    return order;
  }

  std::vector<std::string> readVerticalBytes(const std::string &data) {

    std::map<char, int> remapping = dynamicByteRemapping(data);
    const int numOfBits = 8;
    std::vector<std::string> result(numOfBits);
    int len = data.length();
    for (int i = 0; i < numOfBits; i++) {
      for (int j = 0; j < len; j++) {
        int cur = remapping[data[j]];
        result[i] += ((cur >> i) & 1) + '0';
      }

      std::cout << i << " " << result[i] << "\n";
    }

    return result;
  }

  std::vector<int> runLengthEncode(std::vector<std::string> &data) {

    int idx[2] = {0, 0};
    int cur = 0;
    std::vector<int> result;
    for (int i = 7; i >= 0; i--) {
      for (int j = 0; j < data[i].size(); j++) {
        if (data[i][j] - '0' == cur) {
          idx[cur]++;
        } else {
          result.push_back(idx[cur]);
          cur ^= 1;
          idx[cur] = 1;
        }
      }
    }

    result.push_back(idx[cur]);
    for (auto el : result) {
      std::cout << el << " ";
    }
    return result;
  }
};

void file_io() {
  freopen("debug.txt", "w", stderr);
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
}

int main() {
  file_io();
  std::string text;
  std::cin >> text;
  std::cout << text << "\n";
  Encoder *e = new Encoder(text);
  e->Encode();
}
