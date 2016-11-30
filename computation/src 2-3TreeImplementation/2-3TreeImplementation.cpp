/**
 * @file
 * @author  Mohamed Kalmoua <Mohamed.Kalmoua@gmail.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The Tree class represents a 2-3 self balancing tree.
 * 2-3TreeImplementation.cpp : Defines the entry point for the console application.
 */

#include "stdafx.h"
#include "CNode.h"
#include "CTree.h"
#include "Movie.h"
#include <iostream>
#include <string> 

//https://www.codeproject.com/articles/656056/tree-implementation-in-cplusplus

template<class T> void printOrders (CTree<T>* pTree);
template<class T> void ExecDeleteTestcase(T* pObjects2Insert, int iArrSizeIns, T* pObjects2Delete, int iArrSizeDel);
int _tmain(int argc, _TCHAR* argv[])
{
    int iCnt = 0;
	int iVal2Delete = 0; 
	int iArrSize = 0;
	int iDelArrSize = 0;

    /* Test cases are described in TreeTestcases.pdf */
    /* Testcases001 local rotation, Case 1: Take from right. */
    int iArrDeleteSequence001[]= {4,34,23,88,27,67,14,96}; 
    int iTestcases001[] = {67,34,23,88,96,14,4,27};
    iArrSize = sizeof(iTestcases001)/sizeof(iTestcases001[0]); 
    iDelArrSize = sizeof(iArrDeleteSequence001)/sizeof(iArrDeleteSequence001[0]);
    ExecDeleteTestcase<int>(iTestcases001, iArrSize, iArrDeleteSequence001,iDelArrSize);

    /* Testcases002 local rotation, Case 2: Take from middle first. */	
    int iArrDeleteSequence002[]= {67,17,78,48,68,87,11,37}; 
    int iTestcases002[] = {37,11,17,48,78,87,93,67,68,83};
    iArrSize = sizeof(iTestcases002)/sizeof(iTestcases002[0]); 
    iDelArrSize = sizeof(iArrDeleteSequence002)/sizeof(iArrDeleteSequence002[0]);
    ExecDeleteTestcase<int>(iTestcases002, iArrSize, iArrDeleteSequence002, iDelArrSize);    
	
    /* Testcases003 local rotation, (Case 3: Take from right.) */
    int iArrDeleteSequence003[]= {43,15,56,25,86,70}; 
    int iTestcases003[] = {20,25,43,47,12,56,15,70,86,92};
    iArrSize = sizeof(iTestcases003)/sizeof(iTestcases003[0]); 
    iDelArrSize = sizeof(iArrDeleteSequence003)/sizeof(iArrDeleteSequence003[0]); 
	ExecDeleteTestcase<int>(iTestcases003, iArrSize, iArrDeleteSequence003,iDelArrSize);    

    /* Testcases004 local rotation, (Case 4: Take from right first.) */
    iVal2Delete = 18;
	int iTestcases004[] = {18,47,67,35,96,24,15,45,38,7,11,37};
    iArrSize = sizeof(iTestcases004)/sizeof(iTestcases004[0]); 
    ExecDeleteTestcase<int>(iTestcases004, iArrSize, &iVal2Delete, 1);    
    
    /* Testcases005 local rotation, (Case 5: Take from left.) */
    iVal2Delete = 81;
    int iTestcases005[] = {47,51,16,23,45,79,14,81,37,13,41,90,96,67};
    iArrSize = sizeof(iTestcases005)/sizeof(iTestcases005[0]); 
    ExecDeleteTestcase<int>(iTestcases005, iArrSize, &iVal2Delete, 1);

    /* Testcases006 local rotation, (Case 6: Take from left.) */
    iVal2Delete = 94;
    int iTestcases006[] = {54,70,94,58,25,62,45,17};
    iArrSize = sizeof(iTestcases006)/sizeof(iTestcases006[0]); 
    ExecDeleteTestcase<int>(iTestcases006, iArrSize, &iVal2Delete, 1);

    /* Testcases007 local rotation, (Case 7: Take from middle.) */
    iVal2Delete = 47;
    int iTestcases007[] = {26,25,67,89,57,42,52,47,31,35};
    iArrSize = sizeof(iTestcases007)/sizeof(iTestcases007[0]); 
    ExecDeleteTestcase<int>(iTestcases007, iArrSize, &iVal2Delete, 1);

    /* Testcases008 local rotation, (Case 7: Take from middle.) */
    iVal2Delete = 56;
    int iTestcases008[] = {79,45,94,56,29,64,26,58,50,55};
    iArrSize = sizeof(iTestcases008)/sizeof(iTestcases008[0]); 
    ExecDeleteTestcase<int>(iTestcases008, iArrSize, &iVal2Delete, 1);

    /* Testcases009 global rotation, (Case 9: Take from leftmost child of the middle child of the root.) */
    iVal2Delete = 45;
    int iTestcases009[] = {67,20,72,65,85,47,60,25,45,78,53,58};
    iArrSize = sizeof(iTestcases009)/sizeof(iTestcases009[0]); 
    ExecDeleteTestcase<int>(iTestcases009, iArrSize, &iVal2Delete, 1);

    /* Testcases010 global rotation, (Case 10: Take from rightmost child of the left child of the root.) */
    iVal2Delete = 50;
	int iTestcases010[] = {98,13,46,14,79,38,74,76,41,16,67,92,86,53,50,44};
    iArrSize = sizeof(iTestcases010)/sizeof(iTestcases010[0]); 
    ExecDeleteTestcase<int>(iTestcases010, iArrSize, &iVal2Delete, 1);

    /* Testcases011 global rotation, (Case 11: Take from leftmost child of the right child of the root.) */
    iVal2Delete = 65;
	int iTestcases011[] = {98,72,65,24,87,62,28,84,56,85,48,45};
    iArrSize = sizeof(iTestcases011)/sizeof(iTestcases011[0]); 
    ExecDeleteTestcase<int>(iTestcases011, iArrSize, &iVal2Delete, 1);

    /* Testcases012 global rotation, (Case 12: Take from rightmost child of the middle child of the root.) */
    iVal2Delete = 46;
	int iTestcases012[] = {23,45,67,48,41,56,17,46,54,43,18,13,30,44};
    iArrSize = sizeof(iTestcases012)/sizeof(iTestcases012[0]); 
    ExecDeleteTestcase<int>(iTestcases012, iArrSize, &iVal2Delete, 1);

    /* Testcases013 global rotation, (Case 12: Take from rightmost child of the middle child of the root.) */
    iVal2Delete = 72;
    int iTestcases013[] = {64,38,72,58,25,79,28,76,43};
    iArrSize = sizeof(iTestcases013)/sizeof(iTestcases013[0]); 
    ExecDeleteTestcase<int>(iTestcases013, iArrSize, &iVal2Delete, 1);

    /* Testcases014 global rotation, (Case 14: Take from leftmost child of the right child of the root.) */
    iVal2Delete = 58;
    int iTestcases014[] = {58,79,95,82,57,25,86,81};
    iArrSize = sizeof(iTestcases014)/sizeof(iTestcases014[0]); 
    ExecDeleteTestcase<int>(iTestcases014, iArrSize, &iVal2Delete, 1);

    /* Testcases015 local node merger, (Case 15: Merge to the left.) */
    iVal2Delete = 22;
    int iTestcases015[] = {46,72,70,93,76,36,50,28,22};
    iArrSize = sizeof(iTestcases015)/sizeof(iTestcases015[0]); 
    ExecDeleteTestcase<int>(iTestcases015, iArrSize, &iVal2Delete, 1);

    /* Testcases016 local node merger, (Case 15: Merge to the left.) */
    iVal2Delete = 74;
	int iTestcases016[] = {23,43,46,78,34,83,48,74,61,69};
    iArrSize = sizeof(iTestcases016)/sizeof(iTestcases016[0]); 
    ExecDeleteTestcase<int>(iTestcases016, iArrSize, &iVal2Delete, 1);

    /* Testcases017 local node merger, (Case 17: Merge to the right.) */
    iVal2Delete = 47;
    int iTestcases017[] = {1,12,38,54,48,95,47,81,45};	
    iArrSize = sizeof(iTestcases017)/sizeof(iTestcases017[0]); 
    ExecDeleteTestcase<int>(iTestcases017, iArrSize, &iVal2Delete, 1);

    /* Testcases018 global node merger, Case 18: (Node merger propagates, delete left  node.) */
    iVal2Delete = 73;    
	int iTestcases018[] = {73,24,57,84,56,37,23,75}; 
	iArrSize = sizeof(iTestcases018)/sizeof(iTestcases018[0]); 
    ExecDeleteTestcase<int>(iTestcases018, iArrSize, &iVal2Delete, 1);

    /* Testcases019 global node merger, Case 19: (Node merger propagates, delete right node.) */	
    iVal2Delete = 45;
	int iTestcases019[] = {35,40,45,51,75,82,87};
    iArrSize = sizeof(iTestcases019)/sizeof(iTestcases019[0]); 
    ExecDeleteTestcase<int>(iTestcases019, iArrSize, &iVal2Delete, 1);

    /* Testcases020 removing an internal element. Case 20: (Replace element with Inorder successor.) */ 
    iVal2Delete = 74;
    int iTestcases020[] = {74,65,91,38,46,17,16,84};
    iArrSize = sizeof(iTestcases020)/sizeof(iTestcases020[0]); 
    ExecDeleteTestcase<int>(iTestcases020, iArrSize, &iVal2Delete, 1);

    /* Case 21: Tree insert/delete */ 
    int iArrDeleteSequence021[]= {30,51,32,45,60,75,92}; 
    int iTestcases021[] = {22,27,30,32,35,40,45,51,55,60,75,82,87,92,95,16,10};	
    iArrSize = sizeof(iTestcases021)/sizeof(iTestcases021[0]); 
    iDelArrSize = sizeof(iArrDeleteSequence021)/sizeof(iArrDeleteSequence021[0]); 
    ExecDeleteTestcase<int>(iTestcases021, iArrSize, iArrDeleteSequence021,iDelArrSize);    

    /* Create movie objects and add them to 2-3 tree. */        
    /* Illustrates generic concept of template usage. */
    /* These next test cases can be found on http://www.cs.ucr.edu/cs14/cs14_05win/assignments/as5/as5.html  */
	CTree<CMovie> *pMovieTree = new CTree<CMovie>(); 
    std::cout<<"Testcases001"<<std::endl;
    pMovieTree->insert(new CMovie("Die Hard",1988));
    printOrders (pMovieTree);
    std::cout<<"Testcases002"<<std::endl;
    pMovieTree->insert(new CMovie("Terminator",1984));
    printOrders (pMovieTree);
    std::cout<<"Testcases003"<<std::endl;
    pMovieTree->insert(new CMovie("Kill Bill Volume 1",2010));
    printOrders (pMovieTree);	
    std::cout<<"Testcases004"<<std::endl;
    pMovieTree->insert(new CMovie("Pirates of the Caribbean",2003));
    pMovieTree->insert(new CMovie("The Lord of the Rings",2001));
	printOrders (pMovieTree);		
    std::cout<<"Testcases005"<<std::endl;
    pMovieTree->insert(new CMovie("Star Wars",1977));
    pMovieTree->insert(new CMovie("Gladiator",2000));
	printOrders (pMovieTree);			 
    std::cout<<"Testcases006"<<std::endl;
    pMovieTree->insert(new CMovie("Matrix",1999));
	printOrders (pMovieTree);
    std::cout<<"Testcases007"<<std::endl;
	/* Search for movies in tree */
	if(pMovieTree->find(new CMovie("Matrix")) == 0 ||
       pMovieTree->find(new CMovie("Die Hard")) == 0 || 
       pMovieTree->find(new CMovie("Terminator")) == 0 ||
       pMovieTree->find(new CMovie("Kill Bill Volume 1")) == 0 || 
       pMovieTree->find(new CMovie("Pirates of the Caribbean")) == 0 || 
       pMovieTree->find(new CMovie("The Lord of the Rings")) == 0 ||  
       pMovieTree->find(new CMovie("Star Wars")) == 0 ||  
       pMovieTree->find(new CMovie("Gladiator")) == 0)
    {
        std::cout<<"Search phase failed."<<std::endl;
	}
	else
    {
        std::cout<<"Search phase passed."<<std::endl;
    }

    std::cout<<"Testcases008"<<std::endl;
    CTree<CMovie> *pMovieTree2 = new CTree<CMovie>(); 
	pMovieTree2->deleteItem(new CMovie("Die Hard"));
    printOrders (pMovieTree2);
    pMovieTree2->insert(new CMovie("Die Hard",1988));
	pMovieTree2->deleteItem(new CMovie("Die Hard"));
    printOrders (pMovieTree2);
    pMovieTree2->insert(new CMovie("Die Hard",1988));
    pMovieTree2->insert(new CMovie("Terminator",1984));
	pMovieTree2->deleteItem(new CMovie("Terminator"));
    printOrders (pMovieTree2);
    std::cout<<"Testcases009"<<std::endl;
    pMovieTree2->insert(new CMovie("Terminator",1984));
    pMovieTree2->insert(new CMovie("Kill Bill Volume 1",2010));
    pMovieTree2->deleteItem(new CMovie("Kill Bill Volume 1"));
    printOrders (pMovieTree2);
    std::cout<<"Testcases010"<<std::endl;
    pMovieTree2->insert(new CMovie("Kill Bill Volume 1",2010));
    pMovieTree2->deleteItem(new CMovie("Die Hard"));
    printOrders (pMovieTree2);
    std::cout<<"Testcases011"<<std::endl;
    pMovieTree2->insert(new CMovie("Die Hard",1988));
    pMovieTree2->deleteItem(new CMovie("Terminator"));
    printOrders(pMovieTree2);
	std::cout<<"Testcases012"<<std::endl;
    pMovieTree2->insert(new CMovie("Terminator",1984));
    pMovieTree2->insert(new CMovie("Pirates of the Caribbean",2003));    
    pMovieTree2->deleteItem(new CMovie("Pirates of the Caribbean"));     
    printOrders(pMovieTree2);
    std::cout<<"Testcases013"<<std::endl;
    pMovieTree2->insert(new CMovie("Pirates of the Caribbean",2003));    
    pMovieTree2->insert(new CMovie("The Lord of the Rings",2001));
    pMovieTree2->deleteItem(new CMovie("The Lord of the Rings"));     
    printOrders(pMovieTree2);
    std::cout<<"Testcases014"<<std::endl;
    pMovieTree2->insert(new CMovie("Kill Bill Volume 2",2011));
    pMovieTree2->deleteItem(new CMovie("Kill Bill Volume 2"));
    printOrders(pMovieTree2);
    std::cout<<"Testcases015"<<std::endl;
    pMovieTree2->insert(new CMovie("Kill Bill Volume 2",2011));
    pMovieTree2->deleteItem(new CMovie("Die Hard"));
    printOrders(pMovieTree2);
    std::cout<<"Testcases016"<<std::endl;
	CTree<CMovie> *pMovieTree3 = new CTree<CMovie>(); 
    pMovieTree3->insert(new CMovie("Die Hard",1988));
    pMovieTree3->insert(new CMovie("Terminator",1984));
    pMovieTree3->insert(new CMovie("Kill Bill Volume 1",2010));
    pMovieTree3->insert(new CMovie("Pirates of the Caribbean",2003));
    pMovieTree3->insert(new CMovie("The Lord of the Rings",2001));
    pMovieTree3->insert(new CMovie("Star Wars",1977));
    pMovieTree3->insert(new CMovie("Gladiator",2000));
    pMovieTree3->insert(new CMovie("Matrix",1999));
    pMovieTree3->insert(new CMovie("Clerks",2001));
    pMovieTree3->insert(new CMovie("The Usual Suspects",1997));
    printOrders(pMovieTree3);
    std::cout<<"Testcases017"<<std::endl;
    pMovieTree3->insert(new CMovie("Silence of the Lambs",1992));
    pMovieTree3->insert(new CMovie("Enemy of the State",1994));
    pMovieTree3->insert(new CMovie("Aliens",2010));	
    printOrders(pMovieTree3);
    std::cout<<"Testcases018"<<std::endl;
    pMovieTree3->insert(new CMovie("Spaceballs",1986));	
    printOrders(pMovieTree3);
    std::cout<<"Testcases019"<<std::endl;
    pMovieTree3->insert(new CMovie("Happy Gilmore",1989));	
    printOrders(pMovieTree3);
    std::cout<<"Testcases020"<<std::endl;
    pMovieTree3->insert(new CMovie("Top Gun",1986));	
    printOrders(pMovieTree3);
    std::cout<<"Testcases021"<<std::endl;
    pMovieTree3->insert(new CMovie("Meet the Parents",2000));	
    pMovieTree3->insert(new CMovie("Kill Bill Volume 2",2011));	
    printOrders(pMovieTree3);
    std::cout<<"Testcases022"<<std::endl;
    CTree<CMovie> *pMovieTree4 = new CTree<CMovie>(); 
    pMovieTree4->insert(new CMovie("Aliens",2010));	
    pMovieTree4->insert(new CMovie("Enemy of the State",1994));
    pMovieTree4->insert(new CMovie("Gladiator",2000));
    pMovieTree4->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree4->insert(new CMovie("Matrix",1999));
    pMovieTree4->insert(new CMovie("Happy Gilmore",1989));	
    pMovieTree4->insert(new CMovie("Meet the Parents",2000));	
    pMovieTree4->deleteItem(new CMovie("Aliens"));
    printOrders(pMovieTree4);
	CTree<CMovie> *pMovieTree5 = new CTree<CMovie>(); 
    pMovieTree5->insert(new CMovie("Aliens",2010));	
    pMovieTree5->insert(new CMovie("Enemy of the State",1994));
    pMovieTree5->insert(new CMovie("Gladiator",2000));
    pMovieTree5->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree5->insert(new CMovie("Matrix",1999));
    pMovieTree5->insert(new CMovie("Meet the Parents",2000));	
    pMovieTree5->deleteItem(new CMovie("Aliens"));
    printOrders(pMovieTree5);
	CTree<CMovie> *pMovieTree6 = new CTree<CMovie>(); 
    pMovieTree6->insert(new CMovie("Aliens",2010));	
    pMovieTree6->insert(new CMovie("Enemy of the State",1994));
    pMovieTree6->insert(new CMovie("Gladiator",2000));
    pMovieTree6->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree6->insert(new CMovie("Matrix",1999));
    pMovieTree6->insert(new CMovie("Meet the Parents",2000));	
    pMovieTree6->deleteItem(new CMovie("Gladiator"));
    printOrders(pMovieTree6);
    std::cout<<"Testcases023"<<std::endl;
	CTree<CMovie> *pMovieTree7 = new CTree<CMovie>(); 
    pMovieTree7->insert(new CMovie("Aliens",2010));	
    pMovieTree7->insert(new CMovie("Enemy of the State",1994));
    pMovieTree7->insert(new CMovie("Gladiator",2000));
	pMovieTree7->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree7->insert(new CMovie("Matrix",1999));
    pMovieTree7->insert(new CMovie("Clerks",2001));
    pMovieTree7->deleteItem(new CMovie("Gladiator"));
    printOrders(pMovieTree7);
    std::cout<<"Testcases024"<<std::endl;
	CTree<CMovie> *pMovieTree8 = new CTree<CMovie>(); 
    pMovieTree8->insert(new CMovie("Aliens",2010));	
    pMovieTree8->insert(new CMovie("Enemy of the State",1994));
    pMovieTree8->insert(new CMovie("Gladiator",2000));
	pMovieTree8->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree8->insert(new CMovie("Matrix",1999));
    pMovieTree8->insert(new CMovie("Clerks",2001));
    pMovieTree8->deleteItem(new CMovie("Matrix"));
    printOrders(pMovieTree8);
    std::cout<<"Testcases025"<<std::endl;
	CTree<CMovie> *pMovieTree9 = new CTree<CMovie>(); 
    pMovieTree9->insert(new CMovie("Aliens",2010));	
    pMovieTree9->insert(new CMovie("Enemy of the State",1994));
    pMovieTree9->insert(new CMovie("Gladiator",2000));
	pMovieTree9->insert(new CMovie("Kill Bill Volume 2",2011));	
    pMovieTree9->insert(new CMovie("Matrix",1999));
    pMovieTree9->insert(new CMovie("Clerks",2001));
    pMovieTree9->insert(new CMovie("Happy Gilmore",1989));	
	pMovieTree9->deleteItem(new CMovie("Matrix"));
    printOrders(pMovieTree9);
	std::cin.ignore();
	return 0;
}

template<class T> 
void ExecDeleteTestcase(T* pObjects2Insert, int iArrSizeIns, T* pObjects2Delete, int iArrSizeDel)
{
    int iCnt = 0;
    CTree<int> *pTree = 0;

    pTree = new CTree<T>(); 
    for (iCnt = 0;iCnt < iArrSizeIns; iCnt++)
    {
        pTree->insert(&pObjects2Insert[iCnt]);
    }

    for (iCnt = 0;iCnt < iArrSizeDel; iCnt++)
    {
        pTree->deleteItem(&pObjects2Delete[iCnt]);
		printOrders(pTree);
    }

    if(pTree != 0)
    {
        delete pTree; 
    }
}

template<class T> 
void printOrders (CTree<T>* pTree) 
{
    int iItemCnt = 0; 
    pTree->print(preorder,&iItemCnt);
    std::cout<<"**preorder["<<iItemCnt<<"]"<<std::endl<<std::endl;
    pTree->print(inorder,&iItemCnt);
    std::cout<<"**inorder["<<iItemCnt<<"]"<<std::endl<<std::endl;
    pTree->print(postorder,&iItemCnt);
    std::cout<<"**postorder["<<iItemCnt<<"]"<<std::endl<<std::endl;
}
