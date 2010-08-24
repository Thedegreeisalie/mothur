/*
 *  unweighted.cpp
 *  Mothur
 *
 *  Created by Sarah Westcott on 2/9/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

#include "unweighted.h"

/**************************************************************************************************/

EstOutput Unweighted::getValues(Tree* t) {
	try {
		globaldata = GlobalData::getInstance();
		
		vector<string> groups;
		double UniqueBL;  //a branch length is unique if it's chidren are from the same group
		double totalBL;	//all branch lengths
		double UW;		//Unweighted Value = UniqueBL / totalBL;
		map<string, int>::iterator it;  //iterator to traverse pgroups
		map<string, int> copyIpcount;

	
		//if the users enters no groups then give them the score of all groups
		int numGroups = globaldata->Groups.size();
		
		//calculate number of comparsions
		int numComp = 0;
		for (int r=0; r<numGroups; r++) { 
			for (int l = r+1; l < numGroups; l++) {
				numComp++;
			}
		}

		//numComp+1 for AB, AC, BC, ABC
		data.resize(numComp+1,0);
		
		int count = 0;
		for (int a=0; a<numGroups; a++) { 
			for (int l = a+1; l < numGroups; l++) {
				UniqueBL=0.0000;  //a branch length is unique if it's chidren are from the same group
				totalBL = 0.00;	//all branch lengths
				UW = 0.00;		//Unweighted Value = UniqueBL / totalBL;
				copyIpcount.clear();
				
				//groups in this combo
				groups.push_back(globaldata->Groups[a]); groups.push_back(globaldata->Groups[l]);
		
				for(int i=0;i<t->getNumNodes();i++){
					if (m->control_pressed) {  return data; }
	
					copyIpcount = t->tree[i].pcount;
					for (it = copyIpcount.begin(); it != copyIpcount.end();) {
						if (m->inUsersGroups(it->first, groups) != true) {	
							copyIpcount.erase(it++);	
						}else { it++;  }
					}
			
					//if i's children are from the same group then i's pcount size will be 1 
					//if copyIpcount.size() = 0 they are from a branch that is entirely from a group the user doesn't want
					if (copyIpcount.size() == 0) { }
					else if ((t->tree[i].getBranchLength() != -1) && (copyIpcount.size() == 1)) {  UniqueBL += abs(t->tree[i].getBranchLength());	}
			
					//add i's BL to total if it is from the groups the user wants
					if ((t->tree[i].getBranchLength() != -1) && (copyIpcount.size() != 0)) {  
						totalBL += abs(t->tree[i].getBranchLength()); 
					}
			
				}
		
				UW = (UniqueBL / totalBL);  
		//cout << globaldata->Groups[a] << globaldata->Groups[l] << '\t' << UniqueBL << '\t' << totalBL << endl;
	
				if (isnan(UW) || isinf(UW)) { UW = 0; }
	
				data[count] = UW;
				count++;
				groups.clear();
			}
		}
		
		
		if (numComp != 1) {
			if (numGroups == 0) {
				//get score for all users groups
				for (int i = 0; i < tmap->namesOfGroups.size(); i++) {
					if (tmap->namesOfGroups[i] != "xxx") {
						groups.push_back(tmap->namesOfGroups[i]);
					}
				}
			}else {
				for (int i = 0; i < globaldata->Groups.size(); i++) {
					groups.push_back(globaldata->Groups[i]);
				}
			}
		
			UniqueBL=0.0000;  //a branch length is unique if it's chidren are from the same group
			totalBL = 0.00;	//all branch lengths
			UW = 0.00;		//Unweighted Value = UniqueBL / totalBL;
			copyIpcount.clear();
				
			for(int i=0;i<t->getNumNodes();i++){
			
				if (m->control_pressed) {  return data; }
				
				copyIpcount = t->tree[i].pcount;
				for (it = copyIpcount.begin(); it != copyIpcount.end();) {
					if (m->inUsersGroups(it->first, groups) != true) {	
						copyIpcount.erase(it++);	
					}else {  it++;  }
				}
			
				//if i's children are from the same group then i's pcount size will be 1 
				//if copyIpcount.size() = 0 they are from a branch that is entirely from a group the user doesn't want
				if (copyIpcount.size() == 0) { }
				else if ((t->tree[i].getBranchLength() != -1) && (copyIpcount.size() == 1)) {  UniqueBL += abs(t->tree[i].getBranchLength());	}
			
				//add i's BL to total if it is from the groups the user wants
				if ((t->tree[i].getBranchLength() != -1) && (copyIpcount.size() != 0)) {  
					totalBL += abs(t->tree[i].getBranchLength()); 
				}
			
			}
		
			UW = (UniqueBL / totalBL);  
	
			if (isnan(UW) || isinf(UW)) { UW = 0; }
	
			data[count] = UW;
		}

		return data;
	
	}
	catch(exception& e) {
		m->errorOut(e, "Unweighted", "getValues");
		exit(1);
	}
}

/**************************************************************************************************/

