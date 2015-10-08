##CSCI 104 Team Project Repository

###Student Information
**Developer 1**:
  + Name: Eshed Margalit
  + USC Email: emargali@usc.edu

**Developer 2**:
  + Name: Haig Nalbandian
  + USC Email: hnalband@usc.edu


Details of our Project:

Compile using the ' make' command. C++11 is needed for our implementation.

Run using ./team_project_123 [index file] [advertiser input] [advertiser output]

Part 1:

We store the advertisers in a hash table where the name of the advertiser is the key and a pointer to the advertiser's information (of advertiser class) is the value. In this class we stored bids for words and their owed total.

Every time a search is made, the table is iterated over and if the advertiser contains an ad for the search, it is added to the display. If it is clicked, the ad's bid is added to the Advertiser's total.

The map is then iterated over and printed out upon exit.

Part 2:

Feature 1: PageRank accounts for time spent per page

	When the program is initially run, PageRank works as it did in the previous implementation. However, we are now keeping track of how long the user spends on a specific page, and the total amount of time spent on all pages (a static variable). 
	We initially considered simply adding the time fraction to the PageRank. However, this would greatly skew results. If the lowest ranked page was opened, the user spent a total of 2 seconds, and opened no other pages, this result would be the top ranked subsequently. While it would likely normalize over time, it is skewed. So, we decided to weigh the time fraction per page by the PageRank itself.
	Thus, our PageRank updater is (PageRank) + (PageRank) * (Time_On_Page/Time_On_All_Pages).
	The most a PageRank can improve is being doubled.

Feature 2: Visited links are greyed out

	Once a link is visited, either through the main window or incoming/outgoing links, it will remain greyed out for the remainder of the program run.
	Subsequent searches are not necessary to see the effect of this feautre--it happens in real time.

Feature 3: Suggestions based on search history

	Unique and case-insensitive searches are stored in QStringList which is then iterated over in real time as the user types in a potential query. If what is currently typed in matches a previous search, that item(s) is shown in a Completer box below the LineEdit.

