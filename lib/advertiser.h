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

		vector<Ad>& getAds(){
			return ads;
		}

		void add_ad(string keyword, double bid){
			Ad a(keyword, bid);
			ads.push_back(a);
		}
	
		double getHighestBid(){
			return highest_bid;	
		}
		
		void setHighestBid(double new_highest){
			highest_bid = new_highest;
		}

	private:
		string name;
		vector<Ad> ads;
		double highest_bid;
};

#endif
