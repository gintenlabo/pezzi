#include <jubatus/client/burst_client.hpp>

#include <cmath>
#include <cassert>
#include <random>
#include <iostream>

using jubatus::burst::client::burst;
using jubatus::burst::st_document;

const std::string host = "localhost";
constexpr int port = 9199;
const std::string name = "";
constexpr int timeout_sec = 10;

struct keyword_manager {
  std::string keyword;
  double first_burst_time;
  double burst_length;
  double burst_interval;
  double nonburst_frequency;
  double burst_frequency;

  bool is_bursted_at(double pos) const {
    double pos1 = pos - first_burst_time;

    if (0 <= pos1) {
      pos1 -= std::floor(pos1 / burst_interval) * burst_interval;
      assert(0 <= pos1 && pos1 < burst_interval);
      if (pos1 <= burst_length) {
        return true;
      }
    }
    return false;
  }

  template<class RandGen>
  void append_keyword_randomly_to(std::string& doc, double pos, RandGen& gen) const {
    double frequency =
        this->is_bursted_at(pos) ? burst_frequency : nonburst_frequency;
    std::bernoulli_distribution dist(frequency);
    if (dist(gen)) {
      doc += keyword;
      doc += ' ';
    }
  }
};

template<class RandomGen>
auto make_random_values(RandomGen&& gen, std::size_t n)
    -> std::vector<typename RandomGen::result_type> {
  std::vector<typename RandomGen::result_type> result;
  result.reserve(n);
  for (std::size_t i = 0; i < n; ++i) {
    result.push_back(gen());
  }
  return result;
}

std::mt19937& get_random_generator() {
  static std::mt19937 gen = [] () -> std::mt19937 {
    auto seeds = make_random_values(std::random_device(), 16);
    std::seed_seq sseq(seeds.begin(), seeds.end());
    return std::mt19937(sseq);
  }();
  return gen;
}

int main() {
  keyword_manager const keywords[] = {
    {"aaa", 10, 3, 10, 0.001, 0.1},
    {"bbb", 15, 4, 10, 0.001, 0.1},
  };

  burst client(host, port, name, timeout_sec);

  for (auto&& x : keywords) {
    jubatus::burst::st_keyword kw = {x.keyword, 2, 1};
    client.add_keyword(kw);
  }

  auto& gen = get_random_generator();

  double span = 1;

  std::uniform_real_distribution<> dist(0, span);
  auto generate_pos = [&] (double pos0)  { return pos0 + dist(gen); };

  for (double pos0 = 0; pos0 < 100; pos0 += span) {
    std::vector<st_document> documents;
    documents.reserve(100);
    for (size_t i = 0; i < 100; ++i) {
      double pos = generate_pos(pos0);
      std::string doc = "";
      for (auto&& x : keywords) {
        x.append_keyword_randomly_to(doc, pos, gen);
      }
      documents.emplace_back(pos, doc);
    }
    client.add_documents(documents);

    auto results = client.get_all_bursted_results_at(pos0);
    if (!results.empty()) {
      std::cout << "burst detected: " << pos0;
      for (auto&& result : results) {
        std::cout << " " << result.first;
      }
      std::cout << std::endl;
    }
  }
}