EstOutput Unweighted::getValues(Tree* t, string groupA, string groupB) { 
 try {
	globaldata = GlobalData::getInstance();
		
		vector<string> groups;
		double UniqueBL;  //a branch length is unique if it's chidren are from the same group
		double totalBL;	//all branch lengths
		double UW;		//Unweighted Value = UniqueBL / totalBL;
		map<string, int>::iterator it;  //iterator to traverse pgroups
		map<string, int> copyIpcount;
		copyTree = new Tree;

		//if the users enters no groups then give them the score of all groups
		int numGroups = globaldata->Groups.size();

		//calculate number of comparsions
		int numComp = 0;
		for (int r=0; r<numGroups; r++) { 
			for (int l = r+1; l < numGroups; l++) {
				numComp++;
			}
		}

		//numComp+1 for AB, AC, BC, ABC
		data.resize(numComp+1,0);
		
		int count = 0;
		for (int a=0; a<numGroups; a++) { 
			for (int l = a+1; l < numGroups; l++) {
				UniqueBL=0.0000;  //a branch length is unique if it's chidren are from the same group
				totalBL = 0.00;	//all branch lengths
				UW = 0.00;		//Unweighted Value = UniqueBL / totalBL;
				copyIpcount.clear();
				
				//copy random tree passed in
				copyTree->getCopy(t);
								
				//groups in this combo
				groups.push_back(globaldata->Groups[a]); groups.push_back(globaldata->Groups[l]);
				
				//swap labels in the groups you want to compare
				copyTree->assembleRandomUnifracTree(groups[0], groups[1]);
				
				if (m->control_pressed) { delete copyTree; return data; }
				
				//copyTree->createNewickFile("random"+groupA+toString(count));
				
				for(int i=0;i<copyTree->getNumNodes();i++){
					
					if (m->control_pressed) { delete copyTree; return data; }
					
					/**********************************************************************/
					//This section adds in all lengths that are non leaf
					copyIpcount = copyTree->tree[i].pcount;
					for (it = copyIpcount.begin(); it != copyIpcount.end();) {
						if (m->inUsersGroups(it->first, groups) != true) {	
							copyIpcount.erase(it++);	
						}else { it++;  }
					}
			
					//if i's children are from the same group then i's pcount size will be 1 
					//if copyIpcount.size() = 0 they are from a branch that is entirely from a group the user doesn't want
					if (copyIpcount.size() == 0) { }
					else if ((copyTree->tree[i].getBranchLength() != -1) && (copyIpcount.size() == 1)) {  UniqueBL += abs(copyTree->tree[i].getBranchLength());	}
			
					//add i's BL to total if it is from the groups the user wants
					if ((copyTree->tree[i].getBranchLength() != -1) && (copyIpcount.size() != 0)) {  
						totalBL += abs(copyTree->tree[i].getBranchLength()); 
					}
			
				}
		
				UW = (UniqueBL / totalBL);  
	
				if (isnan(UW) || isinf(UW)) { UW = 0; }
	
				data[count] = UW;
				count++;
				groups.clear();
			}
		}
		
		
		if (numComp != 1) {
			if (numGroups == 0) {
				//get score for all users groups
				for (int i = 0; i < tmap->namesOfGroups.size(); i++) {
					if (tmap->namesOfGroups[i] != "xxx") {
						groups.push_back(tmap->namesOfGroups[i]);
					}
				}
			}else {
				for (int i = 0; i < globaldata->Groups.size(); i++) {
					groups.push_back(globaldata->Groups[i]);
				}
			}
		
			UniqueBL=0.0000;  //a branch length is unique if it's chidren are from the same group
			totalBL = 0.00;	//all branch lengths
			UW = 0.00;		//Unweighted Value = UniqueBL / totalBL;
			copyIpcount.clear();
		
			//copy random tree passed in
			copyTree->getCopy(t);
				
			//swap labels in all the groups you want to compare
			copyTree->assembleRandomUnifracTree(groups);
			
			if (m->control_pressed) { delete copyTree; return data; }

			for(int i=0;i<copyTree->getNumNodes();i++){
				if (m->control_pressed) { delete copyTree; return data; }
			
				copyIpcount = copyTree->tree[i].pcount;
				for (it = copyIpcount.begin(); it != copyIpcount.end();) {
						if (m->inUsersGroups(it->first, groups) != true) {	
							copyIpcount.erase(it++);	
						}else { it++;  }
				}
			
				//if i's children are from the same group then i's pcount size will be 1 
				//if copyIpcount.size() = 0 they are from a branch that is entirely from a group the user doesn't want
				if (copyIpcount.size() == 0) { }
				else if ((copyTree->tree[i].getBranchLength() != -1) && (copyIpcount.size() == 1)) {  abs(UniqueBL += copyTree->tree[i].getBranchLength());	}
			
				//add i's BL to total if it is from the groups the user wants
				if ((copyTree->tree[i].getBranchLength() != -1) && (copyIpcount.size() != 0)) {  
					totalBL += abs(copyTree->tree[i].getBranchLength()); 
				}
			
			}
		
			UW = (UniqueBL / totalBL);  
	
			if (isnan(UW) || isinf(UW)) { UW = 0; }
	
			data[count] = UW;
		}
		
		delete copyTree;
		
		return data;
	
	}
	catch(exception& e) {
		m->errorOut(e, "Unweighted", "getValues");
		exit(1);
	}
}




