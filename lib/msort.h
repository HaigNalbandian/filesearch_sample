#ifndef M_SORT_
#define M_SORT_

#include <vector>

using namespace std;

	//actual merge method
	template <class T, class Comparator>
	void merge(vector<T>& a, vector<T>& temp, int s1, int e1, int s2, int e2, Comparator comp){
		//synchronize temporary vector to most recent master vector
		temp = a;
		int r1 = s1, r2 = s2, w = s1;
		bool left_is_smaller;
		//while both indices are not at end of list...
		while (r1 <= e1 && r2 <= e2){
			//determine whether left or right is bigger
			left_is_smaller = comp(temp[r1], temp[r2]);
			
			//if left is smaller, add it to the master vector
			if (left_is_smaller){
				a[w] = temp[r1];
				r1++;
			}
			else{
				a[w] = temp[r2];
				r2++;
			}
			w++; //advance master index index
		}
		//if I've reached the ends of my lists, check which list still has more to give and add it to the end
		if (r1 > e1){
			for (int i=r2; i<=e2; i++){
				a[w] = temp[i];
				w++;
			}
		}
		else if(r2 > e2){
			for (int i=r1; i<=e1; i++){
				a[w] = temp[i];
				w++;
			}
		}
	}

	//Recursive merge sort function
	template <class T, class Comparator>
	void msort(vector<T>& arr, vector<T>& temp, int start, int end, Comparator comp){
		//base case
		if (start >= end){
			return;
		}
		
		//recursion
		int mid = (start+end)/2;
		msort(arr, temp, start, mid, comp); //sort first half
		msort(arr, temp, mid+1, end, comp); //sort second half
		
		//merge everything back together
		merge(arr, temp, start, mid, mid+1, end, comp);
	}

	//main merge sort method-- creates a temporary array to allow for merging later on and makes initial recursive call
	template <class T, class Comparator>
	void mergeSort (vector<T>& myArray, Comparator comp){
		vector<T> temp = myArray;
		msort(myArray, temp, 0, temp.size()-1, comp);
	}

#endif