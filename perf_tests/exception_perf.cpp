#include <stdexcept>
#include <benchmark/benchmark.h>

template<typename R, R r>
R ex_fun() {  throw r; }

template<typename R, R r>
R ret_fun() {  return r; }

using namespace std;

string s_ret_fun() { return "this is a return val"; }
string s_throw_fun() { throw string("this is a return val"); }

template <typename R, R FN()>
static void ex_handler_fun(benchmark::State& state)
{
  R retval;

  for (auto _ : state)
  {
    try { benchmark::DoNotOptimize(FN()); }
    catch (const R& val) {}
  }
}

template <typename R, R FN()>
static void ret_handler_fun(benchmark::State& state)
{
  R retval;

  for (auto _ : state)
  {
    benchmark::DoNotOptimize(retval = FN());
  }
}

struct result
{
  int val;
  string msg;
};

auto int_ret_fun = ret_handler_fun<int, ret_fun<int, 1>>;
BENCHMARK(int_ret_fun);

auto int_ex_fun = ex_handler_fun<int, ex_fun<int, 1>>;
BENCHMARK(int_ex_fun);

auto int_no_ex_fun = ex_handler_fun<int, ret_fun<int, 1>>;
BENCHMARK(int_no_ex_fun);

auto str_ret_fun = ret_handler_fun<string, s_ret_fun>;
BENCHMARK(str_ret_fun);

auto str_no_ex_fun = ex_handler_fun<string, s_ret_fun>;
BENCHMARK(str_no_ex_fun);

auto str_ex_fun = ex_handler_fun<string, s_throw_fun>;
BENCHMARK(str_ex_fun);

