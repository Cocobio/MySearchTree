/*
-11807, 66, 12720
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#define EXCEL_FILE "universities_followers.csv"

using namespace std;

struct TwitterAccount {
/*

    University: Universidad a la que sigue el usuario en la red social.
    User ID: Número identificador del usuario en Twitter. Este campo es único.
    User Name: Nombre de usuario en Twitter. Este campo es único.
    Number Tweets: Número de tweets publicados por el usuario.
    Friends Count: Número de cuentas que el usuario sigue en la red social.
    Followers Count: Número de seguidores que tiene el usuario en la red social.
    Created At: Fecha en la que el usuario creó su cuenta en la red social.

*/
string university;
unsigned long long user_id;
string user_name;
unsigned int tweets_n;
unsigned int friends_count;
unsigned int followers_count;
string created_at;
	
} typedef TwitterAccount;

// Create TwitterAccount
TwitterAccount TwitterAccount_FromString(string line) {
	TwitterAccount new_account;
	string tmp_s;
	long double tmp_d;

	istringstream csvStream(line);

	// university
	getline(csvStream,new_account.university ,',');

	// user id
	getline(csvStream,tmp_s,',');
	tmp_d = stold(tmp_s);
	new_account.user_id = tmp_d;

	// user name
	getline(csvStream,new_account.user_name,',');
		
	// Number tweets
	getline(csvStream,tmp_s,',');
	new_account.tweets_n = stoul(tmp_s);

	// Friends count
	getline(csvStream,tmp_s,',');
	new_account.friends_count = stoul(tmp_s);

	// Followers count
	getline(csvStream,tmp_s,',');
	new_account.followers_count = stoul(tmp_s);

	// Created at
	getline(csvStream,new_account.created_at);

	return new_account;
}

void print_TwitterAccount(TwitterAccount &account) {
	cout << account.university << endl << account.user_id << endl << account.user_name << endl << account.tweets_n << endl << account.friends_count << endl << account.followers_count << endl << account.created_at << endl;
}

vector<TwitterAccount> readFileToVector(string file_name) {
	ifstream file;
	string tmp;

	vector<TwitterAccount> twitter_accounts;

	file.open(file_name);
	getline(file, tmp);

	while(getline(file, tmp) && tmp.size()!=0)
		twitter_accounts.push_back(TwitterAccount_FromString(tmp));
	file.close();

	return twitter_accounts;
}


#include <map>
#include "MyUnbalancedMap.cpp"
#include "MyAVLMap.cpp"
#include <ctime>
#include <cstdlib>

#include <tuple>
#include <cmath>

tuple<float,float, unsigned> sd_avr_and_biggest(vector<unsigned> c) {
	unsigned biggest=0;
	float avr = 0;

	for(auto &i : c) {
		if (biggest<i) biggest = i;
		avr += i;
	}
	avr /= c.size();

	float sd = 0;

	for(auto &i : c)
		sd += (i-avr)*(i-avr);
	sd /= c.size();
	sd = sqrt(sd);

	return {sd, avr, biggest};
}

template <class T>
T random_key_under(T &max) {
	return rand()%max;
}

template <>
string random_key_under(string &max) {
	string k = "";

	for (int i=0, j; i<10; i++) {
		j = rand()%63;
		if (j<10) k += char('0'+j);
		else if (j<36) k += char('A'+j-10);
		else if (j==36) k += '_';
		else k += char('a'+j-37);
	}

	return k;
}

