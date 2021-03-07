/*
Solution for exercise 1.10 from Learning from Data
*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>
#include <string>
#include <cmath>
#include <math.h>
#include <algorithm>
#include <unordered_map>


using namespace std;

class Coin {
public:
    int id;
    vector<int> history;
    void nflip(int n) {
        for (int i = 0; i < n; ++i)
        {
            int res = rand() % 2;
            history.push_back(res);
        }
    }
    double compute_nu() {
        double t = 0;
        for (auto it = begin(history); it != end(history); ++it)
        {
            t += *it;
        }
        return t / history.size();
    }
};

void find_nu_min(vector<Coin> coins, int* min_idx, double* nu_min, Coin* coin_min, int prohibited_idx) {
    *nu_min = numeric_limits<double>::max();
    int i = 0;
    for (auto it = begin(coins); it != end(coins); ++it)
    {
        if (i != prohibited_idx)
        {
            double nu = (*it).compute_nu();
            if (nu < *nu_min)
            {
                *nu_min = nu;
                *min_idx = i;
                *coin_min = *it;
            }
        }
        i += 1;
    }
}

unordered_map<double, double> frequency_table(double size, vector<double> target) {
    unordered_map<double, double> table;
    // initialize keys with 0's
    for (double i = 0; i <= size; ++i)
    {
        double idx = i / size;
        table[idx] = 0;
    }
    for (auto it = begin(target); it != end(target); ++it)
    {
        table[*it] = table[*it] + 1;
    }
    return table;
}

void xplot_frequency_table(double size, unordered_map<double, double> table, string marker = "x", string title = "", int bucketSize = 1) {
    // size: histogram buckets will vary by 1/ size
    // table: frequency distribution
    // marker (optional): corresponds to 1*bucketSize occurences.
    // title (optional): printed on first line before histogram.
    if (title != "")
    {
        cout << title << endl;
    }
    for (double i = 0; i <= size; ++i)
    {
        double idx = i / size;
        // add decimal to 0 & 1.
        if (idx == 0 || idx == 1) { cout << idx << ".0" << ": "; }
        else { cout << idx << ": "; }

        for (int j = 0; j < floor(table[idx] / bucketSize); ++j) cout << marker;
        cout << endl;

    }
}

double hoeffding(double N, double epsilon) {
    return 2 * exp(-2 * pow(epsilon, 2) * N);
}

double vector_median(vector<double> vec) {
    sort(vec.begin(), vec.end());
    if (vec.size() % 2 == 0) {
        return (vec[(vec.size() - 1) / 2] + vec[vec.size() / 2]) / 2;
    }
    else {
        return vec[vec.size() / 2];
    }
}

double most_frequent_nu(unordered_map<double, double> table) {
    double max_idx = numeric_limits<double>::min() , max_nu = numeric_limits<double>::min();;
    for (auto &it : table) {
        double tmp_idx = it.first;
        double tmp_nu = it.second;
        if (tmp_nu > max_nu) {
            max_idx = tmp_idx;
            max_nu = tmp_nu;
        }      
    }
    vector<double> out = { max_idx , max_nu };
    return max_idx;
}

void find_nus_and_mu(vector<Coin> coins,
    vector<int>* sim_indicies,
    vector<double>* sim_nus,
    vector<Coin>* sim_coins,
    double* sim_mu) {
    const int max_iter = (int)1e3;

    // c1
    Coin coin_1 = coins.front(); double nu_1 = coin_1.compute_nu();
    // coins.erase(coins.begin());
    (*sim_coins).push_back(coin_1);
    (*sim_indicies).push_back(0);
    (*sim_nus).push_back(nu_1);

    // c_rand
    int rand_idx = int(rand() % (coins.size() - 1)) + 1;
    Coin coin_rand = coins[rand_idx];
    double nu_rand = coin_rand.compute_nu();

    (*sim_coins).push_back(coin_rand);
    (*sim_indicies).push_back(rand_idx);
    (*sim_nus).push_back(nu_rand);

    // c_min
    int min_idx; double nu_min; Coin coin_min;
    vector<Coin> coins_r = { coins.begin() + 1, coins.end() };
    find_nu_min(coins_r, &min_idx, &nu_min, &coin_min, rand_idx);


    (*sim_coins).push_back(coin_min);
    (*sim_indicies).push_back(min_idx);
    (*sim_nus).push_back(nu_min);

    // (a) what is mu for 3 selected coins?
    double num = nu_1 * coin_1.history.size() + nu_rand * coin_rand.history.size() + nu_min * coin_min.history.size();
    double denom = (double(coin_1.history.size()) + double(coin_rand.history.size()) + double(coin_min.history.size()));
    *sim_mu =  num/ denom;

}

int main() {

    // 1) run a simulation flipping 1000 fair coins
    // 2) flip each coin independently 10 times
    // 3) focus on 3 coins from this simulation c1, c2, c3:
    //      c1) 1st coin flipped
    //      c2) a coin chosen at random
    //      c3) c_min, coin with min frequency of heads
    //          pick earliest c3 in case of tie.
    // 4) let v_1, v_rand, and v_min be the fraction of heads
    srand((unsigned)time(0));
    int sim_a_length = (int)1e3; int sim_b_length = (int)1e3;
    int independent_flips = 10;
    bool part_a = false; // set true to compute answer to part a

    if (part_a)
    {
        vector<Coin> coins;

        for (int i = 0; i < sim_a_length; ++i) {
            auto c = Coin();
            c.id = i;
            c.nflip(independent_flips);
            coins.push_back(c);
        }

        // c1
        Coin coin_1 = coins.front(); double nu_1 = coin_1.compute_nu();
        coins.erase(coins.begin());

        // c_rand
        int rand_idx = int(rand() % coins.size()); Coin coin_rand = coins[rand_idx];
        double nu_rand = coin_rand.compute_nu();

        // c_min
        int min_idx; double nu_min; Coin coin_min;
        find_nu_min(coins, &min_idx, &nu_min, &coin_min, rand_idx);

        // (a) what is mu for 3 selected coins?
        double mu = nu_1 * coin_1.history.size() + nu_rand * coin_rand.history.size() + nu_min * coin_min.history.size() / (coin_1.history.size() + coin_rand.history.size() + coin_min.history.size());

        // output nu
        cout << "nu_1: " << nu_1 << endl;
        cout << "nu_rand: " << nu_rand << endl;
        cout << "nu_min: " << nu_min << endl;
        // output mu
        cout << "mu_(3 coins): " << mu << endl;
    }

    // (b)
    bool log = false;
    vector<int> sim_one_indicies; vector<int> sim_rand_indicies; vector<int> sim_min_indicies;
    vector<double> sim_one_nus; vector<double> sim_rand_nus; vector<double> sim_min_nus;
    vector<double> sim_mus;
    for (int i = 0; i < sim_b_length; ++i)
    {
        vector<Coin> coins = {};

        for (int i = 0; i < sim_a_length; ++i) {
            auto c = Coin();
            c.id = i;
            c.nflip(independent_flips);
            coins.push_back(c);
        }
        vector<int> sim_indicies; vector<double> sim_nus; vector<Coin> sim_coins; double sim_mu;
        find_nus_and_mu(coins, &sim_indicies, &sim_nus, &sim_coins, &sim_mu);
        sim_one_nus.push_back(sim_nus[0]); sim_rand_nus.push_back(sim_nus[1]);
        sim_min_nus.push_back(sim_nus[2]); sim_mus.push_back(sim_mu);

        if (log)
        {
            cout << "Experiment " << i << ": " << endl;

            cout << "nu_1: " << sim_nus[0] << endl;
            cout << "nu_rand: " << sim_nus[1] << endl;
            cout << "nu_min: " << sim_nus[2] << endl;
            // output mu
            cout << "mu_(3 coins): " << sim_mu << endl;
        }

    }
    int bucketSize = 10;
    unordered_map<double, double> sim_one_map = frequency_table(10, sim_one_nus);
    xplot_frequency_table(10, sim_one_map, "X", "First coin nus", bucketSize);
    unordered_map<double, double> sim_rand_map = frequency_table(10, sim_rand_nus);
    xplot_frequency_table(10, sim_rand_map, "X", "Random coin nus", bucketSize);
    unordered_map<double, double> sim_min_map = frequency_table(10, sim_min_nus);
    xplot_frequency_table(10, sim_min_map, "X", "Minimum coin nus", 30);

    // (c) no graphing library, so we will look at a table
    cout << "\nepsilon vs. Hoeffding\n" << endl;
    double epsilon0 = 0.01;
    for (int i = 0; i < 10; ++i)
    {
        cout << epsilon0 << ": " << hoeffding(independent_flips, epsilon0) << endl;
        epsilon0 = epsilon0 * 2;
        
    }

    // (d) which coins obey hoeffding?
    // from the previous table it looks like an epsilon of around 0.3 seems reasonable
    // d1) first coin, c1: 
    cout << "\nWhich coins obey hoeffding?" << endl;
    double epsilond = 0.35;
    double mf_one_nu = most_frequent_nu(sim_one_map);
    double mf_rand_nu = most_frequent_nu(sim_rand_map);
    double mf_min_nu = most_frequent_nu(sim_min_map);
    cout << "most frequent first nu: " << mf_one_nu << endl;
    cout << "median mu: " << vector_median(sim_mus) << endl;
    cout << "hoeffding(10, " << epsilond << ") " << hoeffding(10, epsilond) << endl;
    cout << "P|mu - v_first| = " << abs(vector_median(sim_mus) - mf_one_nu) << " epsilon: " << epsilond <<  endl;
    cout << "P|mu - v_rand| = " << abs(vector_median(sim_mus) - mf_rand_nu) << " epsilon: " << epsilond << endl;
    cout << "P|mu - v_min| = " << abs(vector_median(sim_mus) - mf_min_nu) << " epsilon: " << epsilond << endl;

}
