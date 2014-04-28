#ifndef ADVERTISER_H_
#define ADVERTISER_H_

#include <vector>
#include <string>

struct Ad{
	Ad(string k, double b) : keyword(k), bid(b) {}
	string keyword;
	double bid;
};

class Advertiser{
	public:
		Advertiser();
		Advertiser(string n) : name(n){}
		~Advertiser();

		//accessor methods
		const string getName(){
			return name;
		}

		const vector<Ad>& getAds(){
			return ads;
		}

		void add_ad(string keyword, double bid){
			Ad a(keyword, bid);
			ads.push_back(a);
		}

	private:
		string name;
		vector<Ad> ads;
};

#endif