template <class T>
void testWithArbitraryKey(vector<TwitterAccount> data, T (*select_key)(TwitterAccount), int present_indexes, int not_present) {
	// Data test compared to balanced tree
	map<T, TwitterAccount> balanced_map;
	MyUnbalancedMap<T, TwitterAccount> my_unbalanced_map;
	MyAVLMap<T,TwitterAccount> my_avl_map;

	double map_t, mymap_t, myavlmap_t, hash_t, myhash_t, myhash_oa_t, myhash_oa_dh_t;
	clock_t t;

	cout << "Containers initialized" << endl;

	int l=0;
	unsigned long long intervalos[] = {1000, 5000, 10000, 15000 , 20000, data.size()+1};

	// Populate times!
	cout << "Tiempo de lectura de map:" << endl;
	t = clock();
	for (int i=0; i<data.size(); i++) {
		balanced_map.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}



	cout << "Tiempo de lectura de avl:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		my_avl_map.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}

	cout << "Tiempo de lectura de unbalancedmap:" << endl;
	t = clock();
	l=0;
	for (int i=0; i<data.size(); i++) {
		my_unbalanced_map.insert(make_pair(select_key(data[i]), data[i]));
		if(i==intervalos[l])
			cout << intervalos[l++] << " elementos: " << ((double)clock()-t)/CLOCKS_PER_SEC << " s" << endl;
	}





	cout << "Data inserted: " << my_unbalanced_map.size() << "\t" << my_avl_map.size() << "\t" << balanced_map.size() << endl;
	cout << endl << endl;
	///////////////////////////////////////////////////////////////////////////////////

	// Set random indexes for test
	vector<int> index(present_indexes);
	srand(time(0));
	for (auto &it :index)
		it = rand()%data.size();
	cout << "Indexes for data to be found CREATED!" << endl;

	// Set indexes not found
	srand(time(0));
	vector<T> index_not_present(not_present);
	T max = (*my_unbalanced_map.max()).first;
	for (auto &it : index_not_present) {
		T k = random_key_under(max);
		while (balanced_map.find(k) != balanced_map.end())
			k = random_key_under(max);
		it = k;
	}
	cout << "Keys not presented CREATED!" << endl << endl;

	cout.precision(5);


	// Test find function

	vector<int> intervals;
	int count;
	for (int i=1000; i<present_indexes+1; i+=1000)
		intervals.push_back(i);

	cout << "intervals created: " << intervals.size() << endl << endl;

	// MY UNBALANCED TREE
	cout << "testing unbalanced tree" << endl;
	t = clock();
	l=0;
	count = 0;
	for (auto &it : index) {
		if (my_unbalanced_map.find(select_key(data[it])) == my_unbalanced_map.end())
			cout << "NOT FOUND" << endl;
		if (++count == intervals[l]){
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	mymap_t = ((double)t)/CLOCKS_PER_SEC;


	// MY AVL TREE
	cout << "testing avl tree" << endl;
	t = clock();
	l=0;
	count = 0;
	for (auto &it : index) {
		if (my_avl_map.find(select_key(data[it])) == my_avl_map.end())
			cout << "NOT FOUND" << endl;
		if (++count == intervals[l]){
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myavlmap_t = ((double)t)/CLOCKS_PER_SEC;





	// MAP
	cout << "testing map" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index) {
		if (balanced_map.find(select_key(data[it])) == balanced_map.end())
			cout << "NOT FOUND" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	map_t = ((double)t)/CLOCKS_PER_SEC;




	cout << "Find " << index.size() << " elements that are contained." << endl;
	cout << "In map: " << map_t << " seconds" << endl;
	cout << "In MyUnbalancedMap: " << mymap_t << " seconds" << endl;
	cout << "In MyAVLMap: " << myavlmap_t << " seconds" << endl;
	cout << "In unordered_map: " << hash_t << " seconds" << endl;
	cout << "In MyHashTable_SC: " << myhash_t << " seconds" << endl;
	cout << "In MyHashTable_OA: " << myhash_oa_t << " seconds" << endl;
	cout << "In MyHashTable_OA DH: " << myhash_oa_dh_t << " seconds" << endl << endl;


	///////////////////////////////////////////////////////////////////////
	/// Not found indexes
	// MY UNBALANCED TREE
	cout << "testing unbalanced tree" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present){
		if (my_unbalanced_map.find(it) != my_unbalanced_map.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	mymap_t = ((double)t)/CLOCKS_PER_SEC;


	// MY AVL TREE
	cout << "testing avl tree" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present){
		if (my_avl_map.find(it) != my_avl_map.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	myavlmap_t = ((double)t)/CLOCKS_PER_SEC;



	// MAP
	cout << "testing map" << endl;
	t = clock();
	l=0, count=0;
	for (auto &it : index_not_present) {
		if (balanced_map.find(it) != balanced_map.end())
			cout << "FOUND!" << endl;
		if (++count == intervals[l]) {
			l++;
			cout << ((double)clock()-t)/CLOCKS_PER_SEC << ", ";
		}
	}
	cout << endl << endl;
	t = clock() - t;
	map_t = ((double)t)/CLOCKS_PER_SEC;



	cout << "Find " << index.size() << " elements that are not contained." << endl;
	cout << "In map: " << map_t << " seconds" << endl;
	cout << "In MyUnbalancedMap: " << mymap_t << " seconds" << endl;
	cout << "In MyAVLMap: " << myavlmap_t << " seconds" << endl;
	cout << "In unordered_map: " << hash_t << " seconds" << endl;
	cout << "In MyHashTable_SC: " << myhash_t << " seconds" << endl;
	cout << "In MyHashTable_OA: " << myhash_oa_t << " seconds" << endl;
	cout << "In MyHashTable_OA DH: " << myhash_oa_dh_t << " seconds" << endl << endl;

	// Check unbalancedmap data
	for(auto it : index)
		if (balanced_map[select_key(data[it])].user_id != my_unbalanced_map[select_key(data[it])].user_id || 
			balanced_map[select_key(data[it])].user_id != my_avl_map[select_key(data[it])].user_id) 
			cout << "ERROR!" << endl;


	cout << "MyUnbalancedMap dpth: " << my_unbalanced_map.depth() << endl << endl;
	cout << "MyAVLMap dpth: " << my_avl_map.depth() << endl << endl;

	cout << "Tamanio unbalanced tree:" << my_unbalanced_map.size_of() << endl;
	cout << "Tamanio avl tree:" << my_avl_map.size_of() << endl;
}

// KEY Accesser
unsigned long long id_user_key(TwitterAccount a) { return a.user_id; }
string user_name_key(TwitterAccount a) { return a.user_name; }

#include <list>
int main() {
	vector<TwitterAccount> data = readFileToVector(EXCEL_FILE);

	int inside_users_N = 400000;
	int not_inside_users_N = 400000;
	
	cout << "N of accounts: " << data.size() << endl;

	cout << endl << "================================================" << endl;
	cout << "USING USER ID: " << endl;
	testWithArbitraryKey(data, id_user_key, inside_users_N, not_inside_users_N);


	cout << endl << "================================================" << endl;
	cout << "USING USER NAME: " << endl;
	testWithArbitraryKey(data, user_name_key, inside_users_N, not_inside_users_N);

	// cout << sizeof(TwitterAccount) << endl;

	return 0;
}
